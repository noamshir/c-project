#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Headers/pre_assembler.h"
#include "Headers/table.h"
#include "Headers/string.h"
#include "Headers/error.h"

void pre_assembler()
{
    int valid;
    FILE *fp;
    char *file_name = "ps.as";
    char *temp_file_name, str[80];
    mcro_item *mcro_table;

    valid = fill_mcro_table(file_name, &mcro_table);

    if (!valid)
    {
        exit(1);
    }
    temp_file_name = remove_mcro_defines(file_name);

    if (temp_file_name == NULL)
    {
        exit(1);
    }

    temp_file_name = replace_mcro_defines(&mcro_table, temp_file_name);

    /* good for debug */
    fp = fopen(temp_file_name, "r");
    if (fp == NULL)
    {
        exit(1);
    }

    while (fgets(str, 80, fp) != NULL)
    {
        printf("line: %s", str);
    }
}

int fill_mcro_table(char *file_name, mcro_item **mcro_table)
{
    FILE *file;
    mcro_item *current_mcro = NULL;
    char line[80];
    char *word, *next_word, *third_word;
    int isMacro = 0;

    file = fopen(file_name, "r");
    if (file == NULL)
    {
        printf("error code is %d\n", PROCESS_ERROR_FAILED_TO_OPEN_FILE);
        return 0;
    }

    while (fgets(line, sizeof(line), file) != NULL)
    {

        word = strtok(strdup(line), " ");
        delete_white_spaces(word);

        if (strcmp(word, "mcro") == 0)
        {

            isMacro = 1;
            next_word = strtok(NULL, " ");
            if (next_word == NULL)
            {
                fclose(file);
                return 0;
            }

            delete_white_spaces(next_word);
            if (!isMcroNameValid(next_word))
            {
                fclose(file);
                printf("error code is %d\n", PROCESS_ERROR_INVALID_MACRO_NAME);
                return 0;
            }

            third_word = strtok(NULL, " ");
            if (third_word != NULL)
            {
                fclose(file);
                printf("error code is %d\n", PROCESS_ERROR_INVALID_MACRO_DECLARATION);
                return 0;
            }

            current_mcro = add_item(mcro_table, strdup(next_word), NULL);

            continue;
        }
        else if (strcmp(word, "mcroend") == 0)
        {
            next_word = strtok(NULL, " ");
            if (next_word != NULL)
            {
                fclose(file);
                printf("error code is %d\n", PROCESS_ERROR_INVALID_MACRO_END_DECLARATION);
                return 0;
            }
            isMacro = 0;
            current_mcro = NULL;
            continue;
        }
        else if (isMacro == 1)
        {
            append_item_value(current_mcro, line);
            continue;
        }
    }

    fclose(file);
    return 1;
}

int isMcroNameValid(char *name)
{
    int i;
    char *commands[] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "jsr", "red", "prn", "rts", "stop"};
    char *instructions[] = {".data", ".string", ".mat", ".entry", ".extern"};

    if (name == NULL)
    {
        return 0;
    }
    if (strlen(name) == 0)
    {
        return 0;
    }

    /* check if name is in commands */
    for (i = 0; i < (sizeof(commands) / sizeof(commands[0])); i++)
    {

        if (strcmp(name, commands[i]) == 0)
        {
            return 0;
        }
    }

    /* check if name is in instructions*/
    for (i = 0; i < (sizeof(instructions) / sizeof(instructions[0])); i++)
    {
        if (strcmp(name, instructions[i]) == 0)
        {
            return 0;
        }
    }
    return 1;
}

char *remove_mcro_defines(char file_name[])
{
    int found_mcro = 0;
    char *token, *temp_file;
    char line[80], line_copy[80];
    FILE *original_file_pointer, *temp_file_pointer;

    original_file_pointer = fopen(file_name, "r");
    if (original_file_pointer == NULL)
    {
        printf("error code is %d\n", PROCESS_ERROR_FAILED_TO_OPEN_FILE);
        return NULL;
    }

    temp_file = strcat(get_file_name_without_extension(file_name), ".am");
    temp_file_pointer = fopen(temp_file, "w");
    if (temp_file_pointer == NULL)
    {
        fclose(original_file_pointer);
        printf("error code is %d\n", PROCESS_ERROR_FAILED_TO_OPEN_FILE);
        return NULL;
    }

    while (fgets(line, 80, original_file_pointer))
    {
        strcpy(line_copy, line);
        token = strtok(line, " \n");

        if (strcmp(token, "mcroend") == 0)
        {
            found_mcro = 0;
            continue;
        }

        if (found_mcro == 1)
        {
            continue;
        }

        if (token == NULL)
        {
            /* empty lines */
            continue;
        }

        if (strcmp(token, "mcro") == 0)
        {
            found_mcro = 1;
            continue;
        }
        else
        {
            fprintf(temp_file_pointer, "%s", line_copy);
        }
    }

    /* close the files we opened */
    fclose(original_file_pointer);
    fclose(temp_file_pointer);

    return temp_file;
}

char *replace_mcro_defines(mcro_item **mcro_table, char file_name[])
{
    char *token, *content;
    mcro_item *table_item;
    char line[80], line_copy[80];
    FILE *original_file_pointer, *temp_file_pointer;

    original_file_pointer = fopen(file_name, "r");
    if (original_file_pointer == NULL)
    {
        printf("error code is %d\n", PROCESS_ERROR_FAILED_TO_OPEN_FILE);
        return NULL;
    }

    temp_file_pointer = fopen("temp2.as", "w");
    if (temp_file_pointer == NULL)
    {
        printf("error code is %d\n", PROCESS_ERROR_FAILED_TO_OPEN_FILE);
        return NULL;
    }

    while (fgets(line, 80, original_file_pointer))
    {
        strcpy(line_copy, line);
        token = strtok(line, " \n");

        delete_white_spaces(token);

        table_item = find_by_name(*mcro_table, token);
        if (table_item == NULL)
        {
            fprintf(temp_file_pointer, "%s", line_copy);
            continue;
        }
        /* replace line with the content */
        content = table_item->value;
        fprintf(temp_file_pointer, "%s", content);
    }

    fclose(original_file_pointer);
    fclose(temp_file_pointer);

    remove(file_name);
    rename("temp2.as", file_name);

    return file_name;
}
