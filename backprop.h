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

float getOutput( float weightedSum );
void weightedSumsAndOutput( Layer *inputLayer, Layer *currentLayer );
void forwardPropagate( Layer *inputLayer, Layer *hiddenLayer, Layer *outputLayer );
void backPropagate( Layer *hiddenLayer, Layer *outputLayer, Layer *desiredLayer );
void updateWeights( Layer *inputLayer, Layer *currentLayer );
void train( TestCase *testCase, Layer *hiddenLayer, Layer *outputLayer );
