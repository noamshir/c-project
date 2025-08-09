#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Headers/abcd_code.h"
#include "Headers/consts.h"

void convert_num_to_abcd_base(int num, char *output)
{
    /* we support only numbers between 0-256 (addresses and array sizes for addresses)*/
    int i, temp;
    char digit, digits[6]; /* numbers between 0-256 is max size 6 in base 4 */
    int length = 0;

    if (num < 0 || num > 256)
        return;

    /* special case for 0 */
    if (num == 0)
    {
        output[0] = 'a';
        output[1] = '\0';
        return;
    }

    /* convert number to base 4 digits in reverse order */
    temp = num;
    while (temp > 0)
    {
        digits[length++] = (char)('0' + (temp % 4));
        temp = temp / 4;
    }

    /* reverse digits into buffer */
    for (i = 0; i < length; i++)
    {
        digit = digits[length - 1 - i];
        output[i] = convert_digit_to_abcd_base(digit);
    }
    output[length] = '\0';

    printf("num: %d, base4 code: %s\n", num, output);
}

void convert_binary_code_to_abcd_base(unsigned int binary_code, char *output)
{
    int i;
    unsigned int value, two_bits;
    /* only 10 bits */
    value = binary_code & TEN_BITS;

    for (i = 0; i < 5; i++)
    {
        /* Extract 2 bits starting from the most significant pair (bits 9-8, 7-6, etc.) */
        two_bits = (value >> ((4 - i) * 2)) & 0x3;
        output[i] = 'a' + two_bits; /* a asci value + the digit (0,1,2,3)  */
    }
    output[5] = '\0';
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
