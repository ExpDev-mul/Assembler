#ifndef ERRORS_H
#define ERRORS_H

#include <stdint.h>

enum { 
    /* Error codes */
    MISSING_ARGUMENTS = 0,
    EXTRANEOUS_TEXT,
    INVALID_COMMAND_NAME,
    INVALID_SOURCE_ADDRESSING,
    INVALID_DEST_ADDRESSING,

    IMMEDIATE_NUMBER_DECLARATION,
    MACRO_DECLARATION,
    EXTERN_IMPORT,
    ENTRY_MISSING_ARGUMENT,
    MISSING_DATA,

    MISSING_NUMBER,
    INVALID_DATA_VALUE,          /* Invalid value in .data declaration */
    INVALID_STRING_FORMAT,       /* Invalid format in .string declaration */
    MISSING_CLOSING_QUOTE,       /* Missing closing quote in .string */
    INVALID_REGISTER_FORMAT,     /* Invalid register format */

    INVALID_IMMEDIATE_VALUE,     /* Invalid immediate value */
    LABEL_ALREADY_DEFINED,       /* Label is already defined */
    LABEL_NOT_FOUND,             /* Label not found */
    INVALID_LABEL_FORMAT,        /* Invalid label format */
    INVALID_OPCODE,              /* Invalid opcode */

    FILE_OPEN_ERROR,             /* Error opening file */
    FILE_WRITE_ERROR,            /* Error writing to file */
    MEMORY_ALLOCATION_ERROR,     /* Memory allocation failed */
    UNEXPECTED_EOF,              /* Unexpected end of file */
    INVALID_SYNTAX,               /* General invalid syntax */

    REGISTER_OUT_OF_BOUNDS,       /* Register number out of bounds */
    EXTERN_ALREADY_DEFINED,        /* Extern label already defined */
    ENTRY_ALREADY_DEFINED,      /* Entry label already defined */
    EMPTY_LABEL_DECLARATION,             /* Empty label */
};

void error_with_code(int code, uint8_t line, uint8_t *errors_counter); /* Function to output an error code, with a given code. */

#endif /* ERRORS_H */