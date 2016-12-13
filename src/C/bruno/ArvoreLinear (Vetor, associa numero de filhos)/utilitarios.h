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
int randomTerminal();

int randomFunctionUn();
int randomFunctionBin();

int randomType();
int randomLeafType();
int randomNodeType();

float randomConst();


float proDiv(float num, float div);
float infDiv(float num, float div);

float proSqrt(float num);
float infSqrt(float num);

float** readTrainingData();

//float** readTrainingData();

#endif // UTILITARIOS_H_INCLUDED
