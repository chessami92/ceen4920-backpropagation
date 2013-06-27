int initPersistence( int argc, char *argv[] );
int buildLayers( Layer **hiddenLayer, Layer **outputLayer );
int persistWeights( int numInputs, Layer *hiddenLayer, Layer *outputlayer );
