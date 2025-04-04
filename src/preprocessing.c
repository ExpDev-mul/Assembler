#include <string.h>
#include <stdio.h>

#include "../header/preprocessing.h"
#include "../header/assembler.h"
#include "../header/labels.h"
#include "../header/lib.h"

void preprocess(FILE* file, FILE* temp){
    /* Line reading buffers */
    char buffer[BUFFER_SIZE];
    char buffer_copy[BUFFER_SIZE];

    /* Macro buffers */
    char macro_buffer[BUFFER_SIZE*10];
    char macro_name[10];

    LinkedList* macros = NULL;
    bool is_reading_macro = false; /* Flag for whetehr we're reading a macro */

    /* Buffer for externs, maximum 10 externs each of length 10 */
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
}