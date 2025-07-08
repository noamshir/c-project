typedef struct mcro_item
{
    char *name;
    char *value;
    struct mcro_item *next;
} mcro_item;

mcro_item *create_item(char *name, char *value);
mcro_item *add_item(mcro_item **head, char *name, char *value);
