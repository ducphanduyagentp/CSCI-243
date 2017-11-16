/// \file jukebox.h
/// \brief Definitions for the jukebox application.
/// Author: Sean Strout (RIT CS)

#ifndef JUKEBOX_H
#define JUKEBOX_H

#include <stdbool.h>  // bool, true, false
#include <stdio.h>    // FILE
#include "table.h"    // Table

/// The longest line in the million track file is 440 characters
const size_t MAX_LINE_LENGTH = 500;

/// Delimiter characters in song file
const char* SEP = "<>";

/// Total number of times to run the simulation
const size_t NUM_SIMULATIONS = 10000;

/// The Song structure
typedef struct Song_t {
    char* artist;  ///< artist name
    char* song;    ///< song name
} Song;

/// Hash function for Song's.  Uses a simple hashing formula that sums
/// the hash codes of the Song's artist plus the Song's names, using
/// strHash from hash.h.
/// @param element The Song
/// @return The hash code for the Song
long songHash(void* element);

/// Compare whether two Song's are equal.  They are equal if both the artist
/// and the song are equal.
/// @param element1 First song
/// @param element2 Second song
/// @return true if equal, false otherwise
bool songEquals(void* element1, void* element2);

/// Debug function for printing out a Song and it's number of plays.
/// @param key The song
/// @param value The play count (a long)
void songPrint(void* key, void* value);

/// Create and load the jukebox with songs from the file.  The Song's themselves
/// are dynamically allocated here and ownership is passed to the caller.
/// The file is formatted per line:
///    track id<SEP>song id<SEP>artist name<SEP>song title
/// @param fp The already opened file pointer
/// @exception assert fails if can't allocate memory
/// @return A new table with the Song's loaded with 0 plays each
Table* loadJukebox(FILE* fp);

/// Display the jukebox statistics from runSimulation.
/// @param jukebox The table of songs and plays
/// @exception assert fails if can't allocate memory
void printStatistics(Table* jukebox);

/// Run the paradox simulation.  Figure out on average how many plays it takes
/// to play a duplicate song.  We will also keep track of overall how many songs
/// are played.
/// @param jukebox The table of songs to plays
/// @exception assert fails if can't allocate memory
void runSimulation(Table* jukebox);

#endif // JUKEBOX_H
