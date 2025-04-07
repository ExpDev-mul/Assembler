#include <string.h>
#include <stdio.h>

#include "../header/preprocessing.h"
#include "../header/assembler.h"
#include "../header/labels.h"
#include "../header/lib.h"

/**
 * @brief Preprocesses the input file for the assembler.
 * 
 * This function processes the input file to handle macros and other preprocessing tasks.
 * It expands macros and writes the processed content to a temporary file for further assembly.
 * 
 * @param file The input file to preprocess.
 * @param temp The temporary file to write the preprocessed content to.
 */
void preprocess(FILE* file, FILE* temp) {
    /* Line reading buffers */
    char buffer[BUFFER_SIZE];
    char buffer_copy[BUFFER_SIZE];

    /* Macro buffers */
    char macro_buffer[BUFFER_SIZE * 10]; /* Buffer to store macro content */
    char macro_name[10];                /* Buffer to store macro name */

    LinkedList* macros = NULL;          /* Linked list to store macros */
    bool is_reading_macro = false;      /* Flag to indicate if we're reading a macro */

    while (1) {
        /* Read a line from the input file */
        if (fgets(buffer, BUFFER_SIZE, file) == NULL) {
            break; /* EOF has been reached, or an error has occurred */
        }

        /* Remove the newline character from the end of the string */
        buffer[strcspn(buffer, "\n")] = '\0';

        /* Create a copy of the buffer for processing */
        strcpy(buffer_copy, buffer);
        char *prefix = strtok(buffer, " "); /* Extract the first token as the command */

        if (prefix == NULL) {
            continue; /* Skip empty lines */
        }

        if (prefix[0] == ';') {
            /* Skip comments (lines starting with ';') */
            continue;
        }

        if (!strcmp(prefix, "mcro")) {
            /* Start of a macro declaration */
            char *arg = strtok(NULL, " "); /* Extract the macro name */
            if (arg == NULL) {
                fprintf(stderr, "Error: Missing macro name.\n");
                continue;
            }

            strcpy(macro_name, arg); /* Store the macro's name */
            is_reading_macro = true; /* Enable macro reading mode */
            continue;
        }

        if (!strcmp(prefix, "mcroend")) {
            /* End of a macro declaration */
            add_label_string(&macros, macro_name, macro_buffer); /* Add the macro to the list */
            memset(macro_buffer, 0, sizeof(macro_buffer));       /* Clear the macro buffer */
            is_reading_macro = false;                           /* Disable macro reading mode */
            continue;
        }

        if (is_reading_macro) {
            /* Append the current line to the macro buffer */
            char *buffer_copy_ptr = buffer_copy;
            skip_leading_spaces(&buffer_copy_ptr);
            strncat(macro_buffer, buffer_copy_ptr, MACRO_SIZE);
            strncat(macro_buffer, "\n", MACRO_SIZE);
            continue;
        }

        /* Check if the current line matches a macro name */
        LinkedList* macro_ptr = get_node_by_label(macros, prefix);
        if (macro_ptr != NULL) {
            /* If the macro exists, write its content to the temporary file */
            fputs(macro_ptr->value.buffer, temp);
            continue;
        }

        /* Write the current line to the temporary file */
        char *buffer_copy_ptr = buffer_copy;
        skip_leading_spaces(&buffer_copy_ptr);
        fputs(buffer_copy_ptr, temp);
        fputc('\n', temp);
    }

    /* Free the memory allocated for the macros linked list */
    free_label_list(macros);
}