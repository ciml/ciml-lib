#ifndef GENETICA_H_INCLUDED
#define GENETICA_H_INCLUDED
#include "arvore.h"

void inicializaCheia(Arvore pop[], int num, int indice);
void inicializaAleatorio(Arvore pop[], int num, int indice);
void inicializaPopulacao(Arvore pop[]);

void avaliaIndividuos(Arvore pop[], float* dados[]);

void imprimePopulacao(Arvore pop[]);
void imprimeMelhor(Arvore pop[]);
void ordenaElite(Arvore pop[], int k);
int selecionaElite(Arvore popAtual[], Arvore popFutura[]);
int torneio(Arvore pop[]);
int criterioDeParada(int iteracoes);



#endif // GENETICA_H_INCLUDED
