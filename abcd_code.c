#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void convert_num_to_abcd_base(int value, char *output) /*converting the input to a dedicate base 4 code for the object file*/
{
    int i;
    char abcd[] = {'a', 'b', 'c', 'd'};

    for (i = 4; i >= 0; i--)
    {
        output[4 - i] = abcd[(value >> (i * 2)) & 0x3];
    }

    output[5] = '\0';
}

void convert_binary_code_to_abcd_base(char *binary_code, char *output)
{
    int i, j, output_counter = 0;
    char abcd[] = {'a', 'b', 'c', 'd'};
    char two_digits[3];

    // binary code is ten digits string and its abcd code is taking 2 digits each time and converting them to abcd (00 is a, b is 01, ...)
    for (i = 0; i < 10; i += 2)
    {
        for (j = i; j < i + 2; j++)
        {
            two_digits[j - i] = binary_code[j];
        }
        two_digits[2] = '\0';

        for (j = 0; j < 4; j++)
        {
            if (strcmp(two_digits, abcd[j]) == 0)
            {
                output[output_counter] = abcd[j];
                output_counter++;
                break;
            }
        }

        output[6] = '\0';
    }
}