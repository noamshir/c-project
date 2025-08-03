#ifndef TABLE_H
#define TABLE_H
typedef struct mcro_item
{
    char *name;
    char *value;
    struct mcro_item *next;
} mcro_item;

/**
 * @brief create mcro item
 * create a new mcro item with given name and value.
 * @param name mcro item name (the mcro declaration name)
 * @param value mcro item value (the mcro declaration content)
 */
mcro_item *create_item(char *name, char *value);
/**
 * @brief adds mcro item to table
 * create a new mcro item with given name and value and adds it to the end of the list.
 * @param head table head pointer
 * @param name mcro item name (the mcro declaration name)
 * @param value mcro item value (the mcro declaration content)
 */
mcro_item *add_item(mcro_item **head, char *name, char *value);
/**
 * @brief concat value to mcro item value
 * Appends string (value) to the mcro item value received as prop.
 * @param item mcro item
 * @param value string to add to the mcro item's value
 */
void append_item_value(mcro_item *item, char *value);
/**
 * @brief finds mcro item by name
 * search the table for a mcro item with the same name as the one received as prop.
 * @param head table head pointer
 * @param name the name we want to search
 */
mcro_item *find_by_name(mcro_item *head, char *name);
#endif
