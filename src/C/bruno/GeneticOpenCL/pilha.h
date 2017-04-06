#ifndef PILHA_H_INCLUDED
#define PILHA_H_INCLUDED

#include "constantes.h"

typedef struct{
    int topo;
    int info[MAX_NOS];
}Pilha;

typedef struct{
    int topo;
    float info[MAX_NOS];
}PilhaEx;

void empilha(Pilha* pilha, int info);
int desempilha(Pilha* pilha);

void empilha2(PilhaEx* pilha, float info);
float desempilha2(PilhaEx* pilha);

#endif // PILHA_H_INCLUDED
