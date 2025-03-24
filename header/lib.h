#ifndef LIB_H
#define LIB_H

typedef enum { false, true } bool;

void print_symbol();  /* Function to display input prompt */
char* trim_spaces_overload(char str[]);  /* Removes leading and trailing spaces */
char* trim_spaces(char* str);  /* Trims spaces and returns new string */

#endif /* LIB_H */