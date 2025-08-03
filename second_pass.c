#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Headers/pre_assembler.h"
#include "Headers/table.h"
#include "Headers/string.h"
#include "Headers/error.h"
#include "Headers/symbol_table.h"
#include "Headers/first_pass.h"
#include "Headers/utils.h"
#include "Headers/consts.h"
#include "Headers/binary_code.h"
#include "Headers/files_extractor.h"
#include "Headers/second_pass.h"

void second_pass(char *file_name_without_postfix, symbol_item *symbol_table, char **array_of_commands, int IC, char **array_of_data, int DC)
{
    FILE *file;
    char *file_name;
    char line[LINE_SIZE]; //one line in a file
    char *word, *main_op;
    int line_num = 0;
    int error_flag = 0;

    char **entryLabels = NULL;
    int *entryAddresses = NULL;
    int entryCount = 0;

    char **externLabels = NULL;
    int *externAddresses = NULL;
    int externCount = 0;

    printf("second pass started\n");

    file_name = malloc(strlen(file_name_without_postfix) + 4); 
    if (file_name == NULL) 
    {
        exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
    }

    /*adding ".am" at the end of the file name*/
    strcpy(file_name, file_name_without_postfix);
    strcat(file_name, ".am");

    /*open the file for reading and check for error*/
    file = fopen(file_name, "r");
    if (file == NULL) 
    {
        printf("error code is %d\n", PROCESS_ERROR_FAILED_TO_OPEN_FILE);
        free(file_name);
        return;
    }

    /*pass on each line at the file*/
    while (fgets(line, sizeof(line), file) != NULL) 
    {
        line_num++;
        printf("line %d: %s\n", line_num, line); //print line & line number (for errors analyse)

        if (is_empty_line(line) || is_comment_line(line)) //ignore empty\comment lines
        {
            continue; 
        }

        /*get the first word in the line*/
        word = strtok(strdup(line), " ");
        delete_white_spaces(word);

        /*ignore lables words and save the command in param*/
        if (is_label(word)) 
        {
            main_op = strtok(NULL, " ");
            delete_white_spaces(main_op);
        } 
        else 
        {
            main_op = strdup(word);
        }

        /*check if the wors is a command that handled in first pass*/
        if (is_data_guide(main_op) || is_string_guide(main_op) || is_mat_guide(main_op) || is_extern_guide(main_op)) 
        {
            continue; // ignore in second pass
        }

        /*handle '.entry'*/
        if (is_entry_guide(main_op))
        {
            char *entry_label = strtok(NULL, " \t\n");

            if (!add_entry_attribute(symbol_table, entry_label)) 
            {
                printf("Error: Symbol %s not found in symbol table (line %d)\n", entry_label, line_num);
                error_flag = 1;
            } 
            else 
            {
                // make "entry" list"
                symbol_item *sym = find_symbol(symbol_table, entry_label);
                if (sym != NULL) {
                    entryLabels = realloc(entryLabels, sizeof(char*) * (entryCount + 1));
                    entryAddresses = realloc(entryAddresses, sizeof(int) * (entryCount + 1));
                    entryLabels[entryCount] = strdup(entry_label);
                    entryAddresses[entryCount] = sym -> address;
                    entryCount++;
                }
            }
        } 
        else 
        {
            /*if it's command line, need to encode it encode it*/
            if (!encode_second_pass_operands(symbol_table, line, array_of_commands, &externLabels, &externAddresses, &externCount)) 
            {
                printf("Line %d: Error while encoding operands.\n", line_num);
                error_flag = 1;
            }
        }
    
    }

    /* creation of final files (.ob, .entry, .extrn)*/
    generateObFile(file_name_without_postfix, array_of_commands, IC, array_of_data, DC);

    if (entryCount > 0) // to not create entry file if there are no entries
    {
        generateEntryFile(file_name_without_postfix, entryLabels, entryAddresses, entryCount);
    }
    if (externCount > 0) // to not create extern file if there are no externs
    {
        generateExternFile(file_name_without_postfix, externLabels, externAddresses, externCount);
    }

    // cleaning memory
    for (int i = 0; i < entryCount; i++) free(entryLabels[i]);
    free(entryLabels);
    free(entryAddresses);

    for (int i = 0; i < externCount; i++) free(externLabels[i]);
    free(externLabels);
    free(externAddresses);

    free(file_name);

    /*print a message for final*/
    if (!error_flag) 
    {
        printf("Second pass finished successfully.\n");
    } 
    else 
    {
        printf("Second pass finished with errors.\n");
    }

    
}


