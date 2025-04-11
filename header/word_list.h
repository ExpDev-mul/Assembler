#ifndef WORD_LIST_H
#define WORD_LIST_H

#include <stdint.h>
#include "./lib.h"
#include "./word.h"

/**
 * @brief Node in the linked list of machine words
 * 
 * Can store either:
 * - Machine word (instruction/data)
 * - Line number (for error reporting)
 */
typedef struct WordList {
    union {
        Word *word;       /* Machine word pointer */
        uint8_t line;     /* Source line number */
    } data;
    bool is_line;         /* true if storing line number */
    struct WordList *next;
} WordList;

/**
 * @brief Adds a machine word to the list in O(1) time complexity
 * @param head Pointer to list head
 * @param word Word to add (takes ownership)
 */
void add_word(WordList **head, Word *word);

/**
 * @brief Adds a line number to the list in O(1) time complexity
 * @param head Pointer to list head
 * @param line Line number to store
 */
void add_line(WordList **head, uint8_t line);

/**
 * @brief Reverses the word list in O(n) time complexity
 * 
 * Required because words are added in reverse order
 * @param head Pointer to list head
 */
void reverse_list(WordList **head);

/**
 * @brief Frees all memory used by the list
 * @param head List to free
 */
void free_word_list(WordList *head);

#endif /* WORD_LIST_H */