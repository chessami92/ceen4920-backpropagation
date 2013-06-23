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

void forwardPropagate( Layer *inputLayer, Layer *hiddenLayer, Layer *outputLayer ) {
    weightedSumsAndOutput( inputLayer, hiddenLayer );
    weightedSumsAndOutput( hiddenLayer, outputLayer );
}

void backPropagate( Layer *hiddenLayer, Layer *outputLayer, Layer *desiredLayer ) {
    Node *currentNode;
    float *currentError;
    int i, j;

    for( i = 0; i < outputLayer->numNodes; ++i ) {
        currentNode = &outputLayer->nodes[i];
        currentNode->error = desiredLayer->nodes[i].output - currentNode->output;
    }

    for( i = 0; i < hiddenLayer->numNodes; ++i ) {
        currentError = &hiddenLayer->nodes[i].error;
        *currentError = 0;
        for( j = 0; j < outputLayer->numNodes; ++j ) {
            currentNode = &outputLayer->nodes[j];
            *currentError += currentNode->error * currentNode->weights[i];
        }
    }
}

void updateWeights( Layer *inputLayer, Layer *currentLayer ) {
    Node *currentNode;
    static float learningRate = 0.5;
    float derivative;
    int i, j;

    for( i = 0; i < currentLayer->numNodes; ++i ) {
        currentNode = &currentLayer->nodes[i];
        derivative = 1 / ( powf( cosh( currentNode->weightedSum ), 2 ) );
        for( j = 0; j < inputLayer->numNodes; ++j ) {
            currentNode->weights[j] += learningRate * currentNode->error * derivative * inputLayer->nodes[j].output;
        }
    }
}

