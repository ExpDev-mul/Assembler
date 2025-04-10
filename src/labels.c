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
 * @param head Pointer to the head of the symbol list.
 * @param label The label name to add.
 * @param number The numeric value to associate with the label.
 */
void add_label_number(SymbolList **head, const char *label, int16_t number) {
    /* Allocate memory for the new node */
    SymbolList *new_node = (SymbolList *)malloc(sizeof(SymbolList));
    if (!new_node) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    /* Duplicate the label string and store it in the new node */
    new_node->label = strdup(label);
    if (!new_node->label) {
        perror("Failed to allocate memory for label");
        free(new_node); /* Free the allocated node if label allocation fails */
        exit(EXIT_FAILURE);
    }

    /* Set the type and value of the new node */
    new_node->type = NUMBER_VALUE;
    new_node->value.number = number;

    /* Insert the new node at the beginning of the linked list */
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
    /* Allocate memory for the new node */
    SymbolList *new_node = (SymbolList *)malloc(sizeof(SymbolList));
    if (!new_node) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    /* Duplicate the label string and store it in the new node */
    new_node->label = strdup(label);
    if (!new_node->label) {
        perror("Failed to allocate memory for label");
        free(new_node); /* Free the allocated node if label allocation fails */
        exit(EXIT_FAILURE);
    }

    /* Set the type and value of the new node */
    new_node->type = STRING_VALUE;
    new_node->value.buffer = strdup(buffer); /* Ensure we duplicate the buffer */
    if (!new_node->value.buffer) {
        perror("Failed to allocate memory for buffer");
        free(new_node->label); /* Free the allocated label if buffer allocation fails */
        free(new_node); /* Free the allocated node */
        exit(EXIT_FAILURE);
    }

    /* Insert the new node at the beginning of the linked list */
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
    /* Return NULL if the head of the list or the label is NULL */
    if (head == NULL || label == NULL) {
        return NULL;
    }
    
    /* Traverse the linked list */
    SymbolList *curr = head;
    while (curr != NULL) {
        /* Compare the current node's label with the target label */
        if (curr->label != NULL && !strcmp(curr->label, label)) {
            return curr; /* Return the matching node */
        }
        curr = curr->next; /* Move to the next node */
    }
    
    /* Return NULL if no matching label is found */
    return NULL;
}

/**
 * @brief Checks whether a label exists in the linked list.
 * 
 * This function searches the linked list for a node with the specified label.
 * If a matching label is found, it returns `true`. If no match is found or if
 * the list or label is `NULL`, it returns `false`.
 * 
 * @param head Pointer to the head of the linked list.
 * @param label The label name to search for.
 * @return `true` if the label exists in the list, `false` otherwise.
 * 
 * @note The function performs a case-sensitive comparison of the label strings.
 *       Ensure that the `label` parameter is not `NULL` before calling this function.
 */
bool is_label_in_list(SymbolList *head, const char *label) {
    /* Return false if the head of the list or the label is NULL */
    if (head == NULL || label == NULL) {
        return false;
    }
    
    /* Traverse the linked list */
    SymbolList *curr = head;
    while (curr != NULL) {
        /* Compare the current node's label with the target label */
        if (curr->label != NULL && !strcmp(curr->label, label)) {
            return true; /* Label found in the list */
        }
        curr = curr->next; /* Move to the next node */
    }
    
    /* Return false if no matching label is found */
    return false;
}

/**
 * @brief Prints the contents of a linked list of labels.
 *
 * This function traverses the linked list pointed to by `head` and prints
 * each label along with its associated value. The value can be either a
 * number or a buffer (interpreted as an integer), depending on the `type`
 * field of each node.
 *
 * @param head Pointer to the head of the SymbolList.
 *
 * If the list is empty (`head == NULL`), a message indicating this is printed.
 * Otherwise, for each element:
 * - If `type == NUMBER_VALUE`, the numerical value is printed.
 * - Otherwise, the buffer is printed as an integer.
 */
void print_labels(SymbolList *head) {
    /* Check if the list is empty */
    if (head == NULL) {
        printf("Label list is empty.\n");
        return;
    }

    /* Traverse the linked list */
    SymbolList *curr = head;
    while (curr != NULL) {
        /* Print the label and its associated value */
        if (curr->type == NUMBER_VALUE) {
            printf("Label: %s, Number: %i\n", curr->label, curr->value.number);
        } else {
            printf("Label: %s, String: %s\n", curr->label, curr->value.buffer);
        }
        curr = curr->next; /* Move to the next node */
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
    SymbolList *curr = head; /* Pointer to traverse the list */
    while (curr != NULL) {
        SymbolList *temp = curr; /* Store the current node */
        curr = curr->next;       /* Move to the next node */

        /* Free the memory allocated for the label */
        free(temp->label);

        /* Free the memory allocated for the buffer if the type is STRING_VALUE */
        if (temp->type == STRING_VALUE) {
            free(temp->value.buffer);
        }

        /* Free the memory allocated for the current node */
        free(temp);
    }
}