/// racer.h - a thread controlling a small figure that races
///		across a character window
/// @author James Heliotis
/// @contributor tjb
/// @contributor bksteele

#ifndef _RACER_H
#define _RACER_H

#define MAX_CAR_LEN 12    // does not include the trailing NUL byte
#define MAX_NAME_LEN 6    // names fit 'inside' a car

#define FINISH_LINE 80 - MAX_CAR_LEN - 2
#define DEFAULT_WAIT 200  // default wait time 

/// Rcr struct represents a racer position and display graphic.
///
typedef struct Rcr {

    /// current distance from starting line of the rear of the car
    ///
    int dist;

    /// vertical position of the racer, i.e. "racing lane"
    ///
    int row;

    /// graphic: the drawable text
    ///
    char *graphic;

} Racer;

/// initRacers - Do setup work for all racers at the start of the program.
/// @param milliseconds length of pause between steps in animation 
///
void initRacers( long milliseconds );

/// makeRacer - Create a new racer.
///
/// @param name the string name to show on the display for this racer
/// @param position the row in which to race
/// @return Racer pointer a dynamically allocated Racer object
/// @pre strlen( name ) <= MAX_NAME_LEN, for display reasons.
///
Racer *makeRacer( char *name, int position );

/// destroyRacer - Destroy all dynamically allocated storage for a racer.
///
/// @param racer the object to be de-allocated
///
void destroyRacer( Racer *racer );

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
void *run( void *racer );

#endif
