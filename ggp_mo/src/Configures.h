#ifndef CONFIGURES_INCLUDED
#define CONFIGURES_INCLUDED
#include <stdlib.h>

class Configures{
public:
    int treeHigh;
    int popSize;
    int iterations;
    int leastSquare = 0;
    int mono;
    double peso0;
    double peso1;
    double elitism;

    int complexity = 0; //0=high 1=terminals

    Configures();

};

extern Configures* conf;

#endif // CONFIGURES_INCLUDED

