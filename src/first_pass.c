#include <string.h>

#include "../header/preprocessing.h"
#include "../header/assembler.h"
#include "../header/labels.h"
#include "../header/lib.h"
#include "../header/first_pass.h"
#include "../header/errors.h"
#include "../header/validators.h"
#include "../header/opcode.h"

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
void first_pass(FILE* file, SymbolList** labels_ptr, 
                SymbolList** entries_ptr, SymbolList** externs_ptr, 
                uint8_t* errors, uint8_t* number_of_lines) {
    /* Check for NULL pointers */
    if (file == NULL) {
        fprintf(stderr, "Error: Input file pointer is NULL.\n");
        return;
    }
    if (labels_ptr == NULL || entries_ptr == NULL || externs_ptr == NULL) {
        fprintf(stderr, "Error: One or more SymbolList pointers are NULL.\n");
        return;
    }
    if (errors == NULL) {
        fprintf(stderr, "Error: Error counter pointer is NULL.\n");
        return;
    }
    if (number_of_lines == NULL) {
        fprintf(stderr, "Error: Number of lines pointer is NULL.\n");
        return;
    }
    
    /* Line reading buffers */
    char buffer[BUFFER_SIZE];

    /* Initialize linked lists */
    SymbolList* labels = *labels_ptr; 
    SymbolList* externs = *externs_ptr;
    SymbolList* entries = *entries_ptr;
    uint8_t line = 0; /* Line reading starts one line before the START_LINE */
    char *prefix; /* Pointer to the first token in the line */
    char *pos;    /* Pointer to the position of ':' in the label */
    char *arg;    /* Pointer to the argument after the command */
    SymbolList* label = NULL; /* Pointer to the label node in the linked list */
    SymbolList* curr = NULL; /* Pointer to the current node in the linked list */
    int i; /* Loop variable */
    uint16_t memory_address = START_LINE; /* Memory address for the instruction */
    bool stay_in_line = false;
    while (1) {
        /* Read a line from the file */
        if (stay_in_line){
            prefix = strtok(NULL, " ");
            stay_in_line = false;
        } else {
            if (fgets(buffer, BUFFER_SIZE, file) == NULL){
                break; /* EOF has been reached, or an error has occured. */
            }
            
            buffer[strcspn(buffer, "\n")] = '\0'; /* Remove newline character automatically inserted by fgets */
            prefix = strtok(buffer, " "); /* Tokenize the command (e.g, mov, add, stop) */
            line++;
        }
 

        (*number_of_lines)++;
        /* Remove the newline character from the end of the string */
        skip_leading_spaces(&prefix); /* Skip leading spaces */
        line++; /* Advance to the next line */

        if (prefix[0] == ';') {
            /* Skip evaluating comments (lines starting with ';') */
            continue;
        }

        if (prefix[strlen(prefix) - 1] == ':') {
            /* Handle label declarations */
            stay_in_line = true; /* Skip to the afterwards contents of the current line */
            pos = strchr(prefix, ':');
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

            if (is_label_in_list(externs, prefix)) {
                /* Check if the label is already defined */
                error_with_code(LABEL_NOT_UNIQUE, line, errors);
                continue;
            }

            add_label_number(&labels, prefix, memory_address); /* Add the label to the linked list */
            continue;
        }

        if (prefix[0] == '.'){
            if (!strcmp(prefix, ".extern")) {
                /* Handle .extern declarations */
                arg = strtok(NULL, " ");
                if (arg == NULL) {
                    /* Check for missing argument */
                    error_with_code(EXTERN_MISSING_ARGUMENT, line, errors);
                    continue;
                }
    
                skip_leading_spaces(&arg);
                if (is_label_in_list(labels, arg)) {
                    /* Check for conflicting entry and extern labels */
                    error_with_code(CONFLICTING_ENTRY_AND_EXTERN, line, errors);
                    continue;
                }
    
                if (is_label_in_list(externs, arg)) {
                    /* Check if the label is already defined as an .extern */
                    error_with_code(EXTERN_NOT_UNIQUE, line, errors);
                    continue;
                }
    
                if (is_label_in_list(entries, arg)) {
                    /* Check for conflicting entry and extern labels */
                    error_with_code(CONFLICTING_ENTRY_AND_EXTERN, line, errors);
                    continue;
                }
    
                add_label_number(&externs, arg, -1); /* Add the extern label to the list */
                continue;
            }
    
            if (!strcmp(prefix, ".entry")) {
                /* Handle .entry declarations */
                arg = strtok(NULL, " ");
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
    
                add_label_number(&entries, arg, memory_address); /* Add the entry label to the list, shifted by 1 downward for 0 indexing */
                continue;
            }
        } else {
            for (i = 0; i < NUM_COMMANDS; i++) {
                if (!strcmp(prefix, commands[i].name)) {
                    memory_address++;

                    if (commands[i].operands_num > 0) {
                        char *arg1 = strtok(NULL, ",");
                        char *arg2 = commands[i].operands_num == 2 ? strtok(NULL, ",") : NULL;
                        
                        if (arg1) {
                            skip_leading_spaces(&arg1);
                            int8_t is_reg = is_valid_reg(arg1);
                            if (!is_reg) {
                                memory_address++;
                            }
                        }

                        if (arg2) {
                            skip_leading_spaces(&arg2);
                            int8_t is_reg = is_valid_reg(arg2);
                            if (!is_reg) {
                                memory_address++;
                            }
                        }
                    }
                    break;
                }
            }
        }
    }

    /* Resolve .entry labels to their corresponding line numbers */
    curr = entries;
    while (curr != NULL) {
        if (!curr->label){ /* Check for NULL label */
            continue;
        }
        
        label = get_node_by_label(labels, curr->label); /* Search for the label */
        if (label != NULL) {
            curr->value.number = label->value.number; /* Assign the label's line number to the entry */
        }

        curr = curr->next; /* Move to next node */
    }

    print_labels(labels); /* Print the labels for debugging */

    /* Update the pointers to the linked lists */
    *labels_ptr = labels;
    *entries_ptr = entries;
    *externs_ptr = externs;
}