// File:         movie.h
// Author        Joe Geigel, Chris Dickens
// Description:  Module that represents a movie
//

#ifndef MOVIE_H_DEFINED
#define MOVIE_H_DEFINED

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "mtime.h"
 
/***************************************************************
 *
 * This structure represents a Movie that is played by a 
 * streaming service.
 * 
 * @author       Joe Geigel
 * @author       Rob Duncan
 * @author       Chris Dickens
 **************************************************************/
typedef struct movie_
{
     char  *director ;
     char  *title ;
     mtime *lastPlayed ;
} movie ;
	
movie *newMovie(char *t, char *d) ;

void movieDelete(movie *m) ;

// returns non-zero if director and title are the same
bool movieEquals(const movie *m, const movie *o) ;

// returns a copy of the source movie
movie *movieCopy(const movie *src) ;

// getDirector - Returns the director
char *movieGetDirector(const movie *m) ;

// getTitle - Returns the title
char *movieGetTitle(const movie *m) ;

// record the last time movie played (NULL if never)
void moviePlay(movie *m, mtime *t) ;

// get the last time the movie was played
mtime *movieGetLastPlayed(const movie *m) ;

// return a string representation of the movie
char *movieToString(const movie *) ;
#endif

