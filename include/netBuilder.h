float* makeWeights( int inputs );
Node* makeNodes( int inputs, int nodes );
Layer* makeLayer( int inputs, int nodes );
void getDefaultTestCase( int numInputs, int numOutputs, TestCase *testCase );

#define MAX_WEIGHTS 100
#define MAX_NODES 25
#define MAX_LAYERS 10
