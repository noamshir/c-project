#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Headers/pre_assembler.h"
#include "Headers/mcro_table.h"
#include "Headers/string.h"
#include "Headers/error.h"
#include "Headers/consts.h"

int pre_assembler(char *file_name_without_postfix)
{
    FILE *as_fp, *am_fp;
    char *am_file_name, *as_file_name;
    mcro_item *mcro_table = NULL;

    printf("pre assembler started\n");

    /* allocates memory to the file name (with .as postfix )*/
    as_file_name = malloc(strlen(file_name_without_postfix) + 4);
    if (as_file_name == NULL)
    {
        exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
    }

    /* add the .as postfix to the file name */
    strcpy(as_file_name, file_name_without_postfix);
    strcat(as_file_name, ".as");

    as_fp = fopen(as_file_name, "r");
    if (as_fp == NULL)
    {
        free_mcro_table(mcro_table);
        free(as_file_name);
        print_error(PROCESS_ERROR_FAILED_TO_OPEN_FILE);
        return 0;
    }

    if (!fill_mcro_table(as_fp, &mcro_table))
    {
        free_mcro_table(mcro_table);
        free(as_file_name);
        fclose(as_fp);
        return 0;
    }

    am_file_name = malloc(strlen(file_name_without_postfix) + 4);
    if (am_file_name == NULL)
    {
        exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
    }

    strcpy(am_file_name, file_name_without_postfix);
    strcat(am_file_name, ".am");

    am_fp = fopen(am_file_name, "w+");
    if (am_fp == NULL)
    {
        free_mcro_table(mcro_table);
        fclose(as_fp);
        free(as_file_name);
        free(am_file_name);
        print_error(PROCESS_ERROR_FAILED_TO_OPEN_FILE);
        return 0;
    }

    remove_mcro_defines(as_fp, am_fp);

    /* close files */
    fclose(am_fp);
    fclose(as_fp);

    if (!replace_mcro_defines(&mcro_table, am_file_name))
    {
        free_mcro_table(mcro_table);
        free(as_file_name);
        free(am_file_name);
        fclose(as_fp);
        return 0;
    }

    /* free file names & mcro table */
    free_mcro_table(mcro_table);
    free(as_file_name);
    free(am_file_name);
    printf("pre assembler finished\n");

    return 1;
}

int fill_mcro_table(FILE *file, mcro_item **mcro_table)
{
    mcro_item *current_mcro = NULL;
    char line[LINE_SIZE + 1];
    char *first_word, *second_word, *third_word;
    int isMacro = 0, line_num = 0;

    printf("started filling mcro table\n");
    rewind(file);

    while (fgets(line, sizeof(line), file) != NULL)
    {
        line_num++;
        if (is_line_too_long(line))
        {
            printf("line (%d) is too long\n", line_num);
            return 0;
        }

        first_word = strtok(strdup(line), " ");
        first_word = delete_white_spaces_start_and_end(first_word);

        /* checks if the first word is mcro declaration */
        if (strcmp(first_word, "mcro") == 0)
        {
            isMacro = 1;
            second_word = strtok(NULL, " ");
            second_word = delete_white_spaces_start_and_end(second_word);

            /* if the first word is mcro than the second word in line is its name */
            if (!is_mcro_name_valid(second_word))
            {
                print_error(PROCESS_ERROR_INVALID_MACRO_NAME);
                return 0;
            }

            /* a valid mcro declaration should have only 2 words... */
            third_word = strtok(NULL, " ");
            if (third_word != NULL)
            {
                print_error(PROCESS_ERROR_INVALID_MACRO_DECLARATION);
                return 0;
            }

            /* if we got here mcro declaration is valid, add it to the mcro table! */
            current_mcro = add_mcro_item(mcro_table, strdup(second_word), NULL);
            continue;
        }
        /* checks if the first word is mcroend declaration */
        else if (strcmp(first_word, "mcroend") == 0)
        {
            /* a valid mcroend declaration should have only 1 word... */
            second_word = strtok(NULL, " ");
            second_word = delete_white_spaces_start_and_end(second_word);
            if (!is_empty_line(second_word))
            {
                print_error(PROCESS_ERROR_INVALID_MACRO_END_DECLARATION);
                return 0;
            }
            isMacro = 0;
            current_mcro = NULL;
            continue;
        }
        /* part of the mcro content, should add its value to the current mcro */
        else if (isMacro == 1)
        {
            append_string_to_mcro_item_value(current_mcro, line);
            continue;
        }
    }

    printf("finished filling mcro table\n");
    return 1;
}

void remove_mcro_defines(FILE *as_fp, FILE *am_fp)
{
    int found_mcro = 0;
    char *token;
    char line[LINE_SIZE], line_copy[LINE_SIZE];

    printf("started removing mcro defines\n");

    /* rewind files */
    rewind(as_fp);
    rewind(am_fp);

    while (fgets(line, LINE_SIZE, as_fp) != NULL)
    {
        strcpy(line_copy, line);
        token = strtok(line, " \n");
        token = delete_white_spaces_start_and_end(token);

        if (token == NULL)
        {
            /* empty lines */
            fprintf(am_fp, "%s", line_copy);
            continue;
        }

        if (strcmp(token, "mcroend") == 0)
        {
            /* turn mcro flag off, no need to write line */
            found_mcro = 0;
            continue;
        }

        if (found_mcro == 1)
        {
            /* inside a mcro definition, no need to write line */
            continue;
        }

        if (strcmp(token, "mcro") == 0)
        {
            /* turn mcro flag on, no need to write line */
            found_mcro = 1;
            continue;
        }
        else
        {
            /* outside a mcro definition, write line */
            fprintf(am_fp, "%s", line_copy);
        }
    }
    printf("finished removing mcro defines\n");
}

int replace_mcro_defines(mcro_item **mcro_table, char *file_name)
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
        return 0;
    }

    /* create temp file */
    temp_file_pointer = fopen("temp2.as", "w");
    if (temp_file_pointer == NULL)
    {
        print_error(PROCESS_ERROR_FAILED_TO_OPEN_FILE);
        return 0;
    }

    while (fgets(line, LINE_SIZE, original_file_pointer))
    {
        strcpy(line_copy, line);
        token = strtok(line, " \n");
        token = delete_white_spaces_start_and_end(token);

        if (token == NULL)
        {
            /* empty lines */
            fprintf(temp_file_pointer, "%s", line_copy);
            continue;
        }

        table_item = find_mcro_item_by_name(*mcro_table, token);
        if (table_item == NULL)
        {
            /* not mcro name or def, needs to be added */
            fprintf(temp_file_pointer, "%s", line_copy);
            continue;
        }

        /* a mcro def, replace line with the content */
        content = table_item->value;
        fprintf(temp_file_pointer, "%s", content);
    }

    fclose(original_file_pointer);
    fclose(temp_file_pointer);

    remove(file_name);
    rename("temp2.as", file_name);

    printf("finished replacing mcro defines\n");
    return 1;
}
