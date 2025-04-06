/* External headers */
#include <stdio.h>

/* Internal headers */
#include "../header/errors.h"

char *errors_table[] = {
    [MISSING_ARGUMENTS] = "Missing arguments",
    [EXTRANEOUS_TEXT] = "Extraneous text after end of command",
    [INVALID_COMMAND_NAME] = "Invalid command name",
    [INVALID_SOURCE_ADDRESSING] = "The command cannot perform this type of addressing for the source argument",
    [INVALID_DEST_ADDRESSING] = "The command cannot perform this type of addressing for the destination argument",
    
    [IMMEDIATE_NUMBER_DECLARATION] = "Immediate number declaration does not follow up with a number",
    [MACRO_DECLARATION] = "Macro declaration missing a name for the macro",
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
    [INVALID_OPCODE] = "Invalid opcode",

    [FILE_OPEN_ERROR] = "Error opening file",
    [FILE_WRITE_ERROR] = "Error writing to file",
    [MEMORY_ALLOCATION_ERROR] = "Memory allocation failed",
    [UNEXPECTED_EOF] = "Unexpected end of file",
    [INVALID_SYNTAX] = "General invalid syntax",

    [REGISTER_OUT_OF_BOUNDS] = "Register number out of bounds",
    [EXTERN_ALREADY_DEFINED] = "Extern label already defined",
    [EMPTY_LABEL_DECLARATION] = "Empty label declaration"
};

void error_with_code(int code, uint8_t line, uint8_t *errors_counter){
    int errors_table_size = sizeof(errors_table)/sizeof(errors_table[0]);
    if (code < 0 || code >= errors_table_size){
        /* Ensure error code is within bounds */
        return;
    }

    (*errors_counter)++;
    printf("Error at Line: %i: %s\n", line, errors_table[code]); /* Output current error */
}