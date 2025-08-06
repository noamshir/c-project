#include <stdio.h>
#include <string.h>
#include "Headers/string.h"

int is_integer(char *str)
{
    if (str == NULL)
    {
        return 0;
    }

    if (strlen(str) == 0)
    {
        return 0;
    }

    int i = 0;
    // check for signs
    if (str[0] == '+' || str[0] == '-')
    {
        i = 1;
    }

    for (; str[i] != '\0'; i++)
    {
        if (!is_char_digit(str[i]))
        {
            // one char is not a digit, string isnt a number
            return 0;
        }
    }

    return 1;
}
