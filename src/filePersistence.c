#include <stdio.h>
#include <string.h>
#include "backprop.h"
#include "netBuilder.h"
#include "persistence.h"
#include "random.h"

char *fileName;

int initPersistence( int argc, char *argv[] ) {
    if( argc < 4 ) {
        return 0;
    }

    fileName = argv[2];

    return 1;
}

static int buildLayer( FILE *definitionFile, int numInputs, Layer *currentLayer ) {
    float *weights;
    int i, j;

    for( i = 0; i < currentLayer->numNodes; ++i ) {
        weights = currentLayer->nodes[i].weights;
        for( j = 0; j <= numInputs; ++j ) {
            if( fscanf( definitionFile, "%f ", &weights[j] ) == 0 ) {
                fprintf( stderr, "ERROR: Could not parse weight definitions.\n" );
                return 0;
            }
            if( weights[j] == 0.0 ) {
                weights[j] = randFloat() * 2 - 1;
            }
        }
    }

    return 1;
}

int buildLayers( Layer **hiddenLayer, Layer **outputLayer ) {
    int numInputs, numHidden, numOutput;
    FILE *definitionFile;
    char throwAway[20];

    definitionFile = fopen( fileName, "r" );

    if( !definitionFile ) {
        fprintf( stderr, "ERROR: Could not open the definition file.\n" );
        return 0;
    }

    if( fscanf( definitionFile, "%s %d\n%s %d\n%s %d\n", throwAway, &numInputs,
    throwAway, &numHidden, throwAway, &numOutput ) != 6 ) {
        fprintf( stderr, "ERROR: Could not parse the layer definitions.\n" );
        return 0;
    }

    if( !( numInputs > 0 && numHidden > 0 && numOutput > 0 ) ) {
        fprintf( stderr, "ERROR: Cannot have 0 for input, hidden nodes, or output nodes.\n" );
        return 0;
    }

    *hiddenLayer = makeLayer( numInputs, numHidden );
    *outputLayer = makeLayer( numHidden, numOutput );

    fscanf( definitionFile, "%s\n", throwAway );
    if( !buildLayer( definitionFile, numInputs, *hiddenLayer ) ) {
        return 0;
    }
    fscanf( definitionFile, "%s\n", throwAway );
    if( !buildLayer( definitionFile, numHidden, *outputLayer ) ) {
        return 0;
    }

    fclose( definitionFile );
    return 1;
}

static void persistLayerWeights( FILE *definitionFile, int numInputs, Layer *currentLayer ) {
    int i, j;

    for( i = 0; i < currentLayer->numNodes; ++i ) {
        for( j = 0; j <= numInputs; ++j ) {
            fprintf( definitionFile, "%+f ", currentLayer->nodes[i].weights[j] );
        }
        fprintf( definitionFile, "\n" );
    }
}

int persistWeights( int numInputs, Layer *hiddenLayer, Layer *outputLayer ) {
    FILE *definitionFile;

    definitionFile = fopen( fileName, "w" );

    if( !definitionFile ) {
        fprintf( stderr, "ERROR: Could not open definition file for writing.\n" );
        return 0;
    }

    fprintf( definitionFile, "InputNodes: %d\nHiddenNodes: %d\nOutputNodes: %d\n",
        numInputs, hiddenLayer->numNodes, outputLayer->numNodes );

    fprintf( definitionFile, "HiddenLayer:\n" );
    persistLayerWeights( definitionFile, numInputs, hiddenLayer );
    fprintf( definitionFile, "OutputLayer:\n" );
    persistLayerWeights( definitionFile, hiddenLayer->numNodes, outputLayer );

    fclose( definitionFile );

    return 1;
}

