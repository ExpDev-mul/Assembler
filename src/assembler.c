/* Standard Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Header Includes */
#include "../header/assembler.h"
#include "../header/word.h"
#include "../header/opcode.h"
#include "../header/lib.h"
#include "../header/labels.h"
#include "../header/preprocessing.h"
#include "../header/errors.h"
#include "../header/files.h"

uint8_t errors; /* Prototype for errors */




/* -----GETTERS START----- */

/**
 * @brief Retrieves the addressing mode of an argument.
 * 
 * This function determines the addressing mode of the given argument based on its prefix.
 * 
 * @param arg The argument whose addressing mode is to be determined.
 * @return The addressing mode as a uint8_t value.
 */
uint8_t get_mode(char *arg) {
    /* Ensure the argument is not NULL */
    if (arg == NULL) {
        return 0;
    }

    /* Determine the addressing mode based on the prefix */
    if (arg[0] == 'r') {
        /* Register case (e.g., r0, r2, r9) */
        return DIRECT_REGISTER_ADRS;
    } else if (arg[0] == '#') {
        /* Immediate number case (e.g., #5, #-3) */
        return IMMEDIATE_ADRS;
    } else if (arg[0] == '&') {
        /* Relative addressing case (e.g., &START, &END) */
        return RELATIVE_ADRS;
    } else {
        /* Direct addressing case (e.g., MAIN, END, basically labels) */
        return DIRECT_ADRS;
    }
}

/**
 * @brief Retrieves the register number from an argument.
 * 
 * This function extracts the register number from an argument that represents a register.
 * 
 * @param arg The argument representing a register (e.g., r0, r1).
 * @return The register number as a uint8_t value.
 */
uint8_t get_reg(char *arg) {
    /* Ensure the argument is not NULL */
    if (arg == NULL) {
        return 0;
    }

    /* Extract and return the register number */
    return (uint8_t)atoi(&arg[1]);
}

/**
 * @brief Extracts a number from an immediate addressing argument.
 * 
 * This function extracts the numeric value from an argument that uses immediate addressing.
 * 
 * @param arg The argument in immediate addressing format (e.g., #5, #-3).
 * @return The extracted number as an int8_t value.
 */
int8_t extract_number(char *arg) {
    int num; /* Variable to store the extracted number */

    /* Ensure the argument starts with '#' */
    if (arg == NULL || arg[0] != '#') {
        fprintf(stderr, "Invalid input format\n");
        exit(EXIT_FAILURE);
    }

    /* Convert the substring after '#' to an integer */
    num = atoi(arg + 1);
    return (int8_t)num;
}

/* -----GETTERS END----- */

/* -----VALIDATORS START----- */

/**
 * @brief Validates if an argument represents a valid register.
 * 
 * This function checks if the given argument is a valid register (e.g., r0, r1).
 * 
 * @param arg The argument to validate.
 * @return true if the argument is a valid register, false otherwise.
 */
bool is_valid_reg(char *arg) {
    int reg_num; /* Variable to store the register number */

    /* Ensure the argument is not NULL and starts with 'r' */
    if (arg == NULL || arg[0] != 'r') {
        return false;
    }

    /* Check if the second character is a digit */
    if (!isdigit(arg[1])) {
        return false;
    }

    /* Convert the register number to an integer */
    reg_num = arg[1] - '0';

    /* Ensure the register number is within bounds (0 to NUM_REGISTERS - 1) */
    return reg_num >= 0 && reg_num < NUM_REGISTERS && arg[2] == '\0';
}

/**
 * @brief Validates if an argument represents a valid immediate number.
 * 
 * This function checks if the given argument is a valid immediate number (e.g., #5, #-3).
 * 
 * @param arg The argument to validate.
 * @return true if the argument is a valid immediate number, false otherwise.
 */
bool is_valid_immediate_number(char *arg) {
    int i; /* Loop variable */

    /* Ensure the argument is not NULL and starts with '#' */
    if (arg == NULL || arg[0] != '#') {
        return false;
    }

    /* Check if the characters after '#' are valid digits or signs */
    for (i = (arg[1] == '+' || arg[1] == '-') ? 2 : 1; arg[i] != '\0'; i++) {
        if (!isdigit(arg[i])) {
            return false; /* Non-digit character found */
        }
    }

    return true; /* Valid immediate number */
}

/**
 * @brief Validates if an argument represents a valid number.
 * 
 * This function checks if the given argument is a valid number (e.g., 5, -3).
 * 
 * @param arg The argument to validate.
 * @return true if the argument is a valid number, false otherwise.
 */
bool is_valid_number(char *arg) {
    int i; /* Loop variable */

    /* Ensure the argument is not NULL */
    if (arg == NULL) {
        return false;
    }

    /* Check if the characters are valid digits or signs */
    for (i = (arg[0] == '+' || arg[0] == '-') ? 1 : 0; arg[i] != '\0'; i++) {
        if (!isdigit(arg[i])) {
            return false; /* Non-digit character found */
        }
    }

    return true; /* Valid number */
}

/**
 * @brief Validates if an argument represents a valid addressing mode.
 * 
 * This function checks if the given argument is a valid addressing mode.
 * 
 * @param arg The argument to validate.
 * @return true if the argument is a valid addressing mode, false otherwise.
 */
bool is_valid_mode(char *arg) {
    /* Ensure the argument is not NULL */
    if (arg == NULL) {
        return false;
    }

    /* Check for valid addressing modes */
    if (arg[0] == 'r' || arg[0] == '#' || arg[0] == '&') {
        return true;
    } else if (isalpha(arg[0])) {
        return true; /* Direct addressing case (e.g., labels) */
    }

    return false; /* Invalid addressing mode */
}

/**
 * @brief Validates if an argument represents a valid string.
 * 
 * This function checks if the given argument is a valid string enclosed in double quotes.
 * 
 * @param arg The argument to validate.
 * @return true if the argument is a valid string, false otherwise.
 */
bool is_valid_string(char *arg) {
    int i; /* Loop variable */

    /* Ensure the argument is not NULL and starts with a double quote */
    if (arg == NULL || arg[0] != '"') {
        return false;
    }

    /* Check for a closing double quote */
    for (i = 1; arg[i] != '\0'; i++) {
        if (arg[i] == '"') {
            return true; /* Valid string */
        }
    }

    return false; /* Invalid string */
}

/* -----VALIDATORS END----- */


/**
 * @brief Processes an operand and creates an extra word if required.
 * 
 * This function handles the logic for creating an extra word based on the addressing mode
 * of the given operand. It validates the operand and generates the appropriate extra word.
 * 
 * @param mode The addressing mode of the operand.
 * @param arg The operand argument.
 * @param labels The linked list of labels.
 * @param externs The linked list of externs.
 * @param line The current line number.
 * @param errors Pointer to the error counter.
 * @return A pointer to the created `Word` if an extra word is required, or NULL otherwise.
 */
Word* process_operand(int8_t mode, char *arg, LinkedList *labels, LinkedList *externs, uint8_t line, uint8_t *errors) {
    Word *extra_instruction = NULL; /* Pointer to the extra instruction */

    switch (mode) {
        case IMMEDIATE_ADRS:
            /* Immediate addressing requires an extra word */
            if (!is_valid_immediate_number(arg)) {
                error_with_code(INVALID_IMMEDIATE_VALUE, line, errors);
                break;
            }
            extra_instruction = create_word_from_number(extract_number(arg), 1, 0, 0);
            break;

        case DIRECT_ADRS: {
            /* Direct addressing requires an extra word */
            LinkedList *ptr = get_node_by_label(labels, arg);
            if (ptr != NULL) {
                extra_instruction = create_word_from_number(ptr->value.number, 0, 1, 0);
            } else {
                ptr = get_node_by_label(externs, arg);
                if (ptr != NULL) {
                    extra_instruction = create_word_from_number(ptr->value.number, 0, 0, 1);
                } else {
                    error_with_code(LABEL_NOT_FOUND, line, errors);
                }
            }
            break;
        }

        case RELATIVE_ADRS: {
            /* Relative addressing requires an extra word */
            arg++; /* Skip the '&' character */
            LinkedList *ptr = get_node_by_label(labels, arg);
            if (ptr != NULL) {
                extra_instruction = create_word_from_number(ptr->value.number - line, 1, 0, 0);
            } else {
                error_with_code(LABEL_NOT_FOUND, line, errors);
            }
            break;
        }

        case DIRECT_REGISTER_ADRS:
            /* Direct register addressing requires no extra word */
            if (!is_valid_reg(arg)) {
                error_with_code(REGISTER_OUT_OF_BOUNDS, line, errors);
            }
            break;

        default:
            break;
    }

    return extra_instruction;
}

typedef struct WordList {
    Word *word; /* Pointer to the word */
    struct WordList *next; /* Pointer to the next word in the list */
} WordList;

/**
 * @brief Adds a label with a numeric value to the linked list.
 * 
 * This function creates a new node with the given label and numeric value
 * and inserts it at the beginning of the linked list for O(1) insertion.
 * 
 * @param head Pointer to the head of the linked list.
 * @param label The label name to add.
 * @param number The numeric value to associate with the label.
 */
void add_word(WordList **head, Word* word) {
    if (!word){
        fprintf(stderr, "Error: Word pointer is NULL\n");
        return;
    }

    WordList *new_node = (WordList *)malloc(sizeof(WordList));
    if (!new_node) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    new_node->word = word;
    new_node->next = *head; /* Insert at the beginning for O(1) insertion */
    *head = new_node;
}

/**
 * @brief Reverses our words linked list, for parsing at the end.
 * 
 * This function traverses the linked list and frees each node and its associated word.
 * With an O(n) time complexity, a sufficienly efficient algorithm.
 * 
 * @param head Pointer to the head of the linked list.
 */
void reverse_list(WordList **head) {
    WordList *prev = NULL;
    WordList *current = *head;
    WordList *next = NULL;

    while (current != NULL) {
        next = current->next; /* Store the next node */
        current->next = prev; /* Reverse the link */
        prev = current;       /* Move prev to current */
        current = next;       /* Move to the next node */
    }

    *head = prev; /* Update the head pointer */
}

Command commands[]; /* Declare variable prototype, imported from opcode.h */

/**
 * @brief Assembles the input file into machine code.
 * 
 * This function performs the preprocessing, first pass, and second pass of the assembler.
 * It processes the input file, extracts labels, macros, and instructions, and generates
 * the corresponding output files for machine code, entries, and externs.
 * 
 * @param file The input file to assemble.
 * @param am The preprocessed file (after macro expansion).
 * @param ob The output file for machine code.
 * @param ent The output file for entry labels.
 * @param ext The output file for extern labels.
 */
void assemble(FILE* file, FILE* am, FILE* ob, FILE* ent, FILE* ext) {
    /* Variable declarations */
    int i;                          /* Loop variable */
    char buffer[BUFFER_SIZE];       /* Buffer for reading lines */
    uint8_t line = START_LINE;      /* Current line number */
    LinkedList *labels = NULL;      /* Linked list for labels */
    LinkedList *entries = NULL;     /* Linked list for entry labels */
    LinkedList *externs = NULL;     /* Linked list for extern labels */
    WordList *data_list = NULL; /* Linked list for data instructions */
    FILE *preprocessed = am;        /* Temporary file for preprocessed content */
    uint8_t errors = 0;             /* Counter for errors during runtime */
    uint8_t ic = 0;                 /* Instruction counter */
    uint8_t dc = 0;                 /* Data counter */
    bool is_command = false;        /* Flag to indicate if a valid command is found */
    bool stay_in_line = false;      /* Flag to indicate if we should stay on the same line */



    /* Step 1: Preprocessing */
    preprocess(file, preprocessed); /* Expand macros and preprocess the input file */
    rewind(preprocessed);           /* Rewind the preprocessed file for further processing */

    /* Step 2: First Pass */
    first_pass(preprocessed, &labels, &entries, &externs, &errors); /* Extract labels and validate syntax */
    rewind(preprocessed);           /* Rewind the preprocessed file for the second pass */

    /* Check for errors after the first pass */
    if (errors > 0) {
        fprintf(stderr, "Errors found in the first pass. Exiting...\n");
        fclose(preprocessed);
        fclose(am);
        fclose(ob);
        fclose(ent);
        fclose(ext);
        return;
    }

    /* Debugging: Print extracted labels */
    print_labels(labels);

    /* Step 3: Second Pass */
    while (1){
        char *command; /* The first token separated by a comma is our actual command. */
        if (stay_in_line){
            stay_in_line = false;
            command = strtok(NULL, " ");
        } else {
            if (fgets(buffer, BUFFER_SIZE, preprocessed) == NULL){
                break; /* EOF has been reached, or an error has occured. */
            }
            
            buffer[strcspn(buffer, "\n")] = '\0'; /* Remove newline character automatically inserted by fgets */
            command = strtok(buffer, " "); /* Tokenize the command (e.g, mov, add, stop) */
        }
        
        if (command == NULL){
            /* If the command tokenized is null, it might mean this is an empty line, just skip it */
            continue;
        }

        /* If the current token ends in a ':', it means this is a label declaration */
        if (command[strlen(command) - 1] == ':'){
            stay_in_line = true; /* Skip to the afterwards contents of the current line */
            continue;
        }

        if (command[0] == '.') {
            /* Handle directives (e.g., .data, .string) */
            command++; /* Skip the '.' character */

            if (!strcmp(command, "data")) {
                /* Handle .data directive */
                char *metadata = strtok(NULL, "");
                if (metadata == NULL) {
                    error_with_code(MISSING_DATA, line, &errors);
                    return;
                }

                char *current = metadata;
                while (*current != '\0') {
                    skip_leading_spaces(&current); /* Skip leading spaces */

                    char *number_start = current; /* Mark the start of the number */

                    /* Find the end of the current number */
                    while (*current != ',' && *current != '\0') {
                        current++;
                    }

                    /* Null-terminate the current number */
                    char temp = *current;
                    *current = '\0';

                    /* Validate and process the number */
                    if (!is_valid_number(number_start)) {
                        error_with_code(INVALID_DATA_VALUE, line, &errors);
                        break;
                    }

                    Word *instruction = create_word_from_only_number((int8_t)atoi(number_start));
                    add_word(&data_list, instruction); /* Add the instruction to the data list */
                    dc++;

                    /* Restore the character and move to the next number */
                    *current = temp;
                    if (*current == ',') {
                        current++; /* Skip the comma */
                    }
                }
            } else if (!strcmp(command, "string")) {
                /* Handle .string directive */
                char *metadata = strtok(NULL, " ");
                if (metadata == NULL) {
                    error_with_code(MISSING_DATA, line, &errors);
                    return;
                }

                if (!is_valid_string(metadata)) {
                    /* Invalid string format */
                    error_with_code(INVALID_STRING_FORMAT, line, &errors);
                } else {
                    metadata++; /* Skip the opening double quote */
                    while (*metadata != '"' && *metadata != '\0') {
                        Word *instruction = create_word_from_only_number((int8_t)(*metadata));
                        add_word(&data_list, instruction); /* Add the instruction to the data list */
                        metadata++;
                        dc++;
                    }

                    /* Add null terminator */
                    Word *instruction = create_word_from_only_number(0);
                    add_word(&data_list, instruction); /* Add the null terminator to the data list */
                    dc++;
                }
            }

            continue;
        }

        char *arg1 = strtok(NULL, ","); /* Tokenize 1st argument */
        skip_leading_spaces(&arg1); /* Skip leading spaces of the arg1 argument (e.g, from __r0 -> r0 ) */
        char *arg2 = strtok(NULL, ","); /* Tokenize 2nd argument */
        skip_leading_spaces(&arg2); /* Skip leading spaces of the arg2 argument (e.g, from __r5 -> r5 ) */
        char *arg3 = strtok(NULL, ","); /* Extraneous tokenized argument, mainly for extraneous text checking */

        /* Loop through commands table to match the command */
        for (i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
            Command cmd = commands[i]; /* Store the command metadata */

            /* Check whether the command and the read command match with their names */
            if (!strcmp(command, cmd.name)) {
                is_command = true;

                /* Declare variables at the beginning of the scope */
                uint8_t opcode = cmd.opcode; /* Command opcode */
                uint8_t funct = cmd.funct;   /* Command function code */
                int8_t src_mode = -1;        /* Source addressing mode (-1 indicates uninitialized) */
                uint8_t src_reg = 0;         /* Source register */
                int8_t dest_mode = -1;       /* Destination addressing mode (-1 indicates uninitialized) */
                uint8_t dest_reg = 0;        /* Destination register */
                uint8_t before_errors = errors; /* Track errors before processing the command */
                Word *extra_instruction_one = NULL; /* Extra instruction for certain addressing modes */
                Word *extra_instruction_two = NULL; /* Extra instruction for certain addressing modes */
                char *arg = NULL;            /* Argument pointer for processing operands */

                /* Handle commands with different operand numbers */
                switch (cmd.operands_num){ /* Respect different operand numbers */
                    case 2:
                        /* Commands with 2 operands */
                        if (arg1 == NULL || arg2 == NULL){
                            /* Missing arguments */
                            error_with_code(MISSING_ARGUMENTS, line, &errors);
                            break;
                        }

                        if (arg3 != NULL){
                            /* Too many arguments */
                            error_with_code(EXTRANEOUS_TEXT, line, &errors);
                            break;
                        }

                        /* Extract destination metadata */
                        if (!is_valid_mode(arg2)){
                            /* Invalid mode */
                            error_with_code(INVALID_DEST_ADDRESSING, line, &errors);
                            break;
                        }
                        
                        dest_mode = get_mode(arg2);
                        dest_reg = get_reg(arg2);

                        /* Extract source metadata */

                        if (!is_valid_mode(arg1)){
                            /* Invalid mode */
                            error_with_code(INVALID_SOURCE_ADDRESSING, line, &errors);
                            break;
                        }

                        src_mode = get_mode(arg1);
                        src_reg = get_reg(arg1);
                        break;

                    case 1:
                        /* Command with 1 operand */
                        if (arg1 == NULL){
                            /* Missing argument */
                            error_with_code(MISSING_ARGUMENTS, line, &errors);
                            break;
                        }

                        if (arg2 != NULL){
                            /* Too many arguments */
                            error_with_code(EXTRANEOUS_TEXT, line, &errors);
                            break;
                        }

                        /* Extract destination metadata */
                        if (!is_valid_mode(arg1)){
                            /* Invalid mode */
                            error_with_code(INVALID_DEST_ADDRESSING, line, &errors);
                            break;
                        }

                        dest_mode = get_mode(arg1);
                        dest_reg = get_reg(arg1);
                        break;
                    case 0:
                        if (arg1 != NULL){
                            /* Too many arguments */
                            error_with_code(EXTRANEOUS_TEXT, line, &errors);
                            break;
                        }

                        break;
                    default:
                        printf("Command %s contains too many operands.", cmd.name);
                        return;
                }
            
                /*
                    Debugging line
                */

                
                /*
                printf("%i,%i,%i,%i,%i,%i\n", opcode, src_mode, src_reg,  dest_mode, dest_reg, funct);
                */

                if (errors > before_errors){
                    /* There was an error throughout the switch statement */
                    continue;
                }

                if (src_mode != -1 && !cmd.addressing_src[src_mode]){
                    /* If there is a valid source mode yet not allowed */
                    error_with_code(INVALID_SOURCE_ADDRESSING, line, &errors);
                    continue;
                }

                if (dest_mode != -1 && !cmd.addressing_dest[dest_mode]){
                    /* If there is a valid dest mode yet not allowed */
                    error_with_code(INVALID_DEST_ADDRESSING, line, &errors);
                    continue;
                }

                if (cmd.operands_num == 2) {
                    /*
                        The only possible scenario now is 2 operands, and a src_mode left untreated.
                        Therefore, we must also look into the src_mode in this scenario.
                    */
                
                    /* Process the source operand */
                    arg = arg1; /* The source argument */
                    extra_instruction_one = process_operand(src_mode, arg, labels, externs, line, &errors);
                }
                
                /* Process the destination operand */
                arg = (cmd.operands_num == 2) ? arg2 : arg1; /* The destination argument */
                extra_instruction_two = process_operand(dest_mode, arg, labels, externs, line, &errors);

                
                /* Return the modes back to 0 if they were -1 (which was temporary flagging for the absence of such operand!) */
                if (src_mode == -1){ src_mode = 0; }
                if (dest_mode == -1){ dest_mode = 0; }

                Word *instruction = create_word(
                    opcode, src_mode, src_reg, 
                    dest_mode, dest_reg,
                    funct, 1, 0, 0
                ); /* Create instruction (absolute, for main instructions) */

                print_word_hex(instruction, &line, ob); /* Output the line to .ob file */

                if (extra_instruction_one != NULL){
                    /* If there is an extra instruction, we will output it to .ob file */
                    print_word_hex(extra_instruction_one, &line, ob);
                }

                if (extra_instruction_two != NULL){
                    /* If there is an extra instruction, we will output it to .ob file */
                    print_word_hex(extra_instruction_two, &line, ob);
                }

                break;
            }
        }

        if (!is_command){
            /* Invalid command name */
            error_with_code(INVALID_COMMAND_NAME, line, &errors);
        }
    }



    /* Print out directives (which come after instructions) */
    reverse_list(&data_list); /* Reverse the data list for correct order */
    WordList *curr_wl = data_list; /* Pointer to traverse the data list */
    while (curr_wl != NULL) {
        print_word_hex(curr_wl->word, &line, ob); /* Output the data instruction to .ob file */
        curr_wl = curr_wl->next; /* Move to the next data instruction */
    }

    /**
     * @brief Writes entries and externs to their respective files and finalizes the output.
     * 
     * This section writes the entry and extern labels to their respective files, frees the
     * allocated memory for the labels list, and appends the instruction and data counters (IC and DC)
     * to the beginning of the object file.
     */

    /* Write entry labels to the .ent file */
    LinkedList *curr = entries; /* Pointer to traverse the entries linked list */
    while (curr != NULL) {
        fprintf(ent, "%s %07d\n", curr->label, curr->value.number); /* Write label and value */
        curr = curr->next; /* Move to the next entry */
    }

    /* Write extern labels to the .ext file */
    curr = externs; /* Reuse the pointer to traverse the externs linked list */
    while (curr != NULL) {
        fprintf(ext, "%s %07d\n", curr->label, curr->value.number); /* Write label and value */
        curr = curr->next; /* Move to the next extern */
    }

    /* Free the memory allocated for the labels linked list */
    free_label_list(labels);

    /* Rewind the object file to append IC and DC at the top */
    rewind(ob);

    /* Write the instruction counter (IC) and data counter (DC) to the object file */
    char status[10]; /* Buffer to hold the IC and DC string */
    sprintf(status, "HELLO\n", ic, dc); /* Format IC and DC into the buffer */

    /* Optionally prepend the status to the object file (if implemented) */
    /* prepend_to_file(ob, status); */

    /* Print the IC and DC to the console for debugging purposes */
    printf("%s", status);
}