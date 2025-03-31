/* TODO: Implement O(1) insert TC (changing direction of linked list insertion...) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../header/labels.h"
#include "../header/lib.h"

/* Adds a label with a numeric value */
void add_label_number(LinkedList **head, const char *label, uint8_t number) {
    LinkedList *new_node = (LinkedList *)malloc(sizeof(LinkedList));
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
    new_node->next = NULL;

    if (*head == NULL) {
        *head = new_node;
    } else {
        LinkedList *curr = *head;
        while (curr->next) {
            curr = curr->next;
        }
        curr->next = new_node;
    }
}

/* Adds a label with a string buffer */
void add_label_string(LinkedList **head, const char *label, const char *buffer) {
    LinkedList *new_node = (LinkedList *)malloc(sizeof(LinkedList));
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

    new_node->next = NULL;

    if (*head == NULL) {
        *head = new_node;
    } else {
        LinkedList *curr = *head;
        while (curr->next) {
            curr = curr->next;
        }
        curr->next = new_node;
    }
}

/* Retrieves a label's stored value */
LinkedList* get_node_by_label(LinkedList *head, const char *label) {
    if (head == NULL || label == NULL) {
        return NULL;
    }
    
    LinkedList *curr = head;
    while (curr != NULL) {
        if (curr->label != NULL && !strcmp(curr->label, label)) {
            return curr;
        }
        
        curr = curr->next;
    }
    
    return NULL;
}

/* Prints all labels and their values */
void print_labels(LinkedList *head) {
    if (head == NULL) {
        printf("Label list is empty.\n");
        return;
    }

    LinkedList *curr = head;
    while (curr != NULL) {
        if (curr->type == NUMBER_VALUE) {
            printf("Label: %s, Number: %u\n", curr->label, curr->value.number);
        } else {
            printf("Label: %s, String: %s\n", curr->label, curr->value.buffer);
        }

        if (curr->next != NULL){
            curr = curr->next;
        } else {
            break; /* Avoid assigning null pointer which leads to a segmentation fault */
        }
    }
}

/* Frees all allocated memory in the list */
void free_label_list(LinkedList *head) {
    
}