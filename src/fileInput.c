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

int populateNextTestCase( TestCase *testCase ) {
    int i, returnVal;
    char throwAway[5];

    returnVal = NEW_INPUT;

    if( fscanf( inputFile, "%s\n", throwAway ) != 1 ) {
        if( fseek( inputFile, 0, SEEK_SET ) != 0 ) {
            return SEEK_FAILED;
        }
        fscanf( inputFile, "%s\n", throwAway );
        returnVal = INPUT_WRAPPED;
    }

    for( i = 0; i < testCase->inputs->numNodes; ++i ) {
        fscanf( inputFile, " %f", &testCase->inputs->nodes[i].output );
    }

    for( i = 0; i < testCase->desiredOutputs->numNodes; ++i ) {
        fscanf( inputFile, "%f", &testCase->desiredOutputs->nodes[i].output );
    }

    return returnVal;
}
