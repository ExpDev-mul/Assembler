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
 * store the modified content and replaces the original file with it.
 * 
 * @param file The file pointer to prepend the string to.
 * @param st The string to prepend to the file.
 */
void prepend_to_file(FILE *file, const char *st) {
    /* Create a temporary file */
    FILE *temp = tmpfile();
    if (temp == NULL) {
        perror("Failed to create a temporary file");
        return;
    }

    /* Write the string to prepend */
    fprintf(temp, "%s", st);

    /* Rewind the original file to read its contents */
    rewind(file);

    /* Append the original file contents to the temporary file */
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE, file)) {
        fputs(buffer, temp);
    }

    /* Rewind both files */
    rewind(file);
    rewind(temp);

    /* Overwrite the original file with the temporary file's content */
    while (fgets(buffer, BUFFER_SIZE, temp)) {
        fputs(buffer, file);
    }

    /* Truncate the file to remove any leftover content */
    fflush(file);
    ftruncate(fileno(file), ftell(file));

    fclose(temp); /* Close the temporary file */
}