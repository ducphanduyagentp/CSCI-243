/// file: table.c
/// description: implementation for hash table - project 2
/// author: Duc Phan - ddp3945@rit.edu

#include <stdbool.h>    // bool
#include <stddef.h>     // size_t
#include <stdio.h>      // input, output
#include <stdlib.h>     // malloc, calloc, realloc
#include <assert.h>     // assert
#include "table.h"

/// Create a new hash table.
/// @param hash The key's hash function
/// @param equals The key's equal function for comparison
/// @param print A print function for the key/value, used for dump debugging
/// @exception Assert fails if can't allocate space
/// @return A newly created table
Table* create(long (*hash)(void* key), bool (*equals)(void* key1, void* key2), void (*print)(void* key1, void* key2)) {
    Table *new = (Table *) malloc(sizeof(Table));
    if (new != NULL) {
        new -> size = 0;
        new -> capacity = INITIAL_CAPACITY;
        new -> collisions = 0;
        new -> rehashes = 0;
        Entry **new_table = (Entry **) calloc(INITIAL_CAPACITY, sizeof(Entry *));
        if (new_table != NULL) {
            new -> table = new_table;
        } else {
            fprintf(stderr, "table::create() failed to allocate space\n");
            assert(NULL);
        }
        new -> hash = hash;
        new -> equals = equals;
        new -> print = print;
    } else {
        fprintf(stderr, "table::create() failed to allocate space\n");
        assert(NULL);
    }
    return new;
}

/// Destroy a table
/// @param t The table to destroy
void destroy(Table* t) {
    if (t != NULL) {
        if (t -> table != NULL) {
            for (size_t i = 0; i < t -> capacity; i++) {
                if (t -> table[i] != NULL) {
                    free(t -> table[i]);
                    t -> table[i] = NULL;
                }
            }
            free(t -> table);
            t -> table = NULL;
        } else {
            fprintf(stderr, "table of entries is NULL\n");
            assert(NULL);
        }
    } else {
        fprintf(stderr, "table is NULL\n");
        assert(NULL);
    }
    free(t);
}

/// Print out information about the hash table (size,
/// capacity, collisions, rehashes).  If full is
/// true, it will also print out the entire contents of the hash table,
/// using the registered print function with each non-null entry.
/// @param t The table to display
/// @param full Do a full dump of entire table contents
void dump(Table* t, bool full) {
    printf("Size: %lu\n"
    "Capacity: %lu\n"
    "Collisions: %lu\n"
    "Rehashes: %lu\n", t -> size, t -> capacity, t -> collisions, t -> rehashes);
    if (full) {
        for (size_t i = 0; i < t -> capacity; i++) {
            printf("%lu: ", i);
            if (t -> table[i] == NULL) {
                puts("null");
                continue;
            }
            printf("(");
            t -> print(t -> table[i] -> key, t -> table[i] -> value);
            printf(")\n");

        }
    }
}

/// Get the value associated with a key from the table.  This function
/// uses the registered hash function to locate the key, and the
/// registered equals function to check for equality.
/// @pre The table must have the key, or else it will assert fail
/// @param t The table
/// @param key The key
/// @return The associated value of the key
void* get(Table* t, void* key) {
    if (t == NULL) {
        fprintf(stderr, "Table is NULL\n");
        assert(NULL);
    }
    if (t -> table == NULL) {
        fprintf(stderr, "Table of entries is NULL\n");
        assert(NULL);
    }
    size_t index = t -> hash(key) % (t -> capacity);

    for (size_t i = 0; i < t -> capacity && t -> table[index] != NULL; i++) {
        if (t -> table[index] != NULL) {
            if (t -> equals(key, t -> table[index] -> key)) {
                return t -> table[index] -> value;
            }
        }
        index = (index + 1) % (t -> capacity);
        t -> collisions++;
    }
    return NULL;
}

/// Check if the table has a key.  This function uses the registered hash
/// function to locate the key, and the registered equals function to
/// check for equality.
/// @param t The table
/// @param key The key
/// @return Whether the key exists in the table.
bool has(Table* t, void* key) {
    if (t == NULL) {
        fprintf(stderr, "Table is NULL\n");
        assert(NULL);
    }
    if (t -> table == NULL) {
        fprintf(stderr, "Table of entries is NULL\n");
        assert(NULL);
    }
    size_t index = t -> hash(key) % (t -> capacity);

    for (size_t i = 0; i < t -> capacity && t -> table[index] != NULL; i++) {
        if (t -> table[index] != NULL) {
            if (t -> equals(key, t -> table[index] -> key)) {
                return true;
            }
        }
        t -> collisions++;
        index = (index + 1) % (t -> capacity);
    }
    return false;
}

/// Get the collection of keys from the table.  This function allocates
/// space to store the keys, which the caller is responsible for freeing.
/// @param t The table
/// @exception Assert fails if can't allocate space
/// @return A dynamic array of keys
void** keys(Table* t) {
    void **keys = calloc(t -> capacity, sizeof(void *));
    size_t num = 0;
    for (size_t i = 0; i < t -> capacity; i++) {
        if (t -> table[i] != NULL) {
            keys[num] = t -> table[i] -> key;
            num++;
        }
    }
    return keys;
}

/// Add a key value pair to the table, or update an existing key's value.
/// This function uses the registered hash function to locate the key,
/// and the registered equals function to check for equality.
/// @param t The table
/// @param key The key
/// @param value The value
/// @exception Assert fails if can't allocate space
/// @return The old value in the table, if one exists.
void* put(Table* t, void* key, void* value) {
    // TODO: rehashing
    if (t -> capacity != 0 && t -> size * 1.0 / t -> capacity >= LOAD_THRESHOLD) {
        Entry **new_table = (Entry **) calloc(t -> capacity * RESIZE_FACTOR, sizeof(Entry *));
        if (new_table != NULL) {
            for (size_t i = 0; i < t -> capacity; i++) {
                if (t -> table[i] == NULL) {
                    continue;
                }
                size_t index = t -> hash(t -> table[i] -> key) % (t -> capacity * RESIZE_FACTOR);
                for (size_t j = 0; j < t -> capacity * RESIZE_FACTOR && new_table[index] != NULL; j++) {
                    index = (index + 1) % (t -> capacity * RESIZE_FACTOR);
                    t -> collisions++;
                }
                if (new_table[index] == NULL) {
                    new_table[index] = t -> table[i];
                } else {
                    fprintf(stderr, "table::put() rehash failed\n");
                    assert(NULL);
                }

            }
            free(t -> table);
            t -> table = new_table;
            t -> capacity *= RESIZE_FACTOR;
            t -> rehashes++;
        } else {
            fprintf(stderr, "table::put() failed to allocate space\n");
            assert(NULL);
        }
    }

    void *oldValue = NULL;
    size_t index = (t -> hash(key)) % (t -> capacity);
    for (size_t i = 0; i < t -> capacity && t -> table[index] != NULL; i++) {
        if (t -> equals(key, t -> table[index] -> key)) {
            oldValue = t -> table[index] -> value;
            break;
        }
        index = (index + 1) % (t -> capacity);
        t -> collisions++;
    }
    
    if (t -> table[index] == NULL) {
        Entry *new = (Entry *) malloc(sizeof(Entry));
        new -> key = key;
        new -> value = value;
        if (new != NULL) {
            t -> table[index] = new;
            t -> size++;
        } else {
            fprintf(stderr, "table::put() failed to allocate space\n");
            assert(NULL);
        }
    } else {
        if (t -> equals(key, t -> table[index] -> key)) {
            t -> table[index] -> value = value;
        } else {
            fprintf(stderr, "table::put() put failed\n");
            assert(NULL);
        }
    }
    
    return oldValue;
}

/// Get the collection of values from the table.  This function allocates
/// space to store the values, which the caller is responsible for freeing.
/// @param t The table
/// @exception Assert fails if can't allocate space
/// @return A dynamic array of values
void** values(Table* t) {
    void **values = calloc(t -> capacity, sizeof(void *));
    size_t num = 0;
    for (size_t i = 0; i < t -> capacity; i++) {
        if (t -> table[i] != NULL) {
            values[num] = t -> table[i] -> value;
            num++;
        }
    }
    return values;
}