#include "./symbol_table.h"
#ifndef GUIDE_H
#define GUIDE_H
/**
 * @brief process and handle a guide line (first pass).
 * This function gets a string representing a guide line and ensure line is valid,
 * and decode its data to the array_of_data.
 * @param symbol_table a pointer to a symbol table.
 * @param line the line to process.
 * @param array_of_data a pointer to the array of data.
 * @param DC a pointer to an integer counting the data items.
 * @return 1 if the line is a valid guide line and was handled successfully, 0 otherwise.
 */
int handle_guide_line(symbol_item **symbol_table, char *line, char ***array_of_data, int *DC);
/**
 * @brief process and handle a data guide declaration (num1, num2, ....).
 * This function gets a string representing a data guide declaration and ensure it's valid,
 * and decode its data to the array_of_data.
 * @param guide_declaration a string representing a guide declaration.
 * @param array_of_data a pointer to the array of data.
 * @param DC a pointer to an integer counting the data items.
 * @return 1 if the guide declaration is a valid data guide declaration and was handled successfully, 0 otherwise.
 */
int handle_data_guide(char *guide_declaration, char ***array_of_data, int *DC);
/**
 * @brief process and handle a string guide declaration ("something").
 * This function gets a string representing a string guide declaration and ensure it's valid,
 * and decode its data to the array_of_data.
 * @param guide_declaration a string representing a guide declaration.
 * @param array_of_data a pointer to the array of data.
 * @param DC a pointer to an integer counting the data items.
 * @return 1 if the guide declaration is a valid string guide declaration and was handled successfully, 0 otherwise.
 */
int handle_string_guide(char *guide_declaration, char ***array_of_data, int *DC);
/**
 * @brief process and handle a mat guide declaration ([num1][num2] num1, num2,...,).
 * This function gets a string representing a mat guide declaration and ensure it's valid,
 * and decode its data to the array_of_data.
 * @param guide_declaration a string representing a guide declaration.
 * @param array_of_data a pointer to the array of data.
 * @param DC a pointer to an integer counting the data items.
 * @return 1 if the guide declaration is a valid mat guide declaration and was handled successfully, 0 otherwise.
 */
int handle_mat_guide(char *guide_declaration, char ***array_of_data, int *DC);
/**
 * @brief sets the rows and columns from the mat declaration in the given integer pointers.
 * @param guide_declaration a string representing mat declaration.
 * @param rows an int pointer to store the mat declaration rows number.
 * @param cols an int pointer to store the mat declaration columns number.
 * @return 1 if the guide declaration is a valid mat guide declaration and the row and cols pointers were setted successfully, 0 otherwise.
 */
int set_rows_and_cols_from_mat_declaration(char *guide_declaration, int *rows, int *cols);
/**
 * @brief process and handle an extern guide line ((label) .extern name).
 * This function gets a string representing an extern guide line and ensure it's valid,
 * and adds its name label to the symbol table.
 * @param symbol_table a pointer to a symbol table.
 * @param line the line to process.
 * @return 1 if the extern guide line is valid and was handled successfully, 0 otherwise.
 */
int handle_extern_guide_line(symbol_item **symbol_table, char *line);

int handle_entry_guide(char *entry_label, symbol_item **symbol_table, char ***entry_labels, int **entry_addresses, int *entry_count);
int add_entry_attribute(symbol_item **symbol_table, char *entry_label);
#endif
