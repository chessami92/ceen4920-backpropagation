#include <stdlib.h>
#include <stdio.h>
#include "backprop.h"
#include "netBuilder.h"
#include "random.h"
#include "persistence.h"

FILE *inputFile;

void getDefaultTestCase( int numInputs, int numOutputs, TestCase *testCase ) {
    Layer *inputs, *outputs;

    inputs = makeLayer( -1, numInputs );
    outputs = makeLayer( -1, numOutputs );

    testCase->inputs = inputs;
    testCase->desiredOutputs = outputs;
}

void populateNextTestCase( TestCase *testCase ) {
    int i;
    char throwAway[5];

    if( fscanf( inputFile, "%s\n", throwAway ) != 1 ) {
        if( fseek( inputFile, 0, SEEK_SET ) != 0 ) {
            return;
        }
        fscanf( inputFile, "%s\n", throwAway );
    }

    for( i = 0; i < testCase->inputs->numNodes; ++i ) {
        fscanf( inputFile, " %f", &testCase->inputs->nodes[i].output );
    }

    for( i = 0; i < testCase->desiredOutputs->numNodes; ++i ) {
        fscanf( inputFile, "%f", &testCase->desiredOutputs->nodes[i].output );
    }
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

    if( !buildLayers( &hiddenLayer, &outputLayer ) ) {
        exit( EXIT_FAILURE );
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
