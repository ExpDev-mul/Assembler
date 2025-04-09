#ifndef VALIDATORS_H
#define VALIDATORS_H

#include "../header/lib.h"

/**
 * @brief Validates if an argument represents a valid register.
 * 
 * This function checks if the given argument is a valid register (e.g., r0, r1).
 * 
 * @param arg The argument to validate.
 * @return true if the argument is a valid register, false otherwise.
 */
bool is_valid_reg(char *arg);

/**
 * @brief Validates if an argument represents a valid immediate number.
 * 
 * This function checks if the given argument is a valid immediate number (e.g., #5, #-3).
 * 
 * @param arg The argument to validate.
 * @return true if the argument is a valid immediate number, false otherwise.
 */
bool is_valid_immediate(char *arg);

/**
 * @brief Validates if an argument represents a valid label.
 * 
 * This function checks if the given argument is a valid label according to assembler rules.
 * 
 * @param arg The argument to validate.
 * @return true if the argument is a valid label, false otherwise.
 */
bool is_valid_label(char *arg);

#endif /* VALIDATORS_H */