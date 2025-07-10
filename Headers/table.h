typedef struct mcro_item
{
    char *name;
    char *value;
    struct mcro_item *next;
} mcro_item;

mcro_item *create_item(char *name, char *value);
mcro_item *add_item(mcro_item **head, char *name, char *value);
void append_item_value(mcro_item *item, char *value);
mcro_item *find_by_name(mcro_item *head, char *name);
