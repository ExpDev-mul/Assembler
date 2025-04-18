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
#include "../header/symbols.h"
#include "../header/preprocessing.h"
#include "../header/errors.h"
#include "../header/validators.h"
#include "../header/second_pass.h" /* Already includes word_list.h */

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
 * @brief Determines the addressing mode based on the argument prefix.
 * 
 * This function identifies the addressing mode of an operand based on its prefix.
 * 
 * @param arg The argument representing the operand (e.g., r0, #5, &START).
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

/**
 * @brief Processes an operand based on its addressing mode and generates an extra instruction if needed.
 * 
 * This function handles the processing of an operand in the second pass of the assembler. It validates
 * the operand, determines its addressing mode, and generates an extra instruction if required. The function
 * also updates the relative address flag if the operand is a label or external reference.
 * 
 * @param mode The addressing mode of the operand (e.g., IMMEDIATE_ADRS, DIRECT_ADRS, etc.).
 * @param arg The operand argument as a string (e.g., "#5", "LABEL", "&LABEL").
 * @param labels A linked list of labels defined in the program.
 * @param externs A linked list of external labels.
 * @param line The current line number being processed in the source file.
 * @param memory_line The current memory line number in the instruction list.
 * @param errors A pointer to the error counter, incremented if an error occurs.
 * 
 * @return A pointer to a `Word` structure representing the extra instruction, or NULL if no extra instruction is needed.
 */
Word* process_operand(int8_t mode, char *arg, 
                      SymbolList **symbols_ptr, uint8_t line, 
                      uint8_t *ic, uint8_t *errors) {
    Word *extra_instruction = NULL;
    SymbolList* symbols = *symbols_ptr;
    SymbolList *ptr = get_symbol_by_label(symbols, arg);

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
            /* Direct addressing creates a word with the label's memory address */
            ptr = get_symbol_by_label(symbols, arg);
            if (ptr != NULL) {
                /*
                printf("%s %i %i\n", arg, ptr->symbol_type, ptr->value.number);
                */
                if (ptr->symbol_type == SYMBOL_DATA || 
                    ptr->symbol_type == SYMBOL_INSTRUCTION ||
                    ptr->symbol_type == SYMBOL_ENTRY) {
                    /* Create word with the label's actual memory address */
                    extra_instruction = create_word_from_number(ptr->value.number, 0, 1, 0);
                }

                if (ptr->symbol_type == SYMBOL_EXTERN){
                    extra_instruction = create_word_from_number(ptr->value.number, 0, 0, 1); /* External word */
                    add_symbol_number(&symbols, arg, START_LINE + *ic, SYMBOL_EXTERN);
                }
            }
            break;
        }

        case RELATIVE_ADRS: {
            /* Relative addressing creates a word with the relative distance */
            arg++; /* Skip the '&' character */
            ptr = get_symbol_by_label(symbols, arg);
            if (ptr != NULL) {
                int16_t target_address = ptr->value.number;
                int16_t current_address = START_LINE + *ic;
                int16_t relative_distance = target_address - current_address;
                
                extra_instruction = create_word_from_number(relative_distance, 1, 0, 0);
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

    *symbols_ptr = symbols; /* Update the symbols pointer */
    return extra_instruction;
}

void second_pass(FILE *preprocessed, SymbolList **symbols_ptr, 
                WordList **inst_list_ptr, WordList **data_list_ptr, 
                uint8_t *ic, uint8_t *dc, uint8_t *errors) {
    /* File-level variables and data structures */
    int i; /* Loop counter for command table traversal */
    SymbolList *symbols = *symbols_ptr; /* Local symbol table reference */
    WordList *inst_list = *inst_list_ptr; /* Local instruction list reference */
    WordList *data_list = *data_list_ptr; /* Local data list reference */
    char buffer[BUFFER_SIZE]; /* Line reading buffer */
    bool stay_in_line = false; /* Flag to continue processing current line */
    uint8_t line = 0; /* Current source file line number */
    bool is_command = false; /* Flag indicating if current token is a valid command */

    /* String processing variables */
    char *current; /* Current position in processing buffer */
    char *number_start; /* Start position of numeric value */
    char temp; /* Temporary character storage for string manipulation */

    /* Command argument pointers */
    char *arg1; /* First command argument */
    char *arg2; /* Second command argument */
    char *arg3; /* Third argument (for error checking) */
    char *arg; /* Current argument being processed */

    /* Command processing data */
    uint8_t opcode; /* Command operation code */
    uint8_t funct; /* Command function code */
    int8_t src_mode; /* Source operand addressing mode */
    uint8_t src_reg; /* Source register number */
    int8_t dest_mode; /* Destination operand addressing mode */
    uint8_t dest_reg; /* Destination register number */

    /* Error tracking */
    uint8_t before_errors; /* Error count before processing current command */

    /* Word generation */
    Word* extra_instruction_one; /* Additional word for source operand */
    Word* extra_instruction_two; /* Additional word for destination operand */
    Word* instruction; /* Main instruction word */

    /* Directive processing */
    char *metadata; /* Directive data (for .data and .string) */
    Command cmd; /* Current command being processed */

    /* Mode tracking */
    bool src_mode_defined; /* Flag indicating source mode was set */
    bool dest_mode_defined; /* Flag indicating destination mode was set */

    
    while (1){
        char *command; /* The first token separated by a comma is our actual command. */
        if (stay_in_line){
            command = strtok(NULL, " ");
            stay_in_line = false;
        } else {
            if (fgets(buffer, BUFFER_SIZE, preprocessed) == NULL){
                break; /* EOF has been reached, or an error has occured. */
            }
            
            buffer[strcspn(buffer, "\n")] = '\0'; /* Remove newline character automatically inserted by fgets */
            command = strtok(buffer, " "); /* Tokenize the command (e.g, mov, add, stop) */
            skip_leading_spaces(&command); /* Skip leading spaces for command */
            line++;
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
                metadata = strtok(NULL, "");
                if (metadata == NULL) {
                    error_with_code(MISSING_DATA, line, errors);
                    return;
                }

                current = metadata;
                while (*current != '\0') {
                    skip_leading_spaces(&current); /* Skip leading spaces */

                    number_start = current; /* Mark the start of the number */

                    /* Find the end of the current number */
                    while (*current != ',' && *current != '\0') {
                        current++;
                    }

                    /* Null-terminate the current number */
                    temp = *current;
                    *current = '\0';

                    /* Validate and process the number */
                    if (!is_valid_number(number_start)) {
                        error_with_code(INVALID_DATA_VALUE, line, errors);
                        break;
                    }

                    instruction = create_word_from_only_number((int8_t)atoi(number_start));
                    add_word(&data_list, instruction); /* Add the instruction to the data list */
                    (*dc)++;

                    /* Restore the character and move to the next number */
                    *current = temp;
                    if (*current == ',') {
                        current++; /* Skip the comma */
                    }
                }
            } else if (!strcmp(command, "string")) {
                /* Handle .string directive */
                metadata = strtok(NULL, "");
                if (metadata == NULL) {
                    error_with_code(MISSING_DATA, line, errors);
                    return;
                }

                if (!is_valid_string(metadata)) {
                    /* Invalid string format */
                    error_with_code(INVALID_STRING_FORMAT, line, errors);
                } else {
                    metadata++; /* Skip the opening double quote */
                    while (*metadata != '"' && *metadata != '\0') {
                        instruction = create_word_from_only_number((int8_t)(*metadata));
                        add_word(&data_list, instruction); /* Add the instruction to the data list */
                        metadata++;
                        (*dc)++;
                    }

                    /* Add null terminator */
                    instruction = create_word_from_only_number(0);
                    add_word(&data_list, instruction); /* Add the null terminator to the data list */
                    (*dc)++;
                }
            }

            continue;
        }

        arg1 = strtok(NULL, ","); /* Tokenize 1st argument */
        skip_leading_spaces(&arg1); /* Skip leading spaces of the arg1 argument (e.g, from __r0 -> r0 ) */
        arg2 = strtok(NULL, ","); /* Tokenize 2nd argument */
        skip_leading_spaces(&arg2); /* Skip leading spaces of the arg2 argument (e.g, from ___#5 -> #5 ) */
        arg3 = strtok(NULL, ","); /* Extraneous tokenized argument, mainly for extraneous text checking */

        /* printf("%s %s %s\n", command, arg1, arg2); */

        /* Loop through commands table to match the command */
        for (i = 0; i < NUM_COMMANDS; i++) {
            cmd = commands[i]; /* Store the command metadata */
            /* Check whether the command and the read command match with their names */
            if (!strcmp(command, cmd.name)) {
                is_command = true;

                /* Declare variables at the beginning of the scope */
                opcode = cmd.opcode; /* Command opcode */
                funct = cmd.funct;   /* Command function code */
                src_mode = -1;        /* Source addressing mode (-1 indicates uninitialized) */
                src_reg = 0;         /* Source register */
                dest_mode = -1;       /* Destination addressing mode (-1 indicates uninitialized) */
                dest_reg = 0;        /* Destination register */
                before_errors = *errors; /* Track errors before processing the command */
                extra_instruction_one = NULL; /* Extra instruction for certain addressing modes */
                extra_instruction_two = NULL; /* Extra instruction for certain addressing modes */
                arg = NULL;            /* Argument pointer for processing operands */

                /* Handle commands with different operand numbers */
                switch (cmd.operands_num){ /* Respect different operand numbers */
                    case 2:
                        /* Commands with 2 operands */
                        if (arg1 == NULL || arg2 == NULL){
                            /* Missing arguments */
                            error_with_code(MISSING_ARGUMENTS, line, errors);
                            break;
                        }

                        if (arg3 != NULL){
                            /* Too many arguments */
                            error_with_code(EXTRANEOUS_TEXT, line, errors);
                            break;
                        }

                        /* Extract destination metadata */
                        if (!is_valid_mode(arg2)){
                            /* Invalid mode */
                            error_with_code(INVALID_DEST_ADDRESSING, line, errors);
                            break;
                        }
                        
                        dest_mode = get_mode(arg2);
                        dest_reg = get_reg(arg2);

                        /* Extract source metadata */

                        if (!is_valid_mode(arg1)){
                            /* Invalid mode */
                            error_with_code(INVALID_SOURCE_ADDRESSING, line, errors);
                            break;
                        }

                        src_mode = get_mode(arg1);
                        src_reg = get_reg(arg1);
                        break;

                    case 1:
                        /* Command with 1 operand */
                        if (arg1 == NULL){
                            /* Missing argument */
                            error_with_code(MISSING_ARGUMENTS, line, errors);
                            break;
                        }

                        if (arg2 != NULL){
                            /* Too many arguments */
                            error_with_code(EXTRANEOUS_TEXT, line, errors);
                            break;
                        }

                        /* Extract destination metadata */
                        if (!is_valid_mode(arg1)){
                            /* Invalid mode */
                            error_with_code(INVALID_DEST_ADDRESSING, line, errors);
                            break;
                        }

                        dest_mode = get_mode(arg1);
                        dest_reg = get_reg(arg1);
                        break;
                    case 0:
                        if (arg1 != NULL){
                            /* Too many arguments */
                            error_with_code(EXTRANEOUS_TEXT, line, errors);
                            break;
                        }

                        break;
                    default:
                        printf("Command %s contains too many operands.", cmd.name);
                        return;
                }

                /*
                printf("%i,%i,%i,%i,%i,%i\n", opcode, src_mode, src_reg,  dest_mode, dest_reg, funct);
                */

                if (*errors > before_errors){
                    /* There was an error throughout the switch statement */
                    continue;
                }

                if (src_mode != -1 && !cmd.addressing_src[src_mode]){
                    /* If there is a valid source mode yet not allowed */
                    error_with_code(INVALID_SOURCE_ADDRESSING, line, errors);
                    continue;
                }

                if (dest_mode != -1 && !cmd.addressing_dest[dest_mode]){
                    /* If there is a valid dest mode yet not allowed */
                    error_with_code(INVALID_DEST_ADDRESSING, line, errors);
                    continue;
                }

                src_mode_defined = (src_mode != -1);
                dest_mode_defined = (dest_mode != -1);
                /* Return the modes back to 0 if they were -1 (which was temporary flagging for the absence of such operand!) */
                if (src_mode == -1){ src_mode = 0; }
                if (dest_mode == -1){ dest_mode = 0; }

                instruction = create_word(
                    opcode, src_mode, src_reg, 
                    dest_mode, dest_reg,
                    funct, 1, 0, 0
                ); /* Create instruction (absolute, for main instructions) */

                (*ic)++;
                add_word(&inst_list, instruction); /* Add the instruction to the instruction list */

                if (cmd.operands_num == 2) {
                    /*
                        The only possible scenario now is 2 operands, and a src_mode left untreated.
                        Therefore, we must also look into the src_mode in this scenario.
                    */
                
                    /* Process the source operand */
                    arg = arg1; /* The source argument */
                    extra_instruction_one = process_operand(src_mode_defined ? src_mode : -1, arg, 
                                                            &symbols, line, ic, errors);
                }
                
                if (extra_instruction_one != NULL){
                    /* If there is an extra instruction, we will output it to .ob file */
                    (*ic)++;
                    add_word(&inst_list, extra_instruction_one); /* Add the instruction to the instruction list */
                }

                
                /* Process the destination operand */
                arg = (cmd.operands_num == 2) ? arg2 : arg1; /* The destination argument */
                extra_instruction_two = process_operand(dest_mode_defined ? dest_mode : -1, arg,
                                                        &symbols, line, ic, errors);
                if (extra_instruction_two != NULL){
                    /* If there is an extra instruction, we will output it to .ob file */
                    (*ic)++;
                    add_word(&inst_list, extra_instruction_two); /* Add the instruction to the instruction list */
                }

                break;
            }
        }

        if (!is_command){
            /* Invalid command name */
            error_with_code(INVALID_COMMAND_NAME, line, errors);
            continue;
        }
    }

    /* Update the pointers to the linked lists */
    *symbols_ptr = symbols;
    *inst_list_ptr = inst_list;
    *data_list_ptr = data_list;
}