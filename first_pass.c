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
#include "Headers/binary_code.h"

void first_pass(char *file_name_without_postfix)
{
    int IC = 0, DC = 0, line_num = 0, i = 0;
    char *temp_file_name, *file_name, line[LINE_SIZE], *word, *main_op, **array_of_commands = NULL, **array_of_data = NULL;
    FILE *file;
    symbol_item *symbol_table = NULL;

    array_of_commands = malloc(sizeof(char *));
    array_of_data = malloc(sizeof(char *));

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

        word = strtok(strdup(line), " ");
        delete_white_spaces(word);
        if (is_label(word))
        {
            main_op = strtok(NULL, " ");
            delete_white_spaces(main_op);
        }
        else
        {
            main_op = strdup(word);
        }

        if (is_data_guide(main_op) || is_string_guide(main_op) || is_mat_guide(main_op))
        {
            handle_guide_line(&symbol_table, line, &array_of_data, &DC);
        }
        else if (is_extern_guide(main_op))
        {
            handle_extern(&symbol_table, line);
        }
        else if (is_entry_guide(main_op))
        {
            /* will deal at second pass */
            continue;
        }
        else
        {
            /* not all the above? must be an command line */
            handle_command_line(&symbol_table, line, &array_of_commands, &IC);
        }
    }

    // loop data array and prints it values
    printf("data array (%d):\n", DC);
    for (i = 0; i < DC; i++)
    {
        printf("data: %s\n", array_of_data[i]);
    }

    // loop commands array and prints it values
    printf("commands array (%d):\n", IC);
    for (i = 0; i < IC; i++)
    {
        printf("command: %s\n", array_of_commands[i]);
    }

    update_data_labels(symbol_table, IC);

    free(file_name);
    free(array_of_commands);
    free(array_of_data);
    free(symbol_table);
    fclose(file);

    printf("first pass finished\n");
}

void handle_guide_line(symbol_item **symbol_table, char *line, char ***array_of_data, int *DC)
{
    char *guide, *label, *label_name;

    label = strtok(strdup(line), " ");
    delete_white_spaces(label);
    if (is_label(label))
    {
        label_name = get_label_name(label);
        add_label_to_symbol_table(symbol_table, label_name, "data", *DC);
        guide = strtok(NULL, " ");
        delete_white_spaces(guide);
    }
    else
    {
        guide = strdup(label);
    }

    if (is_data_guide(guide))
    {
        handle_data_guide(symbol_table, line, array_of_data, DC);
    }
    else if (is_string_guide(guide))
    {
        handle_string_guide(symbol_table, line, array_of_data, DC);
    }
    else if (is_mat_guide(guide))
    {
        handle_mat_guide(symbol_table, array_of_data, line, DC);
    }
}

void handle_data_guide(symbol_item **symbol_table, char *line, char ***array_of_data, int *DC)
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
        *array_of_data = realloc(*array_of_data, (*DC + 1) * sizeof(char *));
        if (*array_of_data == NULL)
        {
            printf("error code is %d\n", PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
            return;
        }
        (*array_of_data)[*DC] = convert_num_to_10_bits(num);
        printf("here is data: %s\n", (*array_of_data)[*DC]);
        i++;
        (*DC)++;
    }

    // loop data array and prints it values
    printf("data array (%d):\n", *DC);
    for (i = 0; i < *DC; i++)
    {
        printf("here is data: %s\n", (*array_of_data)[i]);
    }
}

void handle_string_guide(symbol_item **symbol_table, char *line, char ***array_of_data, int *DC)
{
    // check that line is of type: "label: .string "string""
    int i = 0;
    char *word, *rest_str, c;
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

    rest_str = strtok(NULL, "\n");
    if (rest_str == NULL)
    {
        printf("error code is %d\n", PROCESS_ERROR_INVALID_MACRO_DECLARATION);
        return;
    }

    if (rest_str[0] != '\"' || rest_str[strlen(rest_str) - 1] != '\"')
    {
        printf("first is %c, last is %c\n", rest_str[0], rest_str[strlen(rest_str) - 1]);
        printf("error code is %d\n", PROCESS_ERROR_INVALID_MACRO_DECLARATION);
        return;
    }

    // insert each char asci value to array of data and increment dc until end of quate
    for (i = 1; i < strlen(rest_str) - 1; i++)
    {
        c = rest_str[i];
        printf("c: %c\n", c);
        // allocate memo to array of data
        *array_of_data = realloc(*array_of_data, (*DC + 1) * sizeof(char *));
        if (*array_of_data == NULL)
        {
            printf("error code is %d\n", PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
            return;
        }
        printf("asci val: %d \n", (int)c);
        (*array_of_data)[*DC] = convert_num_to_10_bits((int)c);
        (*DC)++;
    }

    *array_of_data = realloc(*array_of_data, (*DC + 1) * sizeof(char *));
    // add end of string char
    (*array_of_data)[*DC] = convert_num_to_10_bits((int)'\0');
    (*DC)++;
}

void handle_mat_guide(symbol_item **symbol_table, char ***array_of_data, char *line, int *DC)
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
    if (!is_mat_declaration(next_word))
    {
        printf("error code is %d\n, ", PROCESS_ERROR_INVALID_MACRO_DECLARATION);
        return;
    }

    if (!set_rows_and_cols_from_mat_declaration(next_word, &rows, &cols))
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
        *array_of_data = realloc(*array_of_data, (*DC + 1) * sizeof(char *));
        if (*array_of_data == NULL)
        {
            printf("error code is %d\n, ", PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
            return;
        }
        printf("num: %d\n", num);
        (*array_of_data)[*DC] = convert_num_to_10_bits(num);
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

void handle_command_line(symbol_item **symbol_table, char *line, char ***array_of_commands, int *IC)
{
    char *command, *label, *label_name, *rest_of_line;
    int command_index;

    label = strtok(strdup(line), " ");
    delete_white_spaces(label);
    if (is_label(label))
    {
        label_name = get_label_name(label);
        add_label_to_symbol_table(symbol_table, label_name, "code", *IC);
        command = strtok(NULL, " ");
        delete_white_spaces(command);
    }
    else
    {
        command = strdup(label);
    }

    command_index = get_command_index(command);
    if (command_index == -1)
    {
        printf("error code is %d\n", PROCESS_ERROR_INVALID_COMMAND);
        return;
    }

    rest_of_line = strtok(NULL, "\n");
    printf("rest of line: %s\n", rest_of_line);

    switch (command_index)
    {
    case COMMAND_MOV:
    case COMMAND_CMP:
    case COMMAND_ADD:
    case COMMAND_SUB:
    case COMMAND_LEA:
        handle_two_op_line(command_index, rest_of_line, array_of_commands, IC);
        break;
    case COMMAND_CLR:
    case COMMAND_NOT:
    case COMMAND_INC:
    case COMMAND_DEC:
    case COMMAND_JMP:
    case COMMAND_BNE:
    case COMMAND_JSR:
    case COMMAND_RED:
    case COMMAND_PRN:
        handle_one_op_line(command_index, rest_of_line, array_of_commands, IC);
        break;
    case COMMAND_RTS:
    case COMMAND_STOP:
        handle_no_op_line(command_index, rest_of_line, array_of_commands, IC);
        break;
    }
}

void handle_no_op_line(int command_index, char *str, char ***array_of_commands, int *IC)
{
    // ensure str is empty
    if (str != NULL && !is_empty_line(str))
    {
        printf("error code is %d\n", PROCESS_ERROR_INVALID_NO_OP_LINE);
        return;
    }

    return handle_op_line(command_index, str, NULL, NULL, array_of_commands, IC);
}

void handle_one_op_line(int command_index, char *str, char ***array_of_commands, int *IC)
{
    char *dst, *line_binary_code, *src_binary_code, *dst_binary_code;
    int src_type, dst_type, src_space, dst_space, L = 0, i = 0;

    dst = strdup(str);
    if (dst == NULL)
    {
        printf("error code is %d\n", PROCESS_ERROR_INVALID_DST_ALLOCATION);
        return;
    }
    delete_white_spaces(dst);
    printf("dst: %s\n", dst);
    handle_op_line(command_index, str, NULL, dst, array_of_commands, IC);
    return;
}

void handle_two_op_line(int command_index, char *str, char ***array_of_commands, int *IC)
{
    char *src, *dst, *line_binary_code, *src_binary_code, *dst_binary_code;
    int src_type, dst_type, src_space, dst_space, L = 0, i = 0;

    src = strtok(strdup(str), ",");
    if (src == NULL)
    {
        printf("error code is %d\n", PROCESS_ERROR_INVALID_SRC_ALLOCATION);
        return;
    }
    delete_white_spaces(src);

    dst = strtok(NULL, ",");
    if (dst == NULL)
    {
        printf("error code is %d\n", PROCESS_ERROR_INVALID_DST_ALLOCATION);
        return;
    }
    delete_white_spaces(dst);
    printf("src: %s, dst: %s\n", src, dst);
    handle_op_line(command_index, str, src, dst, array_of_commands, IC);
}

void handle_op_line(int command_index, char *str, char *src, char *dst, char ***array_of_commands, int *IC)
{
    char *line_binary_code, *src_binary_code, *dst_binary_code;
    int src_type, dst_type, src_space, dst_space, L = 0;

    src_type = get_allocation_type(src);
    printf("src type: %d\n", src_type);
    dst_type = get_allocation_type(dst);
    printf("dst type: %d\n", dst_type);

    if (!is_command_src_dst_valid(command_index, src_type, dst_type))
    {
        return;
    }

    L = calculate_space(src_type, dst_type, &src_space, &dst_space);
    printf("Space size for line: %d\n", L);

    line_binary_code = malloc(BINARY_CODE_SIZE);
    add_command_line_binary_code(line_binary_code, str, command_index, src_type, dst_type);
    printf("line binary code: %s\n", line_binary_code);

    *array_of_commands = realloc(*array_of_commands, (*IC + L) * sizeof(char *));
    (*array_of_commands)[*IC] = malloc(BINARY_CODE_SIZE);
    (*array_of_commands)[*IC] = strdup(line_binary_code);
    free(line_binary_code);
    (*IC)++;

    printf("handling: operands src: %s, dst: %s\n", src, dst);
    if (src_type == ALLOCATION_REGISTER && dst_type == ALLOCATION_REGISTER)
    {
        (*array_of_commands)[*IC] = get_register_allocations_binary_code(src, dst);
        (*IC)++;
        return;
    }

    encode_first_pass_operands(src, src_type, src_space, IC, array_of_commands);
    encode_first_pass_operands(dst, dst_type, dst_space, IC, array_of_commands);
}

int encode_first_pass_operands(char *op, int type, int space, int *IC, char ***array_of_commands)
{
    if (type == ALLOCATION_REGISTER)
    {
        (*array_of_commands)[*IC] = get_register_allocation_binary_code(op);
    }
    else if (type == ALLOCATION_MAT)
    {
        set_first_pass_mat_allocation_binary_code(op, array_of_commands, *IC);
    }
    else if (type == ALLOCATION_DIRECT)
    {
        (*array_of_commands)[*IC] = NULL;
    }
    else if (type == ALLOCATION_IMMEDIATE)
    {
        (*array_of_commands)[*IC] = get_direct_allocation_binary_code(op);
    }

    (*IC) = *IC + space;
}

int calculate_space(int src_type, int dst_type, int *src_space, int *dst_space)
{
    int L = 1;

    if (src_type == ALLOCATION_REGISTER && dst_type == ALLOCATION_REGISTER)
    {
        return L;
    }

    /* handle src */
    if (src_type == ALLOCATION_REGISTER || src_type == ALLOCATION_IMMEDIATE || src_type == ALLOCATION_DIRECT)
    {
        *src_space = 1;
        L++;
    }

    if (src_type == ALLOCATION_MAT)
    {
        *src_space = 2;
        L = L + 2;
    }

    /* handle dst */
    if (dst_type == ALLOCATION_REGISTER || dst_type == ALLOCATION_IMMEDIATE || dst_type == ALLOCATION_DIRECT)
    {
        *dst_space = 1;
        L++;
    }

    if (dst_type == ALLOCATION_MAT)
    {
        *dst_space = 2;
        L = L + 2;
    }

    return L;
}

int is_command_src_dst_valid(int command_index, int src_type, int dst_type)
{
    int invalid = 0;
    printf("command index: %d, src type: %d, dst type: %d\n", command_index, src_type, dst_type);

    // check src
    switch (command_index)
    {
    case COMMAND_MOV:
    case COMMAND_CMP:
    case COMMAND_ADD:
    case COMMAND_SUB:
        if (src_type != ALLOCATION_IMMEDIATE && src_type != ALLOCATION_DIRECT && src_type != ALLOCATION_MAT && src_type != ALLOCATION_REGISTER)
        {
            invalid = 1;
        }
        break;
    case COMMAND_LEA:
        if (src_type != ALLOCATION_DIRECT && src_type != ALLOCATION_MAT)
        {
            invalid = 1;
        }
        break;
    case COMMAND_CLR:
    case COMMAND_NOT:
    case COMMAND_INC:
    case COMMAND_DEC:
    case COMMAND_JMP:
    case COMMAND_BNE:
    case COMMAND_JSR:
    case COMMAND_RED:
    case COMMAND_PRN:
    case COMMAND_RTS:
    case COMMAND_STOP:
        if (src_type != ALLOCATION_MISSING)
        {
            invalid = 1;
        }
    }

    if (invalid)
    {
        printf("error code is %d\n", PROCESS_ERROR_INVALID_SRC_ALLOCATION);
        return 0;
    }

    invalid = 0;
    // check dst
    switch (command_index)
    {
    case COMMAND_MOV:
    case COMMAND_ADD:
    case COMMAND_SUB:
    case COMMAND_LEA:
    case COMMAND_CLR:
    case COMMAND_NOT:
    case COMMAND_INC:
    case COMMAND_DEC:
    case COMMAND_JMP:
    case COMMAND_BNE:
    case COMMAND_JSR:
    case COMMAND_RED:
        if (dst_type != ALLOCATION_DIRECT && dst_type != ALLOCATION_MAT && dst_type != ALLOCATION_REGISTER)
        {
            invalid = 1;
        }
        break;
    case COMMAND_PRN:
    case COMMAND_CMP:
        if (dst_type != ALLOCATION_IMMEDIATE && dst_type != ALLOCATION_DIRECT && dst_type != ALLOCATION_MAT && dst_type != ALLOCATION_REGISTER)
        {
            invalid = 1;
        }
        break;
    case COMMAND_RTS:
    case COMMAND_STOP:
        if (dst_type != ALLOCATION_MISSING)
        {
            invalid = 1;
        }
        break;
    }

    if (invalid)
    {
        printf("error code is %d\n", PROCESS_ERROR_INVALID_DST_ALLOCATION);
        return 0;
    }

    return 1;
}
