#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "backprop.h"
#include "netBuilder.h"
#include "input.h"

TestCase testCase;

static void populateNextTestCase_test( void ) {
    int i;
    float lastInputs[4], lastOutput;

    assert( populateNextTestCase( &testCase ) == NEW_INPUT );
    for( i = 0; i < testCase.inputs->numNodes; ++i ) {
        lastInputs[i] = testCase.inputs->nodes[i].output;
        assert( lastInputs[i] != 0.0 && "Should have had input set" );
    }
    lastOutput = testCase.desiredOutputs->nodes[0].output;
    assert( lastOutput != 0.0 && "Should have had output set" );

    assert( populateNextTestCase( &testCase ) == NEW_INPUT );
    for( i = 1; i < testCase.inputs->numNodes; ++i ) {
        assert( testCase.inputs->nodes[i].output == lastInputs[i - 1] );
    }

    for( i = 0; i < 998; ++i ) {
        if( populateNextTestCase( &testCase ) == INPUT_WRAPPED ) {
            return;
        }
    }
    assert( 0 == 1 && "Did not get input wrapped soon enough" );
}

int main( void ) {
    getDefaultTestCase( 4, 1, &testCase );
    populateNextTestCase_test();

    exit( EXIT_SUCCESS );
}
