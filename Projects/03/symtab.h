/// file: symtab.h
/// description: interface for symbol tables
/// author: Duc Phan - ddp3945@rit.edu

#ifndef SYMTAB_H
#define SYMTAB_H

#include <stdbool.h>

#define MAX_NAME_LEN 7
#define RESIZE_FACTOR 2
#define RESIZE_THRESHOLD 0.75
#define INIT_CAP 1337

typedef enum type_enum {
    Integer, Float, Unknown
} Type;

typedef union value_union {
    int iVal;       
    float fVal;  
} Value;

typedef struct symbol_struct {
    char name[MAX_NAME_LEN + 1];
    Value value;		
    Type type;
} Symbol;

typedef struct symbolTable {
    Symbol **table;
    size_t cap;
    size_t nEntry;
} SymTab;

/// Create a new empty Symbol Table
/// @return: pointer to a new symbol table
SymTab* createSymTab();

/// Destroy a symbol table
/// @param symtab: pointer to a symbol table
void destroySymTab(SymTab *symtab);

/// Create a new symbol
/// @param name: name of the symbol
/// @param value: the value of the symbol
/// @param type: the type of the symbol
/// @return: the pointer to the symbol
Symbol* createSymbol(char *name, Value value, Type type);

/// Put a symbol into a symbol table
/// @param name: name of the symbol
/// @param value: the value of the symbol
/// @param type: type of the symbol
/// @param symtab: the symbol table to put the symbol into
void put(char *name, Value value, Type type, SymTab *symtab);

/// Get the symbol with the specified name
/// @param name: name of the symbol to get
/// @param symtab: the symbol table to get
/// @return: pointer to the symbol, NULL if not found
Symbol* get(char *name, SymTab *symtab);

/// Determine if a symbol with the specified name is in the table
/// @param name: the name of the symbol
/// @param symtab: the symbol table to check
/// @return: index of the symbol if it's in the table, otherwise -1
int has(char *name, SymTab *symtab);

/// Dump the symbol table in alphabetical order
/// @param symtab: the symbol table to be dumped
void dump(SymTab *symtab);

#endif