#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Headers/table.h"

mcro_item *create_item(char *name, char *value)
{
    mcro_item *item = malloc(sizeof(mcro_item));
    item->name = name;
    item->value = value;
    item->next = NULL;
    return item;
}

mcro_item *add_item(mcro_item **head, char *name, char *value)
{
    mcro_item *item, *temp;
    item = create_item(name, value);
    if (*head == NULL)
    {
        *head = item;
    }
    else
    {
        temp = *head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = item;
    }

    return item;
}
