/* Standard Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Header Includes */
#include "../header/assembler.h"
#include "../header/word.h"
#include "../header/lib.h"
#include "../header/labels.h"
#include "../header/errors.h"
#include "../header/preprocessing.h"
#include "../header/first_pass.h"
#include "../header/second_pass.h"

uint8_t errors; /* Prototype for errors counter, accessed widely through this file context */

void assemble(FILE* file, FILE* am, char* base_name) {
    /* Variable declarations */
    uint8_t line = START_LINE;      /* Current line number */

    /* Note: Our symbols are not a single table but rather split across multiple lists for every symbol decoding possible */
    SymbolList *labels = NULL;      /* Linked list for labels */
    SymbolList *entries = NULL;     /* Linked list for entry labels */
    SymbolList *externs = NULL;     /* Linked list for extern labels */

    WordList *inst_list = NULL;     /* Linked list for instruction instructions */
    WordList *data_list = NULL;     /* Linked list for data instructions */
    FILE *preprocessed = am;        /* Preprocessed is equivalent to 'after macro' in this context */
    uint8_t errors = 0;             /* Counter for errors during runtime */
    uint8_t ic = 0;                 /* Instruction counter */
    uint8_t dc = 0;                 /* Data counter */
    FILE* ob = NULL;                       /* .ob file, to write down on */
    FILE* ent = NULL;                      /* .ent file, to write down on */
    FILE* ext = NULL;                      /* .ext file, to write down on */
    char path[256];                 /* Path buffer for output files */
    uint8_t number_of_lines;        /* Number of lines in the preprocessed file */
    int ic_length;                  /* Length of the instruction counter (IC) */
    int padding;                    /* Computed padding for IC and DC display */
    WordList *curr_wl = NULL;              /* Pointer to traverse the data list */
    WordList* curr_wl_nptr = NULL;         /* Temporary pointer */

    /* Step 1: Preprocessing */
    preprocess(file, preprocessed); /* Expand macros and preprocess the input file */

    /* Step 2: First Pass */
    rewind(preprocessed);

    number_of_lines = 0; /* Initialize number of lines counter */
    first_pass(preprocessed, &labels, 
                &entries, &externs, 
                &errors, &number_of_lines); /* Extract labels and validate syntax, while counting the number of lines and updating number_of_lines */

    /* Check for errors after the first pass */
    if (errors > 0) {
        fprintf(stderr, "Errors found in the first pass. Exiting...\n");
        goto cleanup;
    }

    /* Step 3: Second Pass */
    rewind(preprocessed); /* Rewind the preprocessed file (also the after macro!) to be read again by second_pass */
    second_pass(preprocessed, &labels, 
                &entries, &externs, 
                &inst_list, &data_list, 
                &ic, &dc, &errors, 
                number_of_lines); /* Perform second pass */

    /* Only if no errors occured, create output files */
    if (errors == 0){
        snprintf(path, sizeof(path), "./outputs/%s.ob", base_name); /* Set the proper path */
        ob = fopen(path, "w+"); /* Open the path with writing+ perms */
        if (!ob) {
            fprintf(stderr, "Error opening .ob file for writing: %s\n", path);
            goto cleanup; /* Perform cleanup */
        }

        /* Write the instruction counter (IC) and data counter (DC) to the object file, dynamically */
        ic_length = snprintf(NULL, 0, "%d", ic); /* Compute length for ic */
        padding = 9 - ic_length; /* Formula for padding that matches our scenario */
        fprintf(ob, "%*d %d\n", padding, ic, dc); /* Aligns IC and DC with an 8-character gap */

        /* Print out instructions (which come before data) */
        reverse_list(&inst_list); /* Reverse the data list for correct order */
        curr_wl = inst_list; /* Pointer to traverse the data list */



        
        
        /* Traverse the instruction list and process each node */
        while (curr_wl != NULL) {
            /* Check if the current node represents a line (not a word) */
            print_word_hex(curr_wl->data.word, &line, ob); /* Output the data instruction to the .ob file */

            /* Store the current node in a temporary pointer for cleanup */
            WordList* curr_wl_nptr = curr_wl;

            /* Move to the next node in the linked list */
            curr_wl = curr_wl->next;

            /* Free the word in the current node if it is not a line and contains a word */
            if (!curr_wl_nptr->is_line && curr_wl_nptr->data.word != NULL) {
                free_word(curr_wl_nptr->data.word); /* Free the dynamically allocated word */
            }

            /* Free the memory allocated for the current node */
            free(curr_wl_nptr);
        }

        /* Print out directives (which come after instructions) */
        reverse_list(&data_list); /* Reverse the data list for correct order */
        curr_wl = data_list; /* Pointer to traverse the data list */
        
        /* Traverse the instruction list and process each node */
        while (curr_wl != NULL) {
            /* Check if the current node represents a line (not a word) */
            print_word_hex(curr_wl->data.word, &line, ob); /* Output the data instruction to the .ob file */

            /* Store the current node in a temporary pointer for cleanup */
            WordList* curr_wl_nptr = curr_wl;

            /* Move to the next node in the linked list */
            curr_wl = curr_wl->next;

            /* Free the word in the current node if it is not a line and contains a word */
            if (!curr_wl_nptr->is_line && curr_wl_nptr->data.word != NULL) {
                free_word(curr_wl_nptr->data.word); /* Free the dynamically allocated word */
            }

            /* Free the memory allocated for the current node */
            free(curr_wl_nptr);
        }

        /**
        * @brief Writes entries and externs to their respective files and finalizes the output.
        * 
        * This section writes the entry and extern labels to their respective files, frees the
        * allocated memory for the labels list, and appends the instruction and data counters (IC and DC)
        * to the beginning of the object file.
        */
        SymbolList *curr; /* SymbolList iterator variable */



        /*
        print_labels(entries);
        */



        /* Write entry labels to the .ent file, in case they are non-empty */
        if (entries != NULL){ /* Check if entries list is non-empty */
            snprintf(path, sizeof(path), "./outputs/%s.ent", base_name);
            ent = fopen(path, "w+");
            if (!ent) {
                fprintf(stderr, "Error opening .ent file for writing: %s\n", path);
                goto cleanup;
            }

            curr = entries; /* Pointer to traverse the entries linked list */
            while (curr != NULL) {
                fprintf(ent, "%s %07d\n", curr->label, curr->value.number); /* Write label and value */                
                curr = curr->next; /* Move to the next entry */
            }
        }

        if (externs != NULL){ /* Check if exeterns list is non-empty */
            /* Write extern labels to the .ext file */
            snprintf(path, sizeof(path), "./outputs/%s.ext", base_name);
            ext = fopen(path, "w+");
            if (!ext) {
                fprintf("Error opening .ext file for writing: %s\n", ext);
                goto cleanup;
            }

            curr = externs; /* Reuse the pointer to traverse the externs linked list */
            while (curr != NULL) {
                if (curr->value.number != -1){
                    fprintf(ext, "%s %07d\n", curr->label, curr->value.number); /* Write label and value */
                }

                curr = curr->next; /* Move to the next entry */
            }
        }
    }

   /* Cleanup wrapper, to avoid memory leaks */
   cleanup:
        if(labels) free_label_list(labels);
        if(externs) free_label_list(externs);
        if(entries) free_label_list(entries);
        if(ob) fclose(ob);
        if(ent) fclose(ent);
        if(ext) fclose(ext);
        return;
}