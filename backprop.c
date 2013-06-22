#include <math.h>
#include "backprop.h"

float getOutput( float weightedSum ) {
    return tanh( weightedSum );
}

float weightedSum( Layer *inputLayer, Layer *currentLayer ) {
/*    float returnVal = 0;
    int i;

    for( i = 0; i < inputs->elements; ++i ) {
        returnVal += inputs->a[i] * weights->a[i];
    }

    return returnVal;*/
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

