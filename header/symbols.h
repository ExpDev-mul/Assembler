/**
 * @file symbols.h
 * @brief Header file for managing the symbol table in the assembler.
 * 
 * Defines the symbol table structure and operations for the two-pass assembler.
 * Handles different types of symbols (instructions, data, entries, externals)
 * and their memory address resolution.
 * 
 * Memory Layout:
 * - Instructions: Start at address 100 (START_LINE)
 * - Data: Follows instructions (START_LINE + IC)
 * - External references: Resolved during second pass
 */

 #ifndef SYMBOLS_H
 #define SYMBOLS_H
 
 #include <stdint.h>
 #include "../header/lib.h"
 
 /**
  * @brief Value type stored in a symbol
  */
 typedef enum {
     NUMBER_VALUE,  /* Memory address or immediate value */
     STRING_VALUE   /* String content (for macros) */
 } ValueType;
 
 /**
  * @brief Types of symbols in the assembler
  */
 typedef enum {
     SYMBOL_INSTRUCTION,  /* Label pointing to instruction (address = START_LINE + offset) */
     SYMBOL_DATA,        /* Label pointing to data (address = START_LINE + IC + offset) */
     SYMBOL_ENTRY,       /* Entry point (resolved to actual symbol address) */
     SYMBOL_EXTERN,      /* External reference (address = 0, ARE = 01) */
     SYMBOL_MACRO,        /* Macro definition (stores string content) */
     SYMBOL_LABEL
 } SymbolType;
 
 /**
  * @brief Symbol table entry structure
  * 
  * Stores information about a single symbol including:
  * - Label name
  * - Symbol type (instruction, data, entry, extern, macro)
  * - Value type (number for addresses, string for macros)
  * - Actual value (memory address or string content)
  */
 typedef struct SymbolList {
     char *label;                /* Symbol name */
     ValueType type;             /* Type of value stored */
     SymbolType symbol_type;     /* Type of symbol */
     union {
         int16_t number;         /* Memory address or immediate value */
         char *buffer;           /* String content for macros */
     } value;
     struct SymbolList *next;    /* Next symbol in table */
 } SymbolList;
 
 /**
  * @brief Adds a numeric symbol to the symbol table
  * @param head Pointer to symbol table head
  * @param label Symbol name
  * @param number Memory address or value
  * @param symbol_type Type of symbol (INSTRUCTION, DATA, ENTRY, EXTERN)
  * @return Pointer to the new symbol, or NULL on error
  */
 SymbolList* add_symbol_number(SymbolList **head, const char *label, int16_t number, SymbolType symbol_type);
 
 /**
  * @brief Adds a string symbol to the symbol table (used for macros)
  * @param head Pointer to symbol table head
  * @param label Macro name
  * @param buffer Macro content
  * @param symbol_type Must be SYMBOL_MACRO
  * @return Pointer to the new symbol, or NULL on error
  */
 SymbolList* add_symbol_string(SymbolList **head, const char *label, const char *buffer, SymbolType symbol_type);
 
 /**
  * @brief Finds a symbol by label
  * @param head Symbol table head
  * @param label Symbol to find
  * @return Pointer to symbol if found, NULL otherwise
  */
 SymbolList* get_symbol_by_label(SymbolList *head, const char *label);
 
 /**
  * @brief Finds a symbol by label and type
  * @param head Symbol table head
  * @param label Symbol to find
  * @param filter Required symbol type
  * @return Pointer to symbol if found and matches type, NULL otherwise
  */
 SymbolList* get_symbol_by_label_filter(SymbolList* head, const char* label, SymbolType filter);
 
 /**
  * @brief Checks if a symbol exists
  * @param head Symbol table head
  * @param label Symbol to check
  * @return true if symbol exists, false otherwise
  */
 bool is_symbol_exists(SymbolList *head, const char *label);
 
 /**
  * @brief Frees all memory used by symbol table
  * @param head Symbol table to free
  */
 void free_symbol_list(SymbolList *head);
 
 /**
  * @brief Prints symbol table contents (for debugging)
  * @param head Symbol table to print
  */
 void print_symbols(SymbolList *head);
 
 #endif /* SYMBOLS_H */