#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../header/labels.h"

/**
 * @brief Adds a label with a numeric value to the linked list.
 * 
 * This function creates a new node with the given label and numeric value
 * and inserts it at the beginning of the linked list for O(1) insertion.
 * 
 * @param head Pointer to the head of the linked list.
 * @param label The label name to add.
 * @param number The numeric value to associate with the label.
 */
void add_label_number(SymbolList **head, const char *label, uint8_t number) {
    SymbolList *new_node = (SymbolList *)malloc(sizeof(SymbolList));
    if (!new_node) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    new_node->label = strdup(label);
    if (!new_node->label) {
        perror("Failed to allocate memory for label");
        free(new_node);
        exit(EXIT_FAILURE);
    }

    new_node->type = NUMBER_VALUE;
    new_node->value.number = number;

    /* Insert at the beginning for O(1) insertion */
    new_node->next = *head;
    *head = new_node;
}

/**
 * @brief Adds a label with a string value to the linked list.
 * 
 * This function creates a new node with the given label and string value
 * and inserts it at the beginning of the linked list for O(1) insertion.
 * 
 * @param head Pointer to the head of the linked list.
 * @param label The label name to add.
 * @param buffer The string value to associate with the label.
 */
void add_label_string(SymbolList **head, const char *label, const char *buffer) {
    SymbolList *new_node = (SymbolList *)malloc(sizeof(SymbolList));
    if (!new_node) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    new_node->label = strdup(label);
    if (!new_node->label) {
        perror("Failed to allocate memory for label");
        free(new_node);
        exit(EXIT_FAILURE);
    }

    new_node->type = STRING_VALUE;
    new_node->value.buffer = strdup(buffer);
    if (!new_node->value.buffer) {
        perror("Failed to allocate memory for buffer");
        free(new_node->label);
        free(new_node);
        exit(EXIT_FAILURE);
    }

    /* Insert at the beginning for O(1) insertion */
    new_node->next = *head;
    *head = new_node;
}

/**
 * @brief Retrieves a label's stored value from the linked list.
 * 
 * This function searches for a label in the linked list and returns the
 * corresponding node if found.
 * 
 * @param head Pointer to the head of the linked list.
 * @param label The label name to search for.
 * @return Pointer to the node containing the label, or NULL if not found.
 */
SymbolList* get_node_by_label(SymbolList *head, const char *label) {
    if (head == NULL || label == NULL) {
        return NULL;
    }
    
    SymbolList *curr = head;
    while (curr != NULL) {
        if (curr->label != NULL && !strcmp(curr->label, label)) {
            return curr;
        }
        curr = curr->next;
    }
    
    return NULL;
}

/**
 * @brief Checks whether a label exists in the linked list.
 * 
 * This function searches for a label in the linked list and returns true
 * if the label exists, or false otherwise.
 * 
 * @param head Pointer to the head of the linked list.
 * @param label The label name to search for.
 * @return true if the label exists, false otherwise.
 */
bool is_label_in_list(SymbolList *head, const char *label) {
    if (head == NULL || label == NULL) {
        return false;
    }
    
    SymbolList *curr = head;
    while (curr != NULL) {
        if (curr->label != NULL && !strcmp(curr->label, label)) {
            return true;
        }
        curr = curr->next;
    }
    
    return false;
}

/**
 * @brief Prints all labels and their associated values.
 * 
 * This function iterates through the linked list and prints each label
 * along with its associated value (numeric or string).
 * 
 * @param head Pointer to the head of the linked list.
 */
void print_labels(SymbolList *head) {
    if (head == NULL) {
        printf("Label list is empty.\n");
        return;
    }

    SymbolList *curr = head;
    while (curr != NULL) {
        if (curr->type == NUMBER_VALUE) {
            printf("Label: %s, Number: %u\n", curr->label, curr->value.number);
        } else {
            printf("Label: %s, String: %s\n", curr->label, curr->value.buffer);
        }
        curr = curr->next;
    }
}

/**
 * @brief Frees all allocated memory in the linked list.
 * 
 * This function deallocates all nodes in the linked list, including
 * the memory allocated for labels and string values.
 * 
 * @param head Pointer to the head of the linked list.
 */
void free_label_list(SymbolList *head) {
    SymbolList *curr = head;
    while (curr != NULL) {
        SymbolList *temp = curr;
        curr = curr->next;

        free(temp->label);
        if (temp->type == STRING_VALUE) {
            free(temp->value.buffer);
        }
        free(temp);
    }
}