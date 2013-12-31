#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "backprop.h"
#include "netBuilder.h"
#include "persistence.h"

static char *testFile = "test_definition";

static void initPersistence_test( void ) {
    char *argv[4] = {"", "", testFile, "" };
    initPersistence( 4, argv );
}

static void build_test( void ) {
    Layer *hiddenLayer, *outputLayer;

    assert( buildLayers( &hiddenLayer, &outputLayer ) == 0 && "Cannot open nonexistent file" );
}

static void writeThenBuild_test( void ) {
    Layer *hiddenLayer, *outputLayer;
    int numInput = 4, numHidden = 3, numOutput = 2;

    hiddenLayer = makeLayer( numInput, numHidden );
    outputLayer = makeLayer( numHidden, numOutput );

    assert( hiddenLayer->nodes[0].weights[0] == 0.0 && "Weights should start out at zero" );

    persistWeights( numInput, hiddenLayer, outputLayer );
    buildLayers( &hiddenLayer, &outputLayer );

    assert( hiddenLayer->nodes[0].weights[0] != 0.0 && "Zero weights should have been overwritten" );

}

int main( void ) {
    initRand();

    build_test();
    initPersistence_test();
    writeThenBuild_test();

    remove( testFile );

    exit( EXIT_SUCCESS );
}
