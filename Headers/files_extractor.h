#ifndef FILES_EXTRACTOR_H
#define FILES_EXTRACTOR_H

void generate_ob_file(char *name, unsigned int *commands, int command_length, unsigned int *data, int data_length);
void generate_entry_file(char *name, char **entry_labels, int *addresses, int entry_count);    /* generate entries file */
void generate_extern_file(char *name, char **extern_labels, int *addresses, int extern_count); /* generate externals file */

#endif
