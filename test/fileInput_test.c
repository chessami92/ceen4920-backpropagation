#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "backprop.h"
#include "input.h"

static char *testFile = "test_input";

static void initInput_test() {
    FILE *testInput;
    char *argv[] = {"", "", "", testFile};

    testInput = fopen( testFile, "w" );
    fprintf( testInput, "%f %f %f %f\n", 0.0, 1.0, 2.0, 3.0);
    fprintf( testInput, "%f %f %f %f\n", 4.0, 5.0, 6.0, 7.0);
    fprintf( testInput, "EOF" );
    fclose( testInput );

    assert( initInput( 4, argv ) && "Should have successfully initialized input" );
}

static void populateNextTestCase_test() {
    TestCase testCase;
    int i;

    getDefaultTestCase( 3, 1, &testCase );

    assert( populateNextTestCase( &testCase ) == NEW_INPUT && "Should have gotten new input" );
    for( i = 0; i < testCase.inputs->numNodes; ++i ) {
        assert( ( int )( testCase.inputs->nodes[i].output ) == i );
    }
    assert( testCase.inputs->nodes[i].output == 3.0 );
    assert( populateNextTestCase( &testCase ) == NEW_INPUT && "Should have gotten new input" );
    for( i = 0; i < testCase.inputs->numNodes; ++i ) {
        assert( ( int )( testCase.inputs->nodes[i].output ) == i + 4 );
    }
    assert( testCase.inputs->nodes[i].output == 7.0 );
    assert( populateNextTestCase( &testCase ) == INPUT_WRAPPED && "Should have gotten input after wrapping" );
    for( i = 0; i < testCase.inputs->numNodes; ++i ) {
        assert( ( int )(testCase.inputs->nodes[i].output ) == i );
    }
    assert( populateNextTestCase( &testCase ) == NEW_INPUT && "Should have gotten new input" );
}

int main( void ) {
    initInput_test();
    populateNextTestCase_test();

    remove( testFile );

    exit( EXIT_SUCCESS );
}
