typedef struct {
    float *weights;
    float output;
    float error;
} Node;

typedef struct {
    Node *node;
    int numNodes;
} Layer;

typedef struct {
    Layer *inputs;
    Layer *desiredOutputs;
} TestCase;

float weightedSum( Layer *inputLayer, Layer *currentLayer );
float getOutput( float weightedSum );
void train( TestCase *testCase, Layer *hiddenLayer, Layer *outputLayer );
