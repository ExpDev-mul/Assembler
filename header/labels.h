/**
 * @file labels.h
 * @brief Header file for managing the symbol table in the assembler.
 * 
 * This file defines the `SymbolList` structure and provides functions for managing
 * a linked list of symbols (labels). Each symbol can store either a numeric value
 * or a string value, and the type is determined by the `ValueType` enum.
 * 
 * Key Features:
 * - Defines the `SymbolList` structure for representing labels and their values.
 * - Provides functions to:
 *   - Add labels with numeric or string values.
 *   - Retrieve and check for the existence of labels.
 *   - Print all labels and their associated values.
 *   - Free the memory used by the symbol list.
 */
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
typedef struct SymbolList {
    char *label;                /* The label name */
    ValueType type;             /* The type of value (NUMBER_VALUE or STRING_VALUE) */
    union {
        int16_t number;         /* Stores a numeric value */
        char *buffer;           /* Stores a string value */
    } value;
    struct SymbolList *next;    /* Pointer to the next node in the list */
} SymbolList;

/**
 * @brief Adds a new node with a numeric value to the linked list.
 * 
 * @param head Pointer to the head of the linked list.
 * @param label The label name to add.
 * @param number The numeric value to associate with the label.
 */
void add_label_number(SymbolList **head, const char *label, int16_t number);

/**
 * @brief Adds a new node with a string value to the linked list.
 * 
 * @param head Pointer to the head of the linked list.
 * @param label The label name to add.
 * @param buffer The string value to associate with the label.
 */
void add_label_string(SymbolList **head, const char *label, const char *buffer);

/**
 * @brief Retrieves and prints the value associated with a given label.
 * 
 * @param head Pointer to the head of the linked list.
 * @param label The label name to search for.
 * @return Pointer to the node containing the label, or NULL if not found.
 */
SymbolList* get_node_by_label(SymbolList *head, const char *label);

/**
 * @brief Checks if a label exists in the linked list.
 * 
 * @param head Pointer to the head of the linked list.
 * @param label The label name to search for.
 * @return true if the label exists, false otherwise.
 */
bool is_label_in_list(SymbolList *head, const char *label);

/**
 * @brief Prints all labels and their associated values.
 * 
 * @param head Pointer to the head of the linked list.
 */
void print_labels(SymbolList *head);

/**
 * @brief Frees all allocated memory in the linked list.
 * 
 * @param head Pointer to the head of the linked list.
 */
void free_label_list(SymbolList *head);

#endif /* LABELS_H */