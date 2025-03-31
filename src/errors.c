/* External headers */
#include <stdio.h>

/* Internal headers */
#include "../header/errors.h"

char *errors_table[] = {
    /* 0 */
    "Missing arguments",
    "Extraneous text after end of command",
    "Invalid command name",
    "The command can not perform this type of addressing for the source argument",
    "The command can not perform this type of addressing for the dest argument",
    
    /* 5 */
    "Immediate number declaration does not follow up with a number",
    "Macro declaration missing a name for the macro",
    "Extern import missing an argument"
};

void error_with_code(int code, uint8_t *errors_counter){
    int errors_table_size = sizeof(errors_table)/sizeof(errors_table[0]);
    if (code < 0 || code >= errors_table_size){
        /* Ensure error code is within bounds */
        return;
    }

    (*errors_counter)++;
    printf("Error: %s\n", errors_table[code]); /* Output current error */
}