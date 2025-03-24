#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "../header/word.h"

Word* create_word(uint8_t opcode, uint8_t src_mode, uint8_t src_reg,
                                    uint8_t dest_mode, uint8_t dest_reg, uint8_t funct,
                                    uint8_t A, uint8_t R, uint8_t E) {
    Word* inst = (Word*)malloc(sizeof(Word));
    if (inst == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    inst->word = ((uint32_t)opcode << 18) |
                 ((uint32_t)src_mode << 15) |
                 ((uint32_t)src_reg << 12) |
                 ((uint32_t)dest_mode << 9) |
                 ((uint32_t)dest_reg << 6) |
                 ((uint32_t)funct << 2) |
                 ((uint32_t)A << 2) |
                 ((uint32_t)R << 1) |
                 ((uint32_t)E);
    return inst;
}

Word* create_word_from_number(uint8_t number,
                                    uint8_t A, uint8_t R, uint8_t E) {
    Word* inst = (Word*)malloc(sizeof(Word));
    if (inst == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    
    /* Ensure 'number' fits within 21 bits */
    uint32_t num21 = (uint32_t)(number & 0x1FFFFF);  /* Mask to 21 bits */
    
    /* Construct the 24-bit word */
    inst->word = (num21 << 3) |  /* Store 'number' in the first 21 bits */
                 ((uint32_t)A << 2) |  /* A in bit 2 */
                 ((uint32_t)R << 1) |  /* R in bit 1 */
                 ((uint32_t)E);        /* E in bit 0 */

    return inst;
}

Word* create_word_from_only_number(uint8_t number) {
    Word* inst = (Word*)malloc(sizeof(Word));
    if (inst == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    
    /* Ensure 'number' expands to use all 24 bits */
    uint32_t num24 = (uint32_t)(number) & 0xFFFFFF;  /* Mask to 24 bits */
    
    /* Construct the 24-bit word */
    inst->word = num24;  /* Store 'number' in all 24 bits */
    return inst;
}

void free_word(Word* inst) {
    free(inst);
}

uint32_t word_to_hex(Word* inst) {
    return inst->word & 0xFFFFFF;
}

void print_word_hex(Word* inst) {
    printf("0x%06X\n", word_to_hex(inst));
}