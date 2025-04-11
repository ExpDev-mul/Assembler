/**
 * @file first_pass.h
 * @brief Header file for the first pass of the assembler.
 * 
 * This file defines the functionality for the first pass of the assembler. During this pass,
 * the assembler processes the input file to extract labels, entries, and externs, and validates
 * the syntax of the input. It also records any errors encountered during this stage.
 * 
 * Key Features:
 * - Extracts and stores labels, entries, and externs in their respective symbol lists.
 * - Validates the syntax of the input file.
 * - Tracks and reports errors encountered during the first pass.
 * - Prepares the data for the second pass by building the symbol table.
 */
#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <stdio.h>
#include <stdint.h>

#include "./symbols.h"

/**
 * @brief Performs the first pass of the assembler.
 * 
 * This function processes the input file during the first pass of the assembler.
 * It extracts labels, entries, and externs, and validates the syntax of the input.
 * Any errors encountered during this pass are recorded in the `errors` counter.
 * 
 * @param file The input file to process.
 * @param symbols_ptr Pointer to the linked list of labels.
 * @param errors Pointer to the error counter to track the number of errors.
 * @param number_of_lines Pointer to the variable to store the number of lines in the input file.
 * @param macros Pointer to the linked list of macros.
 */
void first_pass(FILE* file, SymbolList** symbols_ptr, 
                uint8_t* errors, uint8_t* number_of_lines,
                SymbolList* macros);

#endif /* FIRST_PASS_H */