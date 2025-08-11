#ifndef UTILS_H
#define UTILS_H
/**
 * @brief free array of strings.
 * @param array the array to free.
 * @param length the length of the array.
 */
void free_array_of_strings(char **array, int length);
/**
 * @brief build file name with postfix.
 * @param name the base file name.
 * @param postfix the file postfix.
 * @return file name with the postfix.
 */
char *build_file_name_with_postfix(char *name, char *postfix);
/**
 * check if a string is a valid mcro name
 * @param name - the string to check
 * @return  1 if string is a valid mcro name, 0 otherwise.
 */
int is_mcro_name_valid(char *name);
/**
 * check if a string is a valid label name
 * @param str - the string to check
 * @return  1 if string is a valid label name, 0 otherwise.
 */
int is_valid_label_name(char *str);
/**
 * check if a string is a label declaration
 * @param word - the string to check
 * @return  1 if string is a label declaration, 0 otherwise.
 */
int is_label_declaration(char *word);
/**
 * extract label name from string
 * @param word - the string from which we extract the label name
 * @return the label name extracted from the string if its a valid label declaration, NULL otherwise.
 */
char *get_label_name(char *word);
/**
 * check if a string is a guide
 * @param word - the string to check
 * @return 1 if string is a guide, 0 otherwise.
 */
int is_guide(char *word);
/**
 * check if a string is the same as the guide type index sent.
 * @param word - the string to check.
 * @param guide_type - an integer representing the guide type idx.
 * @return 1 if string is the same as the guide type sent (its index), 0 otherwise.
 */
int is_word_guide(char *word, int guide_type);
/**
 * check if a string is data guide
 * @param word - the string to check.
 * @return 1 if string is a data guide, 0 otherwise.
 */
int is_data_guide(char *word);
/**
 * check if a string is string guide
 * @param word - the string to check.
 * @return 1 if string is a string guide, 0 otherwise.
 */
int is_string_guide(char *word);
/**
 * check if a string is mat guide
 * @param word - the string to check.
 * @return 1 if string is a mat guide, 0 otherwise.
 */
int is_mat_guide(char *word);
/**
 * check if a string is extern guide
 * @param word - the string to check.
 * @return 1 if string is a extern guide, 0 otherwise.
 */
int is_extern_guide(char *word);
/**
 * check if a string is entry guide
 * @param word - the string to check.
 * @return 1 if string is a entry guide, 0 otherwise.
 */
int is_entry_guide(char *word);
/**
 * check if a string is a valid data declaration (num1, num2, num3, ..., numn)
 *  @param word - the string to check.
 * @return 1 if string is a data guide declaration, 0 otherwise.
 */
int is_data_guide_declaration(char *word);
/**
 * check if a string is a mat declaration ([num1][num2] num1, num2, ...)
 * @param word - the string to check.
 * @return 1 if string is a mat declaration, 0 otherwise.
 */
int is_mat_declaration(char *word);
/**
 * check if a string is a command and returns its index (if found).
 * @param word - the string to check.
 * @return the index of the command if found, -1 otherwise.
 */
int get_command_index(char *word);
/**
 * check if a string is a command.
 * @param word - the string to check.
 * @return 1 if string is a command, 0 otherwise.
 */
int is_command(char *word);
/**
 * check if a string is a register and returns its index (if found).
 * @param word - the string to check.
 * @return 1 if string is a register, 0 otherwise.
 */
int get_register_index(char *word);
/**
 * check if a string is a register (r0,r1,...,r7)
 * @param word - the string to check.
 * @return 1 if string is a register, 0 otherwise,
 */
int is_register(char *word);
/**
 * check if a string is an allocation type and returns its index (if found).
 * @param word - the string to check.
 * @return the index of the allocation type if found, ALLOCATION_INVALID otherwise.
 */
int get_allocation_type(char *word);
/**
 * check if a string is an immediate allocation (#num).
 * @param word - the string to check.
 * @return 1 if string is an immediate allocation, 0 otherwise.
 */
int is_immediate_allocation(char *word);
/**
 * check if a string is a mat allocation (label[reg1][reg2]).
 * @param word - the string to check.
 * @return 1 if string is a mat allocation, 0 otherwise.
 */
int is_mat_allocation(char *word);
/**
 * @brief sets the label buffer received as param with the mat allocation label.
 * @param mat_def the string representing the mat allocation.
 * @param label a string buffer to fill with the mat allocation label.
 */
void set_label_from_mat_allocation(char *mat_def, char *label);
/**
 * extract registers from the mat allocation definition and set them in the pointers received as props.
 * @param mat_def - the string representing the mat allocation definition (without the label - [reg1][reg2]).
 * @param reg1 - a string to put the first register value in.
 * @param reg2 - a string to put the second register value in.
 * @return - 1 if we extracted the regs successfully, 0 otherwise.
 */
int set_regs_from_mat_allocation_without_label(char *mat_def, char *reg1, char *reg2);
/**
 * check if a string is a register allocation (r0,r1,...,r7)
 * @param word - the string to check.
 * @return 1 if string is a register allocation, 0 otherwise.
 */
int is_register_allocation(char *word);
/**
 * check if a string is a direct allocation (name)
 * @param word - the string to check.
 * @return 1 if string is a direct allocation, 0 otherwise.
 */
int is_direct_allocation(char *word);
#endif
