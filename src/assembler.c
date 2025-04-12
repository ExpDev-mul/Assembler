/* Standard Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Header Includes */
#include "../header/assembler.h"
#include "../header/word.h"
#include "../header/lib.h"
#include "../header/symbols.h"
#include "../header/errors.h"
#include "../header/preprocessing.h"
#include "../header/first_pass.h"
#include "../header/second_pass.h"
#include "../header/word_list.h"
#include "../header/symbols.h"

uint8_t errors; /* Prototype for errors counter, accessed widely through this file context */

void assemble(FILE* file, FILE* am, char* base_name) {
    /* Variable declarations */
    uint8_t line = START_LINE; /* Current line number */
    SymbolList *symbols = NULL;
    WordList *inst_list = NULL; /* Linked list for instruction instructions */
    WordList *data_list = NULL; /* Linked list for data instructions */
    FILE *preprocessed = am; /* Preprocessed is equivalent to 'after macro' in this context */
    uint8_t errors = 0; /* Counter for errors during runtime */
    uint8_t ic = 0; /* Instruction counter */
    uint8_t dc = 0; /* Data counter */
    FILE* ob = NULL; /* .ob file, to write down on */
    FILE* ent = NULL; /* .ent file, to write down on */
    FILE* ext = NULL; /* .ext file, to write down on */
    char path[256]; /* Path buffer for output files */
    uint8_t number_of_lines; /* Number of lines in the preprocessed file */
    int ic_length;/* Length of the instruction counter (IC) */
    int padding; /* Computed padding for IC and DC display */
    WordList *curr_wl = NULL; /* Pointer to traverse the data list */
    WordList* curr_wl_nptr = NULL; /* Temporary pointer */
    SymbolList* macros = NULL; /* Macros that will be modified by preprocess */
    SymbolList* curr; /* SymbolList iterator variable */

    /* Step 1: Preprocessing */
    preprocess(file, preprocessed, &macros); /* Expand macros and preprocess the input file */

    /* Step 2: First Pass */
    rewind(preprocessed);

    number_of_lines = 0; /* Initialize number of lines counter */
    first_pass(preprocessed, &symbols, 
                &errors, &number_of_lines,
                macros); /* Extract labels and validate syntax, while counting the number of lines and updating number_of_lines */

    /* If there are already errors in the first pass, stop the program and perform cleanup */
    if (errors > 0) {
        fprintf(stderr, "Errors found in the first pass. Exiting...\n");
        goto cleanup;
    }

    /* Step 3: Second Pass */
    rewind(preprocessed); /* Rewind the preprocessed file (also the after macro!) to be read again by second_pass */
    second_pass(preprocessed, &symbols, 
                &inst_list, &data_list, 
                &ic, &dc, &errors); /* Perform second pass */

    /* Only if no errors occured, create output files */
    if (errors == 0){
        snprintf(path, sizeof(path), "../outputs/%s.ob", base_name); /* Set the proper path */
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
            curr_wl_nptr = curr_wl;

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
            curr_wl_nptr = curr_wl;

            /* Move to the next node in the linked list */
            curr_wl = curr_wl->next;

            /* Free the word in the current node if it is not a line and contains a word */
            if (!curr_wl_nptr->is_line && curr_wl_nptr->data.word != NULL) {
                free_word(curr_wl_nptr->data.word); /* Free the dynamically allocated word */
            }

            /* Free the memory allocated for the current node */
            free(curr_wl_nptr);
        }








        /*
        print_labels(entries);
        */




        
        /**
         * @brief Handle entry symbols and write to .ent file if any exist
         * 
         * Only creates the .ent file if there are entry symbols to write.
         * Each entry is written in format: "symbol_name memory_address"
         * where memory_address is padded to 7 digits.
         */

         if (count_symbols_by_type(symbols, SYMBOL_ENTRY) > 0) {
            /* Create .ent file only when entries exist */
            snprintf(path, sizeof(path), "../outputs/%s.ent", base_name);
            ent = fopen(path, "w+");
            if (!ent) {
                fprintf(stderr, "Error opening .ent file for writing: %s\n", path);
                goto cleanup;
            }

            /* Write each entry symbol and its resolved address */
            curr = symbols;
            while (curr != NULL) {
                if (curr->symbol_type == SYMBOL_ENTRY) {
                    fprintf(ent, "%s %07d\n", curr->label, curr->value.number); /* Write to .ent */
                }
                curr = curr->next;
            }
        }

        /**
         * @brief Handle external symbols and write to .ext file if any are used
         * 
         * Only creates the .ext file if there are external symbols that were 
         * actually used in the code (value.number >= START_LINE).
         * Each external reference is written in format: "symbol_name usage_address"
         * where usage_address is padded to 7 digits.
         */
        if (count_symbols_by_type(symbols, SYMBOL_EXTERN) > 0) {
            /* Create .ext file only when externals exist and were used */
            snprintf(path, sizeof(path), "../outputs/%s.ext", base_name);
            ext = fopen(path, "w+");
            if (!ext) {
                fprintf(stderr, "Error opening .ext file for writing: %s\n", path);
                goto cleanup;
            }

            /* Write each external symbol and its usage addresses */
            curr = symbols;
            while (curr != NULL) {
                /* Only write externals that were actually used (have an address >= START_LINE) */
                if (curr->symbol_type == SYMBOL_EXTERN && curr->value.number >= START_LINE) {
                    fprintf(ext, "%s %07d\n", curr->label, curr->value.number); /* Create .ext line */
                }
                curr = curr->next;
            }
        }
    }

   /* Cleanup wrapper, significant to avoid memory leaks */
   cleanup:
        if(symbols) free_symbol_list(symbols);
        if(ob) fclose(ob);
        if(ent) fclose(ent);
        if(ext) fclose(ext);
        return;
}