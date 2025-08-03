#ifndef SECOND_PASS_H
#define SECOND_PASS_H

void second_pass(char *file_name_without_postfix, symbol_item *symbol_table, char **array_of_commands, int IC, char **array_of_data, int DC);
int encode_second_pass_operands(symbol_item *symbol_table, char *line, char **array_of_commands,char ***externLabels, int **externAddresses, int *externCount);

#endif