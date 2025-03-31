#include <stdlib.h>

#include "../header/lib.h"

void skip_leading_spaces(char **line){
    if (line == NULL || *line == NULL){
        return; /* Ensure valid pointer */
    }

    while (**line == ' '){
        (*line)++; /* Advance for every space character */
    }
}

char *strdup(const char *s) {
    size_t len = strlen(s) + 1;
    char *new_str = malloc(len);
    if (new_str == NULL) {
        return NULL;
    }
    
    return memcpy(new_str, s, len);
}