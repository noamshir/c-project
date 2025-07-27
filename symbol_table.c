#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Headers/symbol_table.h"
#include "Headers/error.h"

symbol_item *create_symbol_item(char *name, char *type, int address)
{
    symbol_item *item = malloc(sizeof(symbol_item));
    if (item == NULL)
    {
        exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
    }
    item->name = name;
    item->address = address;
    item->type = "data";
    item->next = NULL;
    return item;
}

void add_label_to_symbol_table(symbol_item **head, char *name, char *type, int address)
{
    symbol_item *item, *temp;
    item = create_symbol_item(name, type, address);
    if (*head == NULL)
    {
        *head = item;
    }
    else
    {
        temp = malloc(sizeof(symbol_item));

        if (temp == NULL)
        {
            exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
        }
        temp = *head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = malloc(sizeof(symbol_item));
        if (temp->next == NULL)
        {
            exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
        }
        temp->next = item;
    }
}