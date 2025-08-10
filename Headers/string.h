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
#endif
