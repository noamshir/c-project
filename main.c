#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Headers/table.h"

void pre_assembler();

void fill_mcro_table(char *file_name, mcro_item **mcro_table);

int main(int argc, char const *argv[])
{
    /* code */
    pre_assembler();
    return 0;
}

void pre_assembler()
{
    char *file_name = "ps.as";
    mcro_item *mcro_table;
    fill_mcro_table(file_name, &mcro_table);
}

void fill_mcro_table(char *file_name, mcro_item **mcro_table)
{
    FILE *file = fopen(file_name, "r");
    mcro_item *current_mcro;
    char line[80];
    char *word, *nextWord;
    int isMacro = 0;

    if (file == NULL)
    {
        printf("Error opening file\n");
        return;
    }

    while (fgets(line, sizeof(line), file) != NULL)
    {
        word = strtok(line, " ");
        printf("%s\n", word);
        if (strcmp(word, "mcro") == 0)
        {
            isMacro = 1;
            nextWord = strtok(NULL, " ");
            if (nextWord == NULL)
            {
                printf("No name for mcro! \n");
                return;
            }

            printf("next word: %s\n", nextWord);
            current_mcro = add_item(mcro_table, nextWord, NULL);
            continue;
        }
        else if (strcmp(word, "mcroend") == 0)
        {
            isMacro = 0;
            current_mcro = NULL;
            continue;
        }
        else if (isMacro == 1)
        {
            printf("%s\n", word);
            current_mcro->value = strcat(current_mcro->value, word);
            printf("%s\n", word);
            continue;
        }
    }
}
