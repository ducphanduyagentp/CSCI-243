// 
// File: movietest.c 
// 
// 
// @author csci243 
// 
// Version: 
// $Id$ 
// 
// Revisions: 
// $Log$ 
// 
// // // // // // // // // // // // // // // // // // // // // // // // 


#include <stdio.h>
#include <stdlib.h>

#include "movie.h"
#include "mtime.h"

///
/// main function
/// @author csci243
///
/// @param argc  number of command line arguments, including program name
/// @param argv  supplied command line arguments, including program name
/// @returns errorCode  error Code; EXIT_SUCCESS if no error
///
int main()
{
    mtime *atime = NULL ;

    movie *movie1 = NULL ;
    movie *movie2 = NULL ;
    movie *movie3 = NULL ;

    printf( "Creating a time...\n" ) ;
    atime = newMTime( 6, 30, "pm" ) ;

    printf( "Starting movie tests...\n" ) ;

    movie1 = newMovie( "Star Wars: Episode IV A New Hope", "George Lucas" ) ;
    printf( "First movie initialized...\n" ) ;

    char *director = movieGetDirector( movie1 ) ;
    char *title = movieGetTitle( movie1 ) ;
    printf( "The current movie1 movie is: %s by %s.\n", title, director) ;
    free( director ) ;
    free ( title ) ;

    char * mstr = movieToString( movie1 ) ;
    printf( "Otherwise... %s\n", mstr ) ;
	free(mstr);
    moviePlay( movie1, atime ) ;
    mstr = movieToString( movie1 ) ;
    printf( "After playing the movie... %s\n", mstr ) ;
	free(mstr);
    mtime *glp = movieGetLastPlayed( movie1 ) ;
    char *sglp = mtimeToString(glp) ;
    director = movieGetDirector( movie1 ) ;
    title = movieGetTitle( movie1 ) ;
    printf( "The same movie referencing members is: %s by %s. Last played at: %s\n", 
             title, director, sglp ) ;
    free( director ) ;
    free( title ) ;
    mtimeDelete( glp ) ;
    free( sglp ) ;

    movieDelete( movie1 ) ;

    director = malloc( sizeof( "Steven Spielberg" ) + 1 ) ;
    title = malloc( sizeof( "Jurassic Park" ) + 1 ) ;
    strcpy( director, "Steven Spielberg" ) ;
    strcpy( title, "Jurassic Park" ) ;
    movie2 = newMovie( title, director ) ;
    free( director ) ;
    free( title ) ;
    mstr = movieToString ( movie2 ) ;
    printf( "The movie2 movie is: %s\n", mstr ) ;
	free(mstr);
    free(movie1);
    movie1 = movieCopy( movie2 ) ;
    mstr = movieToString( movie1 ) ;
    printf( "The copy of movie2 movie is: %s \n", mstr ) ;
free(mstr);
    movieDelete( movie2 ) ;
    mstr = movieToString( movie1 ) ;
    printf( "After deleting the original, the copy of movie2 "
            "movie is ... %s\n", mstr ) ;
	free(mstr);
    char *pstr ;
    movie3 = movieCopy( movie1 ) ;
    if ( movieEquals( movie1, movie3 ) )
    {
        mstr = movieToString( movie1 ) ;
        pstr = movieToString( movie3 ) ;
        printf( "The movie3 movie %s\n"
                "    is the same as movie1 movie %s\n", pstr, mstr ) ;
        free( pstr ) ;
free(mstr);
    }
    else {
        printf( "ERROR: movieCopy() failure!\n" ) ;
    }

    movieDelete( movie3 ) ;
    movieDelete( movie1 ) ;
	free(movie3);
    movie3 = newMovie( "A Bug's Life", "John Lasseter" ) ;
    mstr = movieToString( movie3 ) ;
    printf( "The current movie3 movie is: %s\n", mstr ) ;

    moviePlay( movie3, newMTime(4, 42, "am") ) ;
	free(mstr);
    mstr = movieToString( movie3 ) ;
    printf( "The current movie3 movie is: %s\n", mstr ) ;
	free(mstr);
    //movieDelete( movie1 ) ;
    movieDelete( movie3 ) ;
	free(movie2);
	free(movie1);
	free(movie3);
    return EXIT_SUCCESS ;
}
