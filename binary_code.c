#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Headers/string.h"
#include "Headers/error.h"
#include "Headers/consts.h"
#include "Headers/binary_code.h"

void add_operation_line_binary_code(char *binary_code, char *str, int op_index, int src_type, int dst_type, char *array_of_operations, int *IC)
{
    char *op_code, *ARE_code, *src_type_code, *dst_type_code;
    op_code = get_line_op_binary_code(op_index);
    ARE_code = get_line_ARE_binary_code(str);
    src_type_code = get_allocation_type_binary_code(src_type);
    dst_type_code = get_allocation_type_binary_code(dst_type);
    // copy and concat all to binary_code
    strcpy(binary_code, op_code);
    strcat(binary_code, src_type_code);
    strcat(binary_code, dst_type_code);
    strcat(binary_code, ARE_code);
    printf("binary code: %s\n", binary_code);
}

char *get_line_op_binary_code(int op_index)
{
    switch (op_index)
    {
    case 0:
        return "0000";
    case 1:
        return "0001";
    case 2:
        return "0010";
    case 3:
        return "0011";
    case 4:
        return "0100";
    case 5:
        return "0101";
    case 6:
        return "0110";
    case 7:
        return "0111";
    case 8:
        return "1000";
    case 9:
        return "1001";
    case 10:
        return "1010";
    case 11:
        return "1011";
    case 12:
        return "1100";
    case 13:
        return "1101";
    case 14:
        return "1110";
    case 15:
        return "1111";
    }

    return NULL;
}

char *get_line_ARE_binary_code(char *str)
{
    return "00";
}

char *get_allocation_type_binary_code(int allocation_type)
{
    switch (allocation_type)
    {
    case -1:
    case 0:
        return "00";
    case 1:
        return "01";
    case 2:
        return "10";
    case 3:
        return "11";
    }

    return NULL;
}

char *get_direct_allocation_binary_code(char *str)
{
    int num;
    char *temp = strdup(str);
    char *binary_code = malloc(BINARY_CODE_SIZE);
    num = get_num_from_direct_allocation(temp);
    binary_code = convert_num_to_binary_code(num);
    return binary_code;
}

int get_num_from_direct_allocation(char *str)
{
    char *temp = strdup(str);
    temp = strtok(temp, "#");
    delete_white_spaces(temp);
    return atoi(temp);
}

char *convert_num_to_binary_code(int num)
{
    int i;
    char *binary_code = malloc(BINARY_CODE_SIZE);
    // convert num to binary (8 bits) and add 00 at the end
    for (i = 7; i >= 0; i--)
    {
        binary_code[7 - i] = (num & (1 << i)) ? '1' : '0';
    }
    binary_code[8] = '0';
    binary_code[9] = '0';
    binary_code[10] = '\0';
    printf("num: %d, binary code: %s\n", num, binary_code);
    return binary_code;
}
