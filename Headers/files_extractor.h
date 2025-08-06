#ifndef FILES_EXTRACTOR_H
#define FILES_EXTRACTOR_H

void generate_ob_file(char *name, char ***code, int codeLength, char ***data, int dataLength); /*generate object file with the machine code */
void generate_entry_file(char *name, char **entryLabels, int *addresses, int entryCount);      /* generate entries file */
void generate_extern_file(char *name, char **externLabels, int *addresses, int externCount);   /* generate externals file */

#endif
