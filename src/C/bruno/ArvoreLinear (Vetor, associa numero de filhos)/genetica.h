#ifndef GENETICA_H_INCLUDED
#define GENETICA_H_INCLUDED
#include "arvore.h"

void inicializaCheia(Arvore pop[], int num, int indice, int* conjuntoOpTerm, int NUM_OPBIN, int NUM_OPUN, int N);
void inicializaAleatorio(Arvore pop[], int num, int indice, int* conjuntoOpTerm, int NUM_OPBIN, int NUM_OPUN, int N);
void inicializaPopulacao(Arvore pop[], int* conjuntoOpTerm, int NUM_OPBIN, int NUM_OPUN, int N);



void avaliaIndividuos(Arvore pop[], float* dados[], int M, int N);

void imprimePopulacao(Arvore pop[], char** LABELS);
void imprimeMelhor(Arvore pop[], char** LABELS);
void ordenaElite(Arvore pop[], int k);
int selecionaElite(Arvore popAtual[], Arvore popFutura[]);
int torneio(Arvore pop[]);
int criterioDeParada(int iteracoes);



#endif // GENETICA_H_INCLUDED
