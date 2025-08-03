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
int add_label_to_symbol_table(symbol_item **head, char *name, char *type, int address);
symbol_item *find_symbol_item_by_name(symbol_item *head, char *name);
void update_data_labels(symbol_item *head, int ICF);
#endif
