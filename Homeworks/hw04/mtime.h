// time.h
//
// Header file for time functions
//
// Adapted from Joe Geigel
//
// @auther rwd - Robert Duncan
//


#ifndef TIME_H_DEFINED
#define TIME_H_DEFINED

#include <string.h>

// define a type for the time structure
typedef struct mtime_
{
    int hour;
    int min;
    char *timeofday;
} mtime ;

// function to inialize a time structure
mtime *newMTime(int hour, int min, char *tod) ;

// function to delete a time structure
void mtimeDelete(mtime *t) ;

// make a copy of a time (create space if needed)
mtime *mtimeCopy(const mtime *src);

// return this time's hour
int mtimeGetHour(const mtime *t);

// return this time's minutes
int mtimeGetMinute(const mtime *t);

// return this time's am/pm status
char *mtimeGetTOD(const mtime *t);

// return a string representation of the time
char *mtimeToString(const mtime *t) ;

#endif
