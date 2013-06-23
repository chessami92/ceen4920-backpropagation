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
    float layerWeights[3][4] = {{-1, -1, -1, -1}, {1, 1, 1, 1}, {-0.5, 0.5, 0.25, 0.75}};
    Node inputNodes[3] = {{NULL, -1}, {NULL, 0.5}, {NULL, 1}};
    Node outputNodes[3] = {{layerWeights[0]}, {layerWeights[1]}, {layerWeights[2]}};
    Layer inputLayer = {inputNodes, 3};
    Layer outputLayer = {outputNodes, 3};

    weightedSumsAndOutput( &inputLayer, &outputLayer );

    assert( outputLayer.nodes[0].weightedSum == -1.5 && "Output should be -1.5" );
    assert( outputLayer.nodes[1].weightedSum == 1.5 && "Output should be 1.5" );
    assert( outputLayer.nodes[2].weightedSum == 1.75 && "Output should be 1.75" );

    assert( outputLayer.nodes[0].output == getOutput( -1.5 ) );
    assert( outputLayer.nodes[1].output == getOutput( 1.5 ) );
    assert( outputLayer.nodes[2].output == getOutput( 1.75 ) );
}

static void forwardPropagate_test() {
    float hiddenWeights[2][3] = {{-1, 1, -1}, {1, -1, 1}};
    float outputWeights[2][3] = {{0.5, 0.25, 0.25}, {0.25, -0.5, 0.75}};
    Node inputNodes[2];
    Node hiddenNodes[2] = {{hiddenWeights[0]}, {hiddenWeights[1]}};
    Node outputNode[2] = {{outputWeights[0]}, {outputWeights[1]}};
    Layer inputLayer = {inputNodes, 2};
    Layer hiddenLayer = {hiddenNodes, 2};
    Layer outputLayer = {outputNode, 2};

    inputLayer.nodes[0].output = 0.5; inputLayer.nodes[1].output = -0.5;
    forwardPropagate( &inputLayer, &hiddenLayer, &outputLayer );
    assert( fabs( 0.0089929 - outputLayer.nodes[0].output ) < 0.001 && "Output is not as expected" );
    assert( fabs( 0.0269728 - outputLayer.nodes[1].output ) < 0.001 && "Output is not as expected" );
}

static void backPropagate_test( void ) {
    float hiddenWeights[2][3] = {{-1, 1, 0}, {1, -1, 0}};
    float outputWeights[2][3] = {{0.5, 0.25, 0}, {0.25, -0.5, 0}};
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

static void updateWeights_test( void ) {
    float weights[2][3] = {{0.25, 0.5, 0.25}, {-0.75, 0.25, -0.75}};
    Node inputNodes[2] = {{NULL, 1}, {NULL, -1}};
    Node currentNodes[2] = {{weights[0], 0, 2, 0.5}, {weights[1], 0, -2, 0.25}};
    Layer inputLayer = {inputNodes, 2};
    Layer currentLayer = {currentNodes, 2};

    updateWeights( &inputLayer, &currentLayer );

    assert( fabs( 0.2676627 - currentLayer.nodes[0].weights[0] ) < 0.001 && "Weight 0, 0 is not as expected" );
    assert( fabs( 0.4823373 - currentLayer.nodes[0].weights[1] ) < 0.001 && "Weight 0, 1 is not as expected" );
    assert( fabs( 0.2676627 - currentLayer.nodes[0].weights[2] ) < 0.001 && "Weight 0, 2 is not as expected" );
    assert( fabs( -0.741169 - currentLayer.nodes[1].weights[0] ) < 0.001 && "Weight 1, 0 is not as expected" );
    assert( fabs( 0.2411686 - currentLayer.nodes[1].weights[1] ) < 0.001 && "Weight 1, 1 is not as expected" );
    assert( fabs( -0.741169 - currentLayer.nodes[1].weights[2] ) < 0.001 && "Weight 1, 2 is not as expected" );
}

static void train_test( void ) {
    float hiddenWeights[2][3] = {{0, 0, 0}, {0, 0, 0}};
    float outputWeights[1][3] = {{0, 0, 0}};
    Node inputNodes[2] = {{NULL, randFloat() * 2 -1}, {NULL, randFloat() * 2 - 1}};
    Node hiddenNodes[2] = {{hiddenWeights[0]}, {hiddenWeights[1]}};
    Node outputNodes[1] = {{outputWeights[0]}};
    Node desiredOutputNodes[1] = {{NULL, randFloat() - 0.5}};
    Layer inputLayer = {inputNodes, 2};
    Layer hiddenLayer = {hiddenNodes, 2};
    Layer outputLayer = {outputNodes, 1};
    Layer desiredOutputLayer = {desiredOutputNodes, 1};
    TestCase testCase = {&inputLayer, &desiredOutputLayer};

    int i, j;
    for( i = 0; i < 1000; ++i ) {
        j = 0;
        desiredOutputNodes[0].output = randFloat() - 0.5;
        while( fabs( outputLayer.nodes[0].output - desiredOutputNodes[0].output ) > 0.01 ) {
            train( &testCase, &hiddenLayer, &outputLayer );
            if( j++ > 1000 ) {
                printf( "Converged %d sets so far.\n", i -1 );
                printf( "Desired Output: %f, Current Output: %f\n", desiredOutputNodes[0].output, outputLayer.nodes[0].output );
                assert ( 1 == 0 && "Did not converge in a timely manner" );
            }
        }
    }
}

int main( void ) {
    initRand();

    getOutput_test();
    weightedSumsAndOutput_test();
    forwardPropagate_test();
    backPropagate_test();
    updateWeights_test();
    train_test();

    return EXIT_SUCCESS;
}
