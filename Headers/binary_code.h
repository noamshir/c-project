#ifndef BINARY_CODE_H
#define BINARY_CODE_H
void add_command_line_binary_code(char *binary_code, char *str, int command_index, int src_type, int dst_type);
char *get_line_command_binary_code(int command_index);
char *get_ARE_binary_code(int ARE_type);
char *get_allocation_type_binary_code(int allocation_type);
char *get_direct_allocation_binary_code(char *str);
int get_num_from_direct_allocation(char *str);
char *convert_num_to_8_bits(int num);
char *convert_num_to_10_bits(int num);
char *get_register_allocation_binary_code_base_4(char *str);
char *get_register_allocation_binary_code(char *str);
char *get_register_allocations_binary_code(char *src, char *dst);
int set_first_pass_mat_allocation_binary_code(char *str, char ***array_of_commands, int IC);
#endif
