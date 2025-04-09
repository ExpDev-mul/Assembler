#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include <stdio.h>
#include <stdint.h>
#include "../header/word.h"
#include "../header/labels.h"

typedef struct WordList {
    union {
        Word *word;       /* Pointer to the word */
        uint8_t line;     /* Line number to be resolved later */
    } data;               /* Union to store either a word or a line */
    bool is_line;         /* Flag to indicate whether the union stores a line (true) or a word (false) */
    struct WordList *next; /* Pointer to the next word in the list */
} WordList;

/**
 * @brief Adds a Word to the linked list.
 * 
 * This function creates a new node with the given Word pointer
 * and inserts it at the beginning of the linked list for O(1) insertion.
 * 
 * @param head Pointer to the head of the linked list.
 * @param word The Word pointer to add.
 */
void add_word(WordList **head, Word *word);

/**
 * @brief Adds a line to the linked list.
 * 
 * This function creates a new node with the given line number
 * and inserts it at the beginning of the linked list for O(1) insertion.
 * 
 * @param head Pointer to the head of the linked list.
 * @param line The line number to add.
 */
void add_line(WordList **head, uint8_t line);

/**
 * @brief Reverses our words linked list, for parsing at the end.
 * 
 * This function traverses the linked list and frees each node and its associated word.
 * With an O(n) time complexity, a sufficienly efficient algorithm.
 * 
 * @param head Pointer to the head of the linked list.
 */
void reverse_list(WordList **head);

/**
 * @brief Executes the second pass of the assembler.
 * 
 * This function processes the preprocessed file, resolves labels, computes offsets,
 * and generates the final output for the object file.
 * 
 * @param preprocessed The preprocessed file pointer.
 * @param labels Pointer to the linked list of labels.
 * @param entries Pointer to the linked list of entry labels.
 * @param externs Pointer to the linked list of external labels.
 * @param inst_list Pointer to the linked list of instructions.
 * @param data_list Pointer to the linked list of data.
 * @param ic Pointer to the instruction counter.
 * @param dc Pointer to the data counter.
 * @param errors Pointer to the error counter.
 */
void second_pass(FILE *preprocessed, LinkedList **labels, 
                    LinkedList **entries, LinkedList **externs, 
                    WordList **inst_list, WordList **data_list, 
                    uint8_t *ic, uint8_t *dc, 
                    uint8_t *errors, uint8_t number_of_lines,
                    uint8_t *offsets_map);

#endif /* SECOND_PASS_H */