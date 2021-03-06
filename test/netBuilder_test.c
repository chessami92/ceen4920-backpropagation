#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "backprop.h"
#include "netBuilder.h"

static void makeWeights_test( void ) {
    assert( makeWeights( 25 ) && "Should have allocated 25 floats" );
    assert( makeWeights( 25 ) && "Should have allocated 25 floats" );
    assert( makeWeights( 25 ) && "Should have allocated 25 floats" );
    assert( makeWeights( 25 ) && "Should have allocated 25 floats" );
    assert( !makeWeights( 1 ) && "Cannot allocate any more bytes" );
}

static void makeNodes_test( void ) {
    Node *nodeArray;
    int i;

    nodeArray = makeNodes( 4, 25 );
    assert( nodeArray && "Should have allocated 25 nodes" );
    for( i = 0; i < 25; ++i ) {
        assert( nodeArray[i].weights && "Should have valid weights" );
    }

    assert( !makeNodes( 1, 1 ) && "Should have failed creation" );
    assert( !makeWeights( 1 ) && "Should have failed creation" );

    nodeArray = makeNodes( 0, 5 );
    assert( nodeArray && "Should have allocated 5 nodes" );
    for( i = 0; i < 5; ++i ) {
        assert( !(nodeArray[i].weights) && "Should not have any weight array" );
    }
    assert( !makeNodes( 4, 25 ) && "Should not be able to allocate" );
}

static void makeLayer_test( void ) {
    Layer *layer;
    int i;

    layer = makeLayer( 5, 4 );
    assert( layer && "Should have made a full layer" );
    assert( layer->nodes && "Nodes should be a valid array" );
    assert( layer->numNodes == 4  && "Should have four nodes total" );

    layer = makeLayer( -1, 1 );
    assert( !layer->nodes[0].weights && "Node should not have any weights" );

    for( i = 0; i < MAX_LAYERS - 2; ++i ) {
        assert( makeLayer( -1, 1 ) && "Should have made a full layer" );
    }
    assert( !makeLayer( 1, 1 ) && "Should not be able to allocate a third layer" );
}

int main( void ) {
    makeWeights_test();
    makeNodes_test();
    makeLayer_test();

    return EXIT_SUCCESS;
}
