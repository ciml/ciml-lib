#ifndef UTILITARIOS_H_INCLUDED
#define UTILITARIOS_H_INCLUDED
#include "math.h"
#include "bitwise.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void atribuiSemente(int i);
float randomProb();
int randomInterval(int inferior, int superior);
int randomSuperior(int max);
int randomTerminal(int N);
int randomFunctionUn(int NUM_OPUN);
int randomFunctionBin(int NUM_OPBIN);
int randomType(int NUM_OPBIN, int NUM_OPUN, int N);
int randomLeafType(int NUM_OPBIN, int NUM_OPUN, int N);
int randomNodeType(int NUM_OPBIN, int NUM_OPUN, int N);
float randomConst();
float proDiv(float num, float div);
float infDiv(float num, float div);
float proSqrt(float num);
float infSqrt(float num);

float** readTrainingData(int* M, int* N, int* NUM_CTES, int* NUM_OPBIN, int* NUM_OPUN, char*** LABELS, int** conjuntoOpTerm);

//float** readTrainingData();

#endif // UTILITARIOS_H_INCLUDED
