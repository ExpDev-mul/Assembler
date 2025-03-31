#ifndef LIB_H
#define LIB_H

typedef enum { false, true } bool;

void skip_leading_spaces(char **line);

char *strdup(const char *s);

#endif /* LIB_H */