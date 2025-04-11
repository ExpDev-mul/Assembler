#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../header/symbols.h"

/* filepath: /root/Maman_14/src/symbols.c */

/* Modify the add_label_number function to include symbol type */
SymbolList* add_symbol_number(SymbolList **head, const char *label, int16_t number, SymbolType symbol_type) {
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
    new_node->symbol_type = symbol_type;  /* Add symbol type */
    new_node->value.number = number;

    new_node->next = *head;
    *head = new_node;
    return new_node; /* Return the new node for further processing if needed */
}

/* Modify the add_label_string function to include symbol type */
SymbolList* add_symbol_string(SymbolList **head, const char *label, const char *buffer, SymbolType symbol_type) {
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
    new_node->symbol_type = symbol_type;  /* Add symbol type */
    new_node->value.buffer = strdup(buffer);
    if (!new_node->value.buffer) {
        perror("Failed to allocate memory for buffer");
        free(new_node->label);
        free(new_node);
        exit(EXIT_FAILURE);
    }

    new_node->next = *head;
    *head = new_node;
    return new_node; /* Return the new node for further processing if needed */
}

/* Add new function to get symbols by type */
SymbolList* get_symbol_by_label(SymbolList* head, const char* label) {
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

/* Add new function to get symbols by type */
SymbolList* get_symbol_by_label_filter(SymbolList* head, const char* label, SymbolType filter) {
    /* Return NULL if the head of the list or the label is NULL */
    if (head == NULL || label == NULL) {
        return NULL;
    }
    
    /* Traverse the linked list */
    SymbolList *curr = head;
    while (curr != NULL) {
        /* Compare the current node's label with the target label */
        if (curr->label != NULL && !strcmp(curr->label, label) && curr->symbol_type == filter) {
            return curr; /* Return the matching node */
        }
        curr = curr->next; /* Move to the next node */
    }
    
    /* Return NULL if no matching label is found */
    return NULL;
}

bool is_symbol_exists(SymbolList *head, const char *label) {
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

/* Add function to count symbols of a specific type */
int count_symbols_by_type(SymbolList *head, SymbolType symbol_type) {
    int count = 0;
    SymbolList *curr = head;

    while (curr != NULL) {
        if (curr->symbol_type == symbol_type) {
            count++;
        }
        curr = curr->next;
    }

    return count;
}

/* Modify print_labels to include symbol type information */
void print_symbols(SymbolList *head) {
    if (head == NULL) {
        printf("Label list is empty.\n");
        return;
    }

    SymbolList *curr = head;
    while (curr != NULL) {
        /* Add symbol type to output */
        const char *type_str;
        switch (curr->symbol_type) {
            case SYMBOL_ENTRY:  type_str = "ENTRY"; break;
            case SYMBOL_EXTERN: type_str = "EXTERN"; break;
            case SYMBOL_MACRO:  type_str = "MACRO"; break;
            case SYMBOL_LABEL:  type_str = "LABEL"; break;
            case SYMBOL_DATA:   type_str = "DATA"; break;
            case SYMBOL_INSTRUCTION: type_str = "INSTRUCTION"; break;
            default:           type_str = "UNKNOWN"; break;
        }

        if (curr->type == NUMBER_VALUE) {
            printf("Label: %s, Type: %s, Number: %i\n", 
                   curr->label, type_str, curr->value.number);
        } else {
            printf("Label: %s, Type: %s, String: %s\n", 
                   curr->label, type_str, curr->value.buffer);
        }
        curr = curr->next;
    }
}

void free_symbol_list(SymbolList *head) {
    SymbolList *current = head;
    
    /* Traverse the list and free each node */
    while (current != NULL) {
        SymbolList *next = current->next;  /* Store next pointer before freeing current */
        
        /* Free the label string */
        if (current->label != NULL) {
            free(current->label);
        }
        
        /* If it's a string value, free the buffer */
        if (current->type == STRING_VALUE && current->value.buffer != NULL) {
            free(current->value.buffer);
        }
        
        /* Free the node itself */
        free(current);
        
        current = next;  /* Move to next node */
    }
}