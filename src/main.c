#include <stdlib.h>
#include <stdio.h>
#include "backprop.h"
#include "random.h"
#include "persistence.h"
#include "input.h"

char trainingFlag;

static int processArguments( int argc, char *argv[] ) {
#ifdef NUM_ARGS
    if( argc < NUM_ARGS ) {
        return 0;
    }
#endif

#ifdef FLAG_REQUIRED
    if( strcmp( argv[1], "-t" ) == 0 ) {
        trainingFlag = 1;
    } else if( strcmp( argv[1], "-r" ) == 0 ) {
        trainingFlag = 0;
    } else {
        return 0;
    }
#endif

    if( !initPersistence( argc, argv ) || !initInput( argc, argv ) ) {
        return 0;
    }

    return 1;
}

static void printLayer( char *identifier, Layer *layer ) {
    int i;

    printf( "%s", identifier );
    for( i = 0; i < layer->numNodes; ++i ) {
        printf( "%+f, ", layer->nodes[i].output );
    }

}

static void printTestResults( Layer *inputs, Layer *outputs, Layer *desiredOutputs ) {
#ifdef PRINT_INPUTS
    printLayer( "Inputs, ", inputs );
#endif
#ifdef PRINT_OUTPUTS
    printLayer( "Outputs, ", outputs );
#endif
#ifdef PRINT_DESIRED_OUTPUTS
    printLayer( "Desired, ", desiredOutputs );
#endif
    printf( "\n" );
}

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

    numInputs = buildLayers( &hiddenLayer, &outputLayer );
    if( !numInputs ) {
        exit( EXIT_FAILURE );
    }

    getDefaultTestCase( numInputs, outputLayer->numNodes, &testCase );

    if( trainingFlag ) {
        for( i = 0; i < 1000; ++i ) {
            populateNextTestCase( &testCase );
            train( &testCase, hiddenLayer, outputLayer );
        }

        if( !persistWeights( numInputs, hiddenLayer, outputLayer ) ) {
            exit( EXIT_FAILURE );
        }
    } else {
        while( populateNextTestCase( &testCase ) == NEW_INPUT ) {
            forwardPropagate( testCase.inputs, hiddenLayer, outputLayer );
            printTestResults( testCase.inputs, outputLayer, testCase.desiredOutputs );
        }
    }

    exit( EXIT_SUCCESS );
}
