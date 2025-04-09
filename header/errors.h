#ifndef ERRORS_H
#define ERRORS_H

#include <stdint.h>

/**
 * @brief Enum for error codes used throughout the assembler.
 * 
 * Each error code corresponds to a specific type of error that can occur
 * during the assembly process. These codes are used to identify and report
 * errors to the user.
 */
enum { 
    /* Error codes */
    MISSING_ARGUMENTS = 0,          /* Missing arguments for a command */
    EXTRANEOUS_TEXT,                /* Extraneous text after the end of a command */
    INVALID_COMMAND_NAME,           /* Invalid or unrecognized command name */
    INVALID_SOURCE_ADDRESSING,      /* Invalid addressing mode for the source operand */
    INVALID_DEST_ADDRESSING,        /* Invalid addressing mode for the destination operand */

    IMMEDIATE_NUMBER_DECLARATION,   /* Immediate number declaration is invalid */
    MACRO_DECLARATION,              /* Missing or invalid macro declaration */
    EXTERN_IMPORT,                  /* Missing argument for extern import */
    ENTRY_MISSING_ARGUMENT,         /* Missing argument for entry declaration */
    MISSING_DATA,                   /* Missing data after .data directive */

    MISSING_NUMBER,                 /* Missing number after immediate addressing symbol (#) */
    INVALID_DATA_VALUE,             /* Invalid value in .data declaration */
    INVALID_STRING_FORMAT,          /* Invalid format in .string declaration */
    MISSING_CLOSING_QUOTE,          /* Missing closing quote in .string declaration */
    INVALID_REGISTER_FORMAT,        /* Invalid register format (e.g., rX where X is invalid) */

    INVALID_IMMEDIATE_VALUE,        /* Invalid immediate value (e.g., #abc) */
    LABEL_ALREADY_DEFINED,          /* Label is already defined */
    LABEL_NOT_FOUND,                /* Label not found in the symbol table */
    INVALID_LABEL_FORMAT,           /* Invalid label format (e.g., starts with a number) */
    INVALID_OPCODE,                 /* Invalid or unrecognized opcode */

    FILE_OPEN_ERROR,                /* Error opening a file */
    FILE_WRITE_ERROR,               /* Error writing to a file */
    MEMORY_ALLOCATION_ERROR,        /* Memory allocation failed */
    UNEXPECTED_EOF,                 /* Unexpected end of file during parsing */
    INVALID_SYNTAX,                 /* General invalid syntax in the input file */

    REGISTER_OUT_OF_BOUNDS,         /* Register number is out of bounds (e.g., r8) */
    EXTERN_ALREADY_DEFINED,         /* Extern label is already defined */
    ENTRY_ALREADY_DEFINED,          /* Entry label is already defined */
    EMPTY_LABEL_DECLARATION,        /* Empty label declaration */
    CONFLICTING_ENTRY_AND_EXTERN   /* Extern and entry cannot have the same name */
};

/**
 * @brief Outputs an error message corresponding to the given error code.
 * 
 * @param code The error code to report.
 * @param line The line number where the error occurred.
 * @param errors_counter Pointer to the error counter to increment.
 */
void error_with_code(int code, uint8_t line, uint8_t *errors_counter);

#endif /* ERRORS_H */