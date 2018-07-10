/// file: fred.c
/// description: implementation of interpreter for Fred Language
/// author: Duc Phan - ddp3945@rit.edu

#define _BSD_SOURCE
#define PROMPT "\n> "
#define ECHO "::: "

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "utils.h"
#include "symtab.h"

/// Process the input 
/// @param symtabFile: the symbol table file
/// @param programFile: the Fred program file
void processProgram(char *symtabFile, char *programFile) {
    char *cmd = NULL;
    size_t len = 0;
    char *lineptr = NULL;

    SymTab *symtab = createSymTab();
    if (symtabFile != NULL) {
        loadSymTabFile(symtabFile, symtab);
    }

    FILE *fdProg = stdin;
    if (programFile != NULL) {
        fdProg = fopen(programFile, "r");
        if (fdProg == NULL) {
            fprintf(stderr, "Error opening program file %s\n", programFile);
            exit(EXIT_FAILURE);
        }
    }
    
    while(true) {
        
        printf(PROMPT);

        if (getline(&cmd, &len, fdProg) == EOF) {
            puts("");
            break;
        }
        
        printf(ECHO);
        printf("%s", cmd);
        if (cmd[strlen(cmd) - 1] != '\n') {
            puts("");
        }

        processStatement(cmd, symtab);
    }

    dump(symtab);
    if (programFile != NULL && fdProg != NULL) {
        fclose(fdProg);
    }

    if (cmd != NULL) {
        free(cmd);
    }

    if (lineptr != NULL) {
        free(lineptr);
    }
    destroySymTab(symtab);
}

/// Main program
/// @param argc: argument count
/// @param argv: argument vector
/// @return: EXIT_SUCCESS if no error occurred, otherwise EXIT_FAILURE
int main(int argc, char **argv) {
    char *symtabFile = NULL;
    char *programFile = NULL;
    int opt;

    if (argc > 1) {
        if (argc != 3 && argc != 5) {
            fprintf(stderr, "fred  [ -s symbol_table_file ] [ -f program_file ]\n");
            return EXIT_FAILURE;
        }
        opterr = 0;
        while ((opt = getopt(argc, argv, "s:f:")) != -1) {
            switch (opt) {
                case 's':
                    if (symtabFile != NULL) {
                        fprintf(stderr, "fred  [ -s symbol_table_file ] [ -f program_file ]\n");
                        return EXIT_FAILURE;
                    }
                    symtabFile = (char *) malloc(strlen(optarg) + 1);
                    strncpy(symtabFile, optarg, strlen(optarg) + 1);
                    break;
                case 'f':
                    if (programFile != NULL) {
                        fprintf(stderr, "fred  [ -s symbol_table_file ] [ -f program_file ]\n");
                        return EXIT_FAILURE;
                    }
                    programFile = (char *) malloc(strlen(optarg) + 1);
                    strncpy(programFile, optarg, strlen(optarg) + 1);
                    break;
                default:
                    fprintf(stderr, "fred  [ -s symbol_table_file ] [ -f program_file ]\n");
                    return EXIT_FAILURE;
            }
        }
    }

    processProgram(symtabFile, programFile);
    if (programFile != NULL) {
        free(programFile);
    }
    if (symtabFile != NULL) {
        free(symtabFile);
    }
    return EXIT_SUCCESS;
}
