#ifndef PREPROCESSING_C
#define PREPROCESSING_C

#include <stdio.h>

#include "../header/labels.h"

void preprocess(FILE* file, FILE* temp, LinkedList** labels, LinkedList** externs, uint8_t* errors);

#endif