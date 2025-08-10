#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Headers/string.h"
#include "Headers/error.h"
#include "Headers/symbol_table.h"
#include "Headers/mcro_table.h"
#include "Headers/first_pass.h"
#include "Headers/utils.h"
#include "Headers/consts.h"
#include "Headers/guide.h"
#include "Headers/command.h"
#include "Headers/second_pass.h"

int first_pass(char *file_name_without_postfix, mcro_item **mcro_table)
{
    int IC = 0, DC = 0, ICF, DCF, line_num = 0, is_line_valid = 0, has_errors = 0;
    unsigned int array_of_commands[156], array_of_data[156];
    char *file_name, line[LINE_SIZE], *first_word_in_line, *main_op;
    FILE *file;
    symbol_item *symbol_table = NULL;

    printf("first pass started\n");

    file_name = malloc(strlen(file_name_without_postfix) + 4);
    if (file_name == NULL)
    {
        safe_exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
    }

    strcpy(file_name, file_name_without_postfix);
    strcat(file_name, ".am");

    file = fopen(file_name, "r");
    if (file == NULL)
    {
        free(file_name);
        print_error(PROCESS_ERROR_FAILED_TO_OPEN_FILE);
        return 0;
    }

    while (fgets(line, sizeof(line), file) != NULL)
    {
        line_num++;
        printf("line (%d): %s", line_num, line);

        if (is_empty_line(line))
        {
            printf("empty line, skipping\n");
            continue;
        }
        else if (is_comment_line(line))
        {
            printf("comment line, skipping\n");
            continue;
        }

        first_word_in_line = strtok(duplicate_str(line), " ");
        if (is_label_declaration(first_word_in_line))
        {
            if (find_mcro_item_by_name(*mcro_table, first_word_in_line) != NULL)
            {
                print_line_error(PROCESS_ERROR_LABEL_AS_MACRO, line_num);
                continue;
            }
            /* if the first word is label than the second one is the command or the guide */
            main_op = strtok(NULL, " ");
        }
        else
        {
            /* if the first word isn't a label it must be the command or the guide */
            main_op = duplicate_str(first_word_in_line);
        }

        if (is_data_guide(main_op) || is_string_guide(main_op) || is_mat_guide(main_op))
        {
            is_line_valid = handle_guide_line(&symbol_table, line, line_num, array_of_data, &DC);
        }
        else if (is_extern_guide(main_op))
        {
            is_line_valid = handle_extern_guide_line(&symbol_table, line, line_num);
        }
        else if (is_entry_guide(main_op))
        {
            /* will deal at second pass */
            continue;
        }
        else
        {
            /* not all the above? must be a command line */
            is_line_valid = handle_command_line_first_pass(&symbol_table, line, line_num, array_of_commands, &IC);
        }

        if (!is_line_valid)
        {
            has_errors = 1;
        }

        if (((IC + DC) + MEMORY_START_ADDRESS) > MAX_MEMORY_SIZE)
        {
            /* exceeded max memory, should stop */
            free_symbol_table(symbol_table);
            free(file_name);
            fclose(file);
            print_error(PROCESS_ERROR_MAX_MEMORY_SIZE_EXCEEDED);
            return 0;
        }
    }

    if (has_errors)
    {
        free_symbol_table(symbol_table);
        free(file_name);
        fclose(file);
        print_error(PROCESS_ERROR_FIRST_PASS_FAILED);
        return 0;
    }

    ICF = IC;
    DCF = DC;

    update_data_symbol_items_address(&symbol_table, IC + MEMORY_START_ADDRESS);

    free(file_name);
    fclose(file);

    printf("first pass finished\n");

    /* second pass */
    second_pass(file_name_without_postfix, &symbol_table, array_of_commands, ICF, array_of_data, DCF);

    /* free memory */
    free_symbol_table(symbol_table);

    return 1;
}
