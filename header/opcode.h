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

#define EMPTY_SPOT 0

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
Command commands[] = {
    /* Two operands */
    {"mov",  0, EMPTY_SPOT, 2, {0, 1, 0, 1}, {1, 1, 0, 1}},  /* Move data */
    {"cmp",  1, EMPTY_SPOT, 2, {1, 1, 0, 1}, {1, 1, 0, 1}},  /* Compare */
    {"add",  2, 1,          2, {0, 1, 0, 1}, {1, 1, 0, 1}},  /* Add */
    {"sub",  2, 2,          2, {0, 1, 0, 1}, {1, 1, 0, 1}},  /* Subtract */
    {"lea",  4, 0,          2, {0, 1, 0, 1}, {0, 1, 0, 0}},  /* Load effective address */

    /* One operand */
    {"clr",  5, 1,          1, {0, 1, 0, 1}, {0, 0, 0, 0}},  /* Clear */
    {"not",  5, 2,          1, {0, 1, 0, 1}, {0, 0, 0, 0}},  /* Bitwise NOT */
    {"dec",  5, 4,          1, {0, 1, 0, 1}, {0, 0, 0, 0}},  /* Decrement */
    {"jmp",  9, 1,          1, {0, 1, 1, 0}, {0, 0, 0, 0}},  /* Jump */
    {"bne",  9, 2,          1, {0, 1, 1, 0}, {0, 0, 0, 0}},  /* Branch if not equal */
    {"inc",  5, 3,          1, {0, 1, 0, 1}, {0, 0, 0, 0}},  /* Increment */
    {"jsr",  9, 3,          1, {0, 1, 1, 0}, {0, 0, 0, 0}},  /* Jump to subroutine */
    {"red", 12, EMPTY_SPOT, 1, {0, 1, 0, 1}, {0, 0, 0, 0}},  /* Read input */
    {"prn", 13, EMPTY_SPOT, 1, {1, 1, 0, 1}, {0, 0, 0, 0}},  /* Print */

    /* No operands */
    {"rts", 14, EMPTY_SPOT, 0, {0, 0, 0, 0}, {0, 0, 0, 0}},  /* Return from subroutine */
    {"stop", 15, EMPTY_SPOT, 0, {0, 0, 0, 0}, {0, 0, 0, 0}}, /* Stop execution */
};

#endif /* OPCODE_H */