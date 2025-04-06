#ifndef OPCODE_H
#define OPCODE_H

#define EMPTY_SPOT 0

#include "../header/lib.h"

typedef struct {
    char* name; /* Command name */
    int opcode; /* Command opcode */
    int funct; /* Command funct value */
    int operands_num; /* Command's number of operands */

    /*
        Each spot corresponds to whether it's allowed or not.
        1: It is allowed
        0: It is not allowed
    */
    bool addressing_dest[4]; /* Flags of which addressing methods are defined for dest operand */
    bool addressing_src[4]; /* Flags of which addressing methods are defined for src operand */
} Command;

Command commands[] = {
    /* Two operands */
    {"mov", 0, EMPTY_SPOT, 2,   {0, 1, 0, 1}, {1, 1, 0, 1}},
    {"cmp", 1, EMPTY_SPOT, 2,   {1, 1, 0, 1}, {1, 1, 0, 1}},
    {"add", 2, 1, 2,            {0, 1, 0, 1}, {1, 1, 0, 1}},
    {"sub", 2, 2, 2,            {0, 1, 0, 1}, {1, 1, 0, 1}},
    {"lea", 4, 0, 2,            {0, 1, 0, 1}, {0, 1, 0, 0}},

    /* One operand */
    {"clr", 5, 1, 1,            {0, 1, 0, 1}, {0, 0, 0, 0}},
    {"not", 5, 2, 1,            {0, 1, 0, 1}, {0, 0, 0, 0}},
    {"dec", 5, 4, 1,            {0, 1, 0, 1}, {0, 0, 0, 0}},
    {"jmp", 9, 1, 1,            {0, 1, 1, 0}, {0, 0, 0, 0}},
    {"bne", 9, 2, 1,            {0, 1, 1, 0}, {0, 0, 0, 0}},
    {"inc", 5, 3, 1,            {0, 1, 0, 1}, {0, 0, 0, 0}},
    {"jsr", 9, 3, 1,            {0, 1, 1, 0}, {0, 0, 0, 0}},
    {"red", 12, EMPTY_SPOT, 1,  {0, 1, 0, 1}, {0, 0, 0, 0}},
    {"prn", 13, EMPTY_SPOT, 1,  {1, 1, 0, 1}, {0, 0, 0, 0}},

    /* No operands */
    {"rts", 14, EMPTY_SPOT, 0,  {0, 0, 0, 0}, {0, 0, 0, 0}},
    {"stop", 15, EMPTY_SPOT, 0, {0, 0, 0, 0}, {0, 0, 0, 0}},
};



#endif /* OPCODE_H */