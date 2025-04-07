#ifndef LABELS_H
#define LABELS_H

#include <stdint.h>
#include "../header/lib.h"

/**
 * @brief Enum for the type of value stored in a label.
 * 
 * A label can store either a numeric value or a string value.
 */
typedef enum {
    NUMBER_VALUE,  /* Represents a numeric value */
    STRING_VALUE   /* Represents a string value */
} ValueType;

/**
 * @brief Structure for a linked list node.
 * 
 * Each node in the linked list represents a label and its associated value.
 * The value can be either a number or a string, determined by the `type` field.
 */
typedef struct LinkedList {
    char *label;                /* The label name */
    ValueType type;             /* The type of value (NUMBER_VALUE or STRING_VALUE) */
    union {
        uint8_t number;         /* Stores a numeric value */
        char *buffer;           /* Stores a string value */
    } value;
    struct LinkedList *next;    /* Pointer to the next node in the list */
} LinkedList;

/**
 * @brief Adds a new node with a numeric value to the linked list.
 * 
 * @param head Pointer to the head of the linked list.
 * @param label The label name to add.
 * @param number The numeric value to associate with the label.
 */
void add_label_number(LinkedList **head, const char *label, uint8_t number);

/**
 * @brief Adds a new node with a string value to the linked list.
 * 
 * @param head Pointer to the head of the linked list.
 * @param label The label name to add.
 * @param buffer The string value to associate with the label.
 */
void add_label_string(LinkedList **head, const char *label, const char *buffer);

/**
 * @brief Retrieves and prints the value associated with a given label.
 * 
 * @param head Pointer to the head of the linked list.
 * @param label The label name to search for.
 * @return Pointer to the node containing the label, or NULL if not found.
 */
LinkedList* get_node_by_label(LinkedList *head, const char *label);

/**
 * @brief Checks if a label exists in the linked list.
 * 
 * @param head Pointer to the head of the linked list.
 * @param label The label name to search for.
 * @return true if the label exists, false otherwise.
 */
bool is_label_in_list(LinkedList *head, const char *label);

/**
 * @brief Prints all labels and their associated values.
 * 
 * @param head Pointer to the head of the linked list.
 */
void print_labels(LinkedList *head);

/**
 * @brief Frees all allocated memory in the linked list.
 * 
 * @param head Pointer to the head of the linked list.
 */
void free_label_list(LinkedList *head);

#endif /* LABELS_H */