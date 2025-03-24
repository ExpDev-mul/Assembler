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

void assemble(FILE* file);

#endif