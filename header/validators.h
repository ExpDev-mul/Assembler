/**
 * @file validators.h
 * @brief Header file for validation functions in the assembler.
 * 
 * This file provides utility functions for validating various components of the assembler's input,
 * such as registers, immediate numbers, addressing modes, and strings. These functions ensure that
 * the input adheres to the assembler's syntax and rules.
 * 
 * Key Features:
 * - Validates registers (e.g., r0, r1).
 * - Validates immediate numbers (e.g., #5, #-3).
 * - Validates general numbers (e.g., 5, -3).
 * - Validates addressing modes.
 * - Validates strings enclosed in double quotes.
 */
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
bool is_valid_immediate_number(char *arg);

/**
 * @brief Validates if an argument represents a valid number.
 * 
 * This function checks if the given argument is a valid number (e.g., 5, -3).
 * 
 * @param arg The argument to validate.
 * @return true if the argument is a valid number, false otherwise.
 */
bool is_valid_number(char *arg);

/**
 * @brief Validates if an argument represents a valid addressing mode.
 * 
 * This function checks if the given argument is a valid addressing mode.
 * 
 * @param arg The argument to validate.
 * @return true if the argument is a valid addressing mode, false otherwise.
 */
bool is_valid_mode(char *arg);

/**
 * @brief Validates if an argument represents a valid string.
 * 
 * This function checks if the given argument is a valid string enclosed in double quotes.
 * 
 * @param arg The argument to validate.
 * @return true if the argument is a valid string, false otherwise.
 */
bool is_valid_string(char *arg);

#endif /* VALIDATORS_H */