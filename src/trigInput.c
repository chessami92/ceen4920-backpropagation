#include <math.h>
#include "backprop.h"
#include "input.h"

const int NEW_INPUT = 1;
const int INPUT_WRAPPED = 0;
const int SEEK_FAILED = -1;

int initInput( int argc, char *argv[] ) {
    return 1;
}

static float nextValue( void ) {
    static float time = 0;
    time += 0.01;
    return 0.5 * sin( 2 * M_PI * 10 * time );
}

static void advanceInputs( Layer *inputs, float nextValue ) {
    int i;

    for( i = inputs->numNodes - 1; i > 0; --i ) {
        inputs->nodes[i].output = inputs->nodes[i - 1].output;
    }

    inputs->nodes[0].output = nextValue;
}

static void initializeInputs( Layer *inputs ) {
    int i;

    for( i = 0; i < inputs->numNodes; ++i ) {
        advanceInputs( inputs, nextValue() );
    }
}

int populateNextTestCase( TestCase *testCase ) {
    static int firstTestCase = 1;
    static int testCaseCount = 0;
    static float lastValue;

    if( firstTestCase ) {
        initializeInputs( testCase->inputs );
        lastValue = nextValue();
        firstTestCase = 0;
    }

    advanceInputs( testCase->inputs, lastValue );
    lastValue = nextValue();
    testCase->desiredOutputs->nodes[0].output = lastValue;

    ++testCaseCount;
    if( testCaseCount < 1000 ) {
        return NEW_INPUT;
    } else {
        testCaseCount = 0;
        return INPUT_WRAPPED;
    }
}
