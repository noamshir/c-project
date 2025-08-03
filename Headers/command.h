#ifndef COMMAND_H
#define COMMAND_H
int handle_command_line(symbol_item **symbol_table, char *line, char ***array_of_commands, int *IC);
int handle_no_op_line(int command_index, char *str, char ***array_of_commands, int *IC);
int handle_one_op_line(int command_index, char *str, char ***array_of_commands, int *IC);
int handle_two_op_line(int command_index, char *str, char ***array_of_commands, int *IC);
int handle_op_line(int command_index, char *str, char *src, char *dst, char ***array_of_commands, int *IC);
int encode_first_pass_operands(char *op, int type, int space, int *IC, char ***array_of_commands);
int calculate_space(int src_type, int dst_type, int *src_space, int *dst_space);
int is_command_src_dst_valid(int command_index, int src_type, int dst_type);
#endif
