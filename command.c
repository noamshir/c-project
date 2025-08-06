#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Headers/string.h"
#include "Headers/error.h"
#include "Headers/symbol_table.h"
#include "Headers/utils.h"
#include "Headers/consts.h"
#include "Headers/binary_code.h"
#include "Headers/command.h"

int handle_command_line(symbol_item **symbol_table, char *line, char ***array_of_commands, int *IC)
{
    char *command, *label, *label_name, *rest_of_line;
    int command_index;

    label = strtok(strdup(line), " ");
    if (is_label(label))
    {
        label_name = get_label_name(label);
        if (!add_symbol_item(symbol_table, label_name, "code", *IC))
        {
            return 0;
        }
        command = strtok(NULL, " ");
    }
    else
    {
        command = strdup(label);
    }

    command = delete_white_spaces_start_and_end(command);
    command_index = get_command_index(command);
    if (command_index == -1)
    {
        print_error(PROCESS_ERROR_INVALID_COMMAND);
        return 0;
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
        return handle_two_op_line(command_index, rest_of_line, array_of_commands, IC);
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
        return handle_one_op_line(command_index, rest_of_line, array_of_commands, IC);
        break;
    case COMMAND_RTS:
    case COMMAND_STOP:
        return handle_no_op_line(command_index, rest_of_line, array_of_commands, IC);
        break;
    default:
        print_error(PROCESS_ERROR_INVALID_COMMAND);
        return 0;
    }
}

int handle_no_op_line(int command_index, char *str, char ***array_of_commands, int *IC)
{
    // ensure str is empty
    if (str != NULL && !is_empty_line(str))
    {
        print_error(PROCESS_ERROR_INVALID_NO_OP_LINE);
        return 0;
    }

    return handle_op_line(command_index, str, NULL, NULL, array_of_commands, IC);
}

int handle_one_op_line(int command_index, char *str, char ***array_of_commands, int *IC)
{
    char *dst;

    dst = strdup(str);
    dst = delete_white_spaces_start_and_end(dst);
    printf("dst: %s\n", dst);

    if (dst == NULL)
    {
        print_error(PROCESS_ERROR_INVALID_DST_ALLOCATION);
        return 0;
    }

    return handle_op_line(command_index, str, NULL, dst, array_of_commands, IC);
}

int handle_two_op_line(int command_index, char *str, char ***array_of_commands, int *IC)
{
    char *src, *dst;

    src = strtok(strdup(str), ",");
    src = delete_white_spaces_start_and_end(src);
    if (src == NULL)
    {
        print_error(PROCESS_ERROR_INVALID_SRC_ALLOCATION);
        return 0;
    }

    dst = strtok(NULL, ",");
    dst = delete_white_spaces_start_and_end(dst);
    if (dst == NULL)
    {
        print_error(PROCESS_ERROR_INVALID_DST_ALLOCATION);
        return 0;
    }

    printf("src: %s, dst: %s\n", src, dst);
    return handle_op_line(command_index, str, src, dst, array_of_commands, IC);
}

int handle_op_line(int command_index, char *str, char *src, char *dst, char ***array_of_commands, int *IC)
{
    char *line_binary_code;
    int src_type, dst_type, src_space, dst_space, L = 0;

    src_type = get_allocation_type(src);
    printf("src type: %d\n", src_type);
    if (src_type == ALLOCATION_INVALID)
    {
        print_error(PROCESS_ERROR_INVALID_SRC_ALLOCATION);
        return 0;
    }

    dst_type = get_allocation_type(dst);
    printf("dst type: %d\n", dst_type);
    if (dst_type == ALLOCATION_INVALID)
    {
        print_error(PROCESS_ERROR_INVALID_DST_ALLOCATION);
        return 0;
    }

    if (!is_command_src_dst_valid(command_index, src_type, dst_type))
    {
        return 0;
    }

    L = calculate_space(src_type, dst_type, &src_space, &dst_space);
    printf("Space size for line: %d\n", L);

    line_binary_code = malloc(BINARY_CODE_SIZE);
    if (line_binary_code == NULL)
    {
        safe_exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
    }

    add_command_line_binary_code(line_binary_code, str, command_index, src_type, dst_type);
    printf("line binary code: %s\n", line_binary_code);

    *array_of_commands = realloc(*array_of_commands, (*IC + L) * sizeof(char *));
    if (*array_of_commands == NULL)
    {
        safe_exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
    }

    (*array_of_commands)[*IC] = malloc(BINARY_CODE_SIZE);
    if ((*array_of_commands)[*IC] == NULL)
    {
        safe_exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
    }

    (*array_of_commands)[*IC] = strdup(line_binary_code);
    free(line_binary_code);
    (*IC)++;

    printf("handling: operands src: %s, dst: %s\n", src, dst);
    if (src_type == ALLOCATION_REGISTER && dst_type == ALLOCATION_REGISTER)
    {
        (*array_of_commands)[*IC] = get_register_allocations_binary_code(src, dst);
        (*IC)++;
        return 1;
    }

    if (!encode_first_pass_operands(src, src_type, src_space, IC, array_of_commands))
    {
        return 0;
    }

    return encode_first_pass_operands(dst, dst_type, dst_space, IC, array_of_commands);
}

int encode_first_pass_operands(char *op, int type, int space, int *IC, char ***array_of_commands)
{
    if (type == ALLOCATION_MISSING)
    {
        return 1;
    }
    else if (type == ALLOCATION_REGISTER)
    {
        (*array_of_commands)[*IC] = get_register_allocation_binary_code(op);
    }
    else if (type == ALLOCATION_MAT)
    {
        if (!set_first_pass_mat_allocation_binary_code(op, array_of_commands, *IC))
        {
            return 0;
        }
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

    return 1;
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
        print_error(PROCESS_ERROR_INVALID_SRC_ALLOCATION);
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
        print_error(PROCESS_ERROR_INVALID_DST_ALLOCATION);
        return 0;
    }

    return 1;
}
