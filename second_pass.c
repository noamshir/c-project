#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Headers/string.h"
#include "Headers/error.h"
#include "Headers/symbol_table.h"
#include "Headers/utils.h"
#include "Headers/consts.h"
#include "Headers/binary_code.h"
#include "Headers/files_extractor.h"
#include "Headers/second_pass.h"
#include "Headers/command.h"

void second_pass(char *file_name_without_postfix, symbol_item **symbol_table, char ***array_of_commands, int ICF, char ***array_of_data, int DCF)
{
    FILE *file;
    char *file_name;
    char line[LINE_SIZE];
    char *word, *main_op, *label;
    int line_num = 0, error_flag = 0, IC = 0, i;

    char **entry_labels = NULL;
    int *entry_addresses = NULL;
    int entry_count = 0;

    char **extern_labels = NULL;
    int *extern_addresses = NULL;
    int extern_count = 0;

    printf("second pass started\n");

    file_name = malloc(strlen(file_name_without_postfix) + 4);
    if (file_name == NULL)
    {
        exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
    }

    /*adding ".am" at the end of the file name*/
    strcpy(file_name, file_name_without_postfix);
    strcat(file_name, ".am");

    /*open the file for reading and check for error*/
    file = fopen(file_name, "r");
    if (file == NULL)
    {
        printf("error code is %d\n", PROCESS_ERROR_FAILED_TO_OPEN_FILE);
        free(file_name);
        return;
    }

    /* allocate entry */
    entry_labels = malloc(sizeof(char *));
    if (entry_labels == NULL)
    {
        safe_exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
    }
    entry_addresses = malloc(sizeof(int));
    if (entry_addresses == NULL)
    {
        safe_exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
    }

    /* allocate extern */
    extern_labels = malloc(sizeof(char *));
    if (extern_labels == NULL)
    {
        safe_exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
    }
    extern_addresses = malloc(sizeof(int));
    if (extern_addresses == NULL)
    {
        safe_exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
    }

    /*pass on each line at the file*/
    while (fgets(line, sizeof(line), file) != NULL)
    {
        line_num++;
        printf("line %d: %s\n", line_num, line); // print line & line number (for errors analyse)

        if (is_empty_line(line) || is_comment_line(line)) // ignore empty\comment lines
        {
            continue;
        }

        /*get the first word in the line*/
        word = strtok(strdup(line), " ");
        word = delete_white_spaces_start_and_end(word);

        /*ignore lables words and save the command in param*/
        if (is_label_declaration(word))
        {
            main_op = strtok(NULL, " ");
            main_op = delete_white_spaces_start_and_end(main_op);
        }
        else
        {
            main_op = strdup(word);
        }

        /* check if the word is a guide that handled in first pass */
        if (is_data_guide(main_op) || is_string_guide(main_op) || is_mat_guide(main_op) || is_extern_guide(main_op))
        {
            continue; /* ignore in second pass */
        }

        /*handle '.entry'*/
        if (is_entry_guide(main_op))
        {
            label = strtok(NULL, " \t\n");
            label = delete_white_spaces_start_and_end(label);
            if (!handle_entry_guide(label, symbol_table, &entry_labels, &entry_addresses, &entry_count))
            {
                error_flag = 1;
            }
            printf("ENTRY COUNT: %d\n", entry_count);
        }
        else
        {
            /*if it's command line, maybe we need to encode some of its operands */
            handle_command_line_second_pass(symbol_table, line, array_of_commands, &IC, &extern_labels, &extern_addresses, &extern_count);
        }
    }

    /*DEBUG*/
    printf("commands array before second pass (%d):\n", ICF);
    for (i = 0; i < ICF; i++)
    {
        printf("command: %s\n", (*array_of_commands)[i]);
    }

    /* creation of final files (.ob, .entry, .extrn)*/
    if (!error_flag)
    {
        generate_ob_file(file_name_without_postfix, array_of_commands, ICF, array_of_data, DCF);
    }

    if (!error_flag && entry_count > 0) // to not create entry file if there are no entries
    {
        generate_entry_file(file_name_without_postfix, entry_labels, entry_addresses, entry_count);
    }
    if (!error_flag && extern_count > 0) // to not create extern file if there are no externs
    {
        generate_extern_file(file_name_without_postfix, extern_labels, extern_addresses, extern_count);
    }

    // cleaning memory
    for (i = 0; i < entry_count; i++)
        free(entry_labels[i]);
    free(entry_labels);
    free(entry_addresses);

    for (i = 0; i < extern_count; i++)
        free(extern_labels[i]);
    free(extern_labels);
    free(extern_addresses);

    free(file_name);

    /*print a message for final*/
    if (!error_flag)
    {
        printf("Second pass finished successfully.\n");
    }
    else
    {
        printf("Second pass finished with errors.\n");
    }
}

int handle_entry_guide(char *entry_label, symbol_item **symbol_table, char ***entry_labels, int **entry_addresses, int *entry_count)
{
    if (!add_entry_attribute(symbol_table, entry_label))
    {
        return 0;
    }
    else
    {
        printf("Adding entry: %s\n", entry_label);

        /* add entry label and address*/
        symbol_item *sym = find_symbol_item_by_name(*symbol_table, entry_label);
        if (sym != NULL)
        {
            *entry_labels = realloc(*entry_labels, sizeof(char *) * (*entry_count + 1));
            if (*entry_labels == NULL)
            {
                safe_exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
            }

            *entry_addresses = realloc(*entry_addresses, sizeof(int) * (*entry_count + 1));
            if (*entry_addresses == NULL)
            {
                safe_exit(PROCESS_ERROR_MEMORY_ALLOCATION_FAILED);
            }

            /* add label and address */
            (*entry_labels)[*entry_count] = strdup(entry_label);
            (*entry_addresses)[*entry_count] = strcmp(sym->type, "code") == 0 ? MEMORY_START_ADDRESS + sym->address : sym->address;
            (*entry_count)++;
        }
    }

    return 1;
}

int add_entry_attribute(symbol_item **symbol_table, char *entry_label)
{
    symbol_item *curr = find_symbol_item_by_name(*symbol_table, entry_label);
    if (curr == NULL)
    {
        // error label not found
        return 0;
    }

    if (strcmp(curr->type, "external") == 0)
    {
        // error label cant be both
        return 0;
    }

    curr->is_entry = 1;
    return 1;
}

int handle_command_line_second_pass(symbol_item **symbol_table, char *line, char ***array_of_commands, int *IC, char ***extern_labels, int **extern_addresses, int *extern_count)
{
    char *command, *label, *label_name, *rest_of_line;
    int command_index;

    label = strtok(strdup(line), " ");
    if (is_label_declaration(label))
    {
        label_name = get_label_name(label);
        command = strtok(NULL, " ");
    }
    else
    {
        command = strdup(label);
    }

    command = delete_white_spaces_start_and_end(command);
    command_index = get_command_index(command);
    if (command_index == -1)
    {
        print_error(PROCESS_ERROR_INVALID_COMMAND);
        return 0;
    }

    rest_of_line = strtok(NULL, "\n");

    switch (command_index)
    {
    case COMMAND_MOV:
    case COMMAND_CMP:
    case COMMAND_ADD:
    case COMMAND_SUB:
    case COMMAND_LEA:
        return handle_two_op_line_second_pass(symbol_table, command_index, rest_of_line, array_of_commands, IC, extern_labels, extern_addresses, extern_count);
        break;
    case COMMAND_CLR:
    case COMMAND_NOT:
    case COMMAND_INC:
    case COMMAND_DEC:
    case COMMAND_JMP:
    case COMMAND_BNE:
    case COMMAND_JSR:
    case COMMAND_RED:
    case COMMAND_PRN:
        return handle_one_op_line_second_pass(symbol_table, command_index, rest_of_line, array_of_commands, IC, extern_labels, extern_addresses, extern_count);
        break;
    case COMMAND_RTS:
    case COMMAND_STOP:
        return handle_no_op_line_second_pass(symbol_table, command_index, rest_of_line, array_of_commands, IC, extern_labels, extern_addresses, extern_count);
        break;
    default:
        print_error(PROCESS_ERROR_INVALID_COMMAND);
        return 0;
    }
}

int handle_no_op_line_second_pass(symbol_item **symbol_table, int command_index, char *str, char ***array_of_commands, int *IC, char ***extern_labels, int **extern_addresses, int *extern_count)
{
    // ensure str is empty
    if (str != NULL && !is_empty_line(str))
    {
        print_error(PROCESS_ERROR_INVALID_NO_OP_LINE);
        return 0;
    }

    return handle_op_line_second_pass(symbol_table, command_index, NULL, NULL, array_of_commands, IC, extern_labels, extern_addresses, extern_count);
}

int handle_one_op_line_second_pass(symbol_item **symbol_table, int command_index, char *str, char ***array_of_commands, int *IC, char ***extern_labels, int **extern_addresses, int *extern_count)
{
    char *dst;

    dst = strdup(str);
    dst = delete_white_spaces_start_and_end(dst);
    printf("dst: %s\n", dst);

    if (dst == NULL)
    {
        print_error(PROCESS_ERROR_INVALID_DST_ALLOCATION);
        return 0;
    }

    return handle_op_line_second_pass(symbol_table, command_index, NULL, dst, array_of_commands, IC, extern_labels, extern_addresses, extern_count);
}

int handle_two_op_line_second_pass(symbol_item **symbol_table, int command_index, char *str, char ***array_of_commands, int *IC, char ***extern_labels, int **extern_addresses, int *extern_count)
{
    char *src, *dst;

    src = strtok(strdup(str), ",");
    src = delete_white_spaces_start_and_end(src);
    if (src == NULL)
    {
        print_error(PROCESS_ERROR_INVALID_SRC_ALLOCATION);
        return 0;
    }

    dst = strtok(NULL, ",");
    dst = delete_white_spaces_start_and_end(dst);
    if (dst == NULL)
    {
        print_error(PROCESS_ERROR_INVALID_DST_ALLOCATION);
        return 0;
    }

    printf("src: %s, dst: %s\n", src, dst);
    return handle_op_line_second_pass(symbol_table, command_index, src, dst, array_of_commands, IC, extern_labels, extern_addresses, extern_count);
}

int handle_op_line_second_pass(symbol_item **symbol_table, int command_index, char *src, char *dst, char ***array_of_commands, int *IC, char ***extern_labels, int **extern_addresses, int *extern_count)
{
    char *line_binary_code;
    int src_type, dst_type, src_space, dst_space, L = 0;

    src_type = get_allocation_type(src);
    printf("src type: %d\n", src_type);
    if (src_type == ALLOCATION_INVALID)
    {
        print_error(PROCESS_ERROR_INVALID_SRC_ALLOCATION);
        return 0;
    }

    dst_type = get_allocation_type(dst);
    printf("dst type: %d\n", dst_type);
    if (dst_type == ALLOCATION_INVALID)
    {
        print_error(PROCESS_ERROR_INVALID_DST_ALLOCATION);
        return 0;
    }

    L = calculate_space(src_type, dst_type, &src_space, &dst_space);
    printf("Space size for line: %d\n", L);

    /* we already encoded the command line, skip*/
    (*IC)++;

    printf("handling: operands src: %s, dst: %s\n", src, dst);
    if (src_type == ALLOCATION_REGISTER && dst_type == ALLOCATION_REGISTER)
    {
        (*IC)++;
        return 1;
    }

    printf("encoding src: %s\n", src);
    if (!encode_second_pass_operands(src, src_type, src_space, IC, array_of_commands, symbol_table, extern_labels, extern_addresses, extern_count))
    {
        return 0;
    }

    printf("encoding dst: %s\n", dst);
    return encode_second_pass_operands(dst, dst_type, dst_space, IC, array_of_commands, symbol_table, extern_labels, extern_addresses, extern_count);
}

int encode_second_pass_operands(char *op, int type, int space, int *IC, char ***array_of_commands, symbol_item **symbol_table, char ***extern_labels, int **extern_addresses, int *extern_count)
{
    int is_external = 0;
    symbol_item *sym;

    if (type == ALLOCATION_MISSING)
    {
        return 1;
    }
    else if (type == ALLOCATION_REGISTER || type == ALLOCATION_IMMEDIATE)
    {
        /* already handled */
    }
    else if (type == ALLOCATION_MAT)
    {
        if (!set_second_pass_mat_allocation_binary_code(op, array_of_commands, IC, symbol_table, extern_labels, extern_addresses, extern_count))
        {
            return 0;
        }
    }
    else if (type == ALLOCATION_DIRECT)
    {
        op = delete_white_spaces_start_and_end(op);
        sym = find_symbol_item_by_name(*symbol_table, op);
        if (sym == NULL)
        {
            // error symbol not found
            return 0;
        }

        if (strcmp(sym->type, "external") == 0)
        {
            is_external = 1;
        }

        if (is_external)
        {
            (*array_of_commands)[*IC] = convert_num_to_8_bits(sym->address, EXTERNAL_CODE);
            *extern_labels = realloc(*extern_labels, sizeof(char *) * (*extern_count + 1));
            *extern_addresses = realloc(*extern_addresses, sizeof(int) * (*extern_count + 1));
            (*extern_labels)[*extern_count] = strdup(sym->name);
            (*extern_addresses)[*extern_count] = *IC + MEMORY_START_ADDRESS;
            (*extern_count)++;
        }
        else
        {
            (*array_of_commands)[*IC] = convert_num_to_8_bits(sym->address, RELOCATABLE_CODE);
        }
    }

    (*IC) = *IC + space;

    return 1;
}