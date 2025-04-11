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
    [MISSING_ARGUMENTS] = "Missing arguments for a command",
    [EXTRANEOUS_TEXT] = "Extraneous text after the end of a command",
    [INVALID_COMMAND_NAME] = "Invalid or unrecognized command name",
    [INVALID_SOURCE_ADDRESSING] = "Invalid addressing mode for the source operand",
    [INVALID_DEST_ADDRESSING] = "Invalid addressing mode for the destination operand",

    [EXTERN_MISSING_ARGUMENT] = "Missing argument for extern import",
    [ENTRY_MISSING_ARGUMENT] = "Missing argument for entry declaration",
    [MISSING_DATA] = "Missing data after .data directive",
    [INVALID_DATA_VALUE] = "Invalid value in .data declaration",
    [INVALID_STRING_FORMAT] = "Invalid format in .string declaration",

    [INVALID_IMMEDIATE_VALUE] = "Invalid immediate value (e.g., #abc)",
    [LABEL_ALREADY_DEFINED] = "Label is already defined",
    [LABEL_NOT_FOUND] = "Label not found in the symbol table",
    [INVALID_LABEL_FORMAT] = "Invalid label format (e.g., starts with a number)",
    [REGISTER_OUT_OF_BOUNDS] = "Register number is out of bounds (e.g., r8)",

    [EXTERN_ALREADY_DEFINED] = "Extern label is already defined",
    [ENTRY_ALREADY_DEFINED] = "Entry label is already defined",
    [EMPTY_LABEL_DECLARATION] = "Empty label declaration",
    [EXTERN_NOT_UNIQUE] = "Extern label is not unique (a label with the same name already exists)",
    [LABEL_NOT_UNIQUE] = "Label is not unique (an extern with the same name already exists)",
    
    [CONFLICTING_ENTRY_AND_EXTERN] = "Extern and entry cannot have the same name",
    [MACRO_ALREADY_DEFINED] = "Macro with that name already exists",
    [MACRO_NAME_IS_COMMAND] = "Macro name conflicts with a command",
    [LABEL_IS_MACRO_NAME] = "Label name conflicts with a macro name"
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