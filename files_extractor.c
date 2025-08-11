#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Headers/files_extractor.h"
#include "Headers/consts.h"
#include "Headers/abcd_code.h"
#include "Headers/error.h"
#include "Headers/utils.h"

void generate_ob_file(char *name, unsigned int *commands, int command_length, unsigned int *data, int data_length)
{
    FILE *ob_file;
    char *file_name;
    int i;
    int address_counter = MEMORY_START_ADDRESS;
    char abcd_binary_code[6], abcd_address_code[6], abcd_code_length_code[6], abcd_data_length_code[6];

    printf("generating ob file\n");

    /*create object file and open it for writing*/
    file_name = build_file_name_with_postfix(name, ".ob");
    ob_file = fopen(file_name, "w");
    if (ob_file == NULL)
    {
        print_error(PROCESS_ERROR_FAILED_TO_OPEN_FILE);
        return;
    }

    /* add command and data lengths to the title*/
    convert_num_to_abcd_base(command_length, abcd_code_length_code);
    convert_num_to_abcd_base(data_length, abcd_data_length_code);

    /* the first line in the file, presents the length of the data and the code*/
    fprintf(ob_file, "%s\t%s\n", abcd_code_length_code, abcd_data_length_code);

    /* add command binary codes and their addresses to the ob file */
    for (i = 0; i < command_length; i++)
    {
        convert_num_to_abcd_base(MEMORY_START_ADDRESS + i, abcd_address_code);
        convert_binary_code_to_abcd_base(commands[i], abcd_binary_code);
        fprintf(ob_file, "%s\t%s\n", abcd_address_code, abcd_binary_code);

        address_counter++;
    }

    /* add data binary codes and their addresses to the ob file */
    for (i = 0; i < data_length; i++)
    {
        convert_num_to_abcd_base(MEMORY_START_ADDRESS + command_length + i, abcd_address_code);
        convert_binary_code_to_abcd_base(data[i], abcd_binary_code);
        fprintf(ob_file, "%s\t%s\n", abcd_address_code, abcd_binary_code);
        address_counter++;
    }

    printf("Generated ob file: %s\n", file_name);

    fclose(ob_file);
    free(file_name);
}

void generate_entry_file(char *name, char **entry_labels, int *addresses, int entry_count)
{
    FILE *fp;
    char *file_name, encoded[5];
    int i;

    printf("generating entry file\n");

    /* to not create file if there is not lable */
    if (entry_count == 0)
    {
        return;
    }

    /* create entry file */
    file_name = build_file_name_with_postfix(name, ".ent");
    /* open the file for writing */
    fp = fopen(file_name, "w");
    if (!fp)
    {
        print_error(PROCESS_ERROR_FAILED_TO_OPEN_FILE);
        return;
    }

    /*writing labels*/
    for (i = 0; i < entry_count; i++)
    {
        convert_num_to_abcd_base(addresses[i], encoded);
        fprintf(fp, "%s %s\n", entry_labels[i], encoded);
    }

    printf("Generated entry file: %s\n", file_name);

    fclose(fp);
    free(file_name);
}

/* creates extern file */
void generate_extern_file(char *name, char **extern_labels, int *addresses, int extern_count)
{
    FILE *fp;
    char *file_name, encoded[5];
    int i;

    /* to not create file if there is not lable*/
    if (extern_count == 0)
    {
        return;
    }

    printf("generating extern file\n");
    /* create extern file */
    file_name = build_file_name_with_postfix(name, ".ext");
    /* open the file for writing */
    fp = fopen(file_name, "w");
    if (!fp)
    {
        print_error(PROCESS_ERROR_FAILED_TO_OPEN_FILE);
        return;
    }

    /*writing labels*/
    for (i = 0; i < extern_count; i++)
    {
        convert_num_to_abcd_base(addresses[i], encoded);
        fprintf(fp, "%s %s\n", extern_labels[i], encoded);
    }

    printf("Generated extern file: %s\n", file_name);

    fclose(fp);
    free(file_name);
}
