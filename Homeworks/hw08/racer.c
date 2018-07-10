/// file: racer.c
/// description: implementation for racer.h
/// author: Duc Phan - ddp3945@rit.edu

#define _BSD_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <assert.h>
#include "racer.h"
#include "display.h"

pthread_mutex_t myMutex;
long waitTime;

/// initRacers - Do setup work for all racers at the start of the program.
/// @param milliseconds length of pause between steps in animation 
///
void initRacers( long milliseconds ) {
    pthread_mutex_init(&myMutex, NULL);
    waitTime = milliseconds;
}

/// makeRacer - Create a new racer.
///
/// @param name the string name to show on the display for this racer
/// @param position the row in which to race
/// @return Racer pointer a dynamically allocated Racer object
/// @pre strlen( name ) <= MAX_NAME_LEN, for display reasons.
///
Racer *makeRacer( char *name, int position ) {
    Racer *racer = (Racer *) malloc(sizeof(Racer));
    racer -> dist = 0;
    racer -> row = position;
    racer -> graphic = (char *) malloc(MAX_CAR_LEN + 1);
    strncpy(racer -> graphic, "~O=-------o>", MAX_CAR_LEN + 1);
    assert(racer -> graphic[MAX_CAR_LEN] == '\0');
    strncpy(racer -> graphic + 3, name, strlen(name));
    return racer;
}

/// destroyRacer - Destroy all dynamically allocated storage for a racer.
///
/// @param racer the object to be de-allocated
///
void destroyRacer( Racer *racer ) {
    free(racer -> graphic);
    free(racer);
}

/// Run one racer in the race.
/// Initialize the display of the racer*:
///   The racer starts at the start position, column 0.
///   The racer's graphic (a string) is displayed.
///
/// These actions happen repetitively, until its position is at FINISH_LINE:
///
///  Calculate a random waiting period between 0 and 
///    the initial delay value given to initRacers at the start of the race.
///  Sleep for that length of time.
///  If sleep value is less than or equal 3, racer gets flat and can't finish.
///    A flat tire is displayed by 'X' in the second character of the graphic.
///    Display the car with the flat tire and stop further racing.
///  Change the display position of this racer by +1 column this way:
///    Erase the racer's car and name from the display.
///    Update the racer's dist field by +1.
///    Display the racer's graphic (car and name) at the new position.
///
/// The intention is to execute this function many times simultaneously,
/// each in its own thread.
///
/// Note: Be sure to keep the update of the display by one racer "atomic".
///
/// @pre racer cannot be NULL.
/// @param racer Racer object declared as void* for pthread compatibility
/// @return void pointer to status. A NULL represents success.
///
void *run( void *racer ) {
    Racer *trueRacer = (Racer *) racer;
    
    pthread_mutex_lock(&myMutex);
    set_cur_pos(trueRacer -> row, 1);
    puts(trueRacer -> graphic);
    fflush(stdout);
    pthread_mutex_unlock(&myMutex);

    while(true) {
        long waitMilli = (rand() % waitTime);
        usleep(waitMilli * 1000);

        pthread_mutex_lock(&myMutex);
        if (waitMilli <= 3) {
            trueRacer -> graphic[1] = 'X';
        } else {
            set_cur_pos(trueRacer -> row, trueRacer -> dist + 1);
            put(' ');
            trueRacer -> dist += 1;
        }
        set_cur_pos(trueRacer -> row, trueRacer -> dist + 1);
        puts(trueRacer -> graphic);
        fflush(stdout);
        pthread_mutex_unlock(&myMutex);

        if (trueRacer -> dist >= FINISH_LINE || waitMilli <= 3) {
            break;
        }
    }
    return NULL;
}