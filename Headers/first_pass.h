#ifndef FIRST_PASS_H
#define FIRST_PASS_H
/**
 * @brief Assembler first pass.
 *
 * This function reads line by line from the file received as param
 * and decides its type (guide line, command line) and handles it.
 *
 * @param char* file_name_without_postfix A pointer to the file name (without the .am postfix).
 */
void first_pass(char *file_name_without_postfix);
#endif
