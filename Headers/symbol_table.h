#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
typedef struct symbol_item
{
    char *name;
    int address;
    char *type;
    struct symbol_item *next;
} symbol_item;

symbol_item *create_symbol_item(char *name, char *type, int address);
void add_label_to_symbol_table(symbol_item **head, char *name, char *type, int address);
#endif
