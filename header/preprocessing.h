/**
 * @file preprocessing.h
 * @brief Header file for preprocessing tasks in the assembler.
 * 
 * This file defines the functionality for preprocessing the input file before the
 * assembly process. It handles tasks such as macro expansion and cleaning up the
 * input file to prepare it for the first pass.
 * 
 * Key Features:
 * - Provides the `preprocess` function to handle macros and other preprocessing tasks.
 * - Writes the preprocessed content to a temporary file for further assembly.
 */
#ifndef PREPROCESSING_C
#define PREPROCESSING_C

#include <stdio.h>
#include "../header/symbols.h"

/**
 * @brief Preprocesses the input file for the assembler.
 * 
 * This function processes the input file to handle macros and other preprocessing tasks.
 * The processed content is written to a temporary file for further assembly.
 * 
 * @param file The input file to preprocess.
 * @param temp The temporary file to write the preprocessed content to.
 * @param macros_ptr Pointer to the linked list of macros.
 */
void preprocess(FILE* file, FILE* temp, SymbolList** macros_ptr);

#endif /* PREPROCESSING_C */