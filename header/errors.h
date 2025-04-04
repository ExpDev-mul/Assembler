#ifndef ERRORS_H
#define ERRORS_H

#include <stdint.h>

enum { 
    /* Error codes */
    MISSING_ARGUMENTS = 0, /* Init 0 */
    EXTRANEOUS_TEXT,
    INVALID_COMMAND_NAME,
    INVALID_SOURCE_ADDRESSING,
    INVALID_DEST_ADDRESSING,

    /* 5 */
    IMMEDIATE_NUMBER_DECLARATION,
    MACRO_DECLARATION,
    EXTERN_IMPORT,
    ENTRY_MISSING_ARGUMENT,
};

void error_with_code(int code, char* line_buffer, uint8_t *errors_counter); /* Function to output an error code, with a given code. */

#endif /* ERRORS_H */