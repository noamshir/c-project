#ifndef FILES_EXTRACTOR_H
#define FILES_EXTRACTOR_H
/**
 * @brief generate the ob file.
 * This function creates an ob file with the file name received as param and an .ob postfix.
 * The file includes the number of command and data as title and the abcd base 4 for the data and commands binary codes and addresses.
 * @param name the file name.
 * @param commands the array of commands.
 * @param command_length the number of commands.
 * @param data the array of data.
 * @param command_length the number of data items.
 */
void generate_ob_file(char *name, unsigned int *commands, int command_length, unsigned int *data, int data_length);
/**
 * @brief generate the entry file.
 * This function creates an entry file with the file name received as param and an .ent postfix.
 * The file includes the entries and the abcd base 4 for their addresses.
 * @param name the file name.
 * @param entry_labels the array of entry labels.
 * @param addresses the array of addresses.
 * @param entry_count the number of entries.
 */
void generate_entry_file(char *name, char **entry_labels, int *addresses, int entry_count);
/**
 * @brief generate the extern file.
 * This function creates an extern file with the file name received as param and an .ext postfix.
 * The file includes the external labels and the abcd base 4 for their addresses.
 * @param name the file name.
 * @param extern_labels the array of extern labels.
 * @param addresses the array of addresses.
 * @param extern_count the number of extern labels.
 */
void generate_extern_file(char *name, char **extern_labels, int *addresses, int extern_count);

#endif
