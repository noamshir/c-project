#include "./symbol_table.h"
#ifndef FIRST_PASS_H
#define FIRST_PASS_H
void first_pass(char *file_name_without_postfix);
void handle_guide_line(symbol_item **symbol_table, char *line, char *array_of_data, int *DC);
void handle_data_guide(symbol_item **symbol_table, char *line, char *array_of_data, int *DC);
void handle_string_guide(symbol_item **symbol_table, char *line, char *array_of_data, int *DC);
void handle_mat_guide(symbol_item **symbol_table, char *array_of_data, char *line, int *DC);
void handle_extern(symbol_item **symbol_table, char *line);
void handle_operation_line(symbol_item **symbol_table, char *line, char *array_of_operations, int *IC);
void handle_no_op_line(int op_index, char *str, char *array_of_operations, int *IC);
void handle_one_op_line(int op_index, char *str, char *array_of_operations, int *IC);
void handle_two_op_line(int op_index, char *str, char *array_of_operations, int *IC);
void handle_op(int op_index, char *str, char *src, char *dst, char *array_of_operations, int *IC);
int calculate_space(int src_type, int dst_type, int *src_space, int *dst_space);
int is_op_src_dst_valid(int op_index, int src_type, int dst_type);
#endif
