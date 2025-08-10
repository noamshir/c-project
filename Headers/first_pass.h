#ifndef FIRST_PASS_H
#define FIRST_PASS_H
/**
 * @brief Assembler first pass.
 *
 * This function reads line by line from the file received as param
 * and decides its type (guide line, command line) and handles it.
 * After reading the entire file (with no errors) it will continue to the second pass.
 * @param file_name_without_postfix A pointer to the file name (without the .am postfix).
 * @param mcro_table A pointer to the mcro table.
 * @return 1 if successfully processed the file, 0 otherwise.
 */
int first_pass(char *file_name_without_postfix, mcro_item **mcro_table);
#endif
