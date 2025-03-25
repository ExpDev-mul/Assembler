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

uint8_t errors; /* Prototype for errors */

/* Retrieves mode of an argument */
uint8_t get_mode(char *arg){
    if (arg[0] == 'r') {
        /* Register case (e.g., r0, r2, r9) */
        return DIRECT_REGISTER_ADRS;
    } else if (arg[0] == '#') {
        /* Immediate number case (e.g., #5, #-3) */
        return IMMEDIATE_ADRS;
    } else if (arg[0] == '&') {
        /* Relative addressing case (e.g, &START, &END) */
        return RELATIVE_ADRS;
    } else {
        /* Direct addressing case (e.g, MAIN, END, basically labels) */
        return DIRECT_ADRS;
    }

    return 0;
}

/* Retrieves register state of an argument */
uint8_t get_reg(char *arg){
    if (arg[0] == 'r') {
        /* Register case (e.g., r0, r2, r9) */
        if (isdigit(arg[1])){
            return atoi(&arg[1]); /* Extract register number to be returned */
        } else {
            error_with_code(5, &errors);
            return 0;
        }
    }
    
    return 0; /* Default return to 0 for safety */
}

uint8_t extract_number(char *arg) {
    if (arg == NULL || arg[0] != '#') {
        fprintf(stderr, "Invalid input format\n");
        exit(EXIT_FAILURE);
    }

    /* Convert the substring after '#' to an integer */
    int num = atoi(arg + 1);
    
    if (arg[1] != '0' && num == 0){ /* We must distinguish between a 0 and an error case */
        /* When atoi returns 0, it means the parsing went wrong. Meaning this is not a number. */
        error_with_code(5, &errors);
        return 0;
    }

    /* Ensure it fits within 8 bits */
    if (num < 0 || num > 255) {
        fprintf(stderr, "Number out of range for uint8_t\n");
        exit(EXIT_FAILURE);
    }

    return (uint8_t)num;
}

Command commands[]; /* Declare variable prototype, imported from opcode.h */

void assemble(FILE* file){
    int i; /* Loop variable */

    char buffer[BUFFER_SIZE]; /* Declare buffer for lines reading */
    uint8_t line = 100; /* Index of our current reading line */
    LabelNode *head = NULL; /* Head of our labels node */
    FILE *preprocessed = tmpfile(); /* Temporary file, to later be used to store the preprocessed file. */
    uint8_t errors = 0; /* Counter for the numbers of errors during runtime */
    if (!preprocessed){
        perror("Failed to load a file for preprocessing.");
        return;
    }

    preprocess(file, preprocessed, head); /* Preprocess our file, and insert the data into the temporary file declared above. */
    rewind(preprocessed); /* Rewind to the beginning of our file, to be read again. */

    printf("File after Preprocessing:\n");
    printf("-------------------------------\n");
    while (1){
        if (fgets(buffer, BUFFER_SIZE, preprocessed) == NULL){
            break; /* EOF has been reached, or an error has occured. */
        }

        printf("%s", buffer);
    }
    printf("-------------------------------\n");

    rewind(preprocessed); /* Rewind to the beginning of our file, to be read again. */
    while (1){
        if (fgets(buffer, BUFFER_SIZE, preprocessed) == NULL){
            break; /* EOF has been reached, or an error has occured. */
        }
        
        /* Because fgets includes the new line charcter, we remove it from the end of the string. */
        buffer[strcspn(buffer, "\n")] = '\0';

        char *command = strtok(buffer, " "); /* The first token separated by a comma is our actual command. */
        bool is_command = false;

        if (command == NULL){
            continue;
        }

        if (command[0] == '.'){
            /* Variable declaration */
            command++; /* Increase pointer, to dismiss '.' symbol */            
            char *metadata = strtok(NULL, " ");
            if (metadata == NULL) {
                fprintf(stderr, "Error: No metadata found\n");
                return;
            }

            /* Copy metadata into a buffer to prevent strtok issues */
            if (!strcmp(command, "data")) {
                /*
                
                    Every instruction should contain the numerical value (without A,R,E)
                
                */

                char metadata_buffer[BUFFER_SIZE];  /* Adjust size as needed */
                strncpy(metadata_buffer, metadata, sizeof(metadata_buffer) - 1);
                metadata_buffer[sizeof(metadata_buffer) - 1] = '\0';  /* Ensure null termination */

                /* Extract all metadata from variable */
                char *current = strtok(metadata_buffer, ",");
                while (current != NULL) {
                    Word *instruction = create_word_from_only_number((uint8_t)atoi(current)); 
                    print_word_hex(instruction);

                    current = strtok(NULL, ",");
                }
            } else if (!strcmp(command, "string")) {
                /*
                    
                    Every character is converted into its ASCII character, as an instruction, excluding (A,R,E)

                */

                metadata++; /* Advance to ignore the opening (") symbol */
                while (*metadata != '"' && *metadata != '\0') { /* Loop until closing quote or end of string */
                    Word *instruction = create_word_from_only_number((uint8_t)(*metadata)); /* Convert character to instruction */
                    print_word_hex(instruction); /* Print the instruction */
                    
                    metadata++; /* Move to the next character */
                }
            }
            
            continue;
        }
        
        char *arg1 = strtok(NULL, ","); /* The 2nd token should correspond to the first argument */
        char *arg2 = strtok(NULL, ","); /* The 3rd token should correspond to the second argument */
        char *arg3 = strtok(NULL, ","); /* Extra argument! (Strictly more than possible.) */

        for (i = 0; i < sizeof(commands)/sizeof(commands[0]); i++){ /* Iterate over the commands table */
            Command cmd = commands[i]; /* Declare a variable to store the current iterated command */
            if (!strcmp(command, cmd.name)){ /* Check whether the command and the read command match with their names */
                /* 
                    Define all of our values representing the instruction.
                    Later inserted into the word to create a machine instruction.
                */
                
                is_command = true;

                /* Command metadata */
                uint8_t opcode = cmd.opcode;
                uint8_t funct = cmd.funct;

                /* Command arguments metadata */
                uint8_t src_mode = -1;
                uint8_t src_reg = -1;
                uint8_t dest_mode = -1;
                uint8_t dest_reg = -1;

                /* Flags */
                uint8_t A = 1;
                uint8_t R = 0;
                uint8_t E = 0;

                uint8_t before_errors = errors;
                switch (cmd.operands_num){ /* Respect different operand numbers */
                    case 2:
                        /* Commands with 2 operands */
                        if (arg1 == NULL || arg2 == NULL){
                            /* Missing arguments */
                            error_with_code(0, &errors);
                            break;
                        }

                        if (arg3 != NULL){
                            /* Too many arguments */
                            error_with_code(1, &errors);
                            break;
                        }

                        /* Extract source metadata */
                        src_mode = get_mode(arg1);
                        src_reg = get_reg(arg1);

                        /* Extract destination metadata */
                        dest_mode = get_mode(arg2);
                        dest_reg = get_reg(arg2);
                        break;

                    case 1:
                        /* Command with 1 operand */
                        if (arg1 == NULL){
                            /* Missing argument */
                            error_with_code(0, &errors);
                            break;
                        }

                        if (arg2 != NULL){
                            /* Too many arguments */
                            error_with_code(1, &errors);
                            break;
                        }

                        /* Extract destination metadata */
                        dest_mode = get_mode(arg1);
                        dest_reg = get_reg(arg1);
                        break;
                    case 0:
                        if (arg1 != NULL){
                            /* Too many arguments */
                            error_with_code(1, &errors);
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

                /* printf("%i,%i,%i,%i,%i,%i,%i,%i,%i\n", opcode, src_mode, src_reg,  dest_mode, dest_reg, funct, A, R, E); */

                if (errors > before_errors){
                    /* There was an error throughout the switch statement */
                    continue;
                }

                if (src_mode != -1 && !cmd.addressing_src[src_mode]){
                    /* If there is a valid source mode yet not allowed */
                    error_with_code(3, &errors);
                    continue;
                }

                if (dest_mode != -1 && !cmd.addressing_dest[dest_mode]){
                    /* If there is a valid source mode yet not allowed */
                    error_with_code(4, &errors);
                    continue;
                }

                if (src_mode == -1){ src_mode = 0; }
                if (dest_mode == -1){ dest_mode = 0; }

                Word *instruction = create_word(
                    opcode, src_mode, src_reg, 
                    dest_mode, dest_reg,
                    funct, A, R, E
                ); /* Create instruction with respect to the metadata of the line */

                print_word_hex(instruction); /* Output the line in hexadecimal form */

                /* Switch block scope variables to be reused */
                Word *extra_instruction = NULL; /* An extra instruction (only for certain types of commands) */
                switch ((cmd.operands_num == 2) ? src_mode : dest_mode){
                    case IMMEDIATE_ADRS:
                        /*
                        
                            Immediate addressing requires an extra word afterwards.
                    
                        */

                        extra_instruction = create_word_from_number(extract_number(arg1), A, R, E); /* Create the extra instruction from that number */
                        break;
                    case DIRECT_ADRS:
                        /*

                            Direct addressing, requires an extra word afterwards.
                        
                        */

                        extra_instruction = create_word_from_number(
                            get_line_by_label(head, arg1), /* SEGMENTATION FAULT CAUSER */
                            A, R, E
                        ); /* Define the extra instruction, using that number solely */

                        break;
                    default:
                        break;
                }

                if (extra_instruction != NULL){
                    /* If there is an extra instruction, we will output it */
                    print_word_hex(extra_instruction);
                }

                break;
            }
        }

        if (!is_command){
            /* Invalid command name */
            error_with_code(2, &errors);
        }
    }

    free_label_list(head); /* Free labels list memory, that is allocated */
}