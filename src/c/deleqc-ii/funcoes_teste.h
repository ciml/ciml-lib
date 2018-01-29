#include "stdlib.h"

///#define EPSILON_EQUALITY_CONSTRAINTS 0.000001

void Problema_Teste(int prob, int *Restricoes, int *N_Coordenada, double *solucao, double ***bounds, int nCoordenadasNova, int nRestricoesNova, int* nGs, int* nHs);
void Avalia(double *id, int N_Coordenada, int prob, double **A, double** bounds, double epsilonEqualityConstraints);
void boundConstraints(double *id, int dimension, int nGs, int nHs, double** bounds);
void sumViolation(double *individual, int dimension, int nGs, int nHs, double** bounds, int nLHs, double* maxConstraints);
void linearConstraintsData(int prob, double** e, double** eTransposed, double* b);

