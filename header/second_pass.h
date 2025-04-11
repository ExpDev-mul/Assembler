#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "./word_list.h"

/**
 * @brief Second pass of the assembler
 * 
 * Handles:
 * - Label resolution
 * - Memory address calculation
 * - External reference processing
 * - Machine code generation
 * 
 * Memory layout:
 * - Instructions start at START_LINE (100)
 * - Data section follows instructions
 * - External references use special ARE bits
 * 
 * @param preprocessed Preprocessed source file
 * @param symbols_ptr Symbol table pointer
 * @param inst_list Instructions list pointer
 * @param data_list Data list pointer
 * @param ic Instruction counter
 * @param dc Data counter
 * @param errors Error counter
 */
void second_pass(FILE *preprocessed, SymbolList **symbols_ptr, 
                WordList **inst_list, WordList **data_list, 
                uint8_t *ic, uint8_t *dc, 
                uint8_t *errors);

#endif /* SECOND_PASS_H */