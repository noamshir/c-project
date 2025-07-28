#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Headers/pre_assembler.h"
#include "Headers/table.h"
#include "Headers/string.h"
#include "Headers/error.h"
#include "Headers/symbol_table.h"
#include "Headers/first_pass.h"
#include "Headers/utils.h"
#include "Headers/consts.h"

void first_pass(char *file_name_without_postfix)
{
    int IC = 0, DC = 0, is_curr_label = 0, line_num = 0;
    char *temp_file_name, *file_name, line[80], *word, *next_word, *third_word;
    FILE *file;
    symbol_item *symbol_table = NULL;
    int *array_of_data = NULL;

    printf("first pass started\n");

    file_name = malloc(strlen(file_name_without_postfix) + 4);
    if (file_name == NULL)
    {
        exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
    }
    strcpy(file_name, file_name_without_postfix);
    strcat(file_name, ".am");

    file = fopen(file_name, "r");
    if (file == NULL)
    {
        printf("error code is %d\n", PROCESS_ERROR_FAILED_TO_OPEN_FILE);
        return;
    }

    while (fgets(line, sizeof(line), file) != NULL)
    {
        line_num++;
        printf("line: %s \n", line);
        if (is_empty_line(line) || is_comment_line(line))
        {
            continue;
        }

        is_curr_label = 0;
        word = strtok(strdup(line), " ");
        delete_white_spaces(word);
        if (is_label(word))
        {
            is_curr_label = 1;
        }

        next_word = strtok(NULL, " ");
        if (next_word != NULL)
        {
            delete_white_spaces(next_word);
        }

        if (is_data_guide(next_word) || is_string_guide(next_word) || is_mat_guide(next_word))
        {
            handle_guide(&symbol_table, line, array_of_data, &DC);
        }
        else if (is_extern_guide(next_word))
        {
            handle_extern(&symbol_table, line);
        }
        else if (is_entry_guide(next_word))
        {
            /* will deal at second pass */
            continue;
        }
        // else if (is_operation(next_word))
        // {
        //     add_operation_to_symbol_table(symbol_table, line, &IC);
        // }
    }
}

void handle_guide(symbol_item **symbol_table, char *line, int *array_of_data, int *DC)
{
    char *guide, *label;
    label = strtok(strdup(line), " ");
    delete_white_spaces(label);
    guide = strtok(NULL, " ");
    delete_white_spaces(guide);

    if (is_data_guide(guide))
    {
        add_label_to_symbol_table(symbol_table, label, "data", *DC);
        handle_data_guide(symbol_table, line, array_of_data, DC);
    }
    else if (is_string_guide(guide))
    {
        add_label_to_symbol_table(symbol_table, label, "data", *DC);
        handle_string_guide(symbol_table, line, array_of_data, DC);
    }
    else if (is_mat_guide(guide))
    {
        handle_mat_guide(symbol_table, array_of_data, line, DC);
    }
}

void handle_data_guide(symbol_item **symbol_table, char *line, int *array_of_data, int *DC)
{
    // check that line is of type: "label: .data num1, num2, ...., numn"
    char *word, *next_word;
    int num;
    int i = 0;
    word = strtok(strdup(line), " ");
    delete_white_spaces(word);
    if (is_label(word))
    {
        word = strtok(NULL, " ");
        delete_white_spaces(word);
        if (!is_data_guide(word))
        {
            printf("error code is %d\n", PROCESS_ERROR_INVALID_MACRO_DECLARATION);
            return;
        }
    }
    else if (!is_data_guide(word))
    {
        printf("error code is %d\n", PROCESS_ERROR_INVALID_MACRO_DECLARATION);
        return;
    }

    // get all the numbers in line, increase DC and enter them to the array of data
    while (1)
    {
        next_word = strtok(NULL, ",");
        printf("next word: %s\n", next_word);
        if (next_word == NULL)
        {
            break;
        }
        delete_white_spaces(next_word);
        if (!is_integer(next_word))
        {
            printf("error code is %d\n", PROCESS_ERROR_DATA_GUIDE_INVALID_PARAM);
            return;
        }

        num = atoi(next_word);
        // should check if num is integer
        printf("num: %d\n", num);

        // allocate memo to array of data
        array_of_data = realloc(array_of_data, (*DC + 1) * sizeof(int));
        if (array_of_data == NULL)
        {
            printf("error code is %d\n", PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
            return;
        }
        array_of_data[*DC] = num;
        i++;
        (*DC)++;
    }
}

void handle_string_guide(symbol_item **symbol_table, char *line, int *array_of_data, int *DC)
{
    // check that line is of type: "label: .string "string""
    int i = 0;
    char *word, *next_word, c;
    word = strtok(strdup(line), " ");
    delete_white_spaces(word);
    if (is_label(word))
    {
        word = strtok(NULL, " ");
        delete_white_spaces(word);
        if (!is_string_guide(word))
        {
            printf("error code is %d\n", PROCESS_ERROR_INVALID_MACRO_DECLARATION);
            return;
        }
    }
    else if (!is_string_guide(word))
    {
        printf("error code is %d\n", PROCESS_ERROR_INVALID_MACRO_DECLARATION);
        return;
    }

    next_word = strtok(NULL, " ");
    if (next_word == NULL)
    {
        printf("error code is %d\n", PROCESS_ERROR_INVALID_MACRO_DECLARATION);
        return;
    }

    if (next_word[0] != '\"' || next_word[strlen(next_word) - 1] != '\"')
    {
        printf("error code is %d\n", PROCESS_ERROR_INVALID_MACRO_DECLARATION);
        return;
    }

    // insert each char asci value to array of data and increment dc until end of quate
    for (i = 1; i < strlen(next_word) - 1; i++)
    {
        c = next_word[i];
        printf("c: %c\n", c);
        // allocate memo to array of data
        array_of_data = realloc(array_of_data, (*DC + 1) * sizeof(int));
        if (array_of_data == NULL)
        {
            printf("error code is %d\n", PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
            return;
        }
        printf("asci val: %d \n", (int)c);
        array_of_data[*DC] = (int)c;
        (*DC)++;
    }

    // add end of string char
    array_of_data[*DC] = (int)'\0';
    (*DC)++;
}

void handle_mat_guide(symbol_item **symbol_table, int *array_of_data, char *line, int *DC)
{
    // check that line is of type: "label: .mat [num1][num2] optional numbers seperated by comma
    char *word, *next_word;
    int num, rows, cols;
    int i = 0;
    word = strtok(strdup(line), " ");
    delete_white_spaces(word);
    if (is_label(word))
    {
        word = strtok(NULL, " ");
        delete_white_spaces(word);
        if (!is_mat_guide(word))
        {
            printf("error code is %d\n", PROCESS_ERROR_INVALID_MACRO_DECLARATION);
            return;
        }
    }
    else if (!is_mat_guide(word))
    {
        printf("error code is %d\n", PROCESS_ERROR_INVALID_MACRO_DECLARATION);
        return;
    }

    next_word = strtok(NULL, " ");
    if (next_word == NULL)
    {
        printf("error code is %d\n, ", PROCESS_ERROR_INVALID_MACRO_DECLARATION);
        return;
    }

    // check that next word is [num1][num2]
    if (next_word[0] != '[' || next_word[strlen(next_word) - 1] != ']')
    {
        printf("error code is %d\n, ", PROCESS_ERROR_INVALID_MACRO_DECLARATION);
        return;
    }

    rows = next_word[1] - '0';
    cols = next_word[4] - '0';
    if (rows == 0 || cols == 0)
    {
        printf("error code is %d\n, ", PROCESS_ERROR_INVALID_MACRO_DECLARATION);
        return;
    }
    printf("rows: %d, cols: %d\n", rows, cols);

    for (i = 0; i < (rows * cols); i++)
    {
        next_word = strtok(NULL, ",");
        printf("next word: %s\n", next_word);
        if (next_word == NULL)
        {
            num = 0;
        }
        else
        {
            delete_white_spaces(next_word);
            if (!is_integer(next_word))
            {
                printf("error code is %d\n, ", PROCESS_ERROR_INVALID_MACRO_DECLARATION);
                return;
            }
            num = atoi(next_word);
        }
        // allocate memo to array of data
        array_of_data = realloc(array_of_data, (*DC + 1) * sizeof(int));
        if (array_of_data == NULL)
        {
            printf("error code is %d\n, ", PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
            return;
        }
        printf("num: %d\n", num);
        array_of_data[*DC] = num;
        (*DC)++;
    }

    next_word = strtok(NULL, ",");
    printf("next word: %s\n", next_word);
    if (next_word != NULL)
    {
        printf("error code is %d\n", PROCESS_ERROR_INVALID_MACRO_DECLARATION);
        return;
    }
}

void handle_extern(symbol_item **symbol_table, char *line)
{
    // check that line is of type: "label: .extern operand"
    char *word, *next_word;
    int num;
    int i = 0;
    word = strtok(strdup(line), " ");
    delete_white_spaces(word);
    if (is_label(word))
    {
        word = strtok(NULL, " ");
        delete_white_spaces(word);
        if (!is_extern_guide(word))
        {
            printf("error code is %d\n", PROCESS_ERROR_INVALID_MACRO_DECLARATION);
            return;
        }
    }
    else if (!is_extern_guide(word))
    {
        printf("error code is %d\n", PROCESS_ERROR_INVALID_MACRO_DECLARATION);
        return;
    }

    next_word = strtok(NULL, " ");
    if (next_word == NULL)
    {
        printf("error code is %d\n", PROCESS_ERROR_INVALID_MACRO_DECLARATION);
        return;
    }
    delete_white_spaces(next_word);
    printf("next word: %s\n", next_word);
    add_label_to_symbol_table(symbol_table, next_word, "external", 0);
}
