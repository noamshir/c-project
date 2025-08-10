#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Headers/string.h"
#include "Headers/error.h"
#include "Headers/symbol_table.h"
#include "Headers/utils.h"
#include "Headers/consts.h"
#include "Headers/binary_code.h"
#include "Headers/files_extractor.h"
#include "Headers/second_pass.h"
#include "Headers/command.h"
#include "Headers/guide.h"

void second_pass(char *file_name_without_postfix, symbol_item **symbol_table, unsigned int *array_of_commands, int ICF, unsigned int *array_of_data, int DCF)
{
    FILE *file;
    char *file_name;
    char line[LINE_SIZE];
    char *word, *main_op, *label;
    int line_num = 0, error_flag = 0, IC = 0;

    char **entry_labels = NULL;
    int *entry_addresses = NULL;
    int entry_count = 0;

    char **extern_labels = NULL;
    int *extern_addresses = NULL;
    int extern_count = 0;

    printf("second pass started\n");

    file_name = malloc(strlen(file_name_without_postfix) + 4);
    if (file_name == NULL)
    {
        exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
    }

    /*adding ".am" at the end of the file name*/
    strcpy(file_name, file_name_without_postfix);
    strcat(file_name, ".am");

    /*open the file for reading and check for error*/
    file = fopen(file_name, "r");
    if (file == NULL)
    {
        print_error(PROCESS_ERROR_FAILED_TO_OPEN_FILE);
        free(file_name);
        return;
    }

    /* allocate entry */
    entry_labels = malloc(sizeof(char *));
    if (entry_labels == NULL)
    {
        safe_exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
    }
    entry_addresses = malloc(sizeof(int));
    if (entry_addresses == NULL)
    {
        safe_exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
    }

    /* allocate extern */
    extern_labels = malloc(sizeof(char *));
    if (extern_labels == NULL)
    {
        safe_exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
    }
    extern_addresses = malloc(sizeof(int));
    if (extern_addresses == NULL)
    {
        safe_exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
    }

    /*pass on each line at the file*/
    while (fgets(line, sizeof(line), file) != NULL)
    {
        line_num++;
        printf("line %d: %s\n", line_num, line);

        if (is_empty_line(line) || is_comment_line(line))
        {
            /* skip empty/comment lines */
            continue;
        }

        /*get the first word in the line*/
        word = strtok(duplicate_str(line), " ");
        word = delete_white_spaces_start_and_end(word);

        /*ignore lables words and save the command in param*/
        if (is_label_declaration(word))
        {
            main_op = strtok(NULL, " ");
            main_op = delete_white_spaces_start_and_end(main_op);
        }
        else
        {
            main_op = duplicate_str(word);
        }

        /* check if the word is a guide that handled in first pass */
        if (is_data_guide(main_op) || is_string_guide(main_op) || is_mat_guide(main_op) || is_extern_guide(main_op))
        {
            continue; /* ignore in second pass */
        }

        /*handle '.entry'*/
        if (is_entry_guide(main_op))
        {
            label = strtok(NULL, " \t\n");
            label = delete_white_spaces_start_and_end(label);
            if (!handle_entry_guide(label, line_num, symbol_table, &entry_labels, &entry_addresses, &entry_count))
            {
                error_flag = 1;
            }
        }
        else
        {
            /*if it's command line, maybe we need to encode some of its operands */
            handle_command_line_second_pass(symbol_table, line, line_num, array_of_commands, &IC, &extern_labels, &extern_addresses, &extern_count);
        }
    }

    /* creation of final files (.ob, .entry, .extrn)*/
    if (!error_flag)
    {
        generate_ob_file(file_name_without_postfix, array_of_commands, ICF, array_of_data, DCF);
    }

    if (!error_flag && entry_count > 0)
    {
        generate_entry_file(file_name_without_postfix, entry_labels, entry_addresses, entry_count);
    }
    if (!error_flag && extern_count > 0)
    {
        generate_extern_file(file_name_without_postfix, extern_labels, extern_addresses, extern_count);
    }

    /* cleaning memory */
    free_array_of_strings(entry_labels, entry_count);
    free(entry_addresses);

    free_array_of_strings(extern_labels, extern_count);
    free(extern_addresses);

    free(file_name);

    /*print a message for final*/
    if (!error_flag)
    {
        printf("Second pass finished successfully.\n");
    }
    else
    {
        printf("Second pass finished with errors.\n");
    }
}
