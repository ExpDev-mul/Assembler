#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>

/* Macros */
#define BUFFER_SIZE 1024
#define MACRO_SIZE 1024

#define IMMEDIATE_ADRS 0
#define DIRECT_ADRS 1
#define RELATIVE_ADRS 2
#define DIRECT_REGISTER_ADRS 3

#define START_LINE 100

void assemble(FILE* file, FILE* am, FILE* ob, FILE* ent, FILE* ext);

#endif