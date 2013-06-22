#include <math.h>
#include "backprop.h"

float getOutput( float weightedSum ) {
    return tanh( weightedSum );
}

void weightedSumsAndOutput( Layer *inputLayer, Layer *currentLayer ) {
    int i, j;
    Node *currentNode;

    for( i = 0; i < currentLayer->numNodes; ++i ) {
        currentNode = &currentLayer->nodes[i];
        currentNode->weightedSum = 0.0;
        for( j = 0; j < inputLayer->numNodes; ++j ) {
            currentNode->weightedSum += currentNode->weights[j]  * inputLayer->nodes[j].output;
        }
        currentNode->output = getOutput( currentNode->weightedSum );
    }
}

/*void updateWeights( TestCase *testCase, Vector *weights ) {
    float learningRate = 5;
    float in;
    float actualOutput;
    float error;
    float derivative;
    int i;

    in = weightedSum( testCase->inputs, weights );
    actualOutput = getOutput( in );
    error = testCase->desiredOutput - actualOutput;
    derivative = 1 / ( powf( cosh( in ), 2 ) );

    for( i = 0; i < weights->elements; ++i ) {
        weights->a[i] = weights->a[i] + learningRate * error * derivative * testCase->inputs->a[i];
    }
}*/

