#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "backprop.h"
#include "random.h"

static void getOutput_test( void ) {
    float output;
    int i;

    for( i = -20; i <= 20; ++i ) {
        output = getOutput( ( float ) i );
        if( i < 0 ) {
            assert( output < 0 && "Output should be negative" );
        } else if ( i > 0 ) {
            assert( output > 0 && "Output should be positive" );
        } else {
            assert( output == 0 && "Output should be zero" );
        }
    }
}

static void weightedSums_test( void ) {
    float layerWeights[3][3] = {{-1, -1, -1}, {1, 1, 1}, {-0.5, 0.5, 0.25}};
    Node inputNodes[3] = {{NULL, -1, 0}, {NULL, 0.5, 0}, {NULL, 1, 0}};
    Node outputNodes[3] = {{layerWeights[0], 0, 0},
        {layerWeights[1], 0, 0},
        {layerWeights[2], 0, 0}};
    Layer inputLayer = {inputNodes, 3};
    Layer outputLayer = {outputNodes, 3};

    weightedSums( &inputLayer, &outputLayer );

    assert( outputLayer.nodes[0].output == -0.5 && "Output should be -0.5" );
    assert( outputLayer.nodes[1].output == 0.5 && "Output should be 0.5" );
    assert( outputLayer.nodes[2].output == 1.0 && "Output should be 1.0" );
}

int main( void ) {
    getOutput_test();
    weightedSums_test();

    return EXIT_SUCCESS;
}
