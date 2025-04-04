#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <stdio.h>
#include <stdint.h>

void first_pass(FILE* file, LinkedList** labels_ptr, LinkedList** entries_ptr, LinkedList** externs_ptr, uint8_t* errors);

#endif FIRST_PASS_H