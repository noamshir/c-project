#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Headers/error.h"
#include "Headers/consts.h"
#include "Headers/string.h"

char *delete_white_spaces_start_and_end(char *str)
{
    char *temp = str;

    if (str == NULL)
    {
        return NULL;
    }

    while (*temp == ' ' || *temp == '\t' || *temp == '\n')
    {
        ++temp;
    }
    str = temp;
    temp = str;
    while (*temp != '\0')
    {
        temp++;
    }
    temp--;
    while (*temp == ' ' || *temp == '\t' || *temp == '\n')
    {
        *temp = '\0';
        temp--;
    }

    return str;
}

char *duplicate_str(char *str)
{
    char *copy;
    int len = 0;

    if (str == NULL)
    {
        return NULL;
    }

    len = strlen(str) + 1;

    /* Allocate memory (+1 for null terminator) */
    copy = malloc(len + 1);
    if (copy == NULL)
    {
        safe_exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
    }

    strcpy(copy, str);
    return copy;
}

int is_integer(char *str)
{
    int i = 0;

    if (str == NULL)
    {
        return 0;
    }

    if (strlen(str) == 0)
    {
        return 0;
    }

    /* check for signs */
    if (str[0] == '+' || str[0] == '-')
    {
        /* first char is a sign, start check from second char */
        i = 1;
    }

    for (; str[i] != '\0'; i++)
    {
        if (!is_char_digit(str[i]))
        {
            /* one char is not a digit, string isn't a number */
            return 0;
        }
    }

    return 1;
}

int is_char_alphabetical_or_digit(char c)
{
    return is_char_alphabetical(c) || is_char_digit(c);
}

int is_char_alphabetical(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int is_char_digit(char c)
{
    return c >= '0' && c <= '9';
}

int is_line_too_long(char *line)
{
    int line_length = strlen(line);
    /* if line is not empty and last char is not \n it was too big to fit in the allocated location */
    return line_length > 0 && line_length >= LINE_SIZE && line[line_length - 1] != '\n';
}

int is_empty_line(char *line)
{
    if (line == NULL)
    {
        return 1;
    }

    /* loop line until its end */
    while (*line != '\0')
    {
        if (*line != ' ' && *line != '\t' && *line != '\n')
        {
            return 0;
        }
        line++;
    }

    /* line is empty */
    return 1;
}

int is_comment_line(char *line)
{
    char *temp;
    if (is_empty_line(line))
    {
        return 0;
    }

    temp = delete_white_spaces_start_and_end(line);
    if (temp[0] == ';')
    {
        return 1;
    }
    return 0;
}
