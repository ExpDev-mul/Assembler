#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <stdio.h>
#include <stdint.h>

/**
 * @brief Performs the first pass of the assembler.
 * 
 * This function processes the input file during the first pass of the assembler.
 * It extracts labels, entries, and externs, and validates the syntax of the input.
 * Any errors encountered during this pass are recorded in the `errors` counter.
 * 
 * @param file The input file to process.
 * @param labels_ptr Pointer to the linked list of labels.
 * @param entries_ptr Pointer to the linked list of entry labels.
 * @param externs_ptr Pointer to the linked list of extern labels.
 * @param errors Pointer to the error counter to track the number of errors.
 */
void first_pass(FILE* file, LinkedList** labels_ptr, 
                LinkedList** entries_ptr, LinkedList** externs_ptr, 
                uint8_t* errors, uint8_t* number_of_lines);

#endif /* FIRST_PASS_H */