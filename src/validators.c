#include <ctype.h>
#include "../header/validators.h"
#include "../header/lib.h"
#include "../header/assembler.h"

/**
 * @brief Validates if an argument represents a valid register.
 * 
 * This function checks if the given argument is a valid register (e.g., r0, r1).
 * 
 * @param arg The argument to validate.
 * @return true if the argument is a valid register, false otherwise.
 */
bool is_valid_reg(char *arg) {
    int reg_num; /* Variable to store the register number */

    /* Ensure the argument is not NULL and starts with 'r' */
    if (arg == NULL || arg[0] != 'r') {
        return false;
    }

    /* Check if the second character is a digit */
    if (!isdigit(arg[1])) {
        return false;
    }

    /* Convert the register number to an integer */
    reg_num = arg[1] - '0';

    /* Ensure the register number is within bounds (0 to NUM_REGISTERS - 1) */
    return reg_num >= 0 && reg_num < NUM_REGISTERS && arg[2] == '\0';
}

/**
 * @brief Validates if an argument represents a valid immediate number.
 * 
 * This function checks if the given argument is a valid immediate number (e.g., #5, #-3).
 * 
 * @param arg The argument to validate.
 * @return true if the argument is a valid immediate number, false otherwise.
 */
bool is_valid_immediate_number(char *arg) {
    int i; /* Loop variable */

    /* Ensure the argument is not NULL and starts with '#' */
    if (arg == NULL || arg[0] != '#') {
        return false;
    }

    /* Check if the characters after '#' are valid digits or signs */
    for (i = (arg[1] == '+' || arg[1] == '-') ? 2 : 1; arg[i] != '\0'; i++) {
        if (!isdigit(arg[i])) {
            return false; /* Non-digit character found */
        }
    }

    return true; /* Valid immediate number */
}

/**
 * @brief Validates if an argument represents a valid number.
 * 
 * This function checks if the given argument is a valid number (e.g., 5, -3).
 * 
 * @param arg The argument to validate.
 * @return true if the argument is a valid number, false otherwise.
 */
bool is_valid_number(char *arg) {
    int i; /* Loop variable */

    /* Ensure the argument is not NULL */
    if (arg == NULL) {
        return false;
    }

    /* Check if the characters are valid digits or signs */
    for (i = (arg[0] == '+' || arg[0] == '-') ? 1 : 0; arg[i] != '\0'; i++) {
        if (!isdigit(arg[i])) {
            return false; /* Non-digit character found */
        }
    }

    return true; /* Valid number */
}

/**
 * @brief Validates if an argument represents a valid addressing mode.
 * 
 * This function checks if the given argument is a valid addressing mode.
 * 
 * @param arg The argument to validate.
 * @return true if the argument is a valid addressing mode, false otherwise.
 */
bool is_valid_mode(char *arg) {
    /* Ensure the argument is not NULL */
    if (arg == NULL) {
        return false;
    }

    /* Check for valid addressing modes */
    if (arg[0] == 'r' || arg[0] == '#' || arg[0] == '&') {
        return true;
    } else if (isalpha(arg[0])) {
        return true; /* Direct addressing case (e.g., labels) */
    }

    return false; /* Invalid addressing mode */
}

/**
 * @brief Validates if an argument represents a valid string.
 * 
 * This function checks if the given argument is a valid string enclosed in double quotes.
 * 
 * @param arg The argument to validate.
 * @return true if the argument is a valid string, false otherwise.
 */
bool is_valid_string(char *arg) {
    int i; /* Loop variable */

    /* Ensure the argument is not NULL and starts with a double quote */
    if (arg == NULL || arg[0] != '"') {
        return false;
    }

    /* Check for a closing double quote */
    for (i = 1; arg[i] != '\0'; i++) {
        if (arg[i] == '"') {
            return true; /* Valid string */
        }
    }

    return false; /* Invalid string */
}