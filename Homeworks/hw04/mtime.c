// mtime.c
//
// Implementation of the time functions.
//
// Adapted from Joe Geigel
//
// @author rwd - Robert Duncan

#include <stdlib.h>
#include <stdio.h>
#include "mtime.h"

/* function to create a time structure
 *
 * allocates space for a new mtime structure, copies
 * the tod string passed in
 *
 * return - pointer to the new mtime structure
 */
mtime *newMTime(int hour, int min, char *tod)
{
    char *mtod ;
    mtod = malloc(3 * sizeof(char)) ;   // tod is always two characters
    mtime *t = malloc(sizeof(mtime)) ;

    /* verify hour, minute and tod values */
    hour = hour % 12 ;
    if (0 == hour)
    {
        hour = 12 ;
    }
    min = min % 60 ;

    // verify that the tod is "am" or "pm"
    if ((0 != strcmp(tod, "am")) && (0 != strcmp(tod, "pm")))
    {
        strcpy(mtod, "xm") ;
    }
    else
    {
        strcpy(mtod, tod) ;
    }

    /* initialize structure */
    t->hour = hour ;
    t->min = min ;
    t->timeofday = malloc((strlen(mtod) + 1) * sizeof(char)) ;
    strcpy(t->timeofday, mtod) ;

    // clean up before leaving
    free(mtod) ;

    return t ;
} /* newTime() */

// time destructor
void mtimeDelete(mtime *t)
{
    free(t->timeofday) ;   // has to be done explicitly
    free(t) ;
}

/* copy a time structure*/
mtime *mtimeCopy(const mtime *src)
{
    mtime *dest = NULL ;

    dest = malloc(sizeof(mtime)) ;

    dest->hour = src->hour ;
    dest->min = src->min ;
    dest->timeofday = malloc((strlen(src->timeofday) + 1) * sizeof(char)) ;
    strcpy(dest->timeofday, src->timeofday) ;

    return dest ;
} /* time_copy() */

//
// Name:        getHour
//
int mtimeGetHour(const mtime *t)
{
  return t->hour;
}


//
// Name:        getMinute
//
int mtimeGetMinute(const mtime *t)
{
  return t->min;
}

//
// Name:        getTOD
//

char *mtimeGetTOD(const mtime *t)
{
/*    char *ret = NULL ;
    ret = malloc((strlen(t->timeofday) + 1) * sizeof(char)) ;
    strcpy(ret, t->timeofday ) ;
    return ret;

*/    return t->timeofday ;
}

/*
 *  mtimeToString - convert the contents of an mtime structure
 *                  to a string representation
 *
 *  allocates space for the string
 *
 *  returns a pointer to the string
 */
char *mtimeToString(const mtime *t)
{
    char *tstr = NULL ;

    //allocate enough space for the time as a string
    tstr = malloc((2 + 1 + 2 + 2 + 1) * sizeof(char)) ;

    // put the string together
    sprintf(tstr, "%2d:%02d%s", mtimeGetHour(t), mtimeGetMinute(t),
            t->timeofday) ;

    return tstr ;
}   /* mtimeToString() */
