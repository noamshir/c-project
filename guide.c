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
    if (is_label(opening_word))
    {
        label_name = get_label_name(opening_word);
        if (!add_symbol_item(symbol_table, label_name, "data", *DC))
        {
            return 0;
        }

        guide = strtok(NULL, " ");
    }
    else
    {
        guide = strdup(opening_word);
    }

    guide_declaration = strtok(NULL, "\n");
    printf("guide declaration: %s\n", guide_declaration);

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
        return 0;
    }
}

int handle_data_guide(char *guide_declaration, char ***array_of_data, int *DC)
{
    // check that guide_declaration is of type: "num1, num2, ...., numn"
    char *str, *temp;
    int num;
    int i = 0;

    temp = strdup(guide_declaration);
    if (!is_data_guide_declaration(temp))
    {
        print_error(PROCESS_ERROR_DATA_GUIDE_INVALID_PARAM);
        return 0;
    }

    // get all the numbers in guide_declaration, increase DC and enter them to the array of data
    while (1)
    {
        if (i > 0)
        {
            str = strtok(NULL, ",");
        }
        else
        {
            str = strtok(temp, ",");
        }

        str = delete_white_spaces_start_and_end(str);
        printf("next word: %s\n", str);

        if (str == NULL)
        {
            break;
        }

        // should check if num is integer
        if (!is_integer(str))
        {
            print_error(PROCESS_ERROR_DATA_GUIDE_INVALID_PARAM);
            return 0;
        }

        num = atoi(str);
        // should check if num is integer
        printf("data num: %d\n", num);

        // allocate memo to array of data
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
    // check that line is of type: "label: .string "string""
    int i = 0;
    char *temp, c;

    temp = strdup(guide_declaration);

    if (temp == NULL)
    {
        print_error(PROCESS_ERROR_STRING_GUIDE_INVALID_PARAM);
        return 0;
    }

    // here we cant just delete white spaces as the "string" may include spaces!
    temp = delete_white_spaces_start_and_end(temp);
    printf("guide declaration after deleting white spaces at start and end: %s\n", temp);

    if (temp[0] != '\"' || temp[strlen(temp) - 1] != '\"')
    {
        printf("first is %c, last is %c\n", temp[0], temp[strlen(temp) - 1]);
        print_error(PROCESS_ERROR_STRING_GUIDE_INVALID_PARAM);
        return 0;
    }

    // insert each char asci value to array of data and increment dc until end of quate
    for (i = 1; i < strlen(temp) - 1; i++)
    {
        c = temp[i];
        printf("c: %c\n", c);
        // allocate memo to array of data
        *array_of_data = realloc(*array_of_data, (*DC + 1) * sizeof(char *));
        if (*array_of_data == NULL)
        {
            safe_exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
        }
        printf("asci val: %d \n", (int)c);
        (*array_of_data)[*DC] = convert_num_to_10_bits((int)c);
        (*DC)++;
    }

    *array_of_data = realloc(*array_of_data, (*DC + 1) * sizeof(char *));
    if (*array_of_data == NULL)
    {
        safe_exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
    }

    // add end of string char
    (*array_of_data)[*DC] = convert_num_to_10_bits((int)'\0');
    (*DC)++;

    return 1;
}

int handle_mat_guide(char *guide_declaration, char ***array_of_data, int *DC)
{
    // check that line is of type: "label: .mat [num1][num2] optional numbers seperated by comma
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

    // check that guide_declaration is [num1][num2]
    if (!is_mat_declaration(temp))
    {
        print_error(PROCESS_ERROR_MAT_GUIDE_INVALID_PARAM);
        return 0;
    }

    if (!set_rows_and_cols_from_mat_declaration(temp, &rows, &cols))
    {
        print_error(PROCESS_ERROR_MAT_GUIDE_INVALID_PARAM);
        return 0;
    }
    printf("rows: %d, cols: %d\n", rows, cols);

    // now str is num1, num2...
    // if not set it will be empty.
    str = strtok(temp, " ");
    for (i = 0; i < (rows * cols); i++)
    {
        str = strtok(NULL, ",");
        str = delete_white_spaces_start_and_end(str);
        printf("next word: %s\n", str);

        if (str == NULL)
        {
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

        printf("num: %d\n", num);
        (*array_of_data)[*DC] = convert_num_to_10_bits(num);
        (*DC)++;
    }

    str = strtok(NULL, ",");
    printf("next word: %s\n", str);
    if (str != NULL)
    {
        print_error(PROCESS_ERROR_MAT_GUIDE_INVALID_PARAM);
        return 0;
    }

    return 1;
}

int handle_extern(symbol_item **symbol_table, char *line)
{
    // check that line is of type: "label: .extern operand"
    char *word, *next_word;

    word = strtok(strdup(line), " ");
    if (is_label(word))
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

    next_word = delete_white_spaces_start_and_end(next_word);
    printf("next word: %s\n", next_word);

    if (!is_valid_label_name(next_word))
    {
        print_error(PROCESS_ERROR_INVALID_EXTERN_LABEL_NAME);
        return 0;
    }

    return add_symbol_item(symbol_table, next_word, "external", 0);
}
