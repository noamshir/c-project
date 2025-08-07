#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Headers/files_extractor.h"
#include "Headers/consts.h"
#include "Headers/abcd_code.h"

void generate_ob_file(char *name, char ***code, int code_length, char ***data, int data_length)
{
    FILE *ob_file;
    char file_name[256];
    int i; /*parameter for loops*/
    int address_counter = MEMORY_START_ADDRESS;
    char abcd_binary_code[6], abcd_address_code[5], abcd_code_length_code[5], abcd_data_length_code[5];

    printf("generating ob file\n");
    printf("code length: %d, data length: %d\n", code_length, data_length);
    printf("address %d \n", address_counter);
    /*create object file and open it for writing*/
    strcpy(file_name, name);
    strcat(file_name, ".ob");

    ob_file = fopen(file_name, "w");
    if (ob_file == NULL)
    {
        printf("Error: Cannot create file %s\n", file_name);
        return;
    }
    convert_num_to_abcd_base(code_length, abcd_code_length_code);
    convert_num_to_abcd_base(data_length, abcd_data_length_code);

    /* the first line in the file, presents the length of the data and the code*/
    fprintf(ob_file, "%s %s\n", abcd_code_length_code, abcd_data_length_code);

    printf("inserting encoded commands \n");
    for (i = 0; i < code_length; i++)
    {
        convert_num_to_abcd_base(MEMORY_START_ADDRESS + i, abcd_address_code);
        convert_binary_code_to_abcd_base((*code)[i], abcd_binary_code);
        fprintf(ob_file, "%s %s\n", abcd_address_code, abcd_binary_code);

        address_counter++;
    }

    printf("inserting encoded data \n");
    for (i = 0; i < data_length; i++)
    {
        convert_num_to_abcd_base(MEMORY_START_ADDRESS + code_length + i, abcd_address_code);
        convert_binary_code_to_abcd_base((*data)[i], abcd_binary_code);
        fprintf(ob_file, "%s %s\n", abcd_address_code, abcd_binary_code);
        address_counter++;
    }

    fclose(ob_file);
    printf("Generated ob file: %s\n", file_name);
}

void generate_entry_file(char *name, char **entry_labels, int *addresses, int entry_count)
{
    FILE *fp;
    char file_name[256], encoded[5];
    int i;

    printf("generating entry file\n");

    /* to not create file if there is not lable */
    if (entry_count == 0)
    {
        return;
    }

    /* create entry file */
    strcpy(file_name, name);
    strcat(file_name, ".ent");

    /* open the file for writing */
    fp = fopen(file_name, "w");
    if (!fp)
    {
        printf("Error: Can't create file %s\n", file_name);
        return;
    }

    /*writing labels*/
    for (i = 0; i < entry_count; i++)
    {
        convert_num_to_abcd_base(addresses[i], encoded);
        fprintf(fp, "%s %s\n", entry_labels[i], encoded);
    }

    fclose(fp);
    printf("Generated entry file: %s\n", file_name);
}

/* creates extern file */
void generate_extern_file(char *name, char **extern_labels, int *addresses, int extern_count)
{
    FILE *fp;
    char file_name[256], encoded[5];
    int i;

    /* to not create file if there is not lable*/
    if (extern_count == 0)
    {
        return;
    }

    strcpy(file_name, name);
    strcat(file_name, ".ext");

    fp = fopen(file_name, "w");
    if (!fp)
    {
        printf("Error: Cannot create file %s\n", file_name);
        return;
    }

    /*writing lables*/
    for (i = 0; i < extern_count; i++)
    {
        convert_num_to_abcd_base(addresses[i], encoded);
        fprintf(fp, "%s %s\n", extern_labels[i], encoded);
    }

    fclose(fp);
    printf("Generated extern file: %s\n", file_name);
}
