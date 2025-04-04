#ifndef FILES_H
#define FILES_H

#include <stdio.h>
#include <stdlib.h>

/*
    Prepends st into the beginning of file
*/
void insert_file_into_file(FILE* src, FILE* dest);

/*
    Prepends st into the beginning of file
*/
void prepend_to_file(FILE* file, const char *st);

#endif