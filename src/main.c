/* Local includes */
#include "../header/assembler.h"

/* Standard includes */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file.as>\n", argv[0]);
        return EXIT_FAILURE; /* Return execution status code */
    }

    /* Open the input file */
    FILE *file = fopen(argv[1], "r"); /* Open the file and read its contents */
    if (!file) {
        perror("Error opening file");
        return EXIT_FAILURE; /* Return execution status code */
    }

    assemble(file); /* Call the 'assemble' function on our file */
    fclose(file);
}