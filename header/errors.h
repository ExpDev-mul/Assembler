#ifndef ERRORS_H
#define ERRORS_H

#include <stdint.h>

void error_with_code(int code, char* line_buffer, uint8_t *errors_counter); /* Function to output an error code, with a given code. */

#endif /* ERRORS_H */