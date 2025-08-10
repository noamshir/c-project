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

/* FIRST PASS */
int handle_command_line_first_pass(symbol_item **symbol_table, char *line, int line_number, unsigned int *array_of_commands, int *IC)
{
    char *command, *label, *label_name, *rest_of_line;
    int command_index;

    label = strtok(duplicate_str(line), " ");
    if (is_label_declaration(label))
    {
        label_name = get_label_name(label);
        if (!add_symbol_item(symbol_table, label_name, CODE_SYMBOL, *IC, 0, line_number))
        {
            return 0;
        }
        command = strtok(NULL, " ");
    }
    else
    {
        command = duplicate_str(label);
    }

    command = delete_white_spaces_start_and_end(command);
    command_index = get_command_index(command);
    if (command_index == -1)
    {
        print_line_error(PROCESS_ERROR_INVALID_COMMAND, line_number);
        return 0;
    }

    rest_of_line = strtok(NULL, "\n");

    switch (command_index)
    {
    case COMMAND_MOV:
    case COMMAND_CMP:
    case COMMAND_ADD:
    case COMMAND_SUB:
    case COMMAND_LEA:
        return handle_two_op_line_first_pass(command_index, line_number, rest_of_line, array_of_commands, IC);
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
        return handle_one_op_line_first_pass(command_index, line_number, rest_of_line, array_of_commands, IC);
        break;
    case COMMAND_RTS:
    case COMMAND_STOP:
        return handle_no_op_line_first_pass(command_index, line_number, rest_of_line, array_of_commands, IC);
        break;
    default:
        print_line_error(PROCESS_ERROR_INVALID_COMMAND, line_number);
        return 0;
    }
}

int handle_no_op_line_first_pass(int command_index, int line_number, char *str, unsigned int *array_of_commands, int *IC)
{
    /* ensure str is empty (no operands) */
    if (!is_empty_line(str))
    {
        print_line_error(PROCESS_ERROR_INVALID_NO_OP_LINE, line_number);
        return 0;
    }

    return handle_op_line_first_pass(command_index, line_number, NULL, NULL, array_of_commands, IC);
}

int handle_one_op_line_first_pass(int command_index, int line_number, char *str, unsigned int *array_of_commands, int *IC)
{
    char *dst;

    dst = duplicate_str(str);
    dst = delete_white_spaces_start_and_end(dst);

    if (dst == NULL)
    {
        print_line_error(PROCESS_ERROR_INVALID_DST_ALLOCATION, line_number);
        return 0;
    }

    return handle_op_line_first_pass(command_index, line_number, NULL, dst, array_of_commands, IC);
}

int handle_two_op_line_first_pass(int command_index, int line_number, char *str, unsigned int *array_of_commands, int *IC)
{
    char *src, *dst;

    src = strtok(duplicate_str(str), ",");
    src = delete_white_spaces_start_and_end(src);
    if (src == NULL)
    {
        print_line_error(PROCESS_ERROR_INVALID_SRC_ALLOCATION, line_number);
        return 0;
    }

    dst = strtok(NULL, ",");
    dst = delete_white_spaces_start_and_end(dst);
    if (dst == NULL)
    {
        print_line_error(PROCESS_ERROR_INVALID_DST_ALLOCATION, line_number);
        return 0;
    }

    return handle_op_line_first_pass(command_index, line_number, src, dst, array_of_commands, IC);
}

int handle_op_line_first_pass(int command_index, int line_number, char *src, char *dst, unsigned int *array_of_commands, int *IC)
{
    int line_binary_code;
    int src_type, dst_type, src_space, dst_space;

    src_type = get_allocation_type(src);
    if (src_type == ALLOCATION_INVALID)
    {
        print_line_error(PROCESS_ERROR_INVALID_SRC_ALLOCATION, line_number);
        return 0;
    }

    dst_type = get_allocation_type(dst);
    if (dst_type == ALLOCATION_INVALID)
    {
        print_line_error(PROCESS_ERROR_INVALID_DST_ALLOCATION, line_number);
        return 0;
    }

    if (!is_command_src_dst_valid(command_index, line_number, src_type, dst_type))
    {
        return 0;
    }

    calculate_space(src_type, dst_type, &src_space, &dst_space);

    line_binary_code = build_command_line_binary_code(command_index, src_type, dst_type);

    array_of_commands[*IC] = line_binary_code;
    (*IC)++;

    if (src_type == ALLOCATION_REGISTER && dst_type == ALLOCATION_REGISTER)
    {
        array_of_commands[*IC] = get_register_allocations_binary_code(src, dst);
        (*IC)++;
        return 1;
    }

    if (!encode_first_pass_operands(src, src_type, src_space, line_number, 1, IC, array_of_commands))
    {
        return 0;
    }

    return encode_first_pass_operands(dst, dst_type, dst_space, line_number, 0, IC, array_of_commands);
}

int encode_first_pass_operands(char *op, int type, int space, int line_number, int is_src, int *IC, unsigned int *array_of_commands)
{
    if (type == ALLOCATION_MISSING)
    {
        return 1;
    }
    else if (type == ALLOCATION_REGISTER)
    {
        array_of_commands[*IC] = get_register_allocation_binary_code(op, is_src);
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
        array_of_commands[*IC] = 0;
    }
    else if (type == ALLOCATION_IMMEDIATE)
    {
        array_of_commands[*IC] = get_immediate_allocation_binary_code(op);
    }

    (*IC) = *IC + space;

    return 1;
}

/* SECOND PASS */
int handle_command_line_second_pass(symbol_item **symbol_table, char *line, int line_number, unsigned int *array_of_commands, int *IC, char ***extern_labels, int **extern_addresses, int *extern_count)
{
    char *command, *label, *rest_of_line;
    int command_index;

    label = strtok(duplicate_str(line), " ");
    if (is_label_declaration(label))
    {
        command = strtok(NULL, " ");
    }
    else
    {
        command = duplicate_str(label);
    }

    command = delete_white_spaces_start_and_end(command);
    command_index = get_command_index(command);
    if (command_index == -1)
    {
        print_line_error(PROCESS_ERROR_INVALID_COMMAND, line_number);
        return 0;
    }

    rest_of_line = strtok(NULL, "\n");

    switch (command_index)
    {
    case COMMAND_MOV:
    case COMMAND_CMP:
    case COMMAND_ADD:
    case COMMAND_SUB:
    case COMMAND_LEA:
        return handle_two_op_line_second_pass(symbol_table, command_index, line_number, rest_of_line, array_of_commands, IC, extern_labels, extern_addresses, extern_count);
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
        return handle_one_op_line_second_pass(symbol_table, command_index, line_number, rest_of_line, array_of_commands, IC, extern_labels, extern_addresses, extern_count);
        break;
    case COMMAND_RTS:
    case COMMAND_STOP:
        return handle_no_op_line_second_pass(line_number, rest_of_line, IC);
        break;
    default:
        print_line_error(PROCESS_ERROR_INVALID_COMMAND, line_number);
        return 0;
    }
}

int handle_no_op_line_second_pass(int line_number, char *str, int *IC)
{
    /* ensure str is empty */
    if (!is_empty_line(str))
    {
        print_line_error(PROCESS_ERROR_INVALID_NO_OP_LINE, line_number);
        return 0;
    }

    /* we already encoded the command line, skip*/
    (*IC)++;

    /* nothing else to do here*/
    return 1;
}

int handle_one_op_line_second_pass(symbol_item **symbol_table, int command_index, int line_number, char *str, unsigned int *array_of_commands, int *IC, char ***extern_labels, int **extern_addresses, int *extern_count)
{
    char *dst;

    dst = duplicate_str(str);
    dst = delete_white_spaces_start_and_end(dst);

    if (dst == NULL)
    {
        print_line_error(PROCESS_ERROR_INVALID_DST_ALLOCATION, line_number);
        return 0;
    }

    return handle_op_line_second_pass(symbol_table, command_index, line_number, NULL, dst, array_of_commands, IC, extern_labels, extern_addresses, extern_count);
}

int handle_two_op_line_second_pass(symbol_item **symbol_table, int command_index, int line_number, char *str, unsigned int *array_of_commands, int *IC, char ***extern_labels, int **extern_addresses, int *extern_count)
{
    char *src, *dst;

    src = strtok(duplicate_str(str), ",");
    src = delete_white_spaces_start_and_end(src);
    if (src == NULL)
    {
        print_line_error(PROCESS_ERROR_INVALID_SRC_ALLOCATION, line_number);
        return 0;
    }

    dst = strtok(NULL, ",");
    dst = delete_white_spaces_start_and_end(dst);
    if (dst == NULL)
    {
        print_line_error(PROCESS_ERROR_INVALID_DST_ALLOCATION, line_number);
        return 0;
    }

    return handle_op_line_second_pass(symbol_table, command_index, line_number, src, dst, array_of_commands, IC, extern_labels, extern_addresses, extern_count);
}

int handle_op_line_second_pass(symbol_item **symbol_table, int command_index, int line_number, char *src, char *dst, unsigned int *array_of_commands, int *IC, char ***extern_labels, int **extern_addresses, int *extern_count)
{
    int src_type, dst_type, src_space, dst_space;

    src_type = get_allocation_type(src);
    if (src_type == ALLOCATION_INVALID)
    {
        print_line_error(PROCESS_ERROR_INVALID_SRC_ALLOCATION, line_number);
        return 0;
    }

    dst_type = get_allocation_type(dst);
    if (dst_type == ALLOCATION_INVALID)
    {
        print_line_error(PROCESS_ERROR_INVALID_DST_ALLOCATION, line_number);
        return 0;
    }

    calculate_space(src_type, dst_type, &src_space, &dst_space);

    /* we already encoded the command line, skip*/
    (*IC)++;

    /* if both are registers we already encoded, skip*/
    if (src_type == ALLOCATION_REGISTER && dst_type == ALLOCATION_REGISTER)
    {
        (*IC)++;
        /* nothing else to do here*/
        return 1;
    }

    if (!encode_second_pass_operands(src, src_type, src_space, line_number, IC, array_of_commands, symbol_table, extern_labels, extern_addresses, extern_count))
    {
        return 0;
    }

    return encode_second_pass_operands(dst, dst_type, dst_space, line_number, IC, array_of_commands, symbol_table, extern_labels, extern_addresses, extern_count);
}

int encode_second_pass_operands(char *op, int type, int space, int line_number, int *IC, unsigned int *array_of_commands, symbol_item **symbol_table, char ***extern_labels, int **extern_addresses, int *extern_count)
{
    int is_external = 0;
    char label_temp[LABEL_SIZE];
    char *label;
    symbol_item *sym;

    if (type == ALLOCATION_MISSING)
    {
        return 1;
    }
    else if (type == ALLOCATION_REGISTER || type == ALLOCATION_IMMEDIATE)
    {
        /* already handled */
    }
    else if (type == ALLOCATION_MAT || type == ALLOCATION_DIRECT)
    {
        if (type == ALLOCATION_MAT)
        {
            set_label_from_mat_allocation(op, label_temp);
            label = duplicate_str(label_temp);
        }
        else
        {
            label = duplicate_str(op);
        }
        label = delete_white_spaces_start_and_end(label);
        sym = find_symbol_item_by_name(*symbol_table, label);
        if (sym == NULL)
        {
            print_line_error(PROCESS_ERROR_LABEL_NOT_IN_SYMBOL_TABLE, line_number);
            return 0;
        }

        if (strcmp(sym->type, EXTERN_SYMBOL) == 0)
        {
            is_external = 1;
        }

        if (is_external)
        {
            array_of_commands[*IC] = convert_num_to_8_bits(sym->address, EXTERNAL_CODE_ARE);
            *extern_labels = realloc(*extern_labels, sizeof(char *) * (*extern_count + 1));
            *extern_addresses = realloc(*extern_addresses, sizeof(int) * (*extern_count + 1));
            (*extern_labels)[*extern_count] = duplicate_str(sym->name);
            (*extern_addresses)[*extern_count] = *IC + MEMORY_START_ADDRESS;
            (*extern_count)++;
        }
        else
        {
            array_of_commands[*IC] = convert_num_to_8_bits(sym->address, RELOCATABLE_CODE_ARE);
        }
    }

    (*IC) = *IC + space;

    return 1;
}

/* HELPERS */
int calculate_space(int src_type, int dst_type, int *src_space, int *dst_space)
{
    int L = 1;

    if (src_type == ALLOCATION_REGISTER && dst_type == ALLOCATION_REGISTER)
    {
        return L + 1;
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

int is_command_src_dst_valid(int command_index, int line_number, int src_type, int dst_type)
{
    int invalid = 0;

    /* check src */
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
        print_line_error(PROCESS_ERROR_INVALID_SRC_ALLOCATION, line_number);
        return 0;
    }

    invalid = 0;
    /* check dst */
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
        print_line_error(PROCESS_ERROR_INVALID_DST_ALLOCATION, line_number);
        return 0;
    }

    return 1;
}
