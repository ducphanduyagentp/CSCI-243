/// \file hash.h
/// \brief Definitions for some common hashing functions, along with
///    equality checking and debug printing.   This is used by both
///    test_table.c and jukebox.c
/// Author: Sean Strout (RIT CS)

#ifndef HASH_H
#define HASH_H

#include <stdbool.h>  // bool
#include <stdio.h>    // printf

/// A hash function for longs, which is just the long value.
/// @param element The long value
/// @return The hash code
long longHash(void* element);

/// A comparison function for two longs.
/// @param element1 The first long
/// @param element2 The second long
/// @return Whether the longs are equal or not
bool longEquals(void* element1, void* element2);

/// A print function for long keys with c-string values.
/// @param key the integer key
/// @param value the c-string value
void longStrPrint(void* key, void* value);

/// A function for hashing a native c-string.  
/// @param element the c-string to hash
/// @return the hash value of the c-string
long strHash(void* element);

/// A comparison function for c-strings
/// @param element1 first c-string
/// @param element2 second c-string
/// @return Whether the c-string's are equal or not
bool strEquals(void* element1, void* element2);

/// A print function for c-string keys with long values.
/// @param key the c-string key
/// @param value the long value
void strLongPrint(void* key, void* value);

/// A print function for long keys with long values.
/// @param key the long key
/// @param value the long value
void longLongPrint(void* key, void* value);

#endif
