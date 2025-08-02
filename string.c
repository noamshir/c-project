#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Headers/error.h"
#include "Headers/consts.h"
#include "Headers/string.h"
#include "Headers/utils.h"

char *commands[16] = {
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

char *delete_white_spaces_start_and_end(char *str)
{
    if (str == NULL)
    {
        return NULL;
    }

    char *temp = str;
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

char *get_label_name(char *word)
{
    if (!is_label(word))
    {
        return NULL;
    }

    // remove last char in word (:)
    word[strlen(word) - 1] = '\0';
    return word;
}

int is_guide(char *word)
{
    return is_data_guide(word) || is_string_guide(word) || is_mat_guide(word) || is_extern_guide(word) || is_entry_guide(word);
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

int is_data_guide_declaration(char *guide_declaration)
{
    char *num, *temp;

    // check that guide_declaration is of type: "num1, num2, ...., numn"
    printf("checking data guide declaration: %s\n", guide_declaration);

    temp = strdup(guide_declaration);
    temp = delete_white_spaces_start_and_end(temp);
    if (temp == NULL)
    {
        return 0;
    }

    if (strlen(temp) == 0)
    {
        return 0;
    }

    if (temp[0] == ',' || temp[strlen(temp) - 1] == ',')
    {
        return 0;
    }

    num = strtok(strdup(temp), ",");
    num = delete_white_spaces_start_and_end(num);
    if (num == NULL || !is_integer(num))
    {
        return 0;
    }

    while (1)
    {
        num = strtok(NULL, ",");
        num = delete_white_spaces_start_and_end(num);
        if (num == NULL)
        {
            return 1;
        }

        if (!is_integer(num))
        {
            print_error(PROCESS_ERROR_DATA_GUIDE_INVALID_PARAM);
            return 0;
        }
    }
}

int get_command_index(char *word)
{
    int i;

    if (word == NULL)
    {
        return -1;
    }

    for (i = 0; i < 16; i++)
    {
        if (strcmp(word, commands[i]) == 0)
        {
            return i;
        }
    }

    return -1;
}

int is_command(char *word)
{
    return get_command_index(word) != -1;
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

int is_mat_declaration(char *guide_declaration)
{
    // check that next word is [num1][num2] and optional (num1, num2...)
    char *temp;
    temp = strdup(guide_declaration);
    temp = strtok(temp, " ");

    if (temp == NULL || strlen(temp) == 0)
    {
        return 0;
    }

    if (temp[0] != '[' || temp[strlen(temp) - 1] != ']')
    {
        return 0;
    }

    // loop the first [num] and check if valid
    for (temp = temp + 1; *temp != ']'; temp++)
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

    temp = strtok(NULL, "\n");
    if (temp != NULL && strlen(temp) > 0)
    {
        return is_data_guide_declaration(temp);
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

    return ALLOCATION_INVALID;
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
    // check that next word is [reg1][reg2]
    int i = 0;
    char *label, *mat_def, *temp, *reg1, *reg2;
    if (word == NULL)
    {
        return 0;
    }
    if (strlen(word) == 0)
    {
        return 0;
    }

    reg1 = malloc(REG_SIZE);
    if (reg1 == NULL)
    {
        safe_exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
    }

    reg2 = malloc(REG_SIZE);
    if (reg2 == NULL)
    {
        safe_exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
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
            safe_exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
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
    return get_regs_from_mat_allocation(mat_def, reg1, reg2);
}

int get_regs_from_mat_allocation(char *mat_def, char *reg1, char *reg2)
{
    int i = 0;
    char *temp;

    if (mat_def[0] != '[' || mat_def[strlen(mat_def) - 1] != ']')
    {
        return 0;
    }

    temp = strdup(mat_def);
    // loop the first [reg] and check if valid
    for (temp = mat_def + 1; *temp != ']'; temp++)
    {
        if (i > 1)
        {
            return 0;
        }
        reg1[i] = *temp;
        i++;
    }
    reg1[i] = '\0';
    if (!is_register(reg1))
    {
        print_error(PROCESS_ERROR_MAT_ALLOCATION_INVALID_PARAM);
        return 0;
    }

    temp++;
    if (*temp != '[')
    {
        return 0;
    }
    temp++;

    // loop the second [reg] and check if valid
    i = 0;
    for (; *temp != ']'; temp++)
    {
        if (i > 1)
        {
            return 0;
        }
        reg2[i] = *temp;
        i++;
    }
    reg2[i] = '\0';
    if (!is_register(reg2))
    {
        print_error(PROCESS_ERROR_MAT_ALLOCATION_INVALID_PARAM);
        return 0;
    }

    printf("reg1: %s, reg2: %s\n", reg1, reg2);
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