#include <string.h>
#include <stdio.h>

#include "../header/preprocessing.h"
#include "../header/assembler.h"
#include "../header/labels.h"
#include "../header/lib.h"

void skip_leading_spaces(char **line){
    while (**line == ' '){
        (*line)++;
    }
}

void preprocess(FILE* file, FILE* temp, LabelNode* head){
    char buffer[BUFFER_SIZE];
    char buffer_copy[BUFFER_SIZE];
    char macro_buffer[BUFFER_SIZE*10];
    char macro_name[10];
    uint8_t line = 100;

    bool is_reading_macro = false;
    while (1){
        if (fgets(buffer, BUFFER_SIZE, file) == NULL){
            break; /* EOF has been reached, or an error has occured. */
        }

        /* Because fgets includes the new line charcter, we remove it from the end of the string. */
        buffer[strcspn(buffer, "\n")] = '\0';

        strcpy(buffer_copy, buffer);
        char *prefix = strtok(buffer, " "); /* The first token separated by a comma is our actual command. */
        
        if (prefix == NULL){
            continue;
        }

        if (macro_name != NULL){
            if (!strcmp(prefix, macro_name)){\
                char *macro_buffer_ptr = macro_buffer;
                fputs(macro_buffer_ptr, temp);
                continue;
            }
        }
        
        
        if (!strcmp(prefix, "mcro")){
            /* 
                Macro declaration
            */

            char *arg = strtok(NULL, " ");

            strcpy(macro_name, arg);
            is_reading_macro = true;
            continue;
        }

        if (!strcmp(prefix, "mcroend")){
            /* 
                Macro declaration
            */

            is_reading_macro = false;
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
            add_label(&head, prefix, line); /* Insert label into our linked list */

            char *rest = strchr(buffer_copy, ':');
            rest++;

            skip_leading_spaces(&rest);
            fputs(rest, temp);
            fputc('\n', temp);
            continue;
        }

        char *buffer_copy_ptr = buffer_copy;
        skip_leading_spaces(&buffer_copy_ptr);
        fputs(buffer_copy_ptr, temp);
        fputc('\n', temp);
    }
}