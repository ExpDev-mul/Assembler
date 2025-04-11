#include <stdio.h>
#include <stdlib.h>

#include "../header/word_list.h" /* <stdint.h>, word.h are already included within */
#include "../header/assembler.h" /* Mainly for constants extraction */

void add_word(WordList **head, Word *word) {
    if (!word) {
        fprintf(stderr, "Error: Word pointer is NULL\n");
        return;
    }

    WordList *new_node = (WordList *)malloc(sizeof(WordList));
    if (!new_node) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    new_node->data.word = word; /* Store the Word pointer in the union */
    new_node->is_line = false;  /* Indicate that this node stores a Word */
    new_node->next = *head;     /* Insert at the beginning for O(1) insertion */
    *head = new_node;
}

void add_line(WordList **head, uint8_t line) {
    WordList *new_node = (WordList *)malloc(sizeof(WordList));
    if (!new_node) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    new_node->data.line = line; /* Store the line number in the union */
    new_node->is_line = true;   /* Indicate that this node stores a line */
    new_node->next = *head;     /* Insert at the beginning for O(1) insertion */
    *head = new_node;
}

void reverse_list(WordList **head) {
    WordList *prev = NULL;
    WordList *current = *head;
    WordList *next = NULL;

    while (current != NULL) {
        next = current->next; /* Store the next node */
        current->next = prev; /* Reverse the link */
        prev = current;       /* Move prev to current */
        current = next;       /* Move to the next node */
    }

    *head = prev; /* Update the head pointer */
}

uint8_t get_mode(char *arg) {
    /* Ensure the argument is not NULL */
    if (arg == NULL) {
        return 0;
    }

    /* Determine the addressing mode based on the prefix */
    if (arg[0] == 'r') {
        /* Register case (e.g., r0, r2, r9) */
        return DIRECT_REGISTER_ADRS;
    } else if (arg[0] == '#') {
        /* Immediate number case (e.g., #5, #-3) */
        return IMMEDIATE_ADRS;
    } else if (arg[0] == '&') {
        /* Relative addressing case (e.g., &START, &END) */
        return RELATIVE_ADRS;
    } else {
        /* Direct addressing case (e.g., MAIN, END, basically labels) */
        return DIRECT_ADRS;
    }
}