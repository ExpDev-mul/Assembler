/**
 * @file assembler.h
 * @brief Header file for the assembler's core functionality.
 * 
 * This file defines constants, macros, and the main function prototype for assembling
 * input files into machine code. It provides the necessary tools for handling input files,
 * addressing modes, and memory image generation.
 * 
 * Key Features:
 * - Defines constants for buffer sizes, macro sizes, and the number of registers.
 * - Specifies addressing modes for instructions (e.g., immediate, direct, relative).
 * - Provides the `assemble` function, which processes input files and generates output files
 *   such as `.ob` (object file), `.ent` (entries file), and `.ext` (externals file).
 */

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>

/* General Constants */
#define BUFFER_SIZE 81               /* Buffer size for reading lines from input files */
#define MACRO_SIZE (BUFFER_SIZE * 7) /* Maximum size allowed for macro contents */
#define NUM_REGISTERS 8              /* Number of registers available in the assembler (e.g., r0 to r7) */
#define START_LINE 100               /* Starting line number for the memory image (.ob file) */

/* Addressing Modes */
#define IMMEDIATE_ADRS 0          /* Immediate addressing (e.g., #5) */
#define DIRECT_ADRS 1             /* Direct addressing (e.g., a label) */
#define RELATIVE_ADRS 2           /* Relative addressing (e.g., &label) */
#define DIRECT_REGISTER_ADRS 3    /* Direct register addressing (e.g., r0, r1) */

/**
 * @brief Main function to assemble the input file.
 * 
 * @param file Input source file to be assembled.
 * @param am Preprocessed file after macro expansion.
 * @param base_name Base name for the output files (e.g., .ob, .ent, .ext).
 */
void assemble(FILE* file, FILE* am, char* base_name);

#endif