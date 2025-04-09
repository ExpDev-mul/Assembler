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
    [MISSING_ARGUMENTS] = "Missing arguments",
    [EXTRANEOUS_TEXT] = "Extraneous text after end of command",
    [INVALID_COMMAND_NAME] = "Invalid command name",
    [INVALID_SOURCE_ADDRESSING] = "The command cannot perform this type of addressing for the source argument",
    [INVALID_DEST_ADDRESSING] = "The command cannot perform this type of addressing for the destination argument",
    
    [IMMEDIATE_NUMBER_DECLARATION] = "Immediate number declaration does not follow up with a number",
    [EXTERN_IMPORT] = "Extern import missing an argument",
    [ENTRY_MISSING_ARGUMENT] = "Entry missing an argument",
    [MISSING_DATA] = "Missing data after .data",

    [MISSING_NUMBER] = "Missing number after #",
    [INVALID_DATA_VALUE] = "Invalid value in .data declaration",
    [INVALID_STRING_FORMAT] = "Invalid format in .string declaration",
    [MISSING_CLOSING_QUOTE] = "Missing closing quote in .string",
    [INVALID_REGISTER_FORMAT] = "Invalid register format",

    [INVALID_IMMEDIATE_VALUE] = "Invalid immediate value",
    [LABEL_ALREADY_DEFINED] = "Label is already defined",
    [LABEL_NOT_FOUND] = "Label not found",
    [INVALID_LABEL_FORMAT] = "Invalid label format",

    [REGISTER_OUT_OF_BOUNDS] = "Register number out of bounds",
    [EXTERN_ALREADY_DEFINED] = "Extern label already defined",
    [EMPTY_LABEL_DECLARATION] = "Empty label declaration",
    [CONFLICTING_ENTRY_AND_EXTERN] = "Extern and entry cannot have the same name",
    [MACRO_ALREADY_DEFINED] = "Macro with that name already exists",
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