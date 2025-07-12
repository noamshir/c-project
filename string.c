#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Headers/error.h"

void delete_white_spaces(char *str)
{
    char *temp = str;
    do
    {
        while (*temp == ' ' || *temp == '\t' || *temp == '\n')
        {
            ++temp;
        }
    } while ((*str++ = *temp++));
}

char *get_file_name_without_extension(char *file_name)
{
    char *temp = strdup(file_name);
    char *dot_pt = strchr(temp, '.');
    if (dot_pt != NULL)
    {
        /* ends string at dot */
        *dot_pt = '\0';
    }
    return temp;
}
