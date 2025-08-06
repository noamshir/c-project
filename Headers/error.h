typedef enum PROCESS_ERROR_CODES
{
    /* internal */
    PROCESS_ERROR_MEMORY_ALLOCATION_FAILED,
    PROCESS_ERROR_LINE_TOO_LONG,
    PROCESS_ERROR_MAX_MEMORY_SIZE_EXCEEDED,
    /* mcro */
    PROCESS_ERROR_INVALID_MACRO_NAME,
    PROCESS_ERROR_INVALID_MACRO_DECLARATION,
    PROCESS_ERROR_INVALID_MACRO_END_DECLARATION,
    /* files */
    PROCESS_ERROR_FAILED_TO_OPEN_FILE,
    /* first pass */
    PROCESS_ERROR_FIRST_PASS_FAILED,
    PROCESS_ERROR_COMMAND_AS_LABEL,
    PROCESS_ERROR_GUIDE_AS_LABEL,
    PROCESS_ERROR_DATA_GUIDE_INVALID_PARAM,
    PROCESS_ERROR_STRING_GUIDE_INVALID_PARAM,
    PROCESS_ERROR_MAT_GUIDE_INVALID_PARAM,
    PROCESS_ERROR_INVALID_EXTERN_LABEL_NAME,
    PROCESS_ERROR_INVALID_COMMAND,
    PROCESS_ERROR_INVALID_NO_OP_LINE,
    PROCESS_ERROR_INVALID_SRC_ALLOCATION,
    PROCESS_ERROR_INVALID_DST_ALLOCATION,
    PROCESS_ERROR_MAT_ALLOCATION_INVALID_PARAM,
    /* symbol table */
    PROCESS_ERROR_SYMBOL_ALREADY_EXISTS
} PROCESS_ERROR_CODES;

/**
 * @brief Print error.
 *
 * Print a proper according to the error code sent as param.
 *
 * @param error_code an integer representing the error code
 */
void print_error(int error_code);
/**
 * @brief Print error and exit.
 *
 * Print a proper according to the error code sent as param and exit the program.
 *
 * @param error_code an integer representing the error code
 */
void safe_exit(int error_code);
