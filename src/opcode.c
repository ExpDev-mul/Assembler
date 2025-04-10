#include <string.h>
#include "../header/opcode.h"
#include "../header/lib.h"

Command commands[] = {
    /* Two operands */
    {"mov",  0,  EMPTY_SPOT, 2, {0, 1, 0, 1}, {1, 1, 0, 1}},  /* Move data */
    {"cmp",  1,  EMPTY_SPOT, 2, {1, 1, 0, 1}, {1, 1, 0, 1}},  /* Compare */
    {"add",  2,  1,          2, {0, 1, 0, 1}, {1, 1, 0, 1}},  /* Add */
    {"sub",  2,  2,          2, {0, 1, 0, 1}, {1, 1, 0, 1}},  /* Subtract */
    {"lea",  4,  0,          2, {0, 1, 0, 1}, {0, 1, 0, 0}},  /* Load effective address */

    /* One operand */
    {"clr",  5,  1,          1, {0, 1, 0, 1}, {0, 0, 0, 0}},  /* Clear */
    {"not",  5,  2,          1, {0, 1, 0, 1}, {0, 0, 0, 0}},  /* Bitwise NOT */
    {"dec",  5,  4,          1, {0, 1, 0, 1}, {0, 0, 0, 0}},  /* Decrement */
    {"jmp",  9,  1,          1, {0, 1, 1, 0}, {0, 0, 0, 0}},  /* Jump */
    {"bne",  9,  2,          1, {0, 1, 1, 0}, {0, 0, 0, 0}},  /* Branch if not equal */
    {"inc",  5,  3,          1, {0, 1, 0, 1}, {0, 0, 0, 0}},  /* Increment */
    {"jsr",  9,  3,          1, {0, 1, 1, 0}, {0, 0, 0, 0}},  /* Jump to subroutine */
    {"red",  12, EMPTY_SPOT, 1, {0, 1, 0, 1}, {0, 0, 0, 0}},  /* Read input */
    {"prn",  13, EMPTY_SPOT, 1, {1, 1, 0, 1}, {0, 0, 0, 0}},  /* Print */

    /* No operands */
    {"rts",  14, EMPTY_SPOT, 0, {0, 0, 0, 0}, {0, 0, 0, 0}},  /* Return from subroutine */
    {"stop", 15, EMPTY_SPOT, 0, {0, 0, 0, 0}, {0, 0, 0, 0}}, /* Stop execution */
};

bool is_command(char* command_name) {
    int i;
    for (i = 0; i < NUM_COMMANDS; i++) {
        if (!strcmp(command_name, commands[i].name)) {
            return true;
        }
    }

    return false;
}