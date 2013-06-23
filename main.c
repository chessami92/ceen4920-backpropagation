#include <stdlib.h>
#include <stdio.h>
#include "backprop.h"
#include "netBuilder.h"

char trainingFlag;
FILE *definitionFile;
FILE *inputFile;

int processArguments( int argc, char *argv[] ) {
    if( argc < 4 ) {
        return 0;
    }

    if( strcmp( argv[1], "-t" ) == 0 ) {
        trainingFlag = 1;
    } else if( strcmp( argv[1], "-r" ) == 0 ) {
        trainingFlag = 0;
    } else {
        return 0;
    }

    definitionFile = fopen( argv[2], "r+" );
    inputFile = fopen( argv[3], "r" );

    if( !definitionFile || !inputFile ) {
        return 0;
    }

    return 1;
}

int main( int argc, char *argv[] ) {
    if( !processArguments( argc, argv ) ) {
        fprintf( stderr, "Usage: main [-r, -t] [node definition file] [input file, training file]\n" );
        return EXIT_FAILURE;
    }



    return EXIT_SUCCESS;
}
