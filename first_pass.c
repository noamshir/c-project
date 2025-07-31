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
    int IC = 0, DC = 0, line_num = 0;
    char *temp_file_name, *file_name, line[LINE_SIZE], *word, *main_op, *array_of_operations = NULL, *array_of_data = NULL;
    FILE *file;
    symbol_item *symbol_table = NULL;

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
            handle_guide_line(&symbol_table, line, array_of_data, &DC);
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
            /* not all the above? must be an operation line */
            handle_operation_line(&symbol_table, line, array_of_operations, &IC);
        }
    }

    update_data_labels(symbol_table, IC);
}

void handle_guide_line(symbol_item **symbol_table, char *line, char *array_of_data, int *DC)
{
    char *guide, *label;

    label = strtok(strdup(line), " ");
    delete_white_spaces(label);
    if (is_label(label))
    {
        add_label_to_symbol_table(symbol_table, label, "data", *DC);
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

void handle_data_guide(symbol_item **symbol_table, char *line, char *array_of_data, int *DC)
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
        array_of_data = realloc(array_of_data, (*DC + 1) * BINARY_CODE_SIZE);
        if (array_of_data == NULL)
        {
            printf("error code is %d\n", PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
            return;
        }
        array_of_data[*DC] = convert_num_to_10_bits(num);
        i++;
        (*DC)++;
    }
}

void handle_string_guide(symbol_item **symbol_table, char *line, char *array_of_data, int *DC)
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
        array_of_data = realloc(array_of_data, (*DC + 1) * sizeof(int));
        if (array_of_data == NULL)
        {
            printf("error code is %d\n", PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
            return;
        }
        printf("asci val: %d \n", (int)c);
        array_of_data[*DC] = convert_num_to_10_bits((int)c);
        (*DC)++;
    }

    // add end of string char
    array_of_data[*DC] = convert_num_to_10_bits((int)'\0');
    (*DC)++;
}

void handle_mat_guide(symbol_item **symbol_table, char *array_of_data, char *line, int *DC)
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
        array_of_data = realloc(array_of_data, (*DC + 1) * sizeof(int));
        if (array_of_data == NULL)
        {
            printf("error code is %d\n, ", PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
            return;
        }
        printf("num: %d\n", num);
        array_of_data[*DC] = convert_num_to_10_bits(num);
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

void handle_operation_line(symbol_item **symbol_table, char *line, char *array_of_operations, int *IC)
{
    char *op, *label, *rest_of_line;
    int op_index;

    label = strtok(strdup(line), " ");
    delete_white_spaces(label);
    if (is_label(label))
    {
        add_label_to_symbol_table(symbol_table, label, "code", *IC);
        op = strtok(NULL, " ");
        delete_white_spaces(op);
    }
    else
    {
        op = strdup(label);
    }

    op_index = get_operation_index(op);
    if (op_index == -1)
    {
        printf("error code is %d\n", PROCESS_ERROR_INVALID_OPERATION);
        return;
    }

    rest_of_line = strtok(NULL, "\n");
    printf("rest of line: %s\n", rest_of_line);

    switch (op_index)
    {
    case OPERATION_MOV:
    case OPERATION_CMP:
    case OPERATION_ADD:
    case OPERATION_SUB:
    case OPERATION_LEA:
        handle_two_op_line(op_index, rest_of_line, array_of_operations, IC);
        break;
    case OPERATION_CLR:
    case OPERATION_NOT:
    case OPERATION_INC:
    case OPERATION_DEC:
    case OPERATION_JMP:
    case OPERATION_BNE:
    case OPERATION_JSR:
    case OPERATION_RED:
    case OPERATION_PRN:
        handle_one_op_line(op_index, rest_of_line, array_of_operations, IC);
        break;
    case OPERATION_RTS:
    case OPERATION_STOP:
        handle_no_op_line(op_index, rest_of_line, array_of_operations, IC);
        break;
    }
}

void handle_no_op_line(int op_index, char *str, char *array_of_operations, int *IC)
{
    // ensure str is empty
    if (str != NULL && !is_empty_line(str))
    {
        printf("error code is %d\n", PROCESS_ERROR_INVALID_NO_OP_LINE);
        return;
    }

    return handle_op(op_index, str, NULL, NULL, array_of_operations, IC);
}

void handle_one_op_line(int op_index, char *str, char *array_of_operations, int *IC)
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
    handle_op(op_index, str, NULL, dst, array_of_operations, IC);
    return;
}

void handle_two_op_line(int op_index, char *str, char *array_of_operations, int *IC)
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
    handle_op(op_index, str, src, dst, array_of_operations, IC);
}

void handle_op(int op_index, char *str, char *src, char *dst, char *array_of_operations, int *IC)
{
    char *line_binary_code, *src_binary_code, *dst_binary_code;
    int src_type, dst_type, src_space, dst_space, L = 0;

    src_type = get_allocation_type(src);
    printf("src type: %d\n", src_type);
    dst_type = get_allocation_type(dst);
    printf("dst type: %d\n", dst_type);

    if (!is_op_src_dst_valid(op_index, src_type, dst_type))
    {
        return;
    }

    L = calculate_space(src_type, dst_type, &src_space, &dst_space);
    printf("Space size for line: %d\n", L);

    line_binary_code = malloc(BINARY_CODE_SIZE);
    add_operation_line_binary_code(line_binary_code, str, op_index, src_type, dst_type, array_of_operations, IC);
    printf("line binary code: %s\n", line_binary_code);

    array_of_operations = realloc(array_of_operations, (*IC + L) * BINARY_CODE_SIZE);
    array_of_operations[*IC] = strdup(line_binary_code);
    free(line_binary_code);
    (*IC)++;

    if (src_type == ALLOCATION_REGISTER && dst_type == ALLOCATION_REGISTER)
    {
        array_of_operations[*IC] = get_register_allocations_binary_code(src, dst);
        (*IC)++;
        return;
    }

    if (src_type == ALLOCATION_REGISTER)
    {
        array_of_operations[*IC] = get_register_allocation_binary_code(src);
    }
    else if (src_type == ALLOCATION_MAT)
    {
        set_first_pass_mat_allocation_binary_code(src, array_of_operations, *IC);
    }
    else if (src_type == ALLOCATION_DIRECT)
    {
        array_of_operations[*IC] = NULL;
    }
    else if (src_type == ALLOCATION_IMMEDIATE)
    {
        array_of_operations[*IC] = get_direct_allocation_binary_code(src);
    }

    (*IC) = *IC + src_space;

    if (dst_type == ALLOCATION_REGISTER)
    {
        array_of_operations[*IC] = get_register_allocation_binary_code(dst);
    }
    else if (dst_type == ALLOCATION_MAT)
    {
        set_first_pass_mat_allocation_binary_code(dst, array_of_operations, *IC);
    }
    else if (dst_type == ALLOCATION_DIRECT)
    {
        array_of_operations[*IC] = NULL;
    }
    else if (dst_type == ALLOCATION_IMMEDIATE)
    {
        array_of_operations[*IC] = get_direct_allocation_binary_code(dst);
    }

    (*IC) = *IC + dst_space;
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

int is_op_src_dst_valid(int op_index, int src_type, int dst_type)
{
    int invalid = 0;
    printf("op index: %d, src type: %d, dst type: %d\n", op_index, src_type, dst_type);

    // check src
    switch (op_index)
    {
    case OPERATION_MOV:
    case OPERATION_CMP:
    case OPERATION_ADD:
    case OPERATION_SUB:
        if (src_type != ALLOCATION_IMMEDIATE && src_type != ALLOCATION_DIRECT && src_type != ALLOCATION_MAT && src_type != ALLOCATION_REGISTER)
        {
            invalid = 1;
        }
        break;
    case OPERATION_LEA:
        if (src_type != ALLOCATION_DIRECT && src_type != ALLOCATION_MAT)
        {
            invalid = 1;
        }
        break;
    case OPERATION_CLR:
    case OPERATION_NOT:
    case OPERATION_INC:
    case OPERATION_DEC:
    case OPERATION_JMP:
    case OPERATION_BNE:
    case OPERATION_JSR:
    case OPERATION_RED:
    case OPERATION_PRN:
    case OPERATION_RTS:
    case OPERATION_STOP:
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
    switch (op_index)
    {
    case OPERATION_MOV:
    case OPERATION_ADD:
    case OPERATION_SUB:
    case OPERATION_LEA:
    case OPERATION_CLR:
    case OPERATION_NOT:
    case OPERATION_INC:
    case OPERATION_DEC:
    case OPERATION_JMP:
    case OPERATION_BNE:
    case OPERATION_JSR:
    case OPERATION_RED:
        if (dst_type != ALLOCATION_DIRECT && dst_type != ALLOCATION_MAT && dst_type != ALLOCATION_REGISTER)
        {
            invalid = 1;
        }
        break;
    case OPERATION_PRN:
    case OPERATION_CMP:
        if (dst_type != ALLOCATION_IMMEDIATE && dst_type != ALLOCATION_DIRECT && dst_type != ALLOCATION_MAT && dst_type != ALLOCATION_REGISTER)
        {
            invalid = 1;
        }
        break;
    case OPERATION_RTS:
    case OPERATION_STOP:
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
