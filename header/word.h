/**
 * @file word.h
 * @brief Header file for managing machine words in the assembler.
 * 
 * This file defines the `Word` structure and provides functions for creating,
 * managing, and converting machine words. A machine word represents instructions
 * or data in the assembler and includes fields for opcode, addressing modes,
 * registers, function codes, and ARE bits.
 * 
 * Key Features:
 * - Defines the `Word` structure for representing machine words.
 * - Provides functions to:
 *   - Create machine words with various parameters.
 *   - Convert machine words to hexadecimal representation.
 *   - Print machine words to output files.
 *   - Free memory used by machine words.
 */
#ifndef WORD_H
#define WORD_H

#include <stdint.h>

/**
 * @brief Structure representing a machine word.
 * 
 * The `Word` structure stores 24 bits of data within a 32-bit integer.
 * This is used to represent instructions or data in the assembler.
 */
typedef struct {
    uint32_t word; /* Stores 24 bits of data within a 32-bit integer */
} Word;

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
 * @return Pointer to the newly created `Word`, or NULL if allocation fails.
 */
Word* create_word(uint8_t opcode, uint8_t src_mode, uint8_t src_reg,
    uint8_t dest_mode, uint8_t dest_reg, uint8_t funct,
    uint8_t A, uint8_t R, uint8_t E);

/**
 * @brief Creates a machine word from a number with ARE bits.
 * 
 * Allocates memory for a new `Word` and initializes it with the given number
 * and ARE bits.
 * 
 * @param number The number to store in the word.
 * @param A Absolute bit (ARE).
 * @param R Relocatable bit (ARE).
 * @param E External bit (ARE).
 * @return Pointer to the newly created `Word`, or NULL if allocation fails.
 */
Word* create_word_from_number(int16_t number, uint8_t A, uint8_t R, uint8_t E);

/**
 * @brief Creates a machine word from a number without ARE bits.
 * 
 * Allocates memory for a new `Word` and initializes it with the given number.
 * 
 * @param number The number to store in the word.
 * @return Pointer to the newly created `Word`, or NULL if allocation fails.
 */
Word* create_word_from_only_number(int16_t number);

/**
 * @brief Frees a machine word from memory.
 * 
 * Deallocates the memory used by the given `Word`.
 * 
 * @param inst Pointer to the `Word` to free.
 */
void free_word(Word* inst);

/**
 * @brief Converts a machine word to its hexadecimal representation.
 * 
 * Extracts the 24 bits of data from the `Word` and returns it as a 32-bit
 * hexadecimal value.
 * 
 * @param inst Pointer to the `Word` to convert.
 * @return The 24-bit hexadecimal representation of the word.
 */
uint32_t word_to_hex(Word* inst);

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
void print_word_hex(Word* inst, uint8_t *line, FILE* file);

#endif /* WORD_H */