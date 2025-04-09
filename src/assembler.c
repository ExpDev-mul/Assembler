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
    int i;                          /* Loop variable */
    char buffer[BUFFER_SIZE];       /* Buffer for reading lines */
    uint8_t line = START_LINE;      /* Current line number */
    LinkedList *labels = NULL;      /* Linked list for labels */
    LinkedList *entries = NULL;     /* Linked list for entry labels */
    LinkedList *externs = NULL;     /* Linked list for extern labels */
    WordList *inst_list = NULL;     /* Linked list for instruction instructions */
    WordList *data_list = NULL;     /* Linked list for data instructions */
    FILE *preprocessed = am;        /* Temporary file for preprocessed content */
    uint8_t errors = 0;             /* Counter for errors during runtime */
    uint8_t ic = 0;                 /* Instruction counter */
    uint8_t dc = 0;                 /* Data counter */
    bool is_command = false;        /* Flag to indicate if a valid command is found */
    bool stay_in_line = false;      /* Flag to indicate if we should stay on the same line */
    FILE* ob;
    FILE* ent;
    FILE* ext;
    char path[256];

    /* Step 1: Preprocessing */
    preprocess(file, preprocessed); /* Expand macros and preprocess the input file */

    /* Step 2: First Pass */
    rewind(preprocessed);

    uint8_t number_of_lines = 0;
    first_pass(preprocessed, &labels, &entries, &externs, &errors, &number_of_lines); /* Extract labels and validate syntax */
    /* Check for errors after the first pass */
    if (errors > 0) {
        fprintf(stderr, "Errors found in the first pass. Exiting...\n");
        fclose(preprocessed);
        fclose(am);
        return;
    }

    /* Step 3: Second Pass */
    uint8_t *offsets_map = calloc(number_of_lines, sizeof(uint8_t));
    if (!offsets_map) {
        fprintf(stderr, "Failed to allocate memory for offsets_map\n");
        return;
    }

    rewind(preprocessed);
    second_pass(preprocessed, &labels, &entries, &externs, &inst_list, &data_list, &ic, &dc, &errors, number_of_lines, offsets_map);

    /*
    for (i = 0; i < number_of_lines; i++) {
        printf("Line %3d: offset = %d\n", i, offsets_map[i]);
    }
    */

    /* Only if no errors occured, create output files */
    if (errors == 0){
        snprintf(path, sizeof(path), "./outputs/%s.ob", base_name);
        ob = fopen(path, "w+");
        if (!ob) {
            fprintf(stderr, "Error opening .ob file for writing: %s\n", path);
            fclose(file);
            return;
        }

        /* Write the instruction counter (IC) and data counter (DC) to the object file, dynamically */
        int ic_length = snprintf(NULL, 0, "%d", ic); /* Compute length for ic */
        int padding = 9 - ic_length; /* Formula for padding that matches our scenario */
        fprintf(ob, "%*d %d\n", padding, ic, dc); /* Aligns IC and DC with an 8-character gap */

        /* Print out instructions (which come before data) */
        reverse_list(&inst_list); /* Reverse the data list for correct order */
        WordList *curr_wl = inst_list; /* Pointer to traverse the data list */
        while (curr_wl != NULL) {
            if (curr_wl->is_line){
                uint8_t index = curr_wl->data.line - START_LINE - 1;
                if (index >= 0 && index < number_of_lines){
                    Word* inst = create_word_from_number(
                        curr_wl->data.line + offsets_map[index], 0, 1, 0
                    );

                    print_word_hex(inst, &line, ob);
                    free_word(inst);
                }
            } else if (curr_wl->data.word != NULL) {
                print_word_hex(curr_wl->data.word, &line, ob); /* Output the data instruction to .ob file */
            }

            WordList* curr_wl_nptr = curr_wl;
            curr_wl = curr_wl->next; /* Move to the next data instruction */
            free(curr_wl_nptr); /* Free linked list from memory */
        }

        /* Print out directives (which come after instructions) */
        reverse_list(&data_list); /* Reverse the data list for correct order */
        curr_wl = data_list; /* Pointer to traverse the data list */
        while (curr_wl != NULL) {
            if (curr_wl->is_line){
                uint8_t index = curr_wl->data.line - START_LINE - 1;
                if (index >= 0 && index < number_of_lines){
                    Word* inst = create_word_from_number(
                        curr_wl->data.line + offsets_map[index], 0, 1, 0
                    );

                    print_word_hex(inst, &line, ob);
                    free_word(inst);
                }
            } else if (curr_wl->data.word != NULL) {
                print_word_hex(curr_wl->data.word, &line, ob); /* Output the data instruction to .ob file */
            }

            WordList* curr_wl_nptr = curr_wl;
            curr_wl = curr_wl->next; /* Move to the next data instruction */
            free(curr_wl_nptr); /* Free linked list from memory */
        }

        /**
        * @brief Writes entries and externs to their respective files and finalizes the output.
        * 
        * This section writes the entry and extern labels to their respective files, frees the
        * allocated memory for the labels list, and appends the instruction and data counters (IC and DC)
        * to the beginning of the object file.
        */
        LinkedList *curr; /* LinkedList iterator variable */

        /* Write entry labels to the .ent file, in case they are non-empty */
        if (entries != NULL){ /* Check if entries list is non-empty */
            snprintf(path, sizeof(path), "./outputs/%s.ent", base_name);
            ent = fopen(path, "w+");
            if (!ent) {
                fprintf(stderr, "Error opening .ent file for writing: %s\n", path);
                fclose(file);
                return;
            }

            curr = entries; /* Pointer to traverse the entries linked list */
            while (curr != NULL) {
                uint8_t index = curr->value.number - START_LINE - 1;
                if (index >= 0 && index < number_of_lines){
                    fprintf(ent, "%s %07d\n", curr->label, curr->value.number + offsets_map[index]); /* Write label and value */
                }
                curr = curr->next; /* Move to the next entry */
            }
        }

        if (externs != NULL){ /* Check if exeterns list is non-empty */
            /* Write extern labels to the .ext file */
            snprintf(path, sizeof(path), "./outputs/%s.ext", base_name);
            ext = fopen(path, "w+");
            if (!ext) {
                fprintf(stderr, "Error opening .ext file for writing: %s\n", ext);
                fclose(file);
                return;
            }

            curr = externs; /* Reuse the pointer to traverse the externs linked list */
            while (curr != NULL) {
                uint8_t index = curr->value.number - START_LINE - 1;
                if (index >= 0 && index < number_of_lines){
                    fprintf(ext, "%s %07d\n", curr->label, curr->value.number + offsets_map[index]); /* Write label and value */
                }            
                curr = curr->next; /* Move to the next extern */
            }
        }
    }

    /* Free the memory allocated for the labels linked list */
    free_label_list(labels);
}