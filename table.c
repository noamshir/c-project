#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Headers/table.h"
#include "Headers/error.h"

mcro_item *create_item(char *name, char *value)
{
    mcro_item *item = malloc(sizeof(mcro_item));
    if (item == NULL)
    {
        exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
    }
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
        temp = malloc(sizeof(mcro_item));

        if (temp == NULL)
        {
            exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
        }
        temp = *head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = malloc(sizeof(mcro_item));
        if (temp->next == NULL)
        {
            exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
        }
        temp->next = item;
    }
    return item;
}

void append_item_value(mcro_item *item, char *value)
{
    if (item->value == NULL)
    {
        /* allocate memory and copy the value */
        item->value = malloc(strlen(value) + 1);
        if (item->value == NULL)
        {
            printf("malloc failed");
            exit(1);
        }
        strcpy(item->value, value);
    }
    else
    {
        /* Append to existing string */
        size_t new_len = strlen(item->value) + strlen(value) + 1;
        char *temp = realloc(item->value, new_len);
        if (temp == NULL)
        {
            printf("realloc failed");
            free(item->value);
            exit(1);
        }
        item->value = temp;
        strcat(item->value, value);
    }
}

mcro_item *find_by_name(mcro_item *head, char *name)
{
    if (head == NULL)
    {
        return NULL;
    }

    if (strcmp(name, head->name) == 0)
    {

        return head;
    }

    return find_by_name(head->next, name);
}
