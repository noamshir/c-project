#ifndef COMMAND_H
#define COMMAND_H
/**
 * @brief process and handle a command line (first pass).
 * This function gets a string representing a command line and ensure line is valid,
 * and decode its data to the array_of_commands.
 * @param symbol_table a pointer to a symbol table.
 * @param line the line to process.
 * @param array_of_commands a pointer to the array of commands.
 * @param IC a pointer to an integer counting the command items.
 * @return 1 if the line is a valid command line and was handled successfully, 0 otherwise.
 */
int handle_command_line(symbol_item **symbol_table, char *line, char ***array_of_commands, int *IC);
/**
 * @brief process and handle a command line with no operators.
 * This function gets a string representing a command line operands and ensure line is valid,
 * and decode its data to the array_of_commands.
 * @param command_index the command index.
 * @param str the string representing a command line operands (should be an empty string).
 * @param array_of_commands a pointer to the array of commands.
 * @param IC a pointer to an integer counting the command items.
 * @return 1 if the line is a valid command line with no operators and was handled successfully, 0 otherwise.
 */
int handle_no_op_line(int command_index, char *str, char ***array_of_commands, int *IC);
/**
 * @brief process and handle a command line with one operator (op).
 * This function gets a string representing a command line operands and ensure line is valid,
 * and decode its data to the array_of_commands.
 * @param command_index the command index.
 * @param str the string representing a command line operands (should have one operand).
 * @param array_of_commands a pointer to the array of commands.
 * @param IC a pointer to an integer counting the command items.
 * @return 1 if the line is a valid command line with one operator and was handled successfully, 0 otherwise.
 */
int handle_one_op_line(int command_index, char *str, char ***array_of_commands, int *IC);
/**
 * @brief process and handle a command line with two operators (op1, op2).
 * This function gets a string representing a command line operands and ensure line is valid,
 * and decode its data to the array_of_commands.
 * @param command_index the command index.
 * @param str the string representing a command line operands (should have two operand).
 * @param array_of_commands a pointer to the array of commands.
 * @param IC a pointer to an integer counting the command items.
 * @return 1 if the line is a valid command line with two operators and was handled successfully, 0 otherwise.
 */
int handle_two_op_line(int command_index, char *str, char ***array_of_commands, int *IC);
/**
 * @brief process and handle a command line operands.
 * This function gets strings representing a command line src and dst operands (if exists), ensure its valid
 * and decode its data to the array_of_commands.
 * @param command_index the command index.
 * @param src the string representing a command line src operand.
 * @param dst the string representing a command line dst operand.
 * @param array_of_commands a pointer to the array of commands.
 * @param IC a pointer to an integer counting the command items.
 * @return 1 if the command line params are valid and was handled successfully, 0 otherwise.
 */
int handle_op_line(int command_index, char *src, char *dst, char ***array_of_commands, int *IC);
/**
 * @brief encode first pass operands.
 * This function gets an operand, its type and the space it takes
 * (plus array of command and its counter) and encode it to the array of commands.
 * @param op a string representing an operand.
 * @param type the index representing the operand.
 * @param space the space (num of lines) the operand should take in array of commands.
 * @param array_of_commands a pointer to the array of commands.
 * @param IC a pointer to an integer counting the command items.
 * @return 1 if the encoding was successful, 0 otherwise.
 */
int encode_first_pass_operands(char *op, int type, int space, int *IC, char ***array_of_commands);
/**
 * @brief calculate the spaces needed for encoding src and dst operands.
 * This function calculates the numbers of lines the encoding for the given src type and dst type takes,
 * and sets the src and dst spaces which they should take.
 * @param src_type the index representing the src operand
 * @param dst_type the index representing the dst operand
 * @param src_space a pointer for an integer that is set with the space the src operand takes.
 * @param dst_space a pointer for an integer that is set with the space the dst operand takes.
 * @return an integer representing the spaces needed for encoding src and dst operands
 */
int calculate_space(int src_type, int dst_type, int *src_space, int *dst_space);
/**
 * @brief checks if given src and dst types are valid for the given command.
 * This function checks if the given src and dst types are valid for the given command.
 * @param command_index the command index.
 * @param src_type the index representing the src operand.
 * @param dst_type the index representing the dst operand.
 * @return 1 if the types are valid, 0 otherwise.
 */
int is_command_src_dst_valid(int command_index, int src_type, int dst_type);
#endif
