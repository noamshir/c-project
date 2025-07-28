#include "./symbol_table.h"
#ifndef FIRST_PASS_H
#define FIRST_PASS_H
void first_pass(char *file_name_without_postfix);
void handle_guide(symbol_item **symbol_table, char *line, int *array_of_data, int *DC);
void handle_data_guide(symbol_item **symbol_table, char *line, int *array_of_data, int *DC);
void handle_string_guide(symbol_item **symbol_table, char *line, int *array_of_data, int *DC);
void handle_mat_guide(symbol_item **symbol_table, int *array_of_data, char *line, int *DC);
void handle_extern(symbol_item **symbol_table, char *line);
#endif
