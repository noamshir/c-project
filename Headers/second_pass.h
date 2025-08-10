#ifndef SECOND_PASS_H
#define SECOND_PASS_H
/**
 * @brief Assembler second pass.
 * This function reads line by line from the file received as param
 * and decides its type (guide line, command line, entry line) and handles it.
 * After reading the entire file (with no errors) it will generate a .ob file and .ent (if there were entry lines) and .ext (if there were external lines).
 * @param file_name_without_postfix A pointer to the file name (without the .am postfix).
 * @param symbol_table A pointer to the symbol table.
 * @param array_of_commands array of commands filled after the first pass.
 * @param ICF an integer representing the array of commands length.
 * @param array_of_data array of data filled after the first pass.
 * @param DCF an integer representing the array of data length.
 */
void second_pass(char *file_name_without_postfix, symbol_item **symbol_table, unsigned int *array_of_commands, int ICF, unsigned int *array_of_data, int DCF);
#endif
