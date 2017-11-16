/// file: jukebox.c
/// description: implementation for jukebox paradox - project 2
/// author: Duc Phan - ddp3945@rit.edu

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include "hash.h"
#include "jukebox.h"
#include "table.h"

/// Hash function for Song's.  Uses a simple hashing formula that sums
/// the hash codes of the Song's artist plus the Song's names, using
/// strHash from hash.h.
/// @param element The Song
/// @return The hash code for the Song
long songHash(void* element) {
    if (element == NULL) {
        fprintf(stderr, "Element is NULL\n");
        assert(NULL);
    }
    return strHash( ((Song *) element) -> artist) + strHash( ((Song *) element) -> song);
}

/// Compare whether two Song's are equal.  They are equal if both the artist
/// and the song are equal.
/// @param element1 First song
/// @param element2 Second song
/// @return true if equal, false otherwise
bool songEquals(void* element1, void* element2) {
    if (element1 == NULL || element2 == NULL) {
        fprintf(stderr, "One of the elements is NULL");
        assert(NULL);
    }
    return (
        strEquals(((Song *) element1) -> artist, ((Song *) element2) -> artist) &&
        strEquals(((Song *) element1) -> song, ((Song *) element2) -> song)
    );
}

/// Debug function for printing out a Song and it's number of plays.
/// @param key The song
/// @param value The play count (a long)
void songPrint(void* key, void* value) {
    printf("%s by %s, %ld times.\n", 
        ((Song *) key) -> song, 
        ((Song *) key) -> artist, 
        (long) value
    );
}

/// Create and load the jukebox with songs from the file.  The Song's themselves
/// are dynamically allocated here and ownership is passed to the caller.
/// The file is formatted per line:
///    track id<SEP>song id<SEP>artist name<SEP>song title
/// @param fp The already opened file pointer
/// @exception assert fails if can't allocate memory
/// @return A new table with the Song's loaded with 0 plays each
Table* loadJukebox(FILE* fp) {
    Table *jukebox = create(songHash, songEquals, songPrint);

    char *line = (char *) calloc(MAX_LINE_LENGTH + 1, sizeof(char));
    if (line == NULL) {
        fprintf(stderr, "jukebox::loadJukebox() failed to allocate space\n");
        assert(NULL);
    }

    while(fgets(line, MAX_LINE_LENGTH, fp)) {
        Song *song = (Song *) malloc(sizeof(Song));
        if (song == NULL) {
            fprintf(stderr, "jukebox::loadJukebox() failed to allocate space\n");
            assert(NULL);
        }

        char *ptr = strtok(line, SEP);

        for (int i = 0; i < 3 && ptr != NULL; i++) {
            ptr = strtok(NULL, SEP);
            ptr = strtok(NULL, SEP);
            ptr[strcspn(ptr, "\n")] = 0;

            if (ptr != NULL) {
                while (*ptr == ' ') {
                    ptr++;
                }
            }

            if (i == 1) {
                song -> artist = ptr != NULL ? strdup(ptr) : (char *) calloc(1, sizeof(char));
            } else if (i == 2) {
                song -> song = ptr != NULL ? strdup(ptr) : (char *) calloc(1, sizeof(char));
            }
        }

        if (has(jukebox, (void *) song)) {
            free(song -> artist);
            free(song -> song);
            free(song);
        } else {
            put(jukebox, (void *) song, (void *) 0);
        }
    }
    free(line);
    return jukebox;
}

/// Display the jukebox statistics from runSimulation.
/// @param jukebox The table of songs and plays
/// @exception assert fails if can't allocate memory
void printStatistics(Table* jukebox) {
    long nSongPlayed = 0;
    size_t minIdx = 0;
    size_t maxIdx = 0;
    long nMin = LONG_MAX;
    long nMax = 0;
    
    for (size_t i = 0; i < jukebox -> capacity; i++) {
        if (jukebox -> table[i] != NULL) {
            nSongPlayed += (long) jukebox -> table[i] -> value;
            if ((long) jukebox -> table[i] -> value > nMax) {
                maxIdx = i;
                nMax = (long) jukebox -> table[i] -> value;
            }
            if ((long) jukebox -> table[i] -> value < nMin) {
                minIdx = i;
                nMin = (long) jukebox -> table[i] -> value;
            }
        }
    }

    printf("Jukebox statistics\n");
    printf("==================\n");
    printf("Number of songs in jukebox: %lu\n", jukebox -> size);
    printf("Number of simulations run: %lu\n", NUM_SIMULATIONS);
    printf("Total number of songs played: %ld\n", nSongPlayed);
    printf("Average number of plays to get duplicate: %ld\n", nSongPlayed / NUM_SIMULATIONS);
    printf("Most played song: ");
    jukebox -> print(jukebox -> table[maxIdx] -> key, jukebox -> table[maxIdx] -> value);
    printf("Least played song: ");
    jukebox -> print(jukebox -> table[minIdx] -> key, jukebox -> table[minIdx] -> value);
}

/// Run the paradox simulation.  Figure out on average how many plays it takes
/// to play a duplicate song.  We will also keep track of overall how many songs
/// are played.
/// @param jukebox The table of songs to plays
/// @exception assert fails if can't allocate memory
void runSimulation(Table* jukebox) {
    int seed = time(0);
    srand(seed);
    
    Song **allKeys = (Song **) keys(jukebox);
    for (size_t _ = 0; _ < NUM_SIMULATIONS; _++) {
        Table *newTable = create(songHash, songEquals, songPrint);
        while (true) {
            size_t idx = rand() % (jukebox -> size);
            while(allKeys[idx] == NULL) {
                idx = rand() % (jukebox -> size);
            }
            put(jukebox, 
                (void *) allKeys[idx], 
                (void *) ((long) get(
                    jukebox, 
                    (void *) allKeys[idx]
                ) + 1)
            );
            if (has(newTable, (void *) allKeys[idx])) {
                break;
            }
            put(newTable, (void *) allKeys[idx], (void *) 1);
        }
        destroy(newTable);
    }
    free(allKeys);
}

/// The main function runs the jukebox paradox simulation
/// Usage: ./jukebox filename
/// @param argc command line argument count
/// @param argv command line arguments
/// @return EXIT_FAILURE if any errors happen, otherwise EXIT_SUCCESS
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: jukebox filename\n");
        return EXIT_FAILURE;
    }

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "Error reading file: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    Table *jukebox = loadJukebox(fp);
    fclose(fp);

    runSimulation(jukebox);
    printStatistics(jukebox);

    for (size_t i = 0; i < jukebox -> capacity; i++) {
        if (jukebox -> table[i] != NULL) {
            Song *song = (Song *) jukebox -> table[i] -> key;
            free(song -> artist);
            free(song -> song);
            free(song);
        }
    }
    destroy(jukebox);
    
    return 0;
}
