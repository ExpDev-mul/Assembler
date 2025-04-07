#ifndef FILES_H
#define FILES_H

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Inserts the contents of one file into another.
 * 
 * This function reads the contents of the source file (`src`) and appends
 * them to the destination file (`dest`).
 * 
 * @param src The source file to read from.
 * @param dest The destination file to write to.
 */
void insert_file_into_file(FILE* src, FILE* dest);

/**
 * @brief Prepends a string to the beginning of a file.
 * 
 * This function writes the given string (`st`) to the beginning of the file (`file`),
 * shifting the existing contents of the file to make space for the new string.
 * 
 * @param file The file to prepend the string to.
 * @param st The string to prepend to the file.
 */
void prepend_to_file(FILE* file, const char *st);

#endif /* FILES_H */