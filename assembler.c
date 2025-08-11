#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Headers/pre_assembler.h"
#include "Headers/first_pass.h"

/**
 * @authors Noam Shir Melzer and Eliran Bechor.
 * @brief Maman 14 - Assembler Project (2025 b).
 */
int main(int argc, char *argv[])
{
    char *file_name;
    mcro_item *mcro_table = NULL;

    printf("starting assembler\n");

    while (--argc > 0)
    {
        file_name = argv[argc];
        printf("processing file: %s\n", file_name);

        /* pre process file */
        if (!pre_assembler(file_name, &mcro_table))
        {
            free_mcro_table(mcro_table);
            printf("pre process for file: %s failed\n", file_name);
            continue;
        }

        /* first pass */
        if (!first_pass(file_name, &mcro_table))
        {
            free_mcro_table(mcro_table);
            continue;
        }

        free_mcro_table(mcro_table);
        printf("finished processing file: %s\n", file_name);
    }

    printf("finished assembler\n");
    return 0;
}
