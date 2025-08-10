#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Headers/string.h"
#include "Headers/error.h"
#include "Headers/consts.h"
#include "Headers/binary_code.h"
#include "Headers/utils.h"
#include "Headers/symbol_table.h"

unsigned int build_command_line_binary_code(int op_index, int src_type, int dst_type)
{
    unsigned int result = 0;
    /* src_type and dst_type can be -1, representing the command line might not have them.
    support it we convert -1 to 0 */
    src_type = src_type == -1 ? 0 : src_type;
    dst_type = dst_type == -1 ? 0 : dst_type;

    /* op index is bits 6-9 */
    result |= (op_index & 0xF) << 6;

    /* src type is bits 4-5 */
    result |= (src_type & 0x3) << 4;

    /* dst type is bits 2-3 */
    result |= (dst_type & 0x3) << 2;

    /* ARE flags are bits 0-1, in command line its always ABSOLUTE_CODE_ARE (00) */
    result |= ABSOLUTE_CODE_ARE;

    /* ensure 10 bits */
    return result & TEN_BITS;
}

unsigned int get_immediate_allocation_binary_code(char *str)
{
    int num;

    /* extract number from immediate allocation string */
    num = get_num_from_immediate_allocation(str);
    return convert_num_to_8_bits(num, ABSOLUTE_CODE_ARE);
}

int get_num_from_immediate_allocation(char *str)
{
    char *temp = duplicate_str(str);
    temp = strtok(temp, "#");
    return atoi(temp);
}

unsigned int convert_num_to_8_bits(int num, int ARE_type)
{
    unsigned short result;

    /* handle positive and negative separately */
    if (num < 0)
    {
        /* 8-bit 2's complement, then shift left by 2 for ARE flags */
        result = ((unsigned short)(num & 0xFF)) << 2;
    }
    else
    {
        /* Positive num, shift left by 2 for ARE flags */
        result = ((unsigned short)num) << 2;
    }

    /* Set ARE flag according to the param */
    result |= ARE_type;

    return result & TEN_BITS;
}

unsigned int convert_num_to_10_bits(int num)
{
    return num & TEN_BITS;
}

unsigned int get_register_allocation_binary_code(char *str, int is_src)
{
    if (is_src)
    {
        /* the src first than all 0*/
        return get_register_allocations_binary_code(str, "r0");
    }
    else
    {
        /* 4 0's first than dst and than all 0*/
        return get_register_allocations_binary_code("r0", str);
    }
}

unsigned int get_register_allocations_binary_code(char *src, char *dst)
{
    unsigned int result = 0;
    int src_reg_idx, dest_reg_idx;

    src_reg_idx = get_register_index(src);
    dest_reg_idx = get_register_index(dst);

    /* Encode source register in bits 6-9 (4 bits) */
    if (src_reg_idx >= 0 && src_reg_idx <= 7)
    {
        result |= (src_reg_idx & 0xF) << 6;
    }

    /* Encode destination register in bits 2-5 (4 bits) */
    if (dest_reg_idx >= 0 && dest_reg_idx <= 7)
    {
        result |= (dest_reg_idx & 0xF) << 2;
    }

    /* Set ARE flags to absolute (bits 0-1) */
    result |= ABSOLUTE_CODE_ARE;

    return result & TEN_BITS;
}

int set_first_pass_mat_allocation_binary_code(char *str, unsigned int *array_of_commands, int IC)
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
    array_of_commands[IC] = 0;
    IC++;

    /* temp is now [reg1][reg2] (without the label) */
    /* set registers from mat encode */
    valid = set_regs_from_mat_allocation_without_label(temp, reg1, reg2);
    if (valid)
    {
        /* fill array of commands with the binary code for the regs */
        array_of_commands[IC] = get_register_allocations_binary_code(reg1, reg2);
        IC++;
    }

    free(reg1);
    free(reg2);

    return valid;
}
