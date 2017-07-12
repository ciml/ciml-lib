#ifndef UTILITARIOS_H_INCLUDED
#define UTILITARIOS_H_INCLUDED
#if !(AVALOCL || EVOLOCL)
    #include "math.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
#endif // AVALOCL
#include "constantes.h"
#include "bitwise.h"


void atribuiSemente(int i);
float randomProb(int* seed);
int randomInterval(int inferior, int superior);
int randomSuperior(int max);
int randomTerminal(int N, int* seed);
int randomFunctionUn(int NUM_OPUN);
int randomFunctionBin(int NUM_OPBIN);
int randomType(int NUM_OPBIN, int NUM_OPUN, int N, int* seed);
int randomLeafType(int NUM_OPBIN, int NUM_OPUN, int N, int* seed);
int randomNodeType(int NUM_OPBIN, int NUM_OPUN, int N, int* seed);
float randomConst(int* seed, float maxDados, float minDados);
float proDiv(float num, float div);
float infDiv(float num, float div);
float proSqrt(float num);
float infSqrt(float num);
int rand2(int *seed);

void imprimeParametros(int M, int N, int NUM_CTES, int NUM_OPBIN, int NUM_OPUN);
float** readTrainingData(int* M, int* N, int* NUM_CTES, int* NUM_OPBIN, int* NUM_OPUN, char*** LABELS,
                         int** conjuntoOpTerm, float* maxDados, float* minDados, char* filename);


#endif // UTILITARIOS_H_INCLUDED
