#include <stdlib.h>
#include <stdio.h>
#include "backprop.h"
#include "netBuilder.h"
#include "random.h"
#include "persistence.h"
#include "input.h"

char trainingFlag;

int main( int argc, char *argv[] ) {
    int numInputs;
    Layer *hiddenLayer, *outputLayer;
    TestCase testCase;
    int i;

    initRand();

    if( !processArguments( argc, argv ) ) {
        fprintf( stderr, "Usage: main [-r, -t] [node definition file] [input file, training file]\n" );
        exit( EXIT_FAILURE );
    }

    if( !buildLayers( &hiddenLayer, &outputLayer ) ) {
        exit( EXIT_FAILURE );
    }

    if( strcmp( argv[1], "-t" ) == 0 ) {
        trainingFlag = 1;
    } else if( strcmp( argv[1], "-r" ) == 0 ) {
        trainingFlag = 0;
    } else {
        return 0;
    }

    getDefaultTestCase( numInputs, outputLayer->numNodes, &testCase );

    if( trainingFlag ) {
        for( i = 0; i < 1000; ++i ) {
            populateNextTestCase( &testCase );
            train( &testCase, hiddenLayer, outputLayer );
        }

        if( !persistAllWeights( numInputs, hiddenLayer, outputLayer ) ) {
            exit( EXIT_FAILURE );
        }
    } else {
        for( i = 0; i < 4; ++i ) {
            populateNextTestCase( &testCase );
            forwardPropagate( testCase.inputs, hiddenLayer, outputLayer );
            printf( "Inputs: %f, %f; Output: %f\n", testCase.inputs->nodes[0].output, testCase.inputs->nodes[1].output, outputLayer->nodes[0].output );
        }
    }

    exit( EXIT_SUCCESS );
}
