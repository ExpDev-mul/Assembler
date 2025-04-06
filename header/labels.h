#ifndef LABELS_H
#define LABELS_H

#include <stdint.h>

#include "../header/lib.h"

/* Structure for a linked list node */
typedef enum {
    NUMBER_VALUE,
    STRING_VALUE
} ValueType;

typedef struct LinkedList {
    char *label;
    ValueType type;
    union {
        uint8_t number; /* Stores a number value */
        char *buffer; /* Stores a string buffer */
    } value;
    struct LinkedList *next;
} LinkedList;

/* Adds a new node with a numeric value to the linked list */
void add_label_number(LinkedList **head, const char *label, uint8_t number);

/* Adds a new node with a string value to the linked list */
void add_label_string(LinkedList **head, const char *label, const char *buffer);

/* Retrieves and prints the value associated with a given label */
LinkedList* get_node_by_label(LinkedList *head, const char *label);

/* Checks if a label exists in the linked list */
bool is_label_in_list(LinkedList *head, const char *label);

/* Prints all labels and their associated values */
void print_labels(LinkedList *head);

/* Frees all allocated memory in the linked list */
void free_label_list(LinkedList *head);

#endif /* LABELS_H */