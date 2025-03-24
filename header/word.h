#ifndef WORD_H
#define WORD_H

#include <stdint.h>

typedef struct {
    uint32_t word; /* We store 24 of our bits within this 32 bit-integer */
} Word;

/* Create instruction by memory allocation with given parameters */
Word* create_word(uint8_t opcode, uint8_t src_mode, uint8_t src_reg,
    uint8_t dest_mode, uint8_t dest_reg, uint8_t funct,
    uint8_t A, uint8_t R, uint8_t E);

Word* create_word_from_number(uint8_t number,
    uint8_t A, uint8_t R, uint8_t E);

Word* create_word_from_only_number(uint8_t number);

/* Free an instruction from memory */
void free_word(Word* inst);

/* Convert an instruction into its hexadecimal form */
uint32_t word_to_hex(Word* inst);

/* Print out the hexadecimal form of an instruction, mainly for debugging */
void print_word_hex(Word* inst);

#endif /* WORD_H */