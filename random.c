#include <stdlib.h>
#include <time.h>

void initRand( void ) {
    srand( time( NULL ) );
}

float randFloat( void ) {
    return rand() / ( float )RAND_MAX;
}
