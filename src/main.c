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
int process_file(const char *input_file) {
    /* Open the input file */
    FILE *file = fopen(input_file, "r");
    if (!file) {
        perror("Error opening input file");
        return EXIT_FAILURE;
    }

    /* Extract the base name of the input file (without path and extension) */
    const char *base_name = strrchr(input_file, '/');
    base_name = (base_name) ? base_name + 1 : input_file;

    char base_name_copy[256];
    strncpy(base_name_copy, base_name, sizeof(base_name_copy) - 1);
    base_name_copy[sizeof(base_name_copy) - 1] = '\0';

    char *base_name_end = strchr(base_name_copy, '.');
    if (base_name_end) {
        *base_name_end = '\0';
    }

    /* Create output directory path and file paths */
    char path[256];

    snprintf(path, sizeof(path), "./outputs/%s.am", base_name_copy);
    FILE *am = fopen(path, "w+");
    if (!am) {
        fprintf(stderr, "Error opening .am file for writing: %s\n", path);
        fclose(file);
        return EXIT_FAILURE;
    }

    snprintf(path, sizeof(path), "./outputs/%s.ob", base_name_copy);
    FILE *ob = fopen(path, "w");
    if (!ob) {
        fprintf(stderr, "Error opening .ob file for writing: %s\n", path);
        fclose(file);
        fclose(am);
        return EXIT_FAILURE;
    }

    snprintf(path, sizeof(path), "./outputs/%s.ext", base_name_copy);
    FILE *ext = fopen(path, "w");
    if (!ext) {
        fprintf(stderr, "Error opening .ext file for writing: %s\n", path);
        fclose(file);
        fclose(am);
        fclose(ob);
        return EXIT_FAILURE;
    }

    snprintf(path, sizeof(path), "./outputs/%s.ent", base_name_copy);
    FILE *ent = fopen(path, "w");
    if (!ent) {
        fprintf(stderr, "Error opening .ent file for writing: %s\n", path);
        fclose(file);
        fclose(am);
        fclose(ob);
        fclose(ext);
        return EXIT_FAILURE;
    }

    /* Assemble the input file */
    assemble(file, am, ob, ent, ext);

    /* Close all files */
    fclose(file);
    fclose(am);
    fclose(ob);
    fclose(ext);
    fclose(ent);

    return EXIT_SUCCESS;
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
int main(int argc, char *argv[]) {
    int i; /* Loop variable */
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file1.as> [<input_file2.as> ...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* Process each input file */
    for (i = 1; i < argc; i++) {
        printf("Processing file: %s\n", argv[i]);
        if (process_file(argv[i]) == EXIT_FAILURE) {
            fprintf(stderr, "Failed to process file: %s\n", argv[i]);
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}