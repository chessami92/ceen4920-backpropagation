typedef struct {
    float *weights;
    float output;
    float weightedSum;
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

void weightedSumsAndOutput( Layer *inputLayer, Layer *currentLayer );
void forwardPropagate( Layer *inputLayer, Layer *hiddenLayer, Layer *outputLayer );
float getOutput( float weightedSum );
void train( TestCase *testCase, Layer *hiddenLayer, Layer *outputLayer );
