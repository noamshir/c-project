#ifndef ABCD_CODE_H
#define ABCD_CODE_H
/**
 * @brief convert a number (0-256) to abcd base (base 4 where 0 is a, 1 is b, ...) and set it in the string received as param.
 * @param num the number to convert.
 * @param output the buffer to fill with the encoded number value.
 */
void convert_num_to_abcd_base(int num, char *output);
/**
 * @brief convert a binary code (10 bits) to abcd base (base 4 where 0 is a, 1 is b, ...) and set it in the string received as param.
 * @param binary_code an unsigned int representing the binary code to convert.
 * @param output the buffer to fill with the encoded binary code value.
 */
void convert_binary_code_to_abcd_base(unsigned int binary_code, char *output);
#endif
