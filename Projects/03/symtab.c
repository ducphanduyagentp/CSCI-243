/// file: symtab.c
/// description: implementation for symbol tables
/// author: Duc Phan - ddp3945@rit.edu

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "symtab.h"
#include "utils.h"

/// Create a new empty Symbol Table
/// @return: pointer to a new symbol table
SymTab* createSymTab() {
    SymTab *symtab = (SymTab *) malloc(sizeof(SymTab));
    if (symtab == NULL) {
        fprintf(stderr, "symtab::createSymTab() failed to allocate memory.\n");
        exit(EXIT_FAILURE);
    }
    symtab -> cap = INIT_CAP;
    symtab -> nEntry = 0;
    symtab -> table = calloc(symtab -> cap, sizeof(Symbol *));
    if (symtab -> table == NULL) {
        fprintf(stderr, "symtab::createSymTab() failed to allocate memory.\n");
        exit(EXIT_FAILURE);
    }
    return symtab;
}

/// Destroy a symbol table
/// @param symtab: pointer to a symbol table
void destroySymTab(SymTab *symtab) {
    for (size_t i = 0; i < symtab -> nEntry; i++) {
        if (symtab -> table[i] != NULL) {
            free(symtab -> table[i]);
            symtab -> table[i] = NULL;
        }
    }
    free(symtab -> table);
    symtab -> table = NULL;
    free(symtab);
}

/// Create a new symbol
/// @param name: name of the symbol
/// @param value: the value of the symbol
/// @param type: the type of the symbol
/// @return: the pointer to the symbol
Symbol* createSymbol(char *name, Value value, Type type) {
    Symbol *new = (Symbol *) malloc(sizeof(Symbol));
    if (new == NULL) {
        fprintf(stderr, "symtab::createSymbol() failed to allocate memory.\n");
        exit(EXIT_FAILURE);
    }
    strncpy(new -> name, name, MAX_NAME_LEN + 1);
    new -> type = type;
    new -> value = value;
    return new;
}

/// Put a symbol into a symbol table
/// @param name: name of the symbol
/// @param value: the value of the symbol
/// @param type: type of the symbol
/// @param symtab: the symbol table to put the symbol into
void put(char *name, Value value, Type type, SymTab *symtab) {
    int idx = has(name, symtab);
    if (idx == -1) {
        if (symtab -> nEntry >= RESIZE_THRESHOLD * symtab -> cap) {
            symtab -> table = (Symbol **) realloc((void *) symtab -> table, 
            sizeof(Symbol *) * symtab -> cap * RESIZE_FACTOR);
            
            if (symtab -> table == NULL) {
                fprintf(stderr, "symtab::put() failed to allocate memory");
                exit(EXIT_FAILURE);
            }

            symtab -> cap *= RESIZE_FACTOR;
            for (size_t i = symtab -> nEntry; i < symtab -> cap; i++) {
                symtab -> table[i] = NULL;
            }
        }
        Symbol *new = createSymbol(name, value, type);
        symtab -> table[symtab -> nEntry++] = new;
    } else {
        symtab -> table[idx] -> value = value;
    }
}

/// Get the symbol with the specified name
/// @param name: name of the symbol to get
/// @param symtab: the symbol table to get
/// @return: pointer to the symbol, NULL if not found
Symbol* get(char *name, SymTab *symtab) {
    int idx = has(name, symtab);
    if (idx == -1) {
        return NULL;
    }
    return symtab -> table[idx];
}

/// Determine if a symbol with the specified name is in the table
/// @param name: the name of the symbol
/// @param symtab: the symbol table to check
/// @return: index of the symbol if it's in the table, otherwise -1
int has(char *name, SymTab *symtab) {
    for (size_t i = 0; i < symtab -> nEntry; i++) {
        Symbol *entry = symtab -> table[i];
        if (entry != NULL) {
            if (!strncmp(name, entry -> name, MAX_NAME_LEN)) {
                return i;
            }
        }
    }
    return -1;
}

/// Dump the symbol table in alphabetical order
/// @param symtab: the symbol table to be dumped
void dump(SymTab *symtab) {
    qsort((void *) (symtab -> table), symtab -> nEntry, sizeof(Symbol *), symbolCmp);
    puts("Symbol Table Contents");
    puts("Name    Type    Value");
    puts("=====================");
    for (size_t i = 0; i < symtab -> nEntry; i++) {
        Symbol *entry = symtab -> table[i];
        printf("%-8s%-8s", entry -> name, entry -> type == Integer ? "integer" : "real");
        printValSymbol(entry);
        puts("");
    }
}
