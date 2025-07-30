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
    item->name = strdup(name);
    item->address = address;
    item->type = strdup(type);
    item->next = NULL;
    return item;
}

void add_label_to_symbol_table(symbol_item **head, char *name, char *type, int address)
{
    symbol_item *item, *temp;

    item = create_symbol_item(name, type, address);

    if (find_symbol_item_by_name(*head, name) != NULL)
    {
        printf("error code is %d\n", PROCESS_ERROR_SYMBOL_ALREADY_EXISTS);
        return;
    }

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

symbol_item *find_symbol_item_by_name(symbol_item *head, char *name)
{
    if (head == NULL)
    {
        return NULL;
    }

    if (strcmp(name, head->name) == 0)
    {

        return head;
    }

    return find_symbol_item_by_name(head->next, name);
}

void update_data_labels(symbol_item *head, int ICF)
{
    if (head == NULL)
    {
        return;
    }

    if (strcmp("data", head->type) == 0)
    {
        printf("changing label %s address to %d\n", head->name, head->address + ICF);
        head->address = head->address + ICF;
    }

    update_data_labels(head->next, ICF);
}
