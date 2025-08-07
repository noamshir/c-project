#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
typedef struct symbol_item
{
    char *name;
    int address;
    char *type;
    int is_entry;
    struct symbol_item *next;
} symbol_item;

/**
 * @brief create symbol item
 * create a new symbol item with given name, type and address.
 * @param name symbol item name (the label declaration name)
 * @param type symbol item type (code, data, ...)
 * @param address an integer representing the symbol item address in memory
 * @param is_entry a boolean that states wether or not the item is an entry.
 * @return the new symbol item
 */
symbol_item *create_symbol_item(char *name, char *type, int address, int is_entry);
/**
 * @brief adds symbol item to table
 * create a new symbol item with given name, type and address.
 * @param head table head pointer
 * @param name symbol item name (the symbol declaration name)
 * @param type symbol item type (code, data, ...)
 * @param address an integer representing the symbol item address in memory
 * @param is_entry a boolean that states wether or not the item is an entry.
 * @return 1 if added successfully 0 otherwise
 */
int add_symbol_item(symbol_item **head, char *name, char *type, int address, int is_entry);
/**
 * @brief finds symbol item by name
 * search the table for a symbol item with the same name as the one received as prop.
 * @param head table head pointer
 * @param name the name we want to search
 * @return the symbol item we searched or NULL
 */
symbol_item *find_symbol_item_by_name(symbol_item *head, char *name);
/**
 * @brief update all data (type) symbol items addresses.
 * Adds an integer (ICF) to all data items addresses.
 * @param head table head pointer
 * @param ICF an integer to add to all data symbol items
 */
void update_data_symbol_items_address(symbol_item **head, int ICF);

void free_symbol_table(symbol_item *head);

#endif
