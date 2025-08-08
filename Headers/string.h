#ifndef STRING_H
#define STRING_H

/**
 * removes white spaces from its start and its end
 * @param str - the string we want to remove the spaces from.
 * @return the modified string.
 */
char *delete_white_spaces_start_and_end(char *str);
/**
 * duplicate a string.
 * @param str - the string we want to duplicate.
 * @return the string duplicated.
 */
char *duplicate_str(char *str);
/**
 * checks if a string is an integer (including +\-).
 * @param str - the string to check.
 * @return 1 if the string represent an integer, 0 otherwise.
 */
int is_integer(char *str);
/**
 * checks if a char is digit or alphabet letter.
 * @param c - the char to check.
 * @return 1 if the char is digit or alphabet letter, 0 otherwise.
 */
int is_char_alphabetical_or_digit(char c);
/**
 * checks if a char is an alphabet letter.
 * @param c - the char to check.
 * @return 1 if the char is an alphabet letter, 0 otherwise.
 */
int is_char_alphabetical(char c);
/**
 * checks if a char is a digit.
 * @param c - the char to check.
 * @return 1 if the char is an integer, 0 otherwise.
 */
int is_char_digit(char c);
/**
 * checks if a string is too long to be a valid line.
 * @param line - the string we want to check.
 * @return 1 if line is too long, 0 otherwise.
 */
int is_line_too_long(char *line);
/**
 * return a file name without the extension (.something).
 * @param file_name - the string we want to remove its extension.
 * @return a string which represents the file name (without extension if existed).
 */
char *get_file_name_without_extension(char *file_name);
/**
 * checks if a line is empty.
 * @param line - the line we want to check.
 * @return 1 if line is empty, 0 otherwise.
 */
int is_empty_line(char *line);
/**
 * checks if a line is a comment.
 * @param line - the line we want to check.
 * @return 1 if line is a comment, 0 otherwise.
 */
int is_comment_line(char *line);
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
 * extract registers from the mat allocation definition and set them in the pointers received as props.
 * @param mat_def - the string representing the mat allocation definition (without the label - [reg1][reg2]).
 * @param reg1 - a string to put the first register value in.
 * @param reg2 - a string to put the second register value in.
 * @return - 1 if we extracted the regs successfully, 0 otherwise.
 */
int set_regs_from_mat_allocation(char *mat_def, char *reg1, char *reg2);
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
