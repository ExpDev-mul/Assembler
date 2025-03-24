#include <stdio.h>

#include "../header/lib.h"

char *trim_spaces_overload(char str[]){
    /* Remove leading spaces */
    while (*str == ' ') {
        str++;
    }

    /* Remove ending spaces */
    char *end = str + strlen(str) - 1;

    while (end >= str && *end == ' ') {
        *end = '\0';
        end--;
    }

    return str;
}

char *trim_spaces(char *str){
    /*
      Converts the pointer-string into a temporary buffer.
      Then returns the result where spaces have been removed.
    */
    char *bufferptr = (char *)malloc(strlen(str) + 1);
    if (!bufferptr) {
        return NULL;
    }

    strcpy(bufferptr, str);
    char *updated = trim_spaces_overload(bufferptr);
    char *result = (char *)malloc(strlen(updated) + 1);
    if (!result) {
        free(bufferptr);
        return NULL;
    }

    strcpy(result, updated);
    free(bufferptr);
    return result;
}