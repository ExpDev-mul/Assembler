#include <string.h>
#include <stdio.h>

#include "../header/preprocessing.h"
#include "../header/assembler.h"
#include "../header/labels.h"
#include "../header/lib.h"
#include "../header/errors.h"

void preprocess(FILE* file, FILE* temp, LinkedList** labels_ptr, LinkedList** externs_ptr, uint8_t* errors){
    /* Line reading buffers */
    char buffer[BUFFER_SIZE];
    char buffer_copy[BUFFER_SIZE];

    /* Macro buffers */
    char macro_buffer[BUFFER_SIZE*10];
    char macro_name[10];

    LinkedList* labels = *labels_ptr;
    LinkedList* externs = *externs_ptr;

    LinkedList* entries = NULL;
    LinkedList* macros = NULL;
    bool is_reading_macro = false; /* Flag for whetehr we're reading a macro */

    /* Buffer for externs, maximum 10 externs each of length 10 */
    uint8_t line = 100; /* Line reading starts at 0 */
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

        if (prefix[0] == ';'){
            /* This is a comment, skip to next line! */
            continue;
        }
        
        if (!strcmp(prefix, "mcro")){
            /* 
                Macro declaration
            */

            char *arg = strtok(NULL, " "); /* Tokenize macro name */

            if (arg == NULL){
                error_with_code(6, buffer_copy, errors);
            }

            strcpy(macro_name, arg); /* Store the macro's name elsewehere */
            is_reading_macro = true; /* Turn on the macro reading */
            continue;
        }

        if (!strcmp(prefix, "mcroend")){
            /* 
                Macro declaration
            */

            char *macro_name_ptr = macro_name;
            char *macro_buffer_ptr = macro_buffer;
            add_label_string(&macros, macro_name_ptr, macro_buffer_ptr);
            memset(macro_buffer, 0, sizeof(macro_buffer)); /* Clear marco buffer */
            is_reading_macro = false; /* Stop macro reading */
            continue;
        }

        if (is_reading_macro){
            char *buffer_copy_ptr = buffer_copy;
            skip_leading_spaces(&buffer_copy_ptr);
            strncat(macro_buffer, buffer_copy_ptr , MACRO_SIZE);
            strncat(macro_buffer, "\n" , MACRO_SIZE);
            continue;
        }

        if (prefix[strlen(prefix) - 1] == ':'){
            /*
                Declaration of label (labels end with a ':' symbol)
            */

            /* Remove the ':' at the end */
            char *pos = strchr(prefix, ':');
            *pos = '\0';
            add_label_number(&labels, prefix, line); /* Insert label into our linked list */

            char *rest = strchr(buffer_copy, ':');
            rest++;

            skip_leading_spaces(&rest);
            fputs(rest, temp);
            fputc('\n', temp);
            continue;
        }

        if (!strcmp(prefix, ".extern")){
            /*
                Externs
            */

            char *arg = strtok(NULL, " ");
            skip_leading_spaces(&arg);
            if (arg == NULL){
                error_with_code(7, buffer_copy, errors);
                continue;
            }

            add_label_number(&externs, arg, 0);
            continue;
        }

        if (!strcmp(prefix, ".entry")){
            /*
                Entries
            */

            char *arg = strtok(NULL, " ");
            skip_leading_spaces(&arg);
            if (arg == NULL){
                error_with_code(8, buffer_copy, errors);
                continue;
            }

            add_label_number(&entries, arg, 0);
            continue;
        }

        LinkedList* macro_ptr = get_node_by_label(macros, prefix);
        if (macro_ptr != NULL){
            /* If the current label has a certain node it's attached to */
            fputs(macro_ptr->value.buffer, temp);
            continue;
        }
        
        char *buffer_copy_ptr = buffer_copy;
        skip_leading_spaces(&buffer_copy_ptr);
        fputs(buffer_copy_ptr, temp);
        fputc('\n', temp);
    }

    free_label_list(macros);
    *labels_ptr = labels;
    *externs_ptr = externs;

    LinkedList* curr = entries;
    while (curr != NULL){
        LinkedList* label = get_node_by_label(labels, curr->label); /* Search the current .entry argument within the labels */
        if (label != NULL){
            curr->value.number = label->value.number; /* Assign the line of the label, to the value of the entry */
        }

        curr = curr->next;
    }
}