#include "./mcro_table.h"

/**
 * @brief Pre assembler phase.
 * process the file (which name is received as param) line by line and replace mcro defines.
 *
 * @param file_name_without_postfix A pointer to the file name (without the .as postfix).
 */
void pre_assembler(char *file_name_without_postfix);
/**
 * @brief fill mcro table.
 * process the file (which name is received as param) line by line and adds mcro definitions to mcro table
 *
 * @param file_name A pointer to the file name.
 * @param mcro_table A pointer to the mcro table
 * @return 1 if successfully filled mcro table, 0 otherwise
 */
int fill_mcro_table(char *file_name, mcro_item **mcro_table);
/**
 * @brief creates a file_name.am file without the mcro definitions in the original.
 * @param file_name A pointer to the file name.
 * @return a string that represent the .am file without the mcro definitions, or NULL if an error occurs.
 */
char *remove_mcro_defines(char *file_name);
/**
 * @brief replace the mcro declarations with their values.
 * @param mcro_table A pointer to the mcro table
 * @param file_name A pointer to the file name (am).
 * @return a string that represent the .am file without the mcro declarations, or NULL if an error occurs.
 */
char *replace_mcro_defines(mcro_item **mcro_table, char *file_name);
