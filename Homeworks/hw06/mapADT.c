// file: mapADT.c
// description: 
// implementation of map Abstract Data Type - homework 6
// author: Duc Phan, ddp3945@rit.edu
// date: 10/28/2017

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>

#define MAP_ALLOC_UNIT 5

struct mapStruct {
    size_t mapCapacity;
    size_t mapNum;
    void **keyContents;
    void **valueContents;
    bool (*equals)(const void *a, const void *b);
};

typedef struct mapStruct *MapADT;

#define _MAP_IMPL_
#include "mapADT.h"

/// Create a MapADT which uses the supplied function as a comparison
/// routine.
///
/// The comparison function takes two void* parameters, and returns
/// a Boolean result which indicates if the two things are identical: 
///
///	Result	Relationship
///	======	===========
///	 true	a == b
///	 false	a != b
///
/// @param equals the address of the equals function to be used for
///    determining if two keys are equal
/// @return a pointer to a MapADT, or NULL if the allocation fails
MapADT map_create( bool (*equals)(const void *a,const void *b) ) {
    MapADT new;
    new = (MapADT) malloc(sizeof(struct mapStruct));
    if (new != NULL) {
        new -> mapCapacity = 0;
        new -> mapNum = 0;
        new -> keyContents = NULL;
        new -> valueContents = NULL;
        new -> equals = equals;
    }

    return new;
}

/// Tear down and deallocate the supplied MapADT.
///
/// @param map - the MapADT to be manipulated
void map_destroy( MapADT map ) {
    assert(map != NULL);
    map_clear(map);
    free(map);
}

/// Remove all contents from the supplied MapADT, but do not
/// deallocate the MapADT itself.
///
/// @param map - the MapADT to be manipuated
void map_clear( MapADT map ) {
    if (map -> keyContents != 0 && map -> valueContents != 0) {
        for (size_t i = 0; i < map -> mapCapacity; i++) {
            if (map -> keyContents[i] != NULL) {
                free(map -> keyContents[i]);
                map -> keyContents[i] = NULL;
            }
            if (map -> valueContents[i] != NULL) {
                free(map -> valueContents[i]);
                map -> valueContents[i] = NULL;
            }
        }
        free(map -> keyContents);
        free(map -> valueContents);
        map -> keyContents = NULL;
        map -> valueContents = NULL;
    }
    map -> mapCapacity = 0;
    map -> mapNum = 0;
}

/// Check if the specified entry exists in the map
///
/// Uses the map's equals function to determine if an entry with
/// the same key already exists in the map. If so, then return
/// true.
///
/// @param key the key to lookup in the map
/// @return true if the entry exists, else false
bool map_contains(MapADT map, void *key) {
    if (map -> keyContents != NULL) {
        for (size_t i = 0; i < map -> mapCapacity; i++) {
            if (map -> keyContents[i] == NULL) {
                continue;
            }

            if (map -> equals(map -> keyContents[i], key)) {
                return true;
            }
        }
    }
    return false;
}

/// Put the specified entry into the Map
///
/// Uses the map's equals function to determine if an entry with
/// the same key already exists in the map. If so, then replace it 
/// and return a pointer to the old value.
///
/// @param map the MapADT into which the value is to be inserted
/// @param key the key to be inserted
/// @param value the value to be inserted
/// @exception If the value cannot be inserted into the map for
///     whatever reason (usually implementation-dependent), the program
///     should terminate with an error message.  This can be done by
///     printing an appropriate message to the standard error output and
///     then exiting with EXIT_FAILURE, or by having an assert() fail.
void* map_put( MapADT map, void *key, void *value) {
    if (map -> keyContents == NULL && map -> valueContents == NULL) {
        map -> keyContents = calloc(MAP_ALLOC_UNIT, sizeof(void *));
        map -> valueContents = calloc(MAP_ALLOC_UNIT, sizeof(void *));
        assert(map -> keyContents != NULL);
        assert(map -> valueContents != NULL);
        assert(map -> keyContents[0] == NULL);
        map -> mapCapacity = MAP_ALLOC_UNIT;
    }

    if (map -> mapNum >= map -> mapCapacity) {
        void *tmpKey;
        tmpKey = realloc(map -> keyContents, sizeof(void *) * (map -> mapCapacity + MAP_ALLOC_UNIT));
        assert(tmpKey != NULL);

        void *tmpValue;
        tmpValue = realloc(map -> valueContents, sizeof(void *) * (map -> mapCapacity + MAP_ALLOC_UNIT));
        assert(tmpValue != NULL);

        map -> keyContents = tmpKey;
        map -> valueContents = tmpValue;
        map -> mapCapacity += MAP_ALLOC_UNIT;
        for (size_t i = map -> mapNum; i < map -> mapCapacity; i++) {
            map -> keyContents[i] = NULL;
            map -> valueContents[i] = NULL;
        }
    }

    for (size_t i = 0; i < map -> mapCapacity; i++) {
        if (map -> keyContents[i] == NULL) {
            continue;
        }

        if (map -> equals(map -> keyContents[i], key)) {
            void *tmp = map -> valueContents[i];
            map -> valueContents[i] = value;
            assert(map_contains(map, key) && map_get(map, key) == value);
            return tmp;
        }
    }
    
    for (size_t i = 0; i < map -> mapCapacity; i++) {
        if (map -> keyContents[i] == NULL) {
            map -> keyContents[i] = key;
            map -> valueContents[i] = value;
            map -> mapNum++;
            break;
        }
    }
    assert(map_contains(map, key) && map_get(map, key) == value);
    return NULL;
}

/// Find the entry specified by the key and return a pointer to the value
///
/// @param map the MapADT to be manipulated
/// @return the value that corresponds to the key
/// @exception If the map is empty, the program should terminate
///     with an error message.  This can be done by printing an
///     appropriate message to the standard error output and then
///     exiting with EXIT_FAILURE, or by having an assert() fail.
const void *map_get( MapADT map, void *key ) {
    assert(!map_empty(map));
    for (size_t i = 0; i < map -> mapCapacity; i++) {
        if (map -> keyContents[i] == NULL) {
            continue;
        }

        if (map -> equals(map -> keyContents[i], key)) {
            return map -> valueContents[i];
        }
    }
    return NULL;
}

/// Find the entry specified by the key and remove it from the MapADT,
/// returning the value pointer. 
///
/// @param map the MapADT to be manipulated
/// @return the value that was removed from the map
/// @exception If the map is empty, the program should terminate
///     with an error message.  This can be done by printing an
///     appropriate message to the standard error output and then
///     exiting with EXIT_FAILURE, or by having an assert() fail.
void *map_delete( MapADT map, void *key ) {
    assert(!map_empty(map));
    for (size_t i = 0; i < map -> mapCapacity; i++) {
        if (map -> keyContents[i] == NULL) {
            continue;
        }

        if (map -> equals(map -> keyContents[i], key)) {
            void *tmp = map -> valueContents[i];
            free(map -> keyContents[i]);
            map -> keyContents[i] = NULL;
            map -> valueContents[i] = NULL;
            return tmp;
        }
    }
    return NULL;
}

/// Indicate whether or not the supplied Map is empty
///
/// @param the MapADT to be tested
/// @return true if the map is empty, otherwise false
bool map_empty( MapADT map ) {
    return (map -> mapNum == 0);
}
