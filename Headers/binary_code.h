#include "./symbol_table.h"
#ifndef BINARY_CODE_H
#define BINARY_CODE_H

/**
 * @brief build command line binary code.
 * This function builds a command line binary code (command index encoded (4 bits), src type encoded (2 bits), dst type encoded (2 bits) and ARE type encoded (2 bits)).
 * @param command_index the comand index.
 * @param src_type the src type index.
 * @param dst_type the dst type index.
 * @return an unsigned int that represent the command line binary code
 */
unsigned int build_command_line_binary_code(int op_index, int src_type, int dst_type);
/**
 * @brief get direct allocation binary code.
 * This function gets a str representing a direct allocation (#num) and encode it to a binary code.
 * @param str a string representing direct allocation.
 * @return an unsigned int representing the direct allocation binary code.
 */
unsigned int get_immediate_allocation_binary_code(char *str);
/**
 * @brief extract the number (num) inside a direct allocation string (#num)
 * @param str a string representing direct allocation
 * @return a number representing the number inside the direct allocation string.
 */
int get_num_from_immediate_allocation(char *str);
/**
 * This function convert a number to 8 bits binary code
 * and return it as 10 bits binary code (adds ARE coded at the end).
 * @param num the number to convert.
 * @param ARE_type the ARE_type.
 * @return an unsigned int that represents the number received as param binary code in 8 bits plus 00 at the end.
 */
unsigned int convert_num_to_8_bits(int num, int ARE_type);
/**
 * @brief convert a number to 10 bits binary code
 * @param num the number to convert.
 * @return an unsigned int that represents the number received as param binary code in 10 bits.
 */
unsigned int convert_num_to_10_bits(int num);
/**
 * @brief get register allocation binary code.
 * This function gets a string representing a register allocation,
 * and builds its 10 bits binary code (register type 4 base plus 6 zeros).
 * @param str a string representing a register allocation.
 * @param is_src a boolean flag, 1 indicates op is src, 0 dst.
 * @return an unsigned int representing the binary code for the register allocation (str) received as param.
 */
unsigned int get_register_allocation_binary_code(char *str, int is_src);
/**
 * @brief get register allocations binary code.
 * This function gets 2 strings representing the src ans dst operands,
 * and builds its 10 bits binary code (register src type 4 base, register dst type 4 base plus 2 zeros).
 * @param src a string representing src register allocation.
 * @param dst a string representing dst register allocation.
 * @return an unsigned int representing the binary code for the register allocation (str) received as param.
 */
unsigned int get_register_allocations_binary_code(char *src, char *dst);
/**
 * @brief set first pass mat allocation binary code.
 * This function sets the first pass mat allocation binary codes (without the label encoding) in the array of commands.
 * @param str a string representing mat allocation.
 * @param array_of_commands a pointer to the array of commands.
 * @param IC an integer representing the next free spot at the array of commands.
 * @return 1 if the mat allocation binary codes for the first pass were successfully set in array of commands.
 */
int set_first_pass_mat_allocation_binary_code(char *str, unsigned int *array_of_commands, int IC);

int set_second_pass_mat_allocation_binary_code(char *str, unsigned int *array_of_commands, int *IC, symbol_item **symbol_table, char ***extern_labels, int **extern_addresses, int *extern_count);
#endif
