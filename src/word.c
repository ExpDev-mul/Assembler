#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "../header/word.h"

/**
 * @brief Creates a machine word with the given parameters.
 * 
 * Allocates memory for a new `Word` and initializes it with the provided
 * opcode, addressing modes, registers, function code, and ARE bits.
 * 
 * @param opcode The opcode of the instruction.
 * @param src_mode The addressing mode of the source operand.
 * @param src_reg The source register.
 * @param dest_mode The addressing mode of the destination operand.
 * @param dest_reg The destination register.
 * @param funct The function code of the instruction.
 * @param A Absolute bit (ARE).
 * @param R Relocatable bit (ARE).
 * @param E External bit (ARE).
 * @return Pointer to the newly created `Word`, or exits on memory allocation failure.
 */
Word* create_word(uint8_t opcode, uint8_t src_mode, uint8_t src_reg,
                  uint8_t dest_mode, uint8_t dest_reg, uint8_t funct,
                  uint8_t A, uint8_t R, uint8_t E) {
    Word* inst = (Word*)malloc(sizeof(Word));
    if (inst == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    /* Construct the 24-bit word */
    inst->word = ((uint32_t)opcode << 18) |
                 ((uint32_t)src_mode << 16) |
                 ((uint32_t)src_reg << 13) |
                 ((uint32_t)dest_mode << 11) |
                 ((uint32_t)dest_reg << 8) |
                 ((uint32_t)funct << 3) |
                 ((uint32_t)A << 2) |
                 ((uint32_t)R << 1) |
                 ((uint32_t)E);
    return inst;
}

/**
 * @brief Creates a machine word from a number with ARE bits.
 * 
 * Allocates memory for a new `Word` and initializes it with the given number
 * and ARE bits. The number is masked to fit within 21 bits.
 * 
 * @param number The number to store in the word.
 * @param A Absolute bit (ARE).
 * @param R Relocatable bit (ARE).
 * @param E External bit (ARE).
 * @return Pointer to the newly created `Word`, or exits on memory allocation failure.
 */
Word* create_word_from_number(int8_t number, uint8_t A, uint8_t R, uint8_t E) {
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

/**
 * @brief Creates a machine word from a number without ARE bits.
 * 
 * Allocates memory for a new `Word` and initializes it with the given number.
 * The number is masked to fit within 24 bits.
 * 
 * @param number The number to store in the word.
 * @return Pointer to the newly created `Word`, or exits on memory allocation failure.
 */
Word* create_word_from_only_number(int8_t number) {
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

/**
 * @brief Frees a machine word from memory.
 * 
 * Deallocates the memory used by the given `Word`.
 * 
 * @param inst Pointer to the `Word` to free.
 */
void free_word(Word* inst) {
    free(inst);
}

/**
 * @brief Converts a machine word to its hexadecimal representation.
 * 
 * Extracts the 24 bits of data from the `Word` and returns it as a 32-bit
 * hexadecimal value.
 * 
 * @param inst Pointer to the `Word` to convert.
 * @return The 24-bit hexadecimal representation of the word.
 */
uint32_t word_to_hex(Word* inst) {
    return inst->word & 0xFFFFFF;
}

/**
 * @brief Prints the hexadecimal representation of a machine word.
 * 
 * Outputs the hexadecimal value of the given `Word` to the specified file,
 * along with the current line number.
 * 
 * @param inst Pointer to the `Word` to print.
 * @param line Pointer to the current line number.
 * @param file The file to write the output to.
 */
void print_word_hex(Word* inst, uint8_t *line, FILE* file) {
    fprintf(file, "%07d %06x\n", *line, word_to_hex(inst));
    (*line)++;
}