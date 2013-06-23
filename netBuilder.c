#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "backprop.h"
#include "netBuilder.h"

float* makeWeights( int inputs ) {
    static float floatPool[MAX_WEIGHTS];
    static int floatsConsumed = 0;
    float *validPointer = &floatPool[floatsConsumed];

    floatsConsumed += inputs;
    if( floatsConsumed > MAX_WEIGHTS ) {
        fprintf( stderr, "ERROR: Max floats allowed exceeded on request for %d float(s).\n", inputs );
        floatsConsumed = 0;
        validPointer = NULL;
    }

    return validPointer;
}

Node* makeNodes( int inputs, int nodes ) {
    static Node nodePool[MAX_NODES];
    static int nodesConsumed = 0;
    Node *validPointer = &nodePool[nodesConsumed];
    int i;

    nodesConsumed += nodes;
    if( nodesConsumed > MAX_NODES ) {
        fprintf( stderr, "ERROR: Max nodes allowed exceeded on request for %d node(s).\n", nodes );
        nodesConsumed = 0;
        validPointer = NULL;
    } else {
        for( i = 0; i < nodes; ++i ) {
            validPointer[i].weights = makeWeights( inputs + 1 );
            if( !validPointer[i].weights ) {
                fprintf( stderr, "ERROR: Allocated nodes, but couldn't allocate the weights on request for %d node(s).\n", nodes );
                nodesConsumed = 0;
                validPointer = NULL;
                break;
            }
        }
    }

    return validPointer;
}

Layer* makeLayer( int inputs, int nodes ) {
    static Layer layerPool[MAX_LAYERS];
    static int layersConsumed = 0;
    Layer* validPointer = &layerPool[layersConsumed];

    ++layersConsumed;
    if( layersConsumed > MAX_LAYERS ) {
        fprintf( stderr, "ERROR: Max layers allowed exceeded on request for new layer.\n" );
        layersConsumed = 0;
        validPointer = NULL;
    } else {
        validPointer->nodes = makeNodes( inputs, nodes );
        validPointer->numNodes = nodes;
    }

    return validPointer;
}
