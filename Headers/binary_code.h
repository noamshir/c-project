#ifndef BINARY_CODE_H
#define BINARY_CODE_H

/**
 * @brief build command line binary code.
 * This function builds a command line binary code (command index encoded (4 bits), src type encoded (2 bits), dst type encoded (2 bits) and ARE type encoded (2 bits)).
 * @param command_index the comand index.
 * @param src_type the src type index.
 * @param dst_type the dst type index.
 * @return a string that represent the command line binary code
 */
char *build_command_line_binary_code(int command_index, int src_type, int dst_type);
/**
 * @brief get command binary code.
 * @param command_index the comand index.
 * @return a 4 char string that represent the binary code for the command index
 * or NULL if the index doesn't represents a valid command.
 * */
char *get_command_binary_code(int command_index);
/**
 * @brief get ARE binary code.
 * @param ARE_type the ARE type.
 * @return a 2 char string that represent the binary code for the ARE type
 * or NULL if the index doesn't represents a valid command.
 * */
char *get_ARE_binary_code(int ARE_type);
/**
 * @brief get allocation type binary code.
 * @param allocation_type the allocation type.
 * @return a 2 char string that represent the binary code for the allocation type
 * or NULL if the index doesn't represents a valid command.
 * */
char *get_allocation_type_binary_code(int allocation_type);
/**
 * @brief get direct allocation binary code.
 * This function gets an str representing a direct allocation (#num) and encode it to a binary code string.
 * @param str a string representing direct allocation.
 * @return a string representing the direct allocation binary code.
 */
char *get_direct_allocation_binary_code(char *str);
/**
 * @brief extract the number (num) inside a direct allocation string (#num)
 * @param str a string representing direct allocation
 * @return a number representing the number inside the direct allocation string.
 */
int get_num_from_direct_allocation(char *str);
/**
 * This function convert a number to 8 bits binary code
 * and return it as 10 bits binary code (adds ARE coded at the end).
 * @param num the number to convert.
 * @param ARE_type the ARE_type.
 * @return a 10 chars string that represents the number received as param binary code in 8 bits plus 00 at the end.
 */
char *convert_num_to_8_bits(int num, int ARE_type);
/**
 * @brief convert a number to 10 bits binary code
 * @param num the number to convert.
 * @return a 10 chars string that represents the number received as param binary code in 10 bits.
 */
char *convert_num_to_10_bits(int num);
/**
 * @brief get register allocation binary code in base 4 based on the register allocation.
 * This function gets a string representing a register allocation,
 * extract the register allocation type (index) from it and convert it to binary base 4 string.
 * @param str a string representing a register allocation.
 * @return a string representing the base 4 binary code for the register allocation (str) received as param.
 */
char *get_register_allocation_binary_code_base_4(char *str);
/**
 * @brief get register allocation binary code.
 * This function gets a string representing a register allocation,
 * and builds its 10 bits binary code (register type 4 base plus 6 zeros).
 * @param str a string representing a register allocation.
 * @return a string representing the binary code for the register allocation (str) received as param.
 */
char *get_register_allocation_binary_code(char *str);
/**
 * @brief get register allocations binary code.
 * This function gets 2 strings representing the src ans dst operands,
 * and builds its 10 bits binary code (register src type 4 base, register dst type 4 base plus 2 zeros).
 * @param src a string representing src register allocation.
 * @param dst a string representing dst register allocation.
 * @return a string representing the binary code for the register allocation (str) received as param.
 */
char *get_register_allocations_binary_code(char *src, char *dst);
/**
 * @brief set first pass mat allocation binary code.
 * This function sets the first pass mat allocation binary codes (without the label encoding) in the array of commands.
 * @param str a string representing mat allocation.
 * @param array_of_commands a pointer to the array of commands.
 * @param IC an integer representing the next free spot at the array of commands.
 * @return 1 if the mat allocation binary codes for the first pass were successfully set in array of commands.
 */
int set_first_pass_mat_allocation_binary_code(char *str, char ***array_of_commands, int IC);
#endif
