#include <string.h>

#include "../header/preprocessing.h"
#include "../header/assembler.h"
#include "../header/labels.h"
#include "../header/lib.h"
#include "../header/first_pass.h"
#include "../header/errors.h"

/**
 * @brief Performs the first pass of the assembler.
 * 
 * This function processes the input file to extract labels, entries, and externs,
 * while validating the syntax of the input. It also records any errors encountered
 * during the first pass.
 * 
 * @param file The input file to process.
 * @param labels_ptr Pointer to the linked list of labels.
 * @param entries_ptr Pointer to the linked list of entry labels.
 * @param externs_ptr Pointer to the linked list of extern labels.
 * @param errors Pointer to the error counter to track the number of errors.
 */
void first_pass(FILE* file, LinkedList** labels_ptr, LinkedList** entries_ptr, LinkedList** externs_ptr, uint8_t* errors) {
    /* Line reading buffers */
    char buffer[BUFFER_SIZE];
    char buffer_copy[BUFFER_SIZE];

    /* Initialize linked lists */
    LinkedList* labels = *labels_ptr;
    LinkedList* externs = *externs_ptr;
    LinkedList* entries = *entries_ptr;

    uint8_t line = START_LINE - 1; /* Line reading starts one line before the START_LINE */

    while (1) {
        /* Read a line from the file */
        if (fgets(buffer, BUFFER_SIZE, file) == NULL) {
            break; /* EOF has been reached, or an error has occurred */
        }

        /* Remove the newline character from the end of the string */
        buffer[strcspn(buffer, "\n")] = '\0';

        strcpy(buffer_copy, buffer);
        char *prefix = strtok(buffer, " "); /* Extract the first token as the command */

        if (prefix == NULL) {
            continue; /* Skip empty lines */
        }

        line++; /* Advance to the next line */

        if (prefix[0] == ';') {
            /* Skip comments (lines starting with ';') */
            continue;
        }

        if (prefix[strlen(prefix) - 1] == ':') {
            /* Handle label declarations */
            char *pos = strchr(prefix, ':');
            *pos = '\0'; /* Remove the ':' at the end */

            if (strlen(prefix) == 0) {
                /* Check for empty label declarations */
                error_with_code(EMPTY_LABEL_DECLARATION, line, errors);
                continue;
            }

            if (is_label_in_list(labels, prefix)) {
                /* Check if the label is already defined */
                error_with_code(LABEL_ALREADY_DEFINED, line, errors);
                continue;
            }

            add_label_number(&labels, prefix, line); /* Add the label to the linked list */
            continue;
        }

        if (!strcmp(prefix, ".extern")) {
            /* Handle .extern declarations */
            char *arg = strtok(NULL, " ");
            skip_leading_spaces(&arg);

            if (arg == NULL) {
                /* Check for missing argument */
                error_with_code(ENTRY_MISSING_ARGUMENT, line, errors);
                continue;
            }

            if (is_label_in_list(externs, arg)) {
                /* Check if the label is already defined as an .extern */
                error_with_code(EXTERN_ALREADY_DEFINED, line, errors);
                continue;
            }

            if (is_label_in_list(entries, arg)) {
                /* Check for conflicting entry and extern labels */
                error_with_code(CONFLICTING_ENTRY_AND_EXTERN, line, errors);
                continue;
            }

            add_label_number(&externs, arg, line); /* Add the extern label to the list */
            continue;
        }

        if (!strcmp(prefix, ".entry")) {
            /* Handle .entry declarations */
            char *arg = strtok(NULL, " ");
            skip_leading_spaces(&arg);

            if (arg == NULL) {
                /* Check for missing argument */
                error_with_code(ENTRY_MISSING_ARGUMENT, line, errors);
                continue;
            }

            if (is_label_in_list(entries, arg)) {
                /* Check if the label is already defined as an .entry */
                error_with_code(ENTRY_ALREADY_DEFINED, line, errors);
                continue;
            }

            if (is_label_in_list(externs, arg)) {
                /* Check for conflicting entry and extern labels */
                error_with_code(CONFLICTING_ENTRY_AND_EXTERN, line, errors);
                continue;
            }

            add_label_number(&entries, arg, line); /* Add the entry label to the list */
            continue;
        }
    }

    /* Resolve .entry labels to their corresponding line numbers */
    LinkedList* curr = entries;
    while (curr != NULL) {
        LinkedList* label = get_node_by_label(labels, curr->label); /* Search for the label */
        if (label != NULL) {
            curr->value.number = label->value.number; /* Assign the label's line number to the entry */
        }

        curr = curr->next;
    }

    /* Update the pointers to the linked lists */
    *labels_ptr = labels;
    *entries_ptr = entries;
    *externs_ptr = externs;
}