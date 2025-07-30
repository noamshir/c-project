#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Headers/error.h"
#include "Headers/consts.h"
#include "Headers/string.h"
#include "Headers/utils.h"

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

char *registers[8] = {
    "r0",
    "r1",
    "r2",
    "r3",
    "r4",
    "r5",
    "r6",
    "r7"};

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

int is_valid_label_name(char *str)
{
    if (str == NULL)
    {
        return 0;
    }

    if (strlen(str) == 0 || strlen(str) > LABEL_SIZE)
    {
        return 0;
    }

    // loop the string
    while (*str != '\0')
    {
        if (!is_char_alphabetical(*str))
        {
            return 0;
        }
        str++;
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

int is_label(char *word)
{
    if (word == NULL)
    {
        return 0;
    }
    if (strlen(word) == 0 || strlen(word) == 1 || strlen(word) > LABEL_SIZE)
    {
        return 0;
    }

    int end_of_string = strlen(word) - 1;
    if (word[end_of_string] == ':')
    {
        return 1;
    }

    // check that each char is abc or 0-9
    for (int i = 0; i < end_of_string; i++)
    {
        if (!is_char_alphabetical_or_digit(word[i]))
        {
            return 0;
        }
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

int get_operation_index(char *word)
{
    int i;

    if (word == NULL)
    {
        return -1;
    }

    for (i = 0; i < 16; i++)
    {
        if (strcmp(word, operations[i]) == 0)
        {
            return i;
        }
    }

    return -1;
}

int is_operation(char *word)
{
    return get_operation_index(word) != -1;
}

int is_register(char *word)
{
    // check if word is register
    for (int i = 0; i < 8; i++)
    {
        if (strcmp(word, registers[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int is_mat_declaration(char *word)
{
    // check that next word is [num1][num2]
    char *temp;
    if (word == NULL)
    {
        return 0;
    }
    if (strlen(word) == 0)
    {
        return 0;
    }
    if (word[0] != '[' || word[strlen(word) - 1] != ']')
    {
        return 0;
    }

    // loop the first [num] and check if valid
    for (temp = word + 1; *temp != ']'; temp++)
    {
        if (!is_char_digit(*temp))
        {
            return 0;
        }
    }

    temp++;
    if (*temp != '[')
    {
        return 0;
    }
    temp++;

    // loop the second [num] and check if valid
    for (; *temp != ']'; temp++)
    {
        if (!is_char_digit(*temp))
        {
            return 0;
        }
    }

    return 1;
}

int set_rows_and_cols_from_mat_declaration(char *word, int *rows, int *cols)
{
    char *temp, reg[2];
    int i = 0;

    if (!is_mat_declaration(word))
    {
        return 0;
    }

    // loop the first [num] and check if valid
    for (temp = word + 1; *temp != ']'; temp++)
    {
        if (i > 1)
        {
            return 0;
        }
        reg[i] = *temp;
        i++;
    }
    *rows = atoi(reg);

    i = 0;
    temp = temp + 2;
    // loop the second [num] and check if valid
    for (; *temp != ']'; temp++)
    {
        if (i > 1)
        {
            return 0;
        }
        reg[i] = *temp;
        i++;
    }
    *cols = atoi(reg);

    return 1;
}

int get_allocation_type(char *word)
{
    if (word == NULL || is_empty_line(word))
    {
        return ALLOCATION_MISSING;
    }

    if (is_immediate_allocation(word))
    {
        return ALLOCATION_IMMEDIATE;
    }

    if (is_mat_allocation(word))
    {
        return ALLOCATION_MAT;
    }

    if (is_register_allocation(word))
    {
        return ALLOCATION_REGISTER;
    }

    if (is_direct_allocation(word))
    {
        return ALLOCATION_DIRECT;
    }

    return -999;
}

int is_immediate_allocation(char *word)
{
    char *rest;
    // check if str is of type #num
    if (word == NULL)
    {
        return 0;
    }

    if (strlen(word) == 0)
    {
        return 0;
    }
    if (word[0] != '#')
    {
        return 0;
    }

    // rest is from index 1 to end
    rest = word + 1;
    if (is_integer(rest))
    {
        return 1;
    }

    return 0;
}

int is_mat_allocation(char *word)
{
    // check that next word is [num1][num2]
    int i = 0;
    char *label, *mat_def, *temp, reg[3];
    if (word == NULL)
    {
        return 0;
    }
    if (strlen(word) == 0)
    {
        return 0;
    }

    temp = strchr(word, '[');
    // no [ sign? cant be a mat def...
    if (temp == NULL)
    {
        return 0;
    }

    label = malloc(1);
    temp = strdup(word);
    while (*temp != '[')
    {
        i++;
        label = realloc(label, i);
        if (label == NULL)
        {
            printf("error code is %d\n", PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
            return 0;
        }
        label[i - 1] = *temp;
        temp++;
    }

    i = 0;
    if (!is_valid_label_name(label))
    {
        free(label);
        return 0;
    }
    free(label);
    mat_def = strdup(temp);

    if (mat_def[0] != '[' || mat_def[strlen(mat_def) - 1] != ']')
    {
        return 0;
    }

    temp = strdup(mat_def);
    // loop the first [num] and check if valid
    for (temp = mat_def + 1; *temp != ']'; temp++)
    {
        if (i > 1)
        {
            return 0;
        }
        reg[i] = *temp;
        i++;
    }
    reg[i] = '\0';
    if (!is_register(reg))
    {
        printf("error code is %d\n", 90);
        return 0;
    }

    temp++;
    if (*temp != '[')
    {
        return 0;
    }
    temp++;

    // loop the second [num] and check if valid
    i = 0;
    for (; *temp != ']'; temp++)
    {
        if (i > 1)
        {
            return 0;
        }
        reg[i] = *temp;
        i++;
    }
    reg[i] = '\0';
    if (!is_register(reg))
    {
        return 0;
    }

    return 1;
}

int is_register_allocation(char *word)
{
    return is_register(word);
}

int is_direct_allocation(char *word)
{
    // check that each char is abc or 0-9
    for (int i = 0; i < strlen(word); i++)
    {
        if (!is_char_alphabetical_or_digit(word[i]))
        {
            return 0;
        }
    }
}