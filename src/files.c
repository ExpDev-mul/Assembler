#include "../header/files.h"
#include "../header/assembler.h" /* Mainly to include BUFFER_SIZE */

/**
 * @brief Inserts the contents of one file into another.
 * 
 * This function reads the contents of the source file (`src`) line by line
 * and appends them to the destination file (`dest`).
 * 
 * @param dest The destination file to write to.
 * @param src The source file to read from.
 */
void insert_file_into_file(FILE* dest, FILE* src) {
    char buffer[BUFFER_SIZE]; /* Buffer to hold each line of the file */
    while (fgets(buffer, BUFFER_SIZE, src)) {
        printf("%s\n", buffer); /* Print the line to standard output (for debugging) */
        fputs(buffer, dest);    /* Write the line to the destination file */
    }
}

/**
 * @brief Prepends a string to the beginning of a file.
 * 
 * This function writes the given string (`st`) to the beginning of the file (`file`),
 * while preserving the original contents of the file. It uses a temporary file to
 * store the original contents during the operation.
 * 
 * @param file The file to prepend the string to.
 * @param st The string to prepend to the file.
 */
void prepend_to_file(FILE* file, const char *st) {
    /* First, save the original file contents */
    FILE* temp = tmpfile(); /* Create a temporary file */
    if (temp == NULL) {
        perror("Can not create temp file."); /* Print error if temp file creation fails */
        return;
    }
    
    /* Copy original file contents to temp */
    rewind(file); /* Move the file pointer to the beginning */
    insert_file_into_file(temp, file);
    
    /* Clear the original file */
    rewind(file); /* Move the file pointer to the beginning */
    freopen(NULL, "w+", file); /* Reopen in write mode, truncating the file */
    
    /* Write the string to prepend */
    fprintf(file, "%s", st);
    
    /* Now append the original content from temp file */
    rewind(temp); /* Move the temp file pointer to the beginning */
    insert_file_into_file(file, temp);
    
    fclose(temp); /* Close the temporary file */
    return;
}