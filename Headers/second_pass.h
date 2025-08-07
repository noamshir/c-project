#ifndef SECOND_PASS_H
#define SECOND_PASS_H

void second_pass(char *file_name_without_postfix, symbol_item **symbol_table, char ***array_of_commands, int ICF, char ***array_of_data, int DCF);
int handle_entry_guide(char *entry_label, symbol_item **symbol_table, char ***entry_labels, int **entry_addresses, int *entry_count);
int add_entry_attribute(symbol_item **symbol_table, char *entry_label);
int handle_command_line_second_pass(symbol_item **symbol_table, char *line, char ***array_of_commands, int *IC, char ***extern_labels, int **extern_addresses, int *extern_count);
int handle_no_op_line_second_pass(symbol_item **symbol_table, int command_index, char *str, char ***array_of_commands, int *IC, char ***extern_labels, int **extern_addresses, int *extern_count);
int handle_one_op_line_second_pass(symbol_item **symbol_table, int command_index, char *str, char ***array_of_commands, int *IC, char ***extern_labels, int **extern_addresses, int *extern_count);
int handle_two_op_line_second_pass(symbol_item **symbol_table, int command_index, char *str, char ***array_of_commands, int *IC, char ***extern_labels, int **extern_addresses, int *extern_count);
int handle_op_line_second_pass(symbol_item **symbol_table, int command_index, char *src, char *dst, char ***array_of_commands, int *IC, char ***extern_labels, int **extern_addresses, int *extern_count);
int encode_second_pass_operands(char *op, int type, int space, int *IC, char ***array_of_commands, symbol_item **symbol_table, char ***extern_labels, int **extern_addresses, int *extern_count);
#endif
