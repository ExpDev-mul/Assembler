#include <string.h>

#include "../header/preprocessing.h"
#include "../header/assembler.h"
#include "../header/labels.h"
#include "../header/lib.h"
#include "../header/first_pass.h"
#include "../header/errors.h"

void first_pass(FILE* file, LinkedList** labels_ptr, LinkedList** entries_ptr, LinkedList** externs_ptr, uint8_t* errors){
    /* Line reading buffers */
    char buffer[BUFFER_SIZE];
    char buffer_copy[BUFFER_SIZE];

    LinkedList* labels = *labels_ptr;
    LinkedList* externs = *externs_ptr;
    LinkedList* entries = *entries_ptr;

    uint8_t line = START_LINE - 1; /* Line reading starts one line before the START_LINE */
    while (1){
        if (fgets(buffer, BUFFER_SIZE, file) == NULL){
            break; /* EOF has been reached, or an error has occured. */
        }

        /* Because fgets includes the new line charcter, we remove it from the end of the string. */
        buffer[strcspn(buffer, "\n")] = '\0';

        strcpy(buffer_copy, buffer);
        char *prefix = strtok(buffer, " "); /* The first token separated by a comma is our actual command. */
        
        if (prefix == NULL){
            continue; /* Empty line scenario */
        }

        line++; /* Advance to next line */

        if (prefix[0] == ';'){
            /* This is a comment, skip to next line! */
            continue;
        }

        if (prefix[strlen(prefix) - 1] == ':'){
            /*
                Declaration of label (labels end with a ':' symbol)
            */

            /* Remove the ':' at the end */
            char *pos = strchr(prefix, ':');
            *pos = '\0';

            if (strlen(prefix) == 0){
                /* Checks whether the label is empty */
                error_with_code(EMPTY_LABEL_DECLARATION, line, errors);
                continue;
            }

            if (is_label_in_list(labels, prefix)){
                /* Checks whether the label has already been defined */
                error_with_code(LABEL_ALREADY_DEFINED, line, errors);
                continue;
            }

            add_label_number(&labels, prefix, line); /* Insert label into our linked list */
            continue;
        }

        if (!strcmp(prefix, ".extern")){
            /*
                Externs
            */

            char *arg = strtok(NULL, " ");
            skip_leading_spaces(&arg);

            if (arg == NULL){
                /* Checks whether there is a missing argument */
                error_with_code(ENTRY_MISSING_ARGUMENT, line, errors);
                continue;
            }

            if (is_label_in_list(externs, prefix)){
                /* Checks whether the label is already defined as an .extern */
                error_with_code(EXTERN_ALREADY_DEFINED, line, errors);
                continue;
            }

            add_label_number(&externs, arg, line);
            continue;
        }

        if (!strcmp(prefix, ".entry")){
            /*
                Entries
            */

            char *arg = strtok(NULL, " ");
            skip_leading_spaces(&arg);

            if (arg == NULL){
                error_with_code(ENTRY_MISSING_ARGUMENT, line, errors);
                continue;
            }

            if (is_label_in_list(entries, prefix)){
                error_with_code(ENTRY_ALREADY_DEFINED, line, errors);
                continue;
            }

            add_label_number(&entries, arg, line);
            continue;
        }
    }

    LinkedList* curr = entries;
    while (curr != NULL){
        LinkedList* label = get_node_by_label(labels, curr->label); /* Search the current .entry argument within the labels */
        if (label != NULL){
            curr->value.number = label->value.number; /* Assign the line of the label, to the value of the entry */
        }

        curr = curr->next;
    }

    *labels_ptr = labels;
    *entries_ptr = entries;
    *externs_ptr = externs;
}