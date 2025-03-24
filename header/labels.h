#ifndef LABELS_H
#define LABELS_H

#include <stdint.h>

/* Structure for a linked list node */
typedef struct LabelNode {
    char *label;
    uint8_t line;
    struct LabelNode *next;
} LabelNode;

/* Adds a label to the list (creates head if needed) */
void add_label(LabelNode **head, const char *label, uint8_t line);

/* Retrieves the line number associated with a label */
uint8_t get_line_by_label(LabelNode *head, const char *label);

/* Prints all labels and their corresponding line numbers */
void print_labels(LabelNode *head);

/* Frees all allocated memory in the list */
void free_label_list(LabelNode *head);

#endif /* LABELS_H */