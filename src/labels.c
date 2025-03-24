#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct LabelNode {
    char *label;
    uint8_t line;
    struct LabelNode *next;
} LabelNode;

/* Adds a label to the list (creates head if needed) */
void add_label(LabelNode **head, const char *label, uint8_t line) {
    LabelNode *new_node = (LabelNode*)malloc(sizeof(LabelNode));
    if (!new_node) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    new_node->label = label;
    if (!new_node->label) {
        perror("Failed to allocate memory for label");
        free(new_node);
        exit(EXIT_FAILURE);
    }

    new_node->line = line;
    new_node->next = NULL;

    if (*head == NULL) {
        *head = new_node;  /* If list is empty, make new node the head */
    } else {
        LabelNode *curr = *head;
        while (curr->next) {
            curr = curr->next;
        }
        curr->next = new_node;
    }
}

/* Retrieves the line number associated with a label */
uint8_t get_line_by_label(LabelNode *head, const char *label) {
    LabelNode *curr = head;
    while (curr != NULL) {
        if (strcmp(curr->label, label) == 0) {
            return curr->line;
        }
        curr = curr->next;
    }
    return 0;  /* Label not found */
}

/* Prints all labels and their corresponding line numbers */
void print_labels(LabelNode *head) {
    if (!head) {
        printf("Label list is empty.\n");
        return;
    }

    LabelNode *curr = head;
    while (curr != NULL) {
        printf("Label: %s, Line: %d\n", curr->label, curr->line);
        curr = curr->next;
    }
}

/* Frees all allocated memory in the list */
void free_label_list(LabelNode *head) {
    LabelNode *curr = head;
    while (curr != NULL) {
        LabelNode *next = curr->next;
        free(curr);
        curr = next;
    }
}