#ifndef PILHA_H_INCLUDED
#define PILHA_H_INCLUDED

#include "constantes.h"

typedef struct{
    int topo;
    int info[MAX_NOS];
}Pilha;

void empilha(Pilha* pilha, int info);
int desempilha(Pilha* pilha);

#endif // PILHA_H_INCLUDED
