#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Headers/string.h"
#include "Headers/error.h"
#include "Headers/consts.h"
#include "Headers/binary_code.h"
#include "Headers/utils.h"
#include "Headers/symbol_table.h"

char *build_command_line_binary_code(int command_index, int src_type, int dst_type)
{
    char *binary_code, *command_code, *ARE_code, *src_type_code, *dst_type_code;

    /* get the binary code separated parts */
    command_code = get_command_binary_code(command_index);
    src_type_code = get_allocation_type_binary_code(src_type);
    dst_type_code = get_allocation_type_binary_code(dst_type);
    ARE_code = get_ARE_binary_code(ABSOLUTE_CODE);

    binary_code = malloc(BINARY_CODE_SIZE);
    if (binary_code == NULL)
    {
        safe_exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
    }

    /* concat all the parts to a command line binary code */
    strcpy(binary_code, command_code);
    strcat(binary_code, src_type_code);
    strcat(binary_code, dst_type_code);
    strcat(binary_code, ARE_code);

    return binary_code;
}

char *get_command_binary_code(int command_index)
{
    switch (command_index)
    {
    case COMMAND_MOV:
        return "0000";
    case COMMAND_CMP:
        return "0001";
    case COMMAND_ADD:
        return "0010";
    case COMMAND_SUB:
        return "0011";
    case COMMAND_LEA:
        return "0100";
    case COMMAND_CLR:
        return "0101";
    case COMMAND_NOT:
        return "0110";
    case COMMAND_INC:
        return "0111";
    case COMMAND_DEC:
        return "1000";
    case COMMAND_JMP:
        return "1001";
    case COMMAND_BNE:
        return "1010";
    case COMMAND_JSR:
        return "1011";
    case COMMAND_RED:
        return "1100";
    case COMMAND_PRN:
        return "1101";
    case COMMAND_RTS:
        return "1110";
    case COMMAND_STOP:
        return "1111";
    }

    return NULL;
}

char *get_ARE_binary_code(int ARE_type)
{
    switch (ARE_type)
    {
    case ABSOLUTE_CODE:
        return "00";
    case EXTERNAL_CODE:
        return "01";
    case RELOCATABLE_CODE:
        return "10";
    default:
        return NULL;
    }
}

char *get_allocation_type_binary_code(int allocation_type)
{
    switch (allocation_type)
    {
    case ALLOCATION_MISSING:
    case ALLOCATION_IMMEDIATE:
        return "00";
    case ALLOCATION_DIRECT:
        return "01";
    case ALLOCATION_MAT:
        return "10";
    case ALLOCATION_REGISTER:
        return "11";
    default:
        return NULL;
    }
}

char *get_direct_allocation_binary_code(char *str)
{
    int num;
    char *temp = duplicate_str(str);
    char *binary_code = malloc(BINARY_CODE_SIZE);
    if (binary_code == NULL)
    {
        safe_exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
    }

    num = get_num_from_direct_allocation(temp);
    binary_code = convert_num_to_8_bits(num, ABSOLUTE_CODE);
    return binary_code;
}

int get_num_from_direct_allocation(char *str)
{
    char *temp = duplicate_str(str);
    temp = strtok(temp, "#");
    return atoi(temp);
}

char *convert_num_to_8_bits(int num, int ARE_type)
{
    int i;
    char *binary_code, *ARE_code;

    binary_code = malloc(BINARY_CODE_SIZE);
    ARE_code = get_ARE_binary_code(ARE_type);

    if (binary_code == NULL)
    {
        safe_exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
    }

    /* convert num to binary (8 bits) and add ARE code at the end */
    for (i = 7; i >= 0; i--)
    {
        binary_code[7 - i] = (num & (1 << i)) ? '1' : '0';
    }
    binary_code[8] = '\0';
    strcat(binary_code, ARE_code);

    printf("num: %d, binary code: %s\n", num, binary_code);
    return binary_code;
}

char *convert_num_to_10_bits(int num)
{
    int i;
    char *binary_code = malloc(BINARY_CODE_SIZE);
    if (binary_code == NULL)
    {
        safe_exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
    }

    /* convert num to binary (10 bits) */
    for (i = 9; i >= 0; i--)
    {
        binary_code[9 - i] = (num & (1 << i)) ? '1' : '0';
    }
    binary_code[10] = '\0';
    printf("num: %d, binary code: %s\n", num, binary_code);
    return binary_code;
}

char *get_register_allocation_binary_code_base_4(char *str)
{
    int num;
    char *temp = duplicate_str(str);
    temp = strtok(temp, "r");
    num = atoi(temp);
    switch (num)
    {
    case REGISTER_R0:
        return "0000";
    case REGISTER_R1:
        return "0001";
    case REGISTER_R2:
        return "0010";
    case REGISTER_R3:
        return "0011";
    case REGISTER_R4:
        return "0100";
    case REGISTER_R5:
        return "0101";
    case REGISTER_R6:
        return "0110";
    case REGISTER_R7:
        return "0111";
    }

    return NULL;
}

char *get_register_allocation_binary_code(char *str)
{
    char *temp = duplicate_str(str);
    char *binary_code = malloc(BINARY_CODE_SIZE);

    if (binary_code == NULL)
    {
        safe_exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
    }

    strcpy(binary_code, get_register_allocation_binary_code_base_4(temp));
    strcat(binary_code, get_register_allocation_binary_code_base_4("r0"));
    strcat(binary_code, get_ARE_binary_code(ABSOLUTE_CODE));

    printf("register %s binary code: %s\n", str, binary_code);
    return binary_code;
}

char *get_register_allocations_binary_code(char *src, char *dst)
{
    char *src_binary_code = get_register_allocation_binary_code_base_4(src);
    char *dst_binary_code = get_register_allocation_binary_code_base_4(dst);
    char *binary_code = malloc(BINARY_CODE_SIZE);

    if (binary_code == NULL)
    {
        safe_exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
    }

    strcpy(binary_code, src_binary_code);
    strcat(binary_code, dst_binary_code);
    strcat(binary_code, get_ARE_binary_code(ABSOLUTE_CODE));

    printf("register %s and %s binary code: %s\n", src, dst, binary_code);
    return binary_code;
}

int set_first_pass_mat_allocation_binary_code(char *str, char ***array_of_commands, int IC)
{
    /* we dont care about the label encode in first pass, skip its definition */
    int valid;
    char *temp, *reg1, *reg2;

    reg1 = malloc(REG_SIZE);
    if (reg1 == NULL)
    {
        safe_exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
    }
    reg2 = malloc(REG_SIZE);
    if (reg2 == NULL)
    {
        safe_exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
    }

    temp = duplicate_str(str);
    while (*temp != '[')
    {
        temp++;
    }
    (*array_of_commands)[IC] = NULL;
    IC++;

    /* temp is now [reg1][reg2] (without the label) */
    /* set registers from mat encode */
    valid = set_regs_from_mat_allocation(temp, reg1, reg2);
    if (!valid)
    {
        return 0;
    }

    (*array_of_commands)[IC] = get_register_allocations_binary_code(reg1, reg2);
    IC++;

    return 1;
}

int set_second_pass_mat_allocation_binary_code(char *str, char ***array_of_commands, int *IC, symbol_item **symbol_table, char ***extern_labels, int **extern_addresses, int *extern_count)
{
    /* we only care about the label encode in second pass */
    int i = 0, is_external = 0;
    char *temp, label[LABEL_SIZE];
    symbol_item *sym;

    /* build label from mat allocation */
    temp = duplicate_str(str);
    while (*temp != '[')
    {
        label[i] = *temp;
        i++;
        temp++;
    }
    label[i] = '\0';

    sym = find_symbol_item_by_name(*symbol_table, label);
    if (sym == NULL)
    {
        return 0;
    }

    if (strcmp(sym->type, "external") == 0)
    {
        is_external = 1;
    }

    if (is_external)
    {
        (*array_of_commands)[*IC] = convert_num_to_8_bits(sym->address, EXTERNAL_CODE);
        *extern_labels = realloc(*extern_labels, sizeof(char *) * (*extern_count + 1));
        *extern_addresses = realloc(*extern_addresses, sizeof(int) * (*extern_count + 1));
        (*extern_labels)[*extern_count] = duplicate_str(sym->name);
        (*extern_addresses)[*extern_count] = *IC + MEMORY_START_ADDRESS;
        (*extern_count)++;
    }
    else
    {
        (*array_of_commands)[*IC] = convert_num_to_8_bits(sym->address, RELOCATABLE_CODE);
    }

    return 1;
}
