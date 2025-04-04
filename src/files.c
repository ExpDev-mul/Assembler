#include "../header/files.h"
#include "../header/assembler.h" /* Mainly to include BUFFER_SIZE */

void insert_file_into_file(FILE* dest, FILE* src) {
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE, src)) {
        printf("%s\n", buffer);
        fputs(buffer, dest);
    }
}

void prepend_to_file(FILE* file, const char *st) {
    /* First, save the original file contents */
    FILE* temp = tmpfile();
    if (temp == NULL) {
        perror("Can not create temp file.");
        return;
    }
    
    /* Copy original file contents to temp */
    rewind(file);
    insert_file_into_file(temp, file);
    
    /* Clear the original file */
    rewind(file);
    freopen(NULL, "w+", file); /* Reopen in write mode, truncating the file */
    
    /* Write the string to prepend */
    fprintf(file, "%s", st);
    
    /* Now append the original content from temp file */
    rewind(temp);
    insert_file_into_file(file, temp);
    
    fclose(temp);
    return;
}