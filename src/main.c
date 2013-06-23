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

int makeLayers( int *numInputs, Layer **hiddenLayer, Layer **outputLayer ) {
    int numHidden, numOutput;
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

    return 1;
}

void persistWeights( int numInputs, Layer *currentLayer ) {
    int i, j;

    for( i = 0; i < currentLayer->numNodes; ++i ) {
        for( j = 0; j <= numInputs; ++j ) {
            fprintf( definitionFile, "%f ", currentLayer->nodes[i].weights[j] );
        }
        fprintf( definitionFile, "\n" );
    }
}

int persistAllWeights( int numInputs, Layer *hiddenLayer, Layer *outputLayer ) {
    if( fseek( definitionFile, 0, SEEK_SET ) != 0 ) {
        return 0;
    }

    fprintf( definitionFile, "InputNodes: %d\nHiddenNodes %d\nOutputNodes: %d\n",
        numInputs, hiddenLayer->numNodes, outputLayer->numNodes );

    fprintf( definitionFile, "HiddenLayer:\n" );
    persistWeights( numInputs, hiddenLayer );
    fprintf( definitionFile, "OutputLayer:\n" );
    persistWeights( hiddenLayer->numNodes, outputLayer );

    return 1;
}

int main( int argc, char *argv[] ) {
    int numInputs;
    Layer *hiddenLayer, *outputLayer;

    if( !processArguments( argc, argv ) ) {
        fprintf( stderr, "Usage: main [-r, -t] [node definition file] [input file, training file]\n" );
        return EXIT_FAILURE;
    }

    if( !makeLayers( &numInputs, &hiddenLayer, &outputLayer ) ) {
        return EXIT_FAILURE;
    }

    if( !persistAllWeights( numInputs, hiddenLayer, outputLayer ) ) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
