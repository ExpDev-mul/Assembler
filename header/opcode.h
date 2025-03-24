#ifndef OPCODE_H
#define OPCODE_H

#define EMPTY_FUNCT 0

typedef struct {
    char* name;
    int opcode;
    int funct;
    int operands_num;
} Command;

Command commands[] = {
    /* Two operands */
    {"mov", 0, EMPTY_FUNCT, 2},
    {"cmp", 1, EMPTY_FUNCT, 2},
    {"add", 2, 1, 2},
    {"sub", 2, 2, 2},
    {"lea", 4, 0, 2},

    /* One operand */
    {"clr", 5, 1, 1},
    {"not", 5, 2, 1},
    {"inc", 5, 3, 1},
    {"dec", 5, 4, 1},
    {"jmp", 9, 1, 1},
    {"bne", 9, 2, 1},
    {"jsr", 9, 3, 1},
    {"red", 12, EMPTY_FUNCT, 1},
    {"prn", 13, EMPTY_FUNCT, 1},

    /* No operands */
    {"rts", 14, EMPTY_FUNCT, 1},
    {"stop", 15, EMPTY_FUNCT, 1},
};



#endif /* OPCODE_H */