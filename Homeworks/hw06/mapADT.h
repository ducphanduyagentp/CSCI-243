// Description:  Interface to the Map ADT module.
//
// Authors: ckd, wrc

#ifndef _MAPADT_H_
#define _MAPADT_H_

#include <stdbool.h>

//
// Only define the MapADT type if we are _not_ compiling the
// module's implementation file.
//
// In the implementation file, define the MapADT type as you need
// to for the implementation, and then define the symbol _MAP_IMPL_
// immediately before including this header file to prevent a
// redefinition of the type.
//

#ifndef _MAP_IMPL_
typedef struct { } *MapADT;
#endif

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
MapADT map_create( bool (*equals)(const void *a,const void *b) );

/// Tear down and deallocate the supplied MapADT.
///
/// @param map - the MapADT to be manipulated
void map_destroy( MapADT map );

/// Remove all contents from the supplied MapADT, but do not
/// deallocate the MapADT itself.
///
/// @param map - the MapADT to be manipuated
void map_clear( MapADT map );

/// Check if the specified entry exists in the map
///
/// Uses the map's equals function to determine if an entry with
/// the same key already exists in the map. If so, then return
/// true.
///
/// @param key the key to lookup in the map
/// @return true if the entry exists, else false
bool map_contains(MapADT map, void *key);

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
void* map_put( MapADT map, void *key, void *value);

/// Find the entry specified by the key and return a pointer to the value
///
/// @param map the MapADT to be manipulated
/// @return the value that corresponds to the key
/// @exception If the map is empty, the program should terminate
///     with an error message.  This can be done by printing an
///     appropriate message to the standard error output and then
///     exiting with EXIT_FAILURE, or by having an assert() fail.
const void *map_get( MapADT map, void *key );

/// Find the entry specified by the key and remove it from the MapADT,
/// returning the value pointer. 
///
/// @param map the MapADT to be manipulated
/// @return the value that was removed from the map
/// @exception If the map is empty, the program should terminate
///     with an error message.  This can be done by printing an
///     appropriate message to the standard error output and then
///     exiting with EXIT_FAILURE, or by having an assert() fail.
void *map_delete( MapADT map, void *key );

/// Indicate whether or not the supplied Map is empty
///
/// @param the MapADT to be tested
/// @return true if the map is empty, otherwise false
bool map_empty( MapADT map );

#endif

