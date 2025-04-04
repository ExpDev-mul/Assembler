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
uint8_t get_reg(char *arg, char* buffer){
    if (arg[0] == 'r') {
        /* Register case (e.g., r0, r2, r9) */
        if (isdigit(arg[1])){
            return atoi(&arg[1]); /* Extract register number to be returned */
        } else {
            error_with_code(IMMEDIATE_NUMBER_DECLARATION, buffer, &errors);
            return 0;
        }
    }
    
    return 0; /* Default return to 0 for safety */
}

int8_t extract_number(char *arg, char* buffer) {
    if (arg == NULL) {
        fprintf(stderr, "Argument is null\n");
        printf("%s\n", buffer);
        exit(EXIT_FAILURE);
    }
    if (arg[0] != '#'){
        printf("ARG|%s|", arg);
        fprintf(stderr, "Invalid input format\n");
        exit(EXIT_FAILURE);
    }

    /* Convert the substring after '#' to an integer */
    int num = atoi(arg + 1);
    
    if (arg[1] != '0' && num == 0){ /* We must distinguish between a 0 and an error case */
        /* When atoi returns 0, it means the parsing went wrong. Meaning this is not a number. */
        error_with_code(IMMEDIATE_NUMBER_DECLARATION, buffer, &errors);
        return 0;
    }

    return (int8_t)num;
}

Command commands[]; /* Declare variable prototype, imported from opcode.h */

void assemble(FILE* file, FILE* am, FILE* ob, FILE* ent, FILE* ext){
    int i; /* Loop variable */

    char buffer[BUFFER_SIZE]; /* Declare buffer for lines reading */

    uint8_t line = START_LINE; /* Index of our current reading line */

    LinkedList *labels = NULL;
    LinkedList *entries = NULL;
    LinkedList *externs = NULL;

    FILE *preprocessed = am; /* After macro file, we'll denote 'preprocessed' */
    uint8_t errors = 0; /* Counter for the numbers of errors during runtime */

    uint8_t ic = 0; /* Instructions counter */
    uint8_t dc = 0; /* Data counter */



    preprocess(file, preprocessed); /* Preprocess our file, and insert the data into the temporary file declared above. */
    rewind(preprocessed); /* Rewind to the beginning of our file, to be read again. */
    first_pass(preprocessed, &labels, &entries, &externs, &errors); /* First pass to extract labels and externs. */
    rewind(preprocessed); /* Rewind to the beginning of our file, to be read again. */
    if (errors > 0){
        /* If there were errors, we will not continue */
        fprintf(stderr, "Errors found in first pass. Exiting...\n");
        fclose(preprocessed);
        fclose(am);
        fclose(ob);
        fclose(ent);
        fclose(ext);
        return;
    }

    bool is_command = false;
    bool stay_in_line = false;
    while (1){
        char *command; /* The first token separated by a comma is our actual command. */
        if (stay_in_line){
            stay_in_line = false;
            command = strtok(NULL, " ");
        } else {
            if (fgets(buffer, BUFFER_SIZE, preprocessed) == NULL){
                break; /* EOF has been reached, or an error has occured. */
            }
            
            /* Because fgets includes the new line charcter, we remove it from the end of the string. */
            buffer[strcspn(buffer, "\n")] = '\0';
            command = strtok(buffer, " ");
        }
        
        if (command == NULL){
            continue;
        }

        if (command[strlen(command) - 1] == ':'){
            /* If the command ends with a colon, it is a label */
            stay_in_line = true;
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
                    Word *instruction = create_word_from_only_number((int8_t)atoi(current)); 
                    print_word_hex(instruction, &line, ob);

                    current = strtok(NULL, ",");
                    dc++;
                }
            } else if (!strcmp(command, "string")) {
                /*
                    
                    Every character is converted into its ASCII character, as an instruction, excluding (A,R,E)

                */

                uint8_t counter = 0; /* Counter for length of array */
                metadata++; /* Advance to ignore the opening (") symbol */
                while (*metadata != '"' && *metadata != '\0') { /* Loop until closing quote or end of string */
                    Word *instruction = create_word_from_only_number((int8_t)(*metadata)); /* Convert character to instruction */
                    print_word_hex(instruction, &line, ob); /* Print the instruction */
                    
                    metadata++; /* Move to the next character */
                    dc++;
                }

                /* Print out null terminator */
                Word *instruction = create_word_from_only_number(0); /* Null terminator is assigned the 0 value */
                print_word_hex(instruction, &line, ob); /* Print the instruction */
                dc++;
            }
            
            continue;
        }
        
        char *arg1 = strtok(NULL, ","); /* The 2nd token should correspond to the first argument */
        skip_leading_spaces(&arg1);
        char *arg2 = strtok(NULL, ","); /* The 3rd token should correspond to the second argument */
        skip_leading_spaces(&arg2);
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
                int8_t src_mode = -1;
                uint8_t src_reg = 0;
                int8_t dest_mode = -1;
                uint8_t dest_reg = 0;

                uint8_t before_errors = errors;
                switch (cmd.operands_num){ /* Respect different operand numbers */
                    case 2:
                        /* Commands with 2 operands */
                        if (arg1 == NULL || arg2 == NULL){
                            /* Missing arguments */
                            error_with_code(MISSING_ARGUMENTS, buffer, &errors);
                            break;
                        }

                        if (arg3 != NULL){
                            /* Too many arguments */
                            error_with_code(EXTRANEOUS_TEXT, buffer, &errors);
                            break;
                        }

                        /* Extract destination metadata */
                        dest_mode = get_mode(arg2);
                        dest_reg = get_reg(arg2, buffer);

                        /* Extract source metadata */
                        src_mode = get_mode(arg1);
                        src_reg = get_reg(arg1, buffer);
                        break;

                    case 1:
                        /* Command with 1 operand */
                        if (arg1 == NULL){
                            /* Missing argument */
                            error_with_code(MISSING_ARGUMENTS, buffer, &errors);
                            break;
                        }

                        if (arg2 != NULL){
                            /* Too many arguments */
                            error_with_code(EXTRANEOUS_TEXT, buffer, &errors);
                            break;
                        }

                        /* Extract destination metadata */
                        dest_mode = get_mode(arg1);
                        dest_reg = get_reg(arg1, buffer);
                        break;
                    case 0:
                        if (arg1 != NULL){
                            /* Too many arguments */
                            error_with_code(EXTRANEOUS_TEXT, buffer, &errors);
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
                    error_with_code(INVALID_SOURCE_ADDRESSING, buffer, &errors);
                    continue;
                }

                if (dest_mode != -1 && !cmd.addressing_dest[dest_mode]){
                    /* If there is a valid dest mode yet not allowed */
                    error_with_code(INVALID_DEST_ADDRESSING, buffer, &errors);
                    continue;
                }

                /* Switch block scope variables to be reused */
                Word *extra_instruction_one = NULL; /* An extra instruction (only for certain types of commands) */
                Word *extra_instruction_two = NULL; /* An extra instruction (only for certain types of commands) */

                char* arg = (cmd.operands_num == 2 ? arg2 : arg1);
                switch (dest_mode){
                    case IMMEDIATE_ADRS:
                        /*
                        
                            Immediate addressing requires an extra word afterwards.
                    
                        */

                        extra_instruction_one = create_word_from_number(extract_number(arg, buffer), 1, 0, 0); /* Create the extra instruction from that number */
                        break;
                    case DIRECT_ADRS: {
                        /*

                            Direct addressing, requires an extra word afterwards.
                        
                        */

                        LinkedList* ptr = get_node_by_label(labels, arg);
                        
                        if (ptr != NULL){
                            /* If this is a label */
                            extra_instruction_one = create_word_from_number(
                                ptr->value.number,
                                0, 1, 0
                            ); /* Define the extra instruction, using that number solely */
                        
                            add_label_number(&labels, arg, line);
                        } else {
                            ptr = get_node_by_label(externs, arg); /* Search in externs */
                            if (ptr != NULL){
                                extra_instruction_one = create_word_from_number(
                                    ptr->value.number,
                                    0, 0, 1
                                ); /* Define the extra instruction, using that number solely */
                                
                                add_label_number(&externs, arg, line);
                            }
                        }
                        

                        break;
                    }

                    case RELATIVE_ADRS: {
                        /*
                        
                            Relative addressing, requires an extra word afterwards.
                        
                        */

                        arg++; /* Advance from the '&' */
                        LinkedList* ptr = get_node_by_label(labels, arg);
                        
                        if (ptr != NULL){
                            /* If this is a label */
                            extra_instruction_one = create_word_from_number(
                                ptr->value.number - line,
                                1, 0, 0
                            ); /* Define the extra instruction, using that number solely */
                            
                            add_label_number(&labels, arg, line);
                        } else {
                            ptr = get_node_by_label(externs, arg); /* Search in externs */
                            if (ptr != NULL){
                                extra_instruction_one = create_word_from_number(
                                    ptr->value.number - line,
                                    1, 0, 0
                                ); /* Define the extra instruction, using that number solely */
                                
                                add_label_number(&externs, arg, line);
                            }
                        }
                        
                        break;
                    }

                    default:
                        break;
                }

                if (cmd.operands_num == 2){
                    /*
                        The only possible scenario now is 2 operands, and a src_mode left untreated.
                        Therefore we must also look into the dest_mode, in this scenario.
                    */

                    switch (src_mode){
                        case IMMEDIATE_ADRS:
                            /*
                            
                                Immediate addressing requires an extra word afterwards.
                        
                            */
    
                            extra_instruction_two = create_word_from_number(extract_number(arg1, buffer), 1, 0, 0); /* Create the extra instruction from that number */
                            break;
                        case DIRECT_ADRS: {
                            /*
    
                                Direct addressing, requires an extra word afterwards.
                            
                            */
    
                            LinkedList* ptr = get_node_by_label(labels, arg1);
                            
                            if (ptr != NULL){
                                /* If this is a label */
                                extra_instruction_two = create_word_from_number(
                                    ptr->value.number,
                                    0, 1, 0
                                ); /* Define the extra instruction, using that number solely */
                            } else {
                                ptr = get_node_by_label(externs, arg1); /* Search in externs */
                                if (ptr != NULL){
                                    extra_instruction_two = create_word_from_number(
                                        ptr->value.number,
                                        0, 0, 1
                                    ); /* Define the extra instruction, using that number solely */
                                }
                            }
                            
    
                            break;
                        }

                        case RELATIVE_ADRS: {
                            /*
                            
                                Relative addressing, requires an extra word afterwards.
                            
                            */
    
                            arg++;
                            LinkedList* ptr = get_node_by_label(labels, arg);
                            
                            if (ptr != NULL){
                                /* If this is a label */
                                extra_instruction_two = create_word_from_number(
                                    ptr->value.number - line,
                                    1, 0, 0
                                ); /* Define the extra instruction, using that number solely */
                                
                                add_label_number(&labels, arg, line);
                            } else {
                                ptr = get_node_by_label(externs, arg); /* Search in externs */
                                if (ptr != NULL){
                                    extra_instruction_two = create_word_from_number(
                                        ptr->value.number - line,
                                        1, 0, 0
                                    ); /* Define the extra instruction, using that number solely */
                                    
                                    add_label_number(&externs, arg, line);
                                }
                            }
                        }
                        
                        default:
                            break;
                    }
                }

                /*
                    Initialize values back to 0 (the -1 was a temporary flag for the absence of such mode!)
                */
               
                if (src_mode == -1){ src_mode = 0; }
                if (dest_mode == -1){ dest_mode = 0; }

                Word *instruction = create_word(
                    opcode, src_mode, src_reg, 
                    dest_mode, dest_reg,
                    funct, 1, 0, 0
                ); /* Create instruction with respect to the metadata of the line */

                print_word_hex(instruction, &line, ob); /* Output the line in hexadecimal form */

                if (extra_instruction_one != NULL){
                    /* If there is an extra instruction, we will output it */
                    print_word_hex(extra_instruction_one, &line, ob);
                }

                if (extra_instruction_two != NULL){
                    /* If there is an extra instruction, we will output it */
                    print_word_hex(extra_instruction_two, &line, ob);
                }

                break;
            }
        }

        if (!is_command){
            /* Invalid command name */
            error_with_code(INVALID_COMMAND_NAME, buffer, &errors);
        }
    }

    /* Write down entries */
    LinkedList* curr;

    curr = entries;
    while (curr != NULL){
        fprintf(ent, "%s %07d\n", curr->label, curr->value.number);
        curr = curr->next;
    }

    /* Write down externs */
    curr = externs;
    while (curr != NULL){
        fprintf(ext, "%s %07d\n", curr->label, curr->value.number);
        curr = curr->next;
    }

    free_label_list(labels); /* Free labels list memory, that is manually allocated */
    
    rewind(ob); /* Rewind file to append IC and DC at the top */

    char status[10]; /* About ~10 characters */
    sprintf(status, "  %i %i\n", ic, dc);
    /* prepend_to_file(ob, status); */
    printf("%s", status); /* Print for now */
}