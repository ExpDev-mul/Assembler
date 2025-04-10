/**
 * @file errors.h
 * @brief Header file for error handling in the assembler.
 * 
 * This file defines error codes and functions for reporting and managing errors
 * encountered during the assembly process. It ensures consistent error reporting
 * and provides utilities for tracking error occurrences.
 * 
 * Key Features:
 * - Defines a comprehensive list of error codes for various assembler errors.
 * - Provides functions to report errors with or without line numbers.
 * - Tracks the number of errors encountered during the assembly process.
 */
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
    EXTERN_MISSING_ARGUMENT,        /* Missing argument for extern import */
    ENTRY_MISSING_ARGUMENT,         /* Missing argument for entry declaration */
    MISSING_DATA,                   /* Missing data after .data directive */
    INVALID_DATA_VALUE,             /* Invalid value in .data declaration */
    INVALID_STRING_FORMAT,          /* Invalid format in .string declaration */
    INVALID_IMMEDIATE_VALUE,        /* Invalid immediate value (e.g., #abc) */
    LABEL_ALREADY_DEFINED,          /* Label is already defined */
    LABEL_NOT_FOUND,                /* Label not found in the symbol table */
    INVALID_LABEL_FORMAT,           /* Invalid label format (e.g., starts with a number) */
    REGISTER_OUT_OF_BOUNDS,         /* Register number is out of bounds (e.g., r8) */
    EXTERN_ALREADY_DEFINED,         /* Extern label is already defined */
    ENTRY_ALREADY_DEFINED,          /* Entry label is already defined */
    EMPTY_LABEL_DECLARATION,        /* Empty label declaration */
    EXTERN_NOT_UNIQUE,              /* Extern label is not unique */
    LABEL_NOT_UNIQUE,               /* Label is not unique */
    CONFLICTING_ENTRY_AND_EXTERN,   /* Extern and entry cannot have the same name */
    MACRO_ALREADY_DEFINED,          /* Macro with that name already exists */
    MACRO_NAME_IS_COMMAND,          /* Macro name conflicts with a command */
};

/**
 * @brief Outputs an error message corresponding to the given error code.
 * 
 * @param code The error code to report.
 * @param line The line number where the error occurred.
 * @param errors_counter Pointer to the error counter to increment.
 */
void error_with_code(int code, uint8_t line, uint8_t *errors_counter);

/**
 * @brief Outputs an error message corresponding to the given error code.
 * 
 * This function is used when the line number and error counter are not needed.
 * 
 * @param code The error code to report.
 */
void error_with_code_only(int code);

#endif /* ERRORS_H */