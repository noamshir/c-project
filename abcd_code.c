#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Headers/abcd_code.h"

void convert_num_to_abcd_base(int value, char *output) /*converting the input to a dedicate base 4 code for the object file*/
{
    int i;
    char abcd[] = {'a', 'b', 'c', 'd'};

    for (i = 3; i >= 0; i--)
    {
        output[3 - i] = abcd[(value >> (i * 2)) & 0x3];
    }

    output[4] = '\0';
    printf("num: %d, abcd code: %s\n", value, output);
}

void convert_binary_code_to_abcd_base(char *binary_code, char *output)
{
    int i, j, output_counter = 0;
    char abcd[] = {'a', 'b', 'c', 'd'};
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

        for (j = 0; j < 4; j++)
        {
            if (encoded == abcd[j])
            {
                output[output_counter] = abcd[j];
                output_counter++;
                break;
            }
        }
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
