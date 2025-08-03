#ifndef STRING_H
#define STRING_H

char *delete_white_spaces_start_and_end(char *str);
int is_line_too_long(char *line);
char *get_file_name_without_extension(char *file_name);
int is_empty_line(char *line);
int is_comment_line(char *line);
int is_valid_label_name(char *str);
int is_char_alphabetical_or_digit(char c);
int is_char_alphabetical(char c);
int is_char_digit(char c);
int is_label(char *word);
char *get_label_name(char *word);
int is_guide(char *word);
int is_word_guide(char *word, int guide_type);
int is_data_guide(char *word);
int is_string_guide(char *word);
int is_mat_guide(char *word);
int is_extern_guide(char *word);
int is_entry_guide(char *word);
int is_data_guide_declaration(char *word);
int get_command_index(char *word);
int is_command(char *word);
int is_mat_declaration(char *word);
int set_rows_and_cols_from_mat_declaration(char *word, int *rows, int *cols);
int get_allocation_type(char *word);
int is_immediate_allocation(char *word);
int is_mat_allocation(char *word);
int get_regs_from_mat_allocation(char *mat_def, char *reg1, char *reg2);
int is_register_allocation(char *word);
int is_direct_allocation(char *word);

#endif
