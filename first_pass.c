#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Headers/string.h"
#include "Headers/error.h"
#include "Headers/symbol_table.h"
#include "Headers/first_pass.h"
#include "Headers/utils.h"
#include "Headers/consts.h"
#include "Headers/binary_code.h"
#include "Headers/guide.h"
#include "Headers/command.h"

void first_pass(char *file_name_without_postfix)
{
    int IC = 0, DC = 0, line_num = 0, i = 0, is_line_valid = 0, has_errors = 0;
    char *file_name, line[LINE_SIZE], *word, *main_op, **array_of_commands = NULL, **array_of_data = NULL;
    FILE *file;
    symbol_item *symbol_table = NULL;

    array_of_commands = malloc(sizeof(char *));
    array_of_data = malloc(sizeof(char *));

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
        print_error(PROCESS_ERROR_FAILED_TO_OPEN_FILE);
        return;
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

        word = strtok(strdup(line), " ");
        if (is_label(word))
        {
            main_op = strtok(NULL, " ");
        }
        else
        {
            main_op = strdup(word);
        }

        if (is_data_guide(main_op) || is_string_guide(main_op) || is_mat_guide(main_op))
        {
            is_line_valid = handle_guide_line(&symbol_table, line, &array_of_data, &DC);
        }
        else if (is_extern_guide(main_op))
        {
            is_line_valid = handle_extern(&symbol_table, line);
        }
        else if (is_entry_guide(main_op))
        {
            /* will deal at second pass */
            continue;
        }
        else
        {
            /* not all the above? must be an command line */
            is_line_valid = handle_command_line(&symbol_table, line, &array_of_commands, &IC);
        }

        if (!is_line_valid)
        {
            printf("Error in line %d\n", line_num);
            has_errors = 1;
        }

        if ((IC + DC) > MAX_MEMORY_SIZE)
        {
            safe_exit(PROCESS_ERROR_MAX_MEMORY_SIZE_EXCEEDED);
        }
    }

    if (has_errors)
    {
        safe_exit(PROCESS_ERROR_FIRST_PASS_FAILED);
    }

    // loop data array and prints it values
    printf("data array after first pass (%d):\n", DC);
    for (i = 0; i < DC; i++)
    {
        printf("data: %s\n", array_of_data[i]);
    }

    // loop commands array and prints it values
    printf("commands array after first pass (%d):\n", IC);
    for (i = 0; i < IC; i++)
    {
        printf("command: %s\n", array_of_commands[i]);
    }

    update_data_symbol_items_address(symbol_table, IC);

    free(file_name);
    free(array_of_commands);
    free(array_of_data);
    free(symbol_table);
    fclose(file);

    printf("first pass finished\n");
}
