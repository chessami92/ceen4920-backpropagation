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

static void weightedSumsAndOutput_test( void ) {
    float layerWeights[3][3] = {{-1, -1, -1}, {1, 1, 1}, {-0.5, 0.5, 0.25}};
    Node inputNodes[3] = {{NULL, -1}, {NULL, 0.5}, {NULL, 1}};
    Node outputNodes[3] = {{layerWeights[0]}, {layerWeights[1]}, {layerWeights[2]}};
    Layer inputLayer = {inputNodes, 3};
    Layer outputLayer = {outputNodes, 3};

    weightedSumsAndOutput( &inputLayer, &outputLayer );

    assert( outputLayer.nodes[0].weightedSum == -0.5 && "Output should be -0.5" );
    assert( outputLayer.nodes[1].weightedSum == 0.5 && "Output should be 0.5" );
    assert( outputLayer.nodes[2].weightedSum == 1.0 && "Output should be 1.0" );

    assert( outputLayer.nodes[0].output == getOutput( -0.5 ) );
    assert( outputLayer.nodes[1].output == getOutput( 0.5 ) );
    assert( outputLayer.nodes[2].output == getOutput( 1.0 ) );
}

static void forwardPropagate_test() {
    float hiddenWeights[2][2] = {{-1, 1}, {1, -1}};
    float outputWeights[2][2] = {{0.5, 0.25}, {0.25, -0.5}};
    Node inputNodes[2];
    Node hiddenNodes[2] = {{hiddenWeights[0]}, {hiddenWeights[1]}};
    Node outputNode[2] = {{outputWeights[0]}, {outputWeights[1]}};
    Layer inputLayer = {inputNodes, 2};
    Layer hiddenLayer = {hiddenNodes, 2};
    Layer outputLayer = {outputNode, 2};

    inputLayer.nodes[0].output = 0.5; inputLayer.nodes[1].output = -0.5;
    forwardPropagate( &inputLayer, &hiddenLayer, &outputLayer );
    assert( fabs( -0.1881307 - outputLayer.nodes[0].output ) < 0.001 && "Output is not as expected" );
    assert( fabs( -0.5162368 - outputLayer.nodes[1].output ) < 0.001 && "Output is not as expected" );
}

static void backPropagate_test( void ) {
    float hiddenWeights[2][2] = {{-1, 1}, {1, -1}};
    float outputWeights[2][2] = {{0.5, 0.25}, {0.25, -0.5}};
    Node inputNodes[2];
    Node hiddenNodes[2] = {{hiddenWeights[0]}, {hiddenWeights[1]}};
    Node outputNode[2] = {{outputWeights[0]}, {outputWeights[1]}};
    Node desired[2] = {{NULL, -0.5}, {NULL, -0.5}};
    Layer inputLayer = {inputNodes, 2};
    Layer hiddenLayer = {hiddenNodes, 2};
    Layer outputLayer = {outputNode, 2};
    Layer desiredLayer = {desired, 2};

    inputLayer.nodes[0].output = 0.5; inputLayer.nodes[1].output = -0.5;
    forwardPropagate( &inputLayer, &hiddenLayer, &outputLayer );
    backPropagate( &hiddenLayer, &outputLayer, &desiredLayer );

    assert( fabs( -0.3118693 - outputLayer.nodes[0].error ) < 0.001 && "First output node error is not as expected" );
    assert( fabs( 0.0162368 - outputLayer.nodes[1].error ) < 0.001 && "Second output node error is not as expected" );
    assert( fabs( -0.1518755 - hiddenLayer.nodes[0].error ) < 0.001 && "First node of hidden layer's output is not as expected" );
    assert( fabs( -0.0860857 - hiddenLayer.nodes[1].error ) < 0.001 && "Second node of hidden layer's output is not as expected" );
}

int main( void ) {
    getOutput_test();
    weightedSumsAndOutput_test();
    forwardPropagate_test();
    backPropagate_test();

    return EXIT_SUCCESS;
}
