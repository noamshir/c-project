#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Headers/pre_assembler.h"
#include "Headers/mcro_table.h"
#include "Headers/string.h"
#include "Headers/error.h"
#include "Headers/consts.h"

void pre_assembler(char *file_name_without_postfix)
{
    int valid;
    FILE *fp;
    char *temp_file_name, *file_name, str[LINE_SIZE];
    mcro_item *mcro_table = NULL;

    printf("pre assembler started\n");

    file_name = malloc(strlen(file_name_without_postfix) + 4);
    if (file_name == NULL)
    {
        exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
    }

    strcpy(file_name, file_name_without_postfix);
    strcat(file_name, ".as");

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

    while (fgets(str, LINE_SIZE, fp) != NULL)
    {
        printf("line: %s", str);
    }

    fclose(fp);
    free(file_name);
    printf("pre assembler finished\n");
}

int fill_mcro_table(char *file_name, mcro_item **mcro_table)
{
    FILE *file;
    mcro_item *current_mcro = NULL;
    char line[LINE_SIZE + 1];
    char *word, *next_word, *third_word;
    int isMacro = 0, line_num = 0;

    printf("started filling mcro table\n");

    file = fopen(file_name, "r");
    if (file == NULL)
    {
        fclose(file);
        print_error(PROCESS_ERROR_FAILED_TO_OPEN_FILE);
        return 0;
    }

    while (fgets(line, sizeof(line), file) != NULL)
    {
        line_num++;
        if (is_line_too_long(line))
        {
            printf("line (%d) is too long\n", line_num);
            fclose(file);
            print_error(PROCESS_ERROR_LINE_TOO_LONG);
            return 0;
        }

        word = strtok(strdup(line), " ");
        if (strcmp(word, "mcro") == 0)
        {
            isMacro = 1;
            next_word = strtok(NULL, " ");
            next_word = delete_white_spaces_start_and_end(next_word);
            if (!is_mcro_name_valid(next_word))
            {
                fclose(file);
                print_error(PROCESS_ERROR_INVALID_MACRO_NAME);
                return 0;
            }

            third_word = strtok(NULL, " ");
            if (third_word != NULL)
            {
                fclose(file);
                print_error(PROCESS_ERROR_INVALID_MACRO_DECLARATION);
                return 0;
            }

            current_mcro = add_mcro_item(mcro_table, strdup(next_word), NULL);

            continue;
        }
        else if (strcmp(delete_white_spaces_start_and_end(word), "mcroend") == 0)
        {
            next_word = strtok(NULL, " ");
            if (next_word != NULL)
            {
                fclose(file);
                print_error(PROCESS_ERROR_INVALID_MACRO_END_DECLARATION);
                return 0;
            }
            isMacro = 0;
            current_mcro = NULL;
            continue;
        }
        else if (isMacro == 1)
        {
            append_string_to_mcro_item_value(current_mcro, line);
            continue;
        }
    }

    fclose(file);
    printf("finished filling mcro table\n");
    return 1;
}

char *remove_mcro_defines(char *file_name)
{
    int found_mcro = 0;
    char *token, *temp_file;
    char line[LINE_SIZE], line_copy[LINE_SIZE];
    FILE *original_file_pointer, *temp_file_pointer;

    printf("started removing mcro defines\n");

    original_file_pointer = fopen(file_name, "r");
    if (original_file_pointer == NULL)
    {
        print_error(PROCESS_ERROR_FAILED_TO_OPEN_FILE);
        return NULL;
    }

    temp_file = strcat(get_file_name_without_extension(file_name), ".am");
    temp_file_pointer = fopen(temp_file, "w");
    if (temp_file_pointer == NULL)
    {
        fclose(original_file_pointer);
        print_error(PROCESS_ERROR_FAILED_TO_OPEN_FILE);
        return NULL;
    }

    while (fgets(line, LINE_SIZE, original_file_pointer))
    {
        strcpy(line_copy, line);
        token = strtok(line, " \n");

        if (token == NULL)
        {
            /* empty lines */
            fprintf(temp_file_pointer, "%s", line_copy);
            continue;
        }

        if (strcmp(token, "mcroend") == 0)
        {
            found_mcro = 0;
            continue;
        }

        if (found_mcro == 1)
        {
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

    printf("finished removing mcro defines\n");
    return temp_file;
}

char *replace_mcro_defines(mcro_item **mcro_table, char *file_name)
{
    char *token, *content;
    mcro_item *table_item;
    char line[LINE_SIZE], line_copy[LINE_SIZE];
    FILE *original_file_pointer, *temp_file_pointer;

    printf("started replacing mcro defines\n");
    original_file_pointer = fopen(file_name, "r");
    if (original_file_pointer == NULL)
    {
        print_error(PROCESS_ERROR_FAILED_TO_OPEN_FILE);
        return NULL;
    }

    temp_file_pointer = fopen("temp2.as", "w");
    if (temp_file_pointer == NULL)
    {
        print_error(PROCESS_ERROR_FAILED_TO_OPEN_FILE);
        return NULL;
    }

    while (fgets(line, LINE_SIZE, original_file_pointer))
    {
        strcpy(line_copy, line);
        token = strtok(line, " \n");
        token = delete_white_spaces_start_and_end(token);

        if (token == NULL)
        {
            fprintf(temp_file_pointer, "%s", line_copy);
            continue;
        }

        table_item = find_mcro_item_by_name(*mcro_table, token);
        if (table_item == NULL)
        {
            fprintf(temp_file_pointer, "%s", line_copy);
            continue;
        }
        /* replace line with the content */
        content = table_item->value;
        printf("replacing %s with %s\n", token, content);
        fprintf(temp_file_pointer, "%s", content);
    }

    fclose(original_file_pointer);
    fclose(temp_file_pointer);

    remove(file_name);
    rename("temp2.as", file_name);

    printf("finished replacing mcro defines\n");
    return file_name;
}
