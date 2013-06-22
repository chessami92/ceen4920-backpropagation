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
        output = getOutput( i );
        if( i < 0 ) {
            assert( output < 0 && "Output should be negative" );
        } else if ( i > 0 ) {
            assert( output > 0 && "Output should be positive" );
        } else {
            assert( output == 0 && "Output should be zero" );
        }
    }
}

int main( void ) {
    getOutput_test();

    return EXIT_SUCCESS;
}
