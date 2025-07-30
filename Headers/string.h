#ifndef STRING_H
#define STRING_H

void delete_white_spaces(char *s);
char *get_file_name_without_extension(char *file_name);
int is_empty_line(char *line);
int is_comment_line(char *line);
int is_char_alphabetical_or_digit(char c);
int is_char_alphabetical(char c);
int is_char_digit(char c);
int is_label(char *word);
int is_word_guide(char *word, int guide_type);
int is_data_guide(char *word);
int is_string_guide(char *word);
int is_mat_guide(char *word);
int is_extern_guide(char *word);
int is_entry_guide(char *word);
int get_operation_index(char *word);
int is_operation(char *word);
int is_mat_declaration(char *word);
int set_rows_and_cols_from_mat_declaration(char *word, int *rows, int *cols);
int get_allocation_type(char *word);
int is_immediate_allocation(char *word);
int is_mat_allocation(char *word);
int is_register_allocation(char *word);
int is_direct_allocation(char *word);

#endif
