#ifndef CONFIGURES_INCLUDED
#define CONFIGURES_INCLUDED
#include <stdlib.h>
#include "Grammar.h"
#define NONTERMINAL -1
#define ERROR -2


#define simpleParser


class Configures{
public:
    int MAXDEEP;
    int popSize;
    int generations;
    int leastSquare = 0;
    int mono;
    double peso0;
    double peso1;
    double elitism;
    double mutationRate;
    double crossoverRate;
    int numTree;
    int NUM_THREADS = 1;
    int memorySize;
    int cicles;

    int complexity = 0; //0=high 1=terminals

    double* transportVector;

    short constant,variable,bynaryArithmeticOperators,unaryArithmeticOperators,
          binaryLogicalOperators,unaryLogicalOperators,comparisonOperators, leastSquareOperator,
          programOperators;

    Configures();

};

extern Configures* conf;

#endif // CONFIGURES_INCLUDED

