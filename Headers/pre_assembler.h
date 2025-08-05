#include "./mcro_table.h"

/**
 * @brief Pre assembler phase.
 * process the file (which name is received as param) line by line and replace mcro defines.
 *
 * @param file_name_without_postfix A pointer to the file name (without the .as postfix).
 * @return 1 if the file pre process was handled successfully, 0 otherwise.
 */
int pre_assembler(char *file_name_without_postfix);
/**
 * @brief fill mcro table.
 * process the file (which is received as param) line by line and adds mcro definitions to mcro table
 *
 * @param file A pointer to the file.
 * @param mcro_table A pointer to the mcro table
 * @return 1 if successfully filled mcro table, 0 otherwise
 */
int fill_mcro_table(FILE *file, mcro_item **mcro_table);
/**
 * @brief copy the .as file lines to the .am file without the mcro declarations.
 * @param as_fp A pointer to the .as file.
 * @param am_fp A pointer to the .am file.
 */
void remove_mcro_defines(FILE *as_fp, FILE *am_fp);
/**
 * @brief replace the mcro declarations with their values.
 * @param mcro_table A pointer to the mcro table
 * @param file_name A pointer to the file name (am).
 * @return 1 if successfully replaced the mcro definitions in the .am file, 0 otherwise.
 */
int replace_mcro_defines(mcro_item **mcro_table, char *file_name);
