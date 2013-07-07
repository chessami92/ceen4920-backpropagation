#include <stdio.h>
#include "backprop.h"
#include "input.h"

const int NEW_INPUT = 1;
const int INPUT_WRAPPED = 0;
const int SEEK_FAILED = -1;

FILE *inputFile;

int initInput( int argc, char *argv[] ) {
    inputFile = fopen( argv[3], "r" );

    if( !inputFile ) {
        fprintf( stderr, "ERROR: Could not open the input file.\n" );
        return 0;
    }

    return 1;
}

static int populateLayer( Layer *currentLayer ) {
    int i;

    for( i = 0; i < currentLayer->numNodes; ++i ) {
        if( !fscanf( inputFile, "%f", &currentLayer->nodes[i].output ) ) {
            fprintf( stderr, "ERROR: Could not parse test case definitions.\n" );
            return 0;
        }
    }

    return 1;
}

int populateNextTestCase( TestCase *testCase ) {
    int returnVal;
    char *throwAway;

    returnVal = NEW_INPUT;

    if( fscanf( inputFile, "\nEO%c", throwAway ) ) {
        if( fseek( inputFile, 0, SEEK_SET ) != 0 ) {
            fprintf( stderr, "ERROR: Could not seek to beginning of input file.\n" );
            return SEEK_FAILED;
        }
        returnVal = INPUT_WRAPPED;
    }

    if( !populateLayer( testCase->inputs ) || !populateLayer( testCase->desiredOutputs) ) {
        return SEEK_FAILED;
    }

    return returnVal;
}
