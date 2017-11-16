/// \file hash.c
/// \brief Implementations of some common hashing functions, along with
///    equality checking and debug printing.
/// Author: Sean Strout (RIT CS)

#include <string.h>
#include "hash.h"

#include <string.h>  // strcmp

long longHash(void* element) {
    return (long)element;
}

bool longEquals(void* element1, void* element2) {
    return (long)element1 == (long)element2;
}

void longStrPrint(void* key, void* value) {
    printf("%lu : %s", (long)key, (char*)value);
}

/// Uses djb2, A simple c-string hashing algorithm by Dan Bernstein
/// http://www.cse.yorku.ca/~oz/hash.html
long strHash(void* element) {
    unsigned char *str = (unsigned char *) element;
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return (long)hash;
}

bool strEquals(void* element1, void* element2) {
    return strcmp((char*)element1, (char*)element2) == 0;
}

void strLongPrint(void* key, void* value) {
    printf("%s : %lu", (char*)key, (long)value);
}

void longLongPrint(void* key, void* value) {
    printf("%lu : %lu", (long)key, (long)value);
}
