#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Headers/files_extractor.h"

/*this function recieves .as file after macros expand and creates .am file*/
void generateAmFile(char *name, char **expandedLines, int lineCount)
{
    int i; /*parameter for loops*/
    FILE *amFile;
    char fileName[300];

    strcpy(fileName, name);  /*creating am file*/
    strcat(fileName, ".am"); /*adding the .am in the end of the name*/

    amFile = fopen(fileName, "w"); /*open the am file that created to writing*/
    if (amFile == NULL)            /* checks for error while the opening*/
    {
        printf("Error: Can't create file %s\n", fileName);
        return;
    }

    for (i = 0; i < lineCount; i++)
    {
        fprintf(amFile, "%s\n", expandedLines[i]); /*adding the expanded line*/
    }

    fclose(amFile);
    printf("Generated file: %s\n", fileName); /*print a message that .am file has been created*/
}

void convertToSpecialBase4(int value, char *output) /*converting the input to a dedicate base 4 code for the object file*/
{
    int i;
    char base4Chars[] = {'a', 'b', 'c', 'd'};

    for (i = 4; i >= 0; i--)
    {
        output[4 - i] = base4Chars[(value >> (i * 2)) & 0x3];
    }

    output[5] = '\0';
}

void generateObFile(char *name, int *code, int codeLength, int *data, int dataLength)
{
    FILE *obFile;
    char fileName[300];
    int i; /*parameter for loops*/
    char encoded[6];

    /*create object file and open it for writing*/
    strcpy(fileName, name);
    strcat(fileName, ".ob");

    obFile = fopen(fileName, "w");
    if (obFile == NULL)
    {
        printf("Error: Cannot create file %s\n", fileName);
        return;
    }

    /* the first line in the file, presents the length of the data and the code*/
    fprintf(obFile, "%d %d\n", codeLength, dataLength);

    for (i = 0; i < codeLength; i++)
    {
        convertToSpecialBase4(code[i], encoded);
        fprintf(obFile, "%04d %s\n", 100 + i, encoded);
    }

    for (i = 0; i < dataLength; i++)
    {
        convertToSpecialBase4(data[i], encoded);
        fprintf(obFile, "%04d %s\n", 100 + codeLength + i, encoded);
    }

    fclose(obFile);
    printf("Generated file: %s\n", fileName);
}

void generateEntryFile(char *name, char **entryLabels, int *addresses, int entryCount)
{
    FILE *entFile;
    char fileName[300];
    int i;

    /* to not create file if there is not lable*/
    if (entryCount == 0)
    {
        return;
    }

    /* create entry file */
    strcpy(fileName, name);
    strcat(fileName, ".ent");

    /* open the file for writing */
    entFile = fopen(fileName, "w");
    if (!entFile)
    {
        printf("Error: Can't create file %s\n", fileName);
        return;
    }

    /*writing lables*/
    for (i = 0; i < entryCount; i++)
    {
        fprintf(entFile, "%s %04d\n", entryLabels[i], addresses[i]);
    }

    fclose(entFile);
    printf("Generated file: %s\n", fileName);
}

/* creates extern file */
void generateExternFile(char *name, char **externLabels, int *addresses, int externCount)
{
    FILE *extFile;
    char fileName[300];
    int i;

    /* to not create file if there is not lable*/
    if (externCount == 0)
    {
        return;
    }

    strcpy(fileName, name);
    strcat(fileName, ".ext");

    extFile = fopen(fileName, "w");
    if (!extFile)
    {
        printf("Error: Cannot create file %s\n", fileName);
        return;
    }

    /*writing lables*/
    for (i = 0; i < externCount; i++)
    {
        fprintf(extFile, "%s %04d\n", externLabels[i], addresses[i]);
    }

    fclose(extFile);
    printf("Generated file: %s\n", fileName);
}
