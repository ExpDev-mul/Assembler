#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>

/* Buffer size for reading lines from input files */
#define BUFFER_SIZE 81

/* Maximum size allowed for macro contents */
#define MACRO_SIZE 81*5 

/* Number of registers available in the assembler (e.g., r0 to r7) */
#define NUM_REGISTERS 8 

/* Addressing modes for instructions */
#define IMMEDIATE_ADRS 0          /* Immediate addressing (e.g., #5) */
#define DIRECT_ADRS 1             /* Direct addressing (e.g., a label) */
#define RELATIVE_ADRS 2           /* Relative addressing (e.g., &label) */
#define DIRECT_REGISTER_ADRS 3    /* Direct register addressing (e.g., r0, r1) */

/* Starting line number for the assembler */
#define START_LINE 100 

/**
 * @brief Main function to assemble the input file.
 * 
 * @param file Input source file to be assembled.
 * @param am Preprocessed file after macro expansion.
 * @param ob Output file for the object code.
 * @param ent Output file for entries.
 * @param ext Output file for externs.
 */
void assemble(FILE* file, FILE* am, char* base_name);

#endif