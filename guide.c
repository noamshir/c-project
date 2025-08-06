#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Headers/string.h"
#include "Headers/error.h"
#include "Headers/symbol_table.h"
#include "Headers/utils.h"
#include "Headers/consts.h"
#include "Headers/binary_code.h"
#include "Headers/guide.h"

int handle_guide_line(symbol_item **symbol_table, char *line, char ***array_of_data, int *DC)
{
    char *guide, *opening_word, *label_name, *guide_declaration;

    opening_word = strtok(strdup(line), " ");
    if (is_label_declaration(opening_word))
    {
        /* get label name (without the :) and add it to symbol table */
        label_name = get_label_name(opening_word);
        if (!add_symbol_item(symbol_table, label_name, "data", *DC))
        {
            return 0;
        }

        /* first word in line is label, second word must be the guide (.data/.string/...)*/
        guide = strtok(NULL, " ");
    }
    else
    {
        /* first word in line must be the guide (.data/.string/...)*/
        guide = strdup(opening_word);
    }

    /* guide declaration is everything after the guide type until the end of the line */
    guide_declaration = strtok(NULL, "\n");

    if (is_data_guide(guide))
    {
        return handle_data_guide(guide_declaration, array_of_data, DC);
    }
    else if (is_string_guide(guide))
    {
        return handle_string_guide(guide_declaration, array_of_data, DC);
    }
    else if (is_mat_guide(guide))
    {
        return handle_mat_guide(guide_declaration, array_of_data, DC);
    }
    else
    {
        /* should not happen as we call this fn only when line is a .data/.string/.mat line*/
        return 0;
    }
}

int handle_data_guide(char *guide_declaration, char ***array_of_data, int *DC)
{
    /* check that guide_declaration is of type: "num1, num2, ...., numn" */
    char *str, *temp;
    int num;
    int i = 0;

    temp = strdup(guide_declaration);
    if (!is_data_guide_declaration(temp))
    {
        print_error(PROCESS_ERROR_DATA_GUIDE_INVALID_PARAM);
        return 0;
    }

    /* get all the numbers in guide_declaration and enter them to the array of data and increase DC */
    while (1)
    {
        if (i > 0)
        {
            str = strtok(NULL, ",");
        }
        else
        {
            /* first loop, use this */
            str = strtok(temp, ",");
        }

        str = delete_white_spaces_start_and_end(str);
        if (str == NULL)
        {
            break;
        }

        /* should check if num is integer */
        if (!is_integer(str))
        {
            print_error(PROCESS_ERROR_DATA_GUIDE_INVALID_PARAM);
            return 0;
        }

        num = atoi(str);

        /* allocate memo to array of data */
        *array_of_data = realloc(*array_of_data, (*DC + 1) * sizeof(char *));
        if (*array_of_data == NULL)
        {
            safe_exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
        }

        (*array_of_data)[*DC] = convert_num_to_10_bits(num);
        i++;
        (*DC)++;
    }

    return 1;
}

int handle_string_guide(char *guide_declaration, char ***array_of_data, int *DC)
{
    /* check that line is of type: "label: .string "string"" */
    int i = 0;
    char *temp, c;

    temp = strdup(guide_declaration);
    if (temp == NULL)
    {
        print_error(PROCESS_ERROR_STRING_GUIDE_INVALID_PARAM);
        return 0;
    }

    /* here we cant just delete white spaces as the "string" may include spaces! */
    temp = delete_white_spaces_start_and_end(temp);
    if (temp[0] != '\"' || temp[strlen(temp) - 1] != '\"')
    {
        /* temi is not of type "something" */
        print_error(PROCESS_ERROR_STRING_GUIDE_INVALID_PARAM);
        return 0;
    }

    /* insert each char asci value to array of data and increment dc until end of quote */
    for (i = 1; i < strlen(temp) - 1; i++)
    {
        c = temp[i];

        /* allocate memo to array of data */
        *array_of_data = realloc(*array_of_data, (*DC + 1) * sizeof(char *));
        if (*array_of_data == NULL)
        {
            safe_exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
        }

        (*array_of_data)[*DC] = convert_num_to_10_bits((int)c);
        (*DC)++;
    }

    *array_of_data = realloc(*array_of_data, (*DC + 1) * sizeof(char *));
    if (*array_of_data == NULL)
    {
        safe_exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
    }

    /* add end of string char binary code */
    (*array_of_data)[*DC] = convert_num_to_10_bits((int)'\0');
    (*DC)++;

    return 1;
}

int handle_mat_guide(char *guide_declaration, char ***array_of_data, int *DC)
{
    /* check that line is of type: "label: .mat [num1][num2] optional numbers separated by comma */
    char *str, *temp;
    int num, rows, cols;
    int i = 0;

    temp = strdup(guide_declaration);
    if (temp == NULL)
    {
        print_error(PROCESS_ERROR_MAT_GUIDE_INVALID_PARAM);
        return 0;
    }

    temp = delete_white_spaces_start_and_end(temp);

    if (!is_mat_declaration(temp))
    {
        print_error(PROCESS_ERROR_MAT_GUIDE_INVALID_PARAM);
        return 0;
    }

    /* populate the rows and cols vars */
    if (!set_rows_and_cols_from_mat_declaration(temp, &rows, &cols))
    {
        print_error(PROCESS_ERROR_MAT_GUIDE_INVALID_PARAM);
        return 0;
    }

    str = strtok(temp, " ");
    /* now str is num1, num2..., if not set it will be empty */
    for (i = 0; i < (rows * cols); i++)
    {
        str = strtok(NULL, ",");
        str = delete_white_spaces_start_and_end(str);

        if (str == NULL)
        {
            /* fill with 0 if no mat data*/
            num = 0;
        }
        else
        {
            if (!is_integer(str))
            {
                print_error(PROCESS_ERROR_MAT_GUIDE_INVALID_PARAM);
                return 0;
            }
            num = atoi(str);
        }
        // allocate memo to array of data
        *array_of_data = realloc(*array_of_data, (*DC + 1) * sizeof(char *));
        if (*array_of_data == NULL)
        {
            safe_exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
        }

        (*array_of_data)[*DC] = convert_num_to_10_bits(num);
        (*DC)++;
    }

    str = strtok(NULL, ",");
    if (str != NULL)
    {
        /* more params than space in mat */
        print_error(PROCESS_ERROR_MAT_GUIDE_INVALID_PARAM);
        return 0;
    }

    return 1;
}

int set_rows_and_cols_from_mat_declaration(char *guide_declaration, int *rows, int *cols)
{
    char *temp, *num_str;
    int i = 0;

    if (!is_mat_declaration(guide_declaration))
    {
        return 0;
    }

    /* loop the first [num] and check if valid */
    for (temp = guide_declaration + 1; *temp != ']'; temp++)
    {
        if (i >= 3)
        {
            /* if we got here mat space needed is way more than we got */
            return 0;
        }
        else if (i == 0)
        {
            /* first loop, use this */
            num_str = malloc(i + 1);
            if (num_str == NULL)
            {
                safe_exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
            }
        }
        else
        {
            /* reallocate memory */
            num_str = realloc(num_str, i + 1);
            if (num_str == NULL)
            {
                safe_exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
            }
        }
        num_str[i] = *temp;
        i++;
    }
    *rows = atoi(num_str);

    i = 0;
    temp = temp + 2;
    /* loop the second [num] and check if valid (similar to the first one)*/
    for (; *temp != ']'; temp++)
    {
        if (i >= 3)
        {
            return 0;
        }
        else if (i == 0)
        {
            num_str = malloc(i + 1);
            if (num_str == NULL)
            {
                safe_exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
            }
        }
        else
        {
            num_str = realloc(num_str, i + 1);
            if (num_str == NULL)
            {
                safe_exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
            }
        }
        num_str[i] = *temp;
        i++;
    }
    *cols = atoi(num_str);

    free(num_str);
    return 1;
}

int handle_extern_guide_line(symbol_item **symbol_table, char *line)
{
    /* check that line is of type: "label: .extern extern label" */
    char *word, *next_word;

    word = strtok(strdup(line), " ");
    if (is_label_declaration(word))
    {
        word = strtok(NULL, " ");
        if (!is_extern_guide(word))
        {
            print_error(PROCESS_ERROR_INVALID_MACRO_DECLARATION);
            return 0;
        }
    }
    else if (!is_extern_guide(word))
    {
        print_error(PROCESS_ERROR_INVALID_MACRO_DECLARATION);
        return 0;
    }

    next_word = strtok(NULL, "\n");
    if (next_word == NULL)
    {
        print_error(PROCESS_ERROR_INVALID_MACRO_DECLARATION);
        return 0;
    }

    /* check external label and add it to the symbol table */
    next_word = delete_white_spaces_start_and_end(next_word);
    if (!is_valid_label_name(next_word))
    {
        print_error(PROCESS_ERROR_INVALID_EXTERN_LABEL_NAME);
        return 0;
    }
    return add_symbol_item(symbol_table, next_word, "external", 0);
}
