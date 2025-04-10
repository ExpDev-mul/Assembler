/**
 * @file opcode.h
 * @brief Header file for defining assembler commands and their metadata.
 * 
 * This file defines the `Command` structure and provides an array of supported
 * commands in the assembler. Each command includes its name, opcode, function code,
 * number of operands, and allowed addressing modes for source and destination operands.
 * 
 * Key Features:
 * - Defines the `Command` structure for representing assembler commands.
 * - Provides an array of supported commands (`commands[]`) with their metadata.
 * - Specifies allowed addressing modes for each command's operands.
 */
#ifndef OPCODE_H
#define OPCODE_H

#define EMPTY_SPOT 0 /* When a command has no funct */
#define NUM_COMMANDS 16 /* The total number of available commands  */

#include "../header/lib.h"

/**
 * @brief Structure representing a command in the assembler.
 * 
 * Each command has a name, opcode, funct value, number of operands, and flags
 * indicating the allowed addressing modes for source and destination operands.
 */
typedef struct {
    char* name;                /* Command name (e.g., "mov", "add") */
    int opcode;                /* Command opcode */
    int funct;                 /* Command funct value */
    int operands_num;          /* Number of operands required by the command */
    bool addressing_dest[4];   /* Allowed addressing modes for the destination operand */
    bool addressing_src[4];    /* Allowed addressing modes for the source operand */
} Command;

/**
 * @brief Array of all supported commands in the assembler.
 * 
 * Each entry defines the command's name, opcode, funct value, number of operands,
 * and the allowed addressing modes for source and destination operands.
 */
extern Command commands[];

/**
 * @brief Checks if a given command name is valid.
 * 
 * This function compares the provided command name with the names of all supported commands.
 * 
 * @param command_name The command name to check.
 * @return true if the command name is valid, false otherwise.
 */
bool is_command(char* command_name);

#endif /* OPCODE_H */