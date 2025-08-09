#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Headers/symbol_table.h"
#include "Headers/error.h"
#include "Headers/string.h"
#include "Headers/consts.h"

symbol_item *create_symbol_item(char *name, char *type, int address, int is_entry)
{
    symbol_item *item = malloc(sizeof(symbol_item));
    if (item == NULL)
    {
        exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
    }
    item->name = duplicate_str(name);
    item->address = address;
    item->type = duplicate_str(type);
    item->is_entry = is_entry;
    item->next = NULL;
    return item;
}

int add_symbol_item(symbol_item **head, char *name, char *type, int address, int is_entry, int line_number)
{
    symbol_item *item, *temp;

    if (is_command(name))
    {
        print_line_error(PROCESS_ERROR_COMMAND_AS_LABEL, line_number);
        return 0;
    }
    else if (is_guide(name))
    {
        print_line_error(PROCESS_ERROR_GUIDE_AS_LABEL, line_number);
        return 0;
    }
    else if (is_register(name))
    {
        print_line_error(PROCESS_ERROR_REGISTER_AS_LABEL, line_number);
        return 0;
    }

    item = create_symbol_item(name, type, address, is_entry);

    if (find_symbol_item_by_name(*head, name) != NULL)
    {
        print_line_error(PROCESS_ERROR_SYMBOL_ALREADY_EXISTS, line_number);
        return 0;
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

    return 1;
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

void update_data_symbol_items_address(symbol_item **head, int ICF)
{
    if (head == NULL)
    {
        return;
    }

    if (strcmp(DATA_SYMBOL, (*head)->type) == 0)
    {
        printf("changing label %s address to %d\n", (*head)->name, (*head)->address + ICF);
        (*head)->address = (*head)->address + ICF;
    }

    if ((*head)->next != NULL)
        update_data_symbol_items_address(&((*head)->next), ICF);
}

void free_symbol_table(symbol_item *head)
{
    if (head == NULL)
    {
        return;
    }

    if (head->next != NULL)
        free_symbol_table(head->next);

    free(head->name);
    free(head->type);
    free(head);
}
