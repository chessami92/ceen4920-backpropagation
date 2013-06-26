#include <stdio.h>
#include "backprop.h"
#include "netBuilder.h"
#include "input.h"

FILE *inputFile;

int initInput( int argc, char *argv[] ) {
    if( argc < 4 ) {
        return 0;
    }

    inputFile = fopen( argv[3], "r" );

    if( !inputFile ) {
        fprintf( stderr, "ERROR: Could not open the input file.\n" );
        return 0;
    }

    return 1;
}

void getDefaultTestCase( int numInputs, int numOutputs, TestCase *testCase ) {
    Layer *inputs, *outputs;

    inputs = makeLayer( -1, numInputs );
    outputs = makeLayer( -1, numOutputs );

    testCase->inputs = inputs;
    testCase->desiredOutputs = outputs;
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
