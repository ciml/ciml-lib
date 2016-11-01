#include <stdio.h>
#include <stdlib.h>
#include "pilha.h"

void empilha(Pilha* pilha, int info){
    (pilha->topo)++;
    if(pilha->topo < MAX_NOS){
        pilha->info[pilha->topo] = info;
    }else{
        printf("Erro empilhar");
    }
}

int desempilha(Pilha* pilha){
    if(pilha->topo >= 0){
        int val = pilha->info[pilha->topo];
        pilha->topo--;
        return val;
    }else{
        printf("Erro desempilhar");
        return -1;
    }
}

void empilha2(PilhaEx* pilha, float info){
    (pilha->topo)++;
    if(pilha->topo < MAX_NOS){
        pilha->info[pilha->topo] = info;
    }else{
        printf("Erro empilhar 2");
    }
}

float desempilha2(PilhaEx* pilha){
    if(pilha->topo >= 0){
        float val = pilha->info[pilha->topo];
        pilha->topo--;
        return val;
    }else{
        printf("Erro desempilhar 2");
        return -1;
    }
}
