#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Headers/error.h"

void print_error(int error_code)
{
    switch (error_code)
    {
    case PROCESS_ERROR_MEMORY_ALLOCATION_FAILED:
        printf("memory allocation failed\n");
        break;
    case PROCESS_ERROR_LINE_TOO_LONG:
        printf("line too long\n");
        break;
    case PROCESS_ERROR_MAX_MEMORY_SIZE_EXCEEDED:
        printf("max memory size exceeded\n");
        break;
    case PROCESS_ERROR_FAILED_TO_OPEN_FILE:
        printf("failed to open file\n");
        break;
    case PROCESS_ERROR_INVALID_MACRO_NAME:
        printf("invalid macro name\n");
        break;
    case PROCESS_ERROR_INVALID_MACRO_DECLARATION:
        printf("invalid macro declaration\n");
        break;
    case PROCESS_ERROR_INVALID_MACRO_END_DECLARATION:
        printf("invalid macro end declaration\n");
        break;
    case PROCESS_ERROR_FIRST_PASS_FAILED:
        printf("first pass failed\n");
        break;
    case PROCESS_ERROR_COMMAND_AS_LABEL:
        printf("command as label\n");
        break;
    case PROCESS_ERROR_GUIDE_AS_LABEL:
        printf("guide as label\n");
        break;
    case PROCESS_ERROR_DATA_GUIDE_INVALID_PARAM:
        printf("data guide invalid param\n");
        break;
    case PROCESS_ERROR_STRING_GUIDE_INVALID_PARAM:
        printf("string guide invalid param\n");
        break;
    case PROCESS_ERROR_MAT_GUIDE_INVALID_PARAM:
        printf("mat guide invalid param\n");
        break;
    case PROCESS_ERROR_INVALID_EXTERN_LABEL_NAME:
        printf("invalid extern label name\n");
        break;
    case PROCESS_ERROR_INVALID_COMMAND:
        printf("invalid command\n");
        break;
    case PROCESS_ERROR_INVALID_NO_OP_LINE:
        printf("invalid no op line\n");
        break;
    case PROCESS_ERROR_INVALID_SRC_ALLOCATION:
        printf("invalid src allocation\n");
        break;
    case PROCESS_ERROR_INVALID_DST_ALLOCATION:
        printf("invalid dst allocation\n");
        break;
    case PROCESS_ERROR_MAT_ALLOCATION_INVALID_PARAM:
        printf("mat allocation invalid param\n");
        break;
    case PROCESS_ERROR_SYMBOL_ALREADY_EXISTS:
        printf("symbol already exists\n");
        break;

    default:
        printf("unknown error\n");
        break;
    }
}

void safe_exit(int error_code)
{
    print_error(error_code);
    exit(error_code);
}
