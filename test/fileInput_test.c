#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "backprop.h"
#include "input.h"

const int NEW_INPUT = 1;
const int INPUT_WRAPPED = 0;
const int SEEK_FAILED = -1;

static char *testFile = "test_input";

static void initInput_test() {
    FILE *testInput;
    char *argv[] = {"", "", "", testFile};

    testInput = fopen( testFile, "w" );
    fprintf( testInput, "%f %f %f %f\n", 0.1, 0.2, 0.3, 0.4 );
    fprintf( testInput, "%f %f %f %f\n", 0.5, 0.6, 0.7, 0.8 );
    fclose( testInput );

    assert( initInput( 4, argv ) && "Should have successfully initialized input" );
}

static void populateNextTestCase_test() {
    TestCase testCase;

    getDefaultTestCase( 3, 1, &testCase );

    assert( populateNextTestCase( &testCase ) == NEW_INPUT && "Should have gotten new input" );
    assert( populateNextTestCase( &testCase ) == NEW_INPUT && "Should have gotten new input" );
    assert( populateNextTestCase( &testCase ) == INPUT_WRAPPED && "Should have gotten input after wrapping" );
    assert( populateNextTestCase( &testCase ) == NEW_INPUT && "Should have gotten new input" );
}

int main( void ) {
    initInput_test();
    populateNextTestCase_test();

    remove( testFile );

    exit( EXIT_SUCCESS );
}
