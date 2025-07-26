#include "./table.h"

void pre_assembler(char *file_name_without_postfix);
int fill_mcro_table(char *file_name, mcro_item **mcro_table);
char *remove_mcro_defines(char file_name[]);
char *replace_mcro_defines(mcro_item **mcro_table, char file_name[]);
int isMcroNameValid(char *name);
