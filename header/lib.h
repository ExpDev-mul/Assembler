#ifndef LIB_H
#define LIB_H

/**
 * @brief Boolean type definition.
 * 
 * Defines a `bool` type with values `false` and `true` for better readability
 * and compatibility in C programs.
 */
typedef enum { false, true } bool;

/**
 * @brief Skips leading spaces in a string.
 * 
 * This function advances the pointer to the first non-space character in the string.
 * 
 * @param line Pointer to the string to process. The pointer is updated to skip spaces.
 */
void skip_leading_spaces(char **line);

/**
 * @brief Duplicates a string.
 * 
 * This function creates a new copy of the given string in dynamically allocated memory.
 * The caller is responsible for freeing the allocated memory.
 * 
 * @param s The string to duplicate.
 * @return A pointer to the newly allocated copy of the string, or NULL if allocation fails.
 */
char *strdup(const char *s);

#endif /* LIB_H */