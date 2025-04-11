/* External headers */
#include <stdio.h>

/* Internal headers */
#include "../header/errors.h"

/**
 * @brief Array of error messages corresponding to error codes.
 * 
 * Each entry in this array corresponds to an error code defined in `errors.h`.
 * The index of the message matches the error code, ensuring consistency.
 */
char *errors_table[] = {
    /* Command errors */
    "Missing arguments for a command",                          /* MISSING_ARGUMENTS */
    "Extraneous text after the end of a command",               /* EXTRANEOUS_TEXT */
    "Invalid or unrecognized command name",                     /* INVALID_COMMAND_NAME */
    "Invalid addressing mode for the source operand",           /* INVALID_SOURCE_ADDRESSING */
    "Invalid addressing mode for the destination operand",      /* INVALID_DEST_ADDRESSING */

    /* Directive errors */
    "Missing argument for extern import",                       /* EXTERN_MISSING_ARGUMENT */
    "Missing argument for entry declaration",                   /* ENTRY_MISSING_ARGUMENT */
    "Missing data after .data directive",                       /* MISSING_DATA */
    "Invalid value in .data declaration",                       /* INVALID_DATA_VALUE */
    "Invalid format in .string declaration",                    /* INVALID_STRING_FORMAT */

    /* Label and value errors */
    "Invalid immediate value (e.g., #abc)",                     /* INVALID_IMMEDIATE_VALUE */
    "Label is already defined",                                 /* LABEL_ALREADY_DEFINED */
    "Label not found in the symbol table",                      /* LABEL_NOT_FOUND */
    "Invalid label format (e.g., starts with a number)",        /* INVALID_LABEL_FORMAT */
    "Register number is out of bounds (e.g., r8)",              /* REGISTER_OUT_OF_BOUNDS */

    /* Symbol table errors */
    "Extern label is already defined",                          /* EXTERN_ALREADY_DEFINED */
    "Entry label is already defined",                           /* ENTRY_ALREADY_DEFINED */
    "Empty label declaration",                                  /* EMPTY_LABEL_DECLARATION */
    "Extern label is not unique",                               /* EXTERN_NOT_UNIQUE */
    "Label is not unique",                                      /* LABEL_NOT_UNIQUE */
    
    /* Conflict errors */
    "Extern and entry cannot have the same name",               /* CONFLICTING_ENTRY_AND_EXTERN */
    "Macro with that name already exists",                      /* MACRO_ALREADY_DEFINED */
    "Macro name conflicts with a command",                      /* MACRO_NAME_IS_COMMAND */
    "Label name conflicts with a macro name"                    /* LABEL_IS_MACRO_NAME */
};


void error_with_code(int code, uint8_t line, uint8_t *errors_counter) {
    int errors_table_size = sizeof(errors_table) / sizeof(errors_table[0]);

    /* Ensure the error code is within bounds */
    if (code < 0 || code >= errors_table_size) {
        return;
    }

    /* Increment the error counter */
    (*errors_counter)++;

    /* Print the error message */
    printf("Error at Line: %i: %s\n", line, errors_table[code]);
}

void error_with_code_only(int code) {
    int errors_table_size = sizeof(errors_table) / sizeof(errors_table[0]);

    /* Ensure the error code is within bounds */
    if (code < 0 || code >= errors_table_size) {
        return;
    }

    /* Print the error message */
    printf("Error: %s\n", errors_table[code]);
}