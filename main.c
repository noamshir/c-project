#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct mcro_item
{
    char *name;
    char *value;
} mcro_item;

void pre_assembler();

int main(int argc, char const *argv[])
{
    /* code */
    pre_assembler();
    return 0;
}

void pre_assembler()
{
    FILE *file = fopen("ps.as", "r");
    mcro_item *mcroTable;
    char line[80];
    int isMacro = 0;
    char *word;

    if (file == NULL)
    {
        printf("Error opening file\n");
        return;
    }

    while (fgets(line, sizeof(line), file) != NULL)
    {
        word = strtok(line, " ");
        if (strcmp(word, "mcro") == 0)
        {
            isMacro = 1;
            // find, validate (good mcro name and only mcro name at line) and add its name to the mcro table
            continue;
        }
        else if (strcmp(word, "mcroend") == 0)
        {
            isMacro = 0;
            // remove line from file and validate.
            continue;
        }

        if (isMacro == 1)
        {
            // add line to mcro table (to the current mcro value)
            // remove line from file
            continue;
        }
        else
        {
            // add line to file
        }
    }
}
