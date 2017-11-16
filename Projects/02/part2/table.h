/// \file table.h
/// \brief A generic hash table data structure.
/// Author: Sean Strout (RIT CS)

#ifndef TABLE_H
#define TABLE_H

#include <stdbool.h>    // bool
#include <stddef.h>     // size_t

/// Initial capacity of table upon creation
#define INITIAL_CAPACITY 16

/// The load at which the table will rehash
#define LOAD_THRESHOLD 0.75

/// The table size will double upon each rehash
#define RESIZE_FACTOR 2

/// The "buckets" in the table
typedef struct Entry_t {
    void* key;     ///< key must be "hashable"
    void* value;   ///< value can be any type
} Entry;

/// The hash table
typedef struct Table_t {
    Entry** table;    ///< table of Entry pointers
    size_t size;      ///< number of entries in table
    size_t capacity;  ///< total capacity of table
    long (*hash)(void* key);  ///< hash function for key
    bool (*equals)(void *key1, void* key2); ///< equals function for key comparison
    void (*print)(void *key, void* value);  ///< print function for table dump debug
    size_t collisions;      ///< number of collisions throughout life of hash table
    size_t rehashes;        ///< number of rehashes throughout life of hash table
} Table;

/// Create a new hash table.
/// @param hash The key's hash function
/// @param equals The key's equal function for comparison
/// @param print A print function for the key/value, used for dump debugging
/// @exception Assert fails if can't allocate space
/// @return A newly created table
Table* create(long (*hash)(void* key),
              bool (*equals)(void* key1, void* key2),
              void (*print)(void* key1, void* key2));

/// Destroy a table
/// @param t The table to destroy
void destroy(Table* t);

/// Print out information about the hash table (size,
/// capacity, collisions, rehashes).  If full is
/// true, it will also print out the entire contents of the hash table,
/// using the registered print function with each non-null entry.
/// @param t The table to display
/// @param full Do a full dump of entire table contents
void dump(Table* t, bool full);

/// Get the value associated with a key from the table.  This function
/// uses the registered hash function to locate the key, and the
/// registered equals function to check for equality.
/// @pre The table must have the key, or else it will assert fail
/// @param t The table
/// @param key The key
/// @return The associated value of the key
void* get(Table* t, void* key);

/// Check if the table has a key.  This function uses the registered hash
/// function to locate the key, and the registered equals function to
/// check for equality.
/// @param t The table
/// @param key The key
/// @return Whether the key exists in the table.
bool has(Table* t, void* key);

/// Get the collection of keys from the table.  This function allocates
/// space to store the keys, which the caller is responsible for freeing.
/// @param t The table
/// @exception Assert fails if can't allocate space
/// @return A dynamic array of keys
void** keys(Table* t);

/// Add a key value pair to the table, or update an existing key's value.
/// This function uses the registered hash function to locate the key,
/// and the registered equals function to check for equality.
/// @param t The table
/// @param key The key
/// @param value The value
/// @exception Assert fails if can't allocate space
/// @return The old value in the table, if one exists.
void* put(Table* t, void* key, void* value);

/// Get the collection of values from the table.  This function allocates
/// space to store the values, which the caller is responsible for freeing.
/// @param t The table
/// @exception Assert fails if can't allocate space
/// @return A dynamic array of values
void** values(Table* t);

#endif // TABLE_H
