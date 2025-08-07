#include <stdio.h>
#include <stdlib.h>

void free_array_of_strings(char **array, int length)
{
    int i;
    for (i = 0; i < length; i++)
    {
        free(array[i]);
    }
    free(array);
}
