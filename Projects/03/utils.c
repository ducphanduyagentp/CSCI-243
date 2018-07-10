/// file: utils.h
/// description: implemetations for ultility functions
/// author: Duc Phan - ddp3945@rit.edu

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include "symtab.h"
#include "utils.h"
#include "stackADT.h"

/// Load a symbol table file into a symbol table
/// @param symtabFile: the symbol table file
/// @param symtab: the symbol table
void loadSymTabFile(char *symtabFile, SymTab *symtab) {
    FILE *fdSym = fopen(symtabFile, "r");
    if (fdSym == NULL) {
        fprintf(stderr, "Error opening file %s\n", symtabFile);
        exit(EXIT_FAILURE);
    }
    char typeName[10];
    char symName[MAX_NAME_LEN + 1];
    Value value;
    
    char *lineptr = NULL;
    char *lineptr2 = NULL;
    size_t len = 0;

    while(getline(&lineptr2, &len, fdSym) != EOF) {
        assert(strlen(lineptr2) > 1);

        lineptr = strtok(lineptr2, " \n");
        sscanf(lineptr, "%s", typeName);

        lineptr = strtok(NULL, " \n");
        sscanf(lineptr, "%7s", symName);

        lineptr = strtok(NULL, " \n");
        bool isFloat = false;
        for (size_t i = 0; i < strlen(lineptr); i++) {
            if (lineptr[i] == '.') {
                isFloat = true;
                break;
            }
        }

        if (!strcmp("integer", typeName)) {
            if (isFloat) {
                float f = 0.0;
                sscanf(lineptr, "%f", &f);
                f = roundf(f);
                value.iVal = (int) f;
            } else {
                sscanf(lineptr, "%d", &value.iVal);
            }
            put(symName, value, Integer, symtab);
        } else if (!strcmp("real", typeName)) {
            sscanf(lineptr, "%f", &value.fVal);
            put(symName, value, Float, symtab);
        }
        
    }

    if (lineptr2 != NULL) {
        free(lineptr2);
    }
    fclose(fdSym);
}

/// Comparator for symbol entries
/// @param p1: The first symbol
/// @param p2: The second symbol
/// @return: < 0 if p1 < p2, 0 if p1 == p2, > 0 if p1 > p2
int symbolCmp(const void *p1, const void *p2) {
    return strcmp( (*(Symbol **)p1) -> name, (*(Symbol **)p2) -> name);
}

/// Process the define statement
/// @param cmd: the statement
/// @param symtab: the symbol table
void processDefine(char *cmd, SymTab *symtab) {
    char *lineptr = NULL;
    lineptr = strtok(cmd, " ,\n");
    Type type;
    Value value;
    
    value.iVal = 0;
    value.fVal = 0;

    if (!strcmp(lineptr, "integer")) {
        type = Integer;
    } else if (!strcmp(lineptr, "real")) {
        type = Float;
    } else {
        fprintf(stderr, "define: unknown type name '%s'\n", lineptr);
        return;
    }

    lineptr = strtok(NULL, " ,\n");
    
    while(lineptr != NULL) {
        if (has(lineptr, symtab) != -1) {
            fprintf(stderr, "define: symbol '%s' already existed\n", lineptr);
        } else {
            put(lineptr, value, type, symtab);
        }
        lineptr = strtok(NULL, " ,\n");
    }
}

/// Process the prt statement
/// @param cmd: the statement
void processPrt(char *cmd) {
    size_t endIdx = strlen(cmd) - 1;
    while(cmd[endIdx] != cmd[0]) {
        endIdx--;
    }

    for (size_t i = 1; i < endIdx; ) {
        if (cmd[i] == '\\') {
            switch(cmd[i + 1]) {
                case 'n':
                    putchar('\n');
                    break;
                case 't':
                    putchar('\t');
                    break;
                case '\\':
                    putchar('\\');
                    break;
                default:
                    break;
            }
            i += 2;
        } else {
            putchar(cmd[i]);
            i += 1;
        }
        fflush(stdout);
    }
}

/// Process the let statement
/// @param cmd: the statement
/// @param symtab: the symbol table
void processLet(char *cmd, SymTab *symtab) {
    char *lineptr = NULL;
    char *varName = strtok(cmd, " ,\n");
    if (has(varName, symtab) == -1) {
        fprintf(stderr, "let: symbol '%s' does not exist\n", varName);
        return;
    }
    
    strtok(NULL, " ,\n");
    
    StackADT st1;
    st1 = stk_create();

    StackADT st2;
    st2 = stk_create();

    StackADT st3;
    st3 = stk_create();

    // Extract tokens
    lineptr = strtok(NULL, " ,\n");
    while(lineptr != NULL) {
        int opCode = isOperator(lineptr);
        if (opCode == -1) {
            Symbol sym = elementToValue(lineptr, symtab);
            
            if (sym.type == Unknown) {
                fprintf(stderr, "let: symbol '%s' does not exist\n", lineptr);
                while(!stk_empty(st2)) {
                    free(stk_pop(st2));
                }
            
                while(!stk_empty(st1)) {
                    free(stk_pop(st1));
                }
            
                while(!stk_empty(st3)) {
                    free(stk_pop(st3));
                }
            
                stk_destroy(st1);
                stk_destroy(st2);
                stk_destroy(st3);
                return;
            }
            stk_push(st1, (void *) createSymbol(sym.name, sym.value, sym.type));
        } else {
            Value value;
            value.iVal = opCode;
            stk_push(st1, (void *) createSymbol(lineptr, value, Unknown));
        }
        lineptr = strtok(NULL, " ,\n");
    }

    while (!stk_empty(st1)) {
        stk_push(st2, stk_pop(st1));
    }

    // Convert to postfix
    while (!stk_empty(st2)) {
        Symbol *sym = (Symbol *) stk_pop(st2);

        if (sym -> type != Unknown) {
            stk_push(st3, (void *) sym);
        } else {
            if (sym -> name[0] == '(') {
                stk_push(st1, (void *) sym);
            } else if (sym -> name[0] == ')') {
                while (!stk_empty(st1)) {
                    Symbol *top = stk_pop(st1);
                    if (top -> name[0] == '(') {
                        free(top);
                        break;
                    }
                    stk_push(st3, (void *) top);
                }
                free(sym);
            } else {
                while (!stk_empty(st1)) {
                    Symbol *top = stk_top(st1);
                    if (top -> name[0] == '(') {
                         //top = stk_pop(st1);
                         //free(top);
                        break;
                    }
                    if (top -> value.iVal <= sym -> value.iVal) {
                        stk_push(st3, (void *) stk_pop(st1));
                    } else {
                        break;
                    }
                }
                stk_push(st1, (void *) sym);
            }
        }
    }

    while (!stk_empty(st1)) {
        stk_push(st3, stk_pop(st1));
    }
    while (!stk_empty(st3)) {
        stk_push(st2, stk_pop(st3));
    }

    // Evaluate: st1 contains the results. st2 contains the expression
    while (!stk_empty(st2)) {
        Symbol *sym = (Symbol *) stk_pop(st2);
        if (sym -> type == Unknown) {
            Symbol *sym2 = stk_pop(st1);
            Symbol *sym1 = stk_pop(st1);
            if (sym -> name[0] == '%' && (sym1 -> type == Float || sym2 -> type == Float)) {
                fprintf(stderr, "Bad operand ");
                if (sym1 -> type == Float) {
                    fprintf(stderr, "%.3f ", sym1 -> value.fVal);
                }
                if (sym2 -> type == Float) {
                    fprintf(stderr, "%.3f ", sym2 -> value.fVal);
                }
                fprintf(stderr, "with operator %%\n");
                free(sym1);
                free(sym2);
                free(sym);
                break;
            }

            Symbol *result;
            Value value;
            value.iVal = 0;
            value.fVal = 0.0;

            if (sym1 -> type == Float || sym2 -> type == Float) {
                result = createSymbol("result", value, Float);
            } else {
                result = createSymbol("result", value, Integer);
            }

            switch(sym -> name[0]) {
                case '+':
                    if (result -> type == Float) {
                        value.fVal = 
                        (sym1 -> type == Integer ? sym1 -> value.iVal : sym1 -> value.fVal)
                        + (sym2 -> type == Integer ? sym2 -> value.iVal : sym2 -> value.fVal);
                    } else {
                        value.iVal = 
                        (sym1 -> type == Integer ? sym1 -> value.iVal : sym1 -> value.fVal)
                        + (sym2 -> type == Integer ? sym2 -> value.iVal : sym2 -> value.fVal);
                    }
                    break;
                case '-':
                    if (result -> type == Float) {
                        value.fVal = 
                        (sym1 -> type == Integer ? sym1 -> value.iVal : sym1 -> value.fVal)
                        - (sym2 -> type == Integer ? sym2 -> value.iVal : sym2 -> value.fVal);
                    } else {
                        value.iVal = 
                        (sym1 -> type == Integer ? sym1 -> value.iVal : sym1 -> value.fVal)
                        - (sym2 -> type == Integer ? sym2 -> value.iVal : sym2 -> value.fVal);
                    }
                    break;
                case '*':
                    if (result -> type == Float) {
                        value.fVal = 
                        (sym1 -> type == Integer ? sym1 -> value.iVal : sym1 -> value.fVal)
                        * (sym2 -> type == Integer ? sym2 -> value.iVal : sym2 -> value.fVal);
                    } else {
                        value.iVal = 
                        (sym1 -> type == Integer ? sym1 -> value.iVal : sym1 -> value.fVal)
                        * (sym2 -> type == Integer ? sym2 -> value.iVal : sym2 -> value.fVal);
                    }
                    break;
                case '/':
                    if (sym2 -> type == Integer && sym2 -> value.iVal == 0) {
                        value.fVal = 0.0;
                        value.iVal = 0;
                    } else if (sym2 -> type == Float && sym2 -> value.fVal == 0.0) {
                        value.fVal = 0.0;
                        value.iVal = 0;
                    } else {
                        if (result -> type == Float) {
                            value.fVal = 1.0 *
                            (sym1 -> type == Integer ? sym1 -> value.iVal : sym1 -> value.fVal)
                            / (sym2 -> type == Integer ? sym2 -> value.iVal : sym2 -> value.fVal);
                        } else {
                            value.iVal = 1.0 *
                            (sym1 -> type == Integer ? sym1 -> value.iVal : sym1 -> value.fVal)
                            / (sym2 -> type == Integer ? sym2 -> value.iVal : sym2 -> value.fVal);
                        }
                    }
                    break;
                case '%':
                    if (sym2 -> type == Integer && sym2 -> value.iVal == 0) {
                        value.fVal = 0.0;
                        value.iVal = 0;
                    } else if (sym2 -> type == Float && sym2 -> value.fVal == 0.0) {
                        value.fVal = 0.0;
                        value.iVal = 0;
                    } else {
                        if (result -> type == Float) {
                            value.fVal = sym1 -> value.iVal % sym2 -> value.iVal;
                        } else {
                            value.iVal = sym1 -> value.iVal % sym2 -> value.iVal;
                        }
                    }
                    break;
                default:
                    break;
            }

            result -> value = value;

            free(sym1);
            free(sym2);
            free(sym);
            stk_push(st1, (void *) result);
        } else {
            stk_push(st1, (void *) sym);
        }
    }

    Symbol *result = stk_pop(st1);
    Symbol *currentSym = get(varName, symtab);
    if (currentSym -> type == Integer && result -> type == Float) {
        currentSym -> value.iVal = (int) roundf(result -> value.fVal);
    } else if (currentSym -> type == Float && result -> type == Integer) {
        currentSym -> value.fVal = (float) result -> value.iVal;
    } else {
        currentSym -> value = result -> value;
    }
    free(result);

    while(!stk_empty(st2)) {
        free(stk_pop(st2));
    }

    while(!stk_empty(st1)) {
        free(stk_pop(st1));
    }

    while(!stk_empty(st3)) {
        free(stk_pop(st3));
    }

    stk_destroy(st1);
    stk_destroy(st2);
    stk_destroy(st3);
}

/// Process the if statement
/// @param cmd: the statement
/// @param symtab: the symbol table
void processIf(char *cmd, SymTab *symtab) {
    char *element1 = strtok(cmd, " ,\n");
    char *bop = strtok(NULL, " ,\n");
    char *element2 = strtok(NULL, " ,\n");
    
    Symbol sym1 = elementToValue(element1, symtab);
    if (sym1.type == Unknown) {
        fprintf(stderr, "if: symbol '%s' does not exist\n", element1);
        return;
    }

    Symbol sym2 = elementToValue(element2, symtab);
    if (sym2.type == Unknown) {
        fprintf(stderr, "if: symbol '%s' does not exist\n", element2);
        return;
    }

    if (bop[0] != '!') {
        switch(bop[0]) {
            case '=':
                if (!(
                    (sym1.type == Integer ? sym1.value.iVal : sym1.value.fVal)
                    == (sym2.type == Integer ? sym2.value.iVal : sym2.value.fVal)
                )) {
                    return;
                }
                break;
            case '>':
                if (!(
                    (sym1.type == Integer ? sym1.value.iVal : sym1.value.fVal)
                    > (sym2.type == Integer ? sym2.value.iVal : sym2.value.fVal)
                )) {
                    return;
                }
                break;
            case '<':
                if (!(
                    (sym1.type == Integer ? sym1.value.iVal : sym1.value.fVal)
                    < (sym2.type == Integer ? sym2.value.iVal : sym2.value.fVal)
                )) {
                    return;
                }
                break;
            default:
                break;
        }
    } else {
        switch(bop[1]) {
            case '=':
                if (!(
                    (sym1.type == Integer ? sym1.value.iVal : sym1.value.fVal)
                    != (sym2.type == Integer ? sym2.value.iVal : sym2.value.fVal)
                )) {
                    return;
                }
                break;
            case '>':
                if (!(
                    (sym1.type == Integer ? sym1.value.iVal : sym1.value.fVal)
                    <= (sym2.type == Integer ? sym2.value.iVal : sym2.value.fVal)
                )) {
                    return;
                }
                break;
            case '<':
                if (!(
                    (sym1.type == Integer ? sym1.value.iVal : sym1.value.fVal)
                    >= (sym2.type == Integer ? sym2.value.iVal : sym2.value.fVal)
                )) {
                    return;
                }
                break;
            default:
                break;
        }
    }

    char *lineptr = NULL;    
    strtok(NULL, " ,\n");
    lineptr = strtok(NULL, "");
    processStatement(lineptr, symtab);
}

/// Process the display statement
/// @param cmd: the statement
/// @param symtab: the symbol table
void processDisplay(char *cmd, SymTab *symtab) {
    char *lineptr = NULL;
    lineptr = strtok(cmd, " ,\n");

    while(lineptr != NULL) {
        if (strlen(lineptr) > 0) {
            Symbol entry = elementToValue(lineptr, symtab);
            if (entry.type != Unknown) {
                printf(" ");
                printValSymbol(&entry);
            } else {
                fprintf(stderr, "display: symbol '%s' does not exist\n", lineptr);
            }
        }
        lineptr = strtok(NULL, " ,\n");
    }
}

/// Print symbol's value
/// @param entry: the symbol
void printValSymbol(Symbol *entry) {
    if (entry -> type == Integer) {
        printf("%d", entry -> value.iVal);
    } else {
        printf("%.3f", entry -> value.fVal);
    }
}

/// Retrieve the value of an element as a symbol
/// @param lineptr: the line contain the element
/// @param symtab: the symbol table
/// @return: a symbol contain the element
Symbol elementToValue(char *lineptr, SymTab *symtab) {
    Symbol symbol;
    strcpy(symbol.name, "element");

    if (strlen(lineptr) > 0) {
        if (('A' <= lineptr[0] && lineptr[0] <= 'Z') || ('a' <= lineptr[0] && lineptr[0] <= 'z')) {
            if (has(lineptr, symtab) == -1) {
                symbol.type = Unknown;
                return symbol;
            }
            Symbol *entry = get(lineptr, symtab);
            symbol.value = entry -> value;
            symbol.type = entry -> type;
        } else {
            bool isFloat = false;
            for (size_t i = 0; i < strlen(lineptr); i++) {
                if (lineptr[i] == '.') {
                    isFloat = true;
                    break;
                }
            }
            if (isFloat) {
                sscanf(lineptr, "%f", &symbol.value.fVal);
                symbol.type = Float;
            } else {
                sscanf(lineptr, "%d", &symbol.value.iVal);
                symbol.type = Integer;
            }
        }
    }
    return symbol;
}

/// Determine if a string is an operator or not
/// @param lineptr: the string to check
/// @return: 0 if it's a bracket, 1 if it's a mop, 2 if it's a aop, 
///         -1 if it's not an operator
int isOperator(char *lineptr) {

    if (strlen(lineptr) != 1) {
        return -1;
    }
    if (lineptr[0] == '(' || lineptr[0] == ')') {
        return 0;
    } else if (lineptr[0] == '*' || lineptr[0] == '/' || lineptr[0] == '%') {
        return 1;
    } else if (lineptr[0] == '+' || lineptr[0] == '-') {
        return 2;
    }

    return -1;
}

/// Process a Fred statement
/// @param lineptr: the string to check
/// @param symtab: the symbol table
void processStatement(char *cmd, SymTab *symtab) {
    size_t idx = 0;
    while(cmd[idx] == ' ') {
        idx++;
    }

    if (!strncmp(cmd + idx, "prt ", strlen("ptr "))) {
        while(cmd[idx] != ' ') {
            idx++;
        }
        while(cmd[idx] == ' ') {
            idx++;
        }
        processPrt(cmd + idx);
    } else if (!strncmp(cmd + idx, "display ", strlen("display "))) {
        while(cmd[idx] != ' ') {
            idx++;
        }
        while(cmd[idx] == ' ') {
            idx++;
        }
        processDisplay(cmd + idx, symtab);
    } else if (!strncmp(cmd + idx, "define ", strlen("define "))) {
        while(cmd[idx] != ' ') {
            idx++;
        }
        while(cmd[idx] == ' ') {
            idx++;
        }
        processDefine(cmd + idx, symtab);
    } else if (!strncmp(cmd + idx, "let ", strlen("let "))) {
        while(cmd[idx] != ' ') {
            idx++;
        }
        while(cmd[idx] == ' ') {
            idx++;
        }
        processLet(cmd + idx, symtab);
    } else if (!strncmp(cmd + idx, "if ", strlen("if "))) {
        while(cmd[idx] != ' ') {
            idx++;
        }
        while(cmd[idx] == ' ') {
            idx++;
        }
        processIf(cmd + idx, symtab);
    } else {
        fprintf(stderr, "Unknown statement %s\n", cmd + idx);
    }
}