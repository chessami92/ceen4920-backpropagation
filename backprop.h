typedef struct {
    float *weights;
    float output;
    float error;
} Node;

typedef struct {
    Node *nodes;
    int numNodes;
} Layer;

typedef struct {
    Layer *inputs;
    Layer *desiredOutputs;
} TestCase;

void forwardPropagate( Layer *inputLayer, Layer *currentLayer );
float getOutput( float weightedSum );
void train( TestCase *testCase, Layer *hiddenLayer, Layer *outputLayer );
