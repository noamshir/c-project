#include "./symbol_table.h"
#ifndef GUIDE_H
#define GUIDE_H
int handle_guide_line(symbol_item **symbol_table, char *line, char ***array_of_data, int *DC);
int handle_data_guide(char *guide_declaration, char ***array_of_data, int *DC);
int handle_string_guide(char *guide_declaration, char ***array_of_data, int *DC);
int handle_mat_guide(char *guide_declaration, char ***array_of_data, int *DC);
int handle_extern(symbol_item **symbol_table, char *line);
#endif
