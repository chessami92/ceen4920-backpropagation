#include <stdlib.h>
#include <stdio.h>
#include "backprop.h"
#include "netBuilder.h"
#include "random.h"

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

int makeLayers( int *numInputs, Layer **hiddenLayer, Layer **outputLayer ) {
    int numHidden, numOutput;
    int i, j;
    char throwAway[20];

    if( fscanf( definitionFile, "%s %d\n%s %d\n%s %d\n", throwAway, numInputs,
                throwAway, &numHidden, throwAway, &numOutput ) != 6 ) {
        fprintf( stderr, "ERROR: Could not parse the definition file.\n" );
        return 0;
    }

    if( !( *numInputs > 0 && numHidden > 0 && numOutput > 0 ) ) {
        fprintf( stderr, "ERROR: Cannot have 0 for input, hidden nodes, or output nodes.\n" );
        return 0;
    }

    *hiddenLayer = makeLayer( *numInputs, numHidden );
    *outputLayer = makeLayer( numHidden, numOutput );

    fscanf( definitionFile, "%s\n", throwAway );
    for( i = 0; i < numHidden; ++i ) {
        for( j = 0; j <= *numInputs; ++j ) {
            fscanf( definitionFile, "%f ", &( *hiddenLayer )->nodes[i].weights[j] );
            if( ( *hiddenLayer )->nodes[i].weights[j] == 0.0 ) {
                ( *hiddenLayer )->nodes[i].weights[j] = randFloat() * 2 - 1;
            }
        }
    }

    fscanf( definitionFile, "%s\n", throwAway );
    for( i = 0; i < numOutput; ++i ) {
        for( j = 0; j <= numHidden; ++j ) {
            fscanf( definitionFile, "%f ", &( *outputLayer )->nodes[i].weights[j] );
            if( ( *outputLayer )->nodes[i].weights[j] == 0.0 ) {
                ( *outputLayer )->nodes[i].weights[j] = randFloat() / 20;
            }
        }
    }

    return 1;
}

void persistWeights( int numInputs, Layer *currentLayer ) {
    int i, j;

    for( i = 0; i < currentLayer->numNodes; ++i ) {
        for( j = 0; j <= numInputs; ++j ) {
            fprintf( definitionFile, "%+f ", currentLayer->nodes[i].weights[j] );
        }
        fprintf( definitionFile, "\n" );
    }
}

int persistAllWeights( int numInputs, Layer *hiddenLayer, Layer *outputLayer ) {
    if( fseek( definitionFile, 0, SEEK_SET ) != 0 ) {
        return 0;
    }

    fprintf( definitionFile, "InputNodes: %d\nHiddenNodes: %d\nOutputNodes: %d\n",
        numInputs, hiddenLayer->numNodes, outputLayer->numNodes );

    fprintf( definitionFile, "HiddenLayer:\n" );
    persistWeights( numInputs, hiddenLayer );
    fprintf( definitionFile, "OutputLayer:\n" );
    persistWeights( hiddenLayer->numNodes, outputLayer );

    return 1;
}

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
        return EXIT_FAILURE;
    }

    if( !makeLayers( &numInputs, &hiddenLayer, &outputLayer ) ) {
        return EXIT_FAILURE;
    }

    getDefaultTestCase( numInputs, outputLayer->numNodes, &testCase );

    if( trainingFlag ) {
        for( i = 0; i < 1000; ++i ) {
            populateNextTestCase( &testCase );
            train( &testCase, hiddenLayer, outputLayer );
        }
    } else {
        for( i = 0; i < 4; ++i ) {
            populateNextTestCase( &testCase );
            forwardPropagate( testCase.inputs, hiddenLayer, outputLayer );
            printf( "Inputs: %f, %f; Output: %f\n", testCase.inputs->nodes[0].output, testCase.inputs->nodes[1].output, outputLayer->nodes[0].output );
        }
    }

    if( !persistAllWeights( numInputs, hiddenLayer, outputLayer ) ) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
