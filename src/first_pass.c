#include <string.h>

#include "../header/first_pass.h"
#include "../header/preprocessing.h"
#include "../header/assembler.h"
#include "../header/lib.h"
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
void first_pass(FILE* file, SymbolList** symbols_ptr,
                uint8_t* errors, uint8_t* number_of_lines,
                SymbolList* macros) {    
    /* Null check and initialization */
    if (!symbols_ptr) {
        fprintf(stderr, "Error: symbols_ptr is NULL.\n");
        return;
    }  
    
    /* Line reading buffers */
    char buffer[BUFFER_SIZE];

    /* Initialize linked lists */
    SymbolList* symbols = *symbols_ptr;
    uint8_t line = 0; /* Line counter */
    char *prefix; /* Pointer to the first token in the line */
    char *pos;    /* Pointer to the position of ':' in the label */
    char *arg;    /* Pointer to the argument after the command */
    SymbolList* curr = NULL; /* Pointer to the current node in the linked list */
    int i; /* Loop variable */
    bool stay_in_line = false;
    SymbolList* line_label = NULL; /* Pointer to the line label, when staying in line */
    uint8_t ic = 0;
    uint8_t dc = 0;
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

            if (is_symbol_exists(macros, prefix)) {
                /* Check if the label is a macro */
                error_with_code(LABEL_IS_MACRO_NAME, line, errors);
                continue;
            }

            if (is_symbol_exists(symbols, prefix)) {
                /* Check for duplicate label declarations */
                SymbolList* sym = get_symbol_by_label(symbols, prefix);
                if (sym->symbol_type == SYMBOL_LABEL){
                    /* Check if the label is already defined */
                    error_with_code(LABEL_ALREADY_DEFINED, line, errors);
                    continue;
                }
            }

            line_label = add_symbol_number(&symbols, prefix, ic, SYMBOL_LABEL); /* Add the label to the linked list */
            continue;
        }

        if (prefix[0] == '.'){
            /* Handle directives */
            if (!strcmp(prefix, ".data") || !strcmp(prefix, ".string")) {
                /* Update line label with respect to the current line */
                if (line_label){
                    line_label->symbol_type = SYMBOL_DATA; /* Set the type of the label to SYMBOL_DATA */
                    line_label->value.number = dc; /* Set the value of the label to the current data counter */
                    line_label = NULL; /* Reset the line label */
                }
            }

            if (!strcmp(prefix, ".data")){
                /* Count the data */
                arg = strtok(NULL, " ");

                while (arg != NULL){
                    dc++;
                    arg = strtok(NULL, ",");
                }
            }

            if (!strcmp(prefix, ".string")){
                /* Compute string length */
                arg = strtok(NULL, " ");
                dc += strlen(arg) - 2 + 1; /* Subtract 2 for the quotes, add 1 for null terminator */
            }


            line_label = NULL; /* Reset the line label */


            if (!strcmp(prefix, ".extern")) {
                /* Handle .extern declarations */
                arg = strtok(NULL, " ");
                if (arg == NULL) {
                    /* Check for missing argument */
                    error_with_code(EXTERN_MISSING_ARGUMENT, line, errors);
                    continue;
                }
    
                skip_leading_spaces(&arg);
                if (is_symbol_exists(symbols, arg)) {
                    SymbolList* sym = get_symbol_by_label(symbols, arg);
                    if (sym->symbol_type == SYMBOL_EXTERN){
                        /* Check if the label is already defined */
                        error_with_code(EXTERN_NOT_UNIQUE, line, errors);
                        continue;
                    }

                    if (sym->symbol_type == SYMBOL_ENTRY){
                        /* Check for conflicting entry and extern labels */
                        error_with_code(CONFLICTING_ENTRY_AND_EXTERN, line, errors);
                        continue;
                    }
                }
    
                add_symbol_number(&symbols, arg, -1, SYMBOL_EXTERN); /* Add the extern label to the list */
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

                if (is_symbol_exists(symbols, arg)) {
                    SymbolList* sym = get_symbol_by_label(symbols, arg);
                    if (sym->symbol_type == SYMBOL_ENTRY){
                        /* Check if the label is already defined */
                        error_with_code(ENTRY_ALREADY_DEFINED, line, errors);
                        continue;
                    }

                    if (sym->symbol_type == SYMBOL_EXTERN){
                        /* Check for conflicting entry and extern labels */
                        error_with_code(CONFLICTING_ENTRY_AND_EXTERN, line, errors);
                        continue;
                    }
                }
    
                add_symbol_number(&symbols, arg, 0, SYMBOL_ENTRY); /* Add the entry label to the list*/
                continue;
            }
        } else {
            for (i = 0; i < NUM_COMMANDS; i++) {
                if (!strcmp(prefix, commands[i].name)) {
                    if (line_label){
                        line_label->symbol_type = SYMBOL_INSTRUCTION; /* Set the type of the label to SYMBOL_DATA */
                    }
                    ic++;

                    if (commands[i].operands_num > 0) {
                        char *arg1 = strtok(NULL, ",");
                        char *arg2 = commands[i].operands_num == 2 ? strtok(NULL, ",") : NULL;
                        
                        if (arg1) {
                            skip_leading_spaces(&arg1);
                            int8_t is_reg = is_valid_reg(arg1);
                            if (!is_reg) {
                                ic++;
                            }
                        }

                        if (arg2) {
                            skip_leading_spaces(&arg2);
                            int8_t is_reg = is_valid_reg(arg2);
                            if (!is_reg) {
                                ic++;
                            }
                        }
                    }
                    break;
                }
            }

            line_label = NULL; /* Reset the line label */
        }


    }

    /**
     * Memory Address Resolution Phase
     * -----------------------------
     * This phase adjusts all symbol addresses to their final memory locations.
     * Memory layout is organized as follows:
     * 1. Instructions start at address 100 (START_LINE)
     * 2. Data section follows immediately after instructions
     * 3. Entry symbols point to their target label's address
     */

    /* Step 1: Adjust addresses for instruction and data labels */
    curr = symbols;
    while (curr != NULL) {
        if (curr->symbol_type != SYMBOL_ENTRY) {
            if (curr->symbol_type == SYMBOL_DATA) {
                /* Data section comes after instructions, so add:
                * - START_LINE (base address 100)
                * - ic (size of instruction section)
                * - curr->value.number (offset within data section)
                */
                curr->value.number = START_LINE + ic + curr->value.number;
            } else if (curr->symbol_type == SYMBOL_INSTRUCTION) {
                /* Instructions start at START_LINE (100), so add:
                * - START_LINE (base address 100)
                * - curr->value.number (offset within instruction section)
                */
                curr->value.number = START_LINE + curr->value.number;
            }
        }
        curr = curr->next;
    }

    /* Step 2: Resolve entry symbol addresses
    * Entry symbols need to point to their target label's final address.
    * An entry can point to either:
    * - An instruction label (address = START_LINE + offset)
    * - A data label (address = START_LINE + ic + offset)
    */
    curr = symbols;
    while (curr != NULL) {
        if (curr->symbol_type == SYMBOL_ENTRY) {
            /* First try to find an instruction label with this name */
            SymbolList* inst = get_symbol_by_label_filter(symbols, curr->label, SYMBOL_INSTRUCTION);
            if (inst != NULL) {
                /* Found instruction label - use its address (already includes START_LINE) */
                curr->value.number = inst->value.number;
            }

            /* If no instruction found, try to find a data label */
            SymbolList* data = get_symbol_by_label_filter(symbols, curr->label, SYMBOL_DATA);
            if (data != NULL) {
                /* Found data label - use its address (already includes START_LINE + ic) */
                curr->value.number = data->value.number;
            }
        }
        curr = curr->next;
    }
    
    /* DEBUG: Displays symbols list immediately after first-pass. */
    /* print_symbols(symbols) */

    /* Update the pointers to the linked lists */
    *symbols_ptr = symbols;
}