#ifndef PREPROCESSING_C
#define PREPROCESSING_C

#include <stdio.h>
#include "../header/labels.h"

/**
 * @brief Preprocesses the input file for the assembler.
 * 
 * This function processes the input file to handle macros and other preprocessing tasks.
 * The processed content is written to a temporary file for further assembly.
 * 
 * @param file The input file to preprocess.
 * @param temp The temporary file to write the preprocessed content to.
 */
void preprocess(FILE* file, FILE* temp);

#endif /* PREPROCESSING_C */