#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Headers/error.h"
#include "Headers/consts.h"

char *operations[16] = {
    "mov",
    "cmp",
    "add",
    "sub",
    "lea",
    "clr",
    "not",
    "inc",
    "dec",
    "jmp",
    "bne",
    "jsr",
    "red",
    "prn",
    "rts",
    "stop"};

char *guides[5] = {
    ".data",
    ".string",
    ".mat",
    ".entry",
    ".extern"};

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

int is_empty_line(char *line)
{
    // loop the string
    while (*line != '\0')
    {
        //
        if (*line != ' ' && *line != '\t' && *line != '\n')
        {
            return 0;
        }
        line++;
    }
    return 1;
}

int is_comment_line(char *line)
{
    if (line == NULL)
    {
        return 0;
    }

    if (strlen(line) == 0)
    {
        /* code */
    }

    if (line[0] == ';')
    {
        return 1;
    }
    return 0;
}

int is_label(char *word)
{
    if (word == NULL)
    {
        return 0;
    }
    if (strlen(word) == 0 || strlen(word) == 1)
    {
        return 0;
    }

    int end_of_string = strlen(word) - 1;
    if (word[end_of_string] == ':')
    {
        return 1;
    }
    return 0;
}

int is_word_guide(char *word, int guide_type)
{
    if (word == NULL)
    {
        return 0;
    }
    if (strcmp(word, guides[guide_type]) == 0)
    {
        return 1;
    }
    return 0;
}

int is_data_guide(char *word)
{
    return is_word_guide(word, GUIDE_DATA);
}

int is_string_guide(char *word)
{
    return is_word_guide(word, GUIDE_STRING);
}

int is_mat_guide(char *word)
{
    return is_word_guide(word, GUIDE_MAT);
}

int is_extern_guide(char *word)
{
    return is_word_guide(word, GUIDE_EXTERN);
}

int is_entry_guide(char *word)
{
    return is_word_guide(word, GUIDE_ENTRY);
}

int is_operation(char *word)
{
    int i;

    if (word == NULL)
    {
        return 0;
    }

    for (i = 0; i < 16; i++)
    {
        if (strcmp(word, operations[i]) == 0)
        {
            return 1;
        }
    }

    return 0;
}
