/* Local includes */
#include "../header/assembler.h"

/* Standard includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Processes a single input file and generates the corresponding output files.
 * 
 * This function handles opening the input file, creating the necessary output files,
 * and invoking the assembler to process the input.
 * 
 * @param input_file The path to the input file.
 * @return EXIT_SUCCESS on success, or EXIT_FAILURE on failure.
 */
void process_file(const char *input_file) {
    FILE *file = NULL;
    FILE *am = NULL;

    /* Construct the full path to the input file in the inputs/ directory */
    char input_path[256];
    int res = snprintf(input_path, sizeof(input_path), "./inputs/%s.as", input_file);
    if (res < 0 || res >= sizeof(input_path)) {
        fprintf(stderr, "Error creating input file path\n");
        return;
    }

    /* Open the input file */
    file = fopen(input_path, "r");
    if (!file) {
        perror("Error opening input file");
        return;
    }

    /* Extract the base name of the input file (without path and extension) */
    const char *base_name = input_file; /* Use the input_file name directly as the base name */

    /* Create output directory path and file paths */
    char path[256];

    res = snprintf(path, sizeof(path), "./outputs/%s.am", base_name);
    if (res < 0 || res >= sizeof(path)) {
        fprintf(stderr, "Error creating .am file path\n");
        goto cleanup;
    }

    am = fopen(path, "w+");
    if (!am) {
        fprintf(stderr, "Error opening .am file for writing: %s\n", path);
        goto cleanup;
    }

    /* Assemble the input file */
    assemble(file, am, base_name);

    /* Cleanup wrapper to close files after execution */
    cleanup:
        if (file) {
            fclose(file);
        }

        if (am) {
            fclose(am);
        }

        return;
}

/**
 * @brief Main entry point for the assembler program.
 * 
 * This function processes command-line arguments to handle multiple input files
 * and invokes the assembler for each file.
 * 
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return EXIT_SUCCESS on success, or EXIT_FAILURE on failure.
 */
void main(int argc, char *argv[]) {
    int i; /* Loop variable */
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file1.as> [<input_file2.as> ...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* Process each input file */
    for (i = 1; i < argc; i++) {
        printf("Processing file: %s\n", argv[i]);
        process_file(argv[i]);
    }

    return EXIT_SUCCESS;
}