// File:         movie.c
// Authors:      Joe Geigel, Chris Dickens
// Description:  Representation of a movie
// Revisions:
//              $Log$
//
//

#include <stdio.h>
#include <string.h>

#include "movie.h"

/*
 * Test if two movies are equal
 *
 */
bool movieEquals(const movie *m, const movie *o)
{
    // Two movies are equal if their titles and directors are equal
    if((NULL != m) && (NULL != o))
    {
        if ((0 == strcmp(m->director, o->director)) &&
            (0 == strcmp(m->title, o->title)))
        {
            return true ;
        }
    }
    return false ;
}


/*
 * Initialize a movie structure with the given Title and Director. Set
 * the time last played to NULL.
 */
movie *newMovie(char *t, char *a)
{
    movie *m = NULL ;
 
    m = malloc(sizeof(movie)) ;

    m->director = malloc(strlen(a) + 1) ;
    strcpy(m->director, a) ;

    m->title = malloc(strlen(t) + 1) ;
    strcpy(m->title, t) ;

    m->lastPlayed = NULL ;

    return m ;
}

//
// Name:        (destructor)
//
void movieDelete(movie *m)
{
    //director
	    free(m->director) ;
    //title
	    free(m->title) ;
    //time
    if(NULL != m->lastPlayed)
        mtimeDelete(m->lastPlayed) ;
}

char *movieGetDirector(const movie *m) 
{
    char *tmp = NULL ;
    tmp = malloc(strlen(m->director)+1) ;
    strcpy(tmp, m->director) ;
    return tmp ;
}

char *movieGetTitle(const movie *m)
{
    char *tmp = NULL ;
    tmp = malloc(strlen(m->title)+1) ;
    strcpy(tmp, m->title) ;
    return tmp ;
}

mtime *movieGetLastPlayed(const movie *m)
{
    mtime *tmp = NULL ;
    tmp = mtimeCopy(m->lastPlayed) ;
    return tmp ;
}

void moviePlay(movie *m, mtime *t)
{
    if (NULL != m->lastPlayed)
        free(m->lastPlayed) ;
    m->lastPlayed = t ;
}

movie *movieCopy(const movie *m)
{
    movie *d = NULL ;
    mtime *tmp = NULL ;

    d = malloc(sizeof(movie)) ;

    d->director = malloc(strlen(m->director) + 1) ;
    strcpy(d->director, m->director) ;

    d->title = malloc(strlen(m->title) + 1) ;
    strcpy(d->title, m->title) ;

    if (NULL != m->lastPlayed)
    {
        // copy the last played
        tmp = mtimeCopy(m->lastPlayed) ;
        d->lastPlayed = tmp ;
    }
    else
    {
        // set lastPlayed to NULL
        d->lastPlayed = NULL ;
    }

    return d ;
}

char *movieToString(const movie *m)
{
    char *sstr = NULL ;
    char *st = NULL ;
    char *sd = NULL ;
    char *tt = NULL ;
    int len = 0 ;

    st = movieGetTitle(m) ;
    sd = movieGetDirector(m) ;

    // calculate the total string length needed.
    len = strlen("Title: ") + strlen(st) +
                  strlen("  Director: ") + strlen(sd) + 1 ;

    if (NULL != m->lastPlayed)
    {
        tt = mtimeToString(m->lastPlayed) ;
        len += strlen(" at ") + strlen(tt) ;
    }

    // allocate enough space for the movie
    sstr = malloc( len ) ;

    sprintf(sstr, "Title: %s  Director: %s", st, sd) ;

    if (NULL != m->lastPlayed)
    {
        sstr = strcat(sstr, " at ") ;
        sstr = strcat(sstr, tt) ;
    }

    free(sd) ;
    free(st) ;
    free(tt) ;
    return sstr ;
}
