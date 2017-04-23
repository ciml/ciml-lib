#ifndef REPRESENTACAO_H_INCLUDED
#define REPRESENTACAO_H_INCLUDED

#include "parametros_ag.h"

typedef struct{
    short genotipo[TAMANHO_INDIVIDUO];
    //char genotipo[TAMANHO_INDIVIDUO];
    float aptidao;
    int **matrizDeConfusao;
    int valido;
    int tamanho;
    int profundidade;
} individuo;

#endif // REPRESENTACAO_H_INCLUDED
