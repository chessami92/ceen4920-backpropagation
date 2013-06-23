#include <stdlib.h>
#include <stdio.h>
#include "backprop.h"
#include "netBuilder.h"

int main( int argc, char *argv[] ) {
    char trainFlag;

    if( argc < 4 ) {
        fprintf( stderr, "Usage: main [-r][-t] [node definition file] [input file]\n" );
        return EXIT_FAILURE;
    }

    if( strcmp( argv[1], "-t" ) == 0 ) {
        trainFlag = 1;
    } else if( strcmp( argv[1], "-r" ) == 0 ) {
        trainFlag = 0;
    } else {
        return EXIT_FAILURE;
    }



    return EXIT_SUCCESS;
}
