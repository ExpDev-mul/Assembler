#include <stdlib.h>
#include <string.h>

#include "../header/lib.h"

/**
 * @brief Skips leading spaces in a string.
 * 
 * This function advances the pointer to the first non-space character
 * in the string. It handles both spaces (' ') and tab characters ('\t').
 * 
 * @param line Pointer to the string to process. The pointer is updated
 *             to skip spaces.
 */
void skip_leading_spaces(char **line) {
    if (line == NULL || *line == NULL) {
        return; /* Ensure valid pointer */
    }

    while (**line == ' ' || **line == '\t') {
        (*line)++; /* Advance for every space or tab character */
    }
}

/**
 * @brief Duplicates a string.
 * 
 * This function creates a new copy of the given string in dynamically
 * allocated memory. The caller is responsible for freeing the allocated memory.
 * 
 * @param s The string to duplicate.
 * @return A pointer to the newly allocated copy of the string, or NULL if
 *         memory allocation fails.
 */
char *strdup(const char *s) {
    size_t len = strlen(s) + 1; /* Calculate the length of the string, including the null terminator */
    char *new_str = malloc(len); /* Allocate memory for the new string */
    if (new_str == NULL) {
        return NULL; /* Return NULL if memory allocation fails */
    }

    return memcpy(new_str, s, len); /* Copy the string into the newly allocated memory */
}