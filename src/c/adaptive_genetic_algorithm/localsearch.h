#ifndef LOCALSEARCH_H
#define LOCALSEARCH_H

#include "geneticalgorithm.h"

ind IGindividual;

void iteratedGreedy(int countInd, int *countEvaluation);
void destructionPhase(int countInd, int nd, /*ind *IGindividual,*/ int *d);
void constructionPhase(int nd, /*ind *IGindividual, */int *d);
void LocalSearch(char *fileName, char *nRepeat, char pop[]);

#endif //LOCALSEARCH_H
