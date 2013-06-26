int initInput( int argc, char *argv[] );
void getDefaultTestCase( int numInputs, int numOutputs, TestCase *testCase );
int populateNextTestCase( TestCase *testCase );

extern const int NEW_INPUT;
extern const int INPUT_WRAPPED;
extern const int SEEK_FAILED;
