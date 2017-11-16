// file: use_getopt.c
// use_getopt.c demonstrate use of the getopt command line processing tool.
// author: bksteele
//

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>  // processing for "-flag" command line args

/// default is not verbose
static size_t VERBOSE = 0;

/// module value for size set by a command line argument pair.
/// default size if no argument is 10.
static size_t size = 10;

/// main : [-v] [-h pos_num] -> int
int main( int argc, char * argv[] ) {
    int opt;
    int tmpsize = 0;
    while ( (opt = getopt( argc, argv, "vh:p:") ) != -1 ) {
        switch ( opt ) {
        case 'v':
            VERBOSE = 1;
            break;
        case 'p':
	    tmpsize = (int)strtol( optarg, NULL, 10 );
            printf( "%s == %d\n", "p", tmpsize );
            break;
        case 'h':
	    tmpsize = (int)strtol( optarg, NULL, 10 );
            if ( tmpsize > 0 ) {
                size = (size_t)tmpsize;
                break;
            }
            // else fall through and fail
        default:
            // some unknown, possibly unacceptable option flag
            fprintf( stderr, "usage: use_getopt [-v] [-pNUM] [-h pos_num]\n" );
            exit( EXIT_FAILURE );
        }
    }
    printf( "settings: " );
    printf( "%s == %zu; ", "VERBOSE", VERBOSE );
    printf( "%s == %zu; ", "size", size );
    printf( "\n" );
    // optind should equal index of next argument at this point.
    // if there are additional arguments, optind is index of the first one.
    printf( "remaining arguments: " );
    for ( int j = optind; j < argc; ++j ) {
        printf( "[%d] %s; ", j, argv[j] );
    }
    printf( "\n" );

    return 0;
}

