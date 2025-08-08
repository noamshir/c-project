#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Headers/abcd_code.h"

char *convert_num_to_base4(int num, char *output)
{
    /* we support only numbers between 0-256 (addresses and array sizes for addresses)*/
    int output_max_size = 6;
    int i;
    int temp;
    char digits[6]; /* same size of output_max_size */
    int pos = 0;

    if (num < 0 || num > 256)
        return NULL;

    /* special case for 0 */
    if (num == 0)
    {
        if (output_max_size < 2) /* need space for "0" + '\0' */
            return NULL;
        output[0] = '0';
        output[1] = '\0';
        return output;
    }

    /* convert number to base 4 digits in reverse order */
    temp = num;
    while (temp > 0)
    {
        digits[pos++] = (char)('0' + (temp % 4));
        temp = temp / 4;
    }

    /* reverse digits into buffer */
    for (i = 0; i < pos; i++)
    {
        output[i] = digits[pos - 1 - i];
    }
    output[pos] = '\0';

    printf("num: %d, base4 code: %s\n", num, output);
    return output;
}

void convert_num_to_abcd_base(int value, char *output) /*converting the input to a dedicate base 4 code for the object file*/
{
    int i, output_length;
    char buffer[6];
    convert_num_to_base4(value, buffer);

    output_length = 0;
    i = 0;
    while (buffer[i] != '\0')
    {
        output[i] = convert_digit_to_abcd_base(buffer[i]);
        i++;
        output_length++;
    }

    output[output_length] = '\0';
    printf("num: %d, abcd code: %s\n", value, output);
}

void convert_binary_code_to_abcd_base(char *binary_code, char *output)
{
    int i, j, output_counter = 0;
    char two_digits[3];
    char encoded;

    /* binary code is ten digits string and its abcd code is taking 2 digits each time and converting them to abcd (00 is a, b is 01, ...) */
    for (i = 0; i < 9; i += 2)
    {
        for (j = i; j < i + 2; j++)
        {
            two_digits[j - i] = binary_code[j];
        }
        two_digits[2] = '\0';
        encoded = convert_two_bits_binary_code_to_abcd_base(two_digits);
        output[output_counter] = encoded;
        output_counter++;
    }

    output[6] = '\0';
    printf("binary code: %s, abcd code: %s\n", binary_code, output);
}

char convert_two_bits_binary_code_to_abcd_base(char *binary_code)
{
    if (strcmp(binary_code, "00") == 0)
    {
        return 'a';
    }
    else if (strcmp(binary_code, "01") == 0)
    {
        return 'b';
    }
    else if (strcmp(binary_code, "10") == 0)
    {
        return 'c';
    }
    else if (strcmp(binary_code, "11") == 0)
    {
        return 'd';
    }

    return '\0';
}

char convert_digit_to_abcd_base(char digit)
{
    switch (digit)
    {
    case '0':
        return 'a';
    case '1':
        return 'b';
    case '2':
        return 'c';
    case '3':
        return 'd';
    default:
        return '\0';
    }
}
