#ifndef LOCALSEARCH_H
#define LOCALSEARCH_H

#include "geneticalgorithm.h"

ind IGindividual;
int *d;

void iteratedGreedy(int countInd, int *countEvaluation);
void destructionPhase(int countInd, int nd);
void constructionPhase(int nd);
void LocalSearch(char *fileName, char *nRepeat, char pop[]);

#endif //LOCALSEARCH_H
