#ifndef FILES_EXTRACTOR_H
#define FILES_EXTRACTOR_H

void generateAmFile(char* name, char** expandedLines, int lineCount); /* generate AM file after macro expansion */
void generateObFile(char* name, int* code, int codeLength, int* data, int dataLength); /*generate object file with the machine code */
void generateEntryFile(char* name, char** entryLabels, int* addresses, int entryCount);/* generate entries file */
void generateExternFile(char* name, char** externLabels, int* addresses, int externCount);/* generate externals file */

#endif
