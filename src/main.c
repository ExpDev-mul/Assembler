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

    /* Open the input file */
    file = fopen(input_file, "r");
    if (!file) {
        perror("Error opening input file");
        return;
    }

    /* Extract the base name of the input file (without path and extension) */
    const char *base_name = strrchr(input_file, '/');
    base_name = (base_name) ? base_name + 1 : input_file;

    char base_name_copy[256];
    strncpy(base_name_copy, base_name, sizeof(base_name_copy) - 1);
    base_name_copy[sizeof(base_name_copy) - 1] = '\0';

    char *base_name_end = strchr(base_name_copy, '.');
    if (base_name_end != NULL) {
        *base_name_end = '\0';
    }

    /* Create output directory path and file paths */
    char path[256];

    int res = snprintf(path, sizeof(path), "./outputs/%s.am", base_name_copy);
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
    assemble(file, am, base_name_copy);

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