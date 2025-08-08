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

char *get_file_name_without_extension(char *file_name)
{
    char *temp = duplicate_str(file_name);
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
    if (line == NULL)
    {
        return 1;
    }

    while (*line != '\0')
    {
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

int is_mcro_name_valid(char *name)
{
    if (name == NULL)
    {
        return 0;
    }
    if (strlen(name) == 0)
    {
        return 0;
    }

    if (is_command(name) || is_guide(name))
    {
        return 0;
    }

    while (*name != '\0')
    {
        if (!is_char_alphabetical_or_digit(*name) && *name != '_')
        {
            return 0;
        }
        name++;
    }

    return 1;
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

    while (*str != '\0')
    {
        if (!is_char_alphabetical_or_digit(*str))
        {
            return 0;
        }
        str++;
    }
    return 1;
}

int is_label_declaration(char *word)
{
    int end_of_string, i;
    if (word == NULL)
    {
        return 0;
    }
    if (strlen(word) == 0 || strlen(word) == 1 || strlen(word) > LABEL_SIZE)
    {
        return 0;
    }

    end_of_string = strlen(word) - 1;
    if (word[end_of_string] == ':')
    {
        return 1;
    }

    /* check that each char is abc or 0-9 */
    for (i = 0; i < end_of_string; i++)
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
    if (!is_label_declaration(word))
    {
        return NULL;
    }

    /* remove last char in word (:) */
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

    /* check that guide_declaration is of type: "num1, num2, ...., numn" */
    printf("checking data guide declaration: %s\n", guide_declaration);

    temp = duplicate_str(guide_declaration);
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

    num = strtok(duplicate_str(temp), ",");
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

int is_mat_declaration(char *guide_declaration)
{
    /* check that next word is [num1][num2] and optional (num1, num2...) */
    char *temp;
    temp = duplicate_str(guide_declaration);
    temp = strtok(temp, " ");

    if (temp == NULL || strlen(temp) == 0)
    {
        return 0;
    }

    if (temp[0] != '[' || temp[strlen(temp) - 1] != ']')
    {
        return 0;
    }

    /* loop the first [num] and check if valid */
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

    /* loop the second [num] and check if valid */
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
    int i;
    /* check if word is register */
    for (i = 0; i < 8; i++)
    {
        if (strcmp(word, registers[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
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
    /* check if str is of type #num */
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

    /* rest is from index 1 to end */
    rest = word + 1;
    if (is_integer(rest))
    {
        return 1;
    }

    return 0;
}

int is_mat_allocation(char *word)
{
    /* check that next word is [reg1][reg2] */
    int i = 0;
    char label[LABEL_SIZE], *mat_def, *temp, *reg1, *reg2;
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
    /* no [ sign? cant be a mat def... */
    if (temp == NULL)
    {
        return 0;
    }

    /* build label from mat allocation */
    temp = duplicate_str(word);
    while (*temp != '[')
    {
        label[i] = *temp;
        i++;
        temp++;
    }
    label[i] = '\0';

    i = 0;
    if (!is_valid_label_name(label))
    {
        return 0;
    }

    /* temp is now [reg1][reg2] (without the label) */
    mat_def = duplicate_str(temp);
    return set_regs_from_mat_allocation(mat_def, reg1, reg2);
}

int set_regs_from_mat_allocation(char *mat_def, char *reg1, char *reg2)
{
    int i = 0;
    char *temp;

    if (mat_def[0] != '[' || mat_def[strlen(mat_def) - 1] != ']')
    {
        return 0;
    }

    temp = duplicate_str(mat_def);
    /* loop the first [reg] and check if valid */
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

    /* loop the second [reg] and check if valid */
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
    int i;
    /* check that each char is abc or 0-9 */
    for (i = 0; i < strlen(word); i++)
    {
        if (!is_char_alphabetical_or_digit(word[i]))
        {
            return 0;
        }
    }

    return 1;
}
