#include "pilha.h"

void empilha(Pilha* pilha, int info){
    (pilha->topo)++;
    if(pilha->topo < MAX_NOS){
        pilha->info[pilha->topo] = info;
    }else{
        //printf("Erro empilhar\n");
    }
}

int desempilha(Pilha* pilha){
    if(pilha->topo >= 0){
        int val = pilha->info[pilha->topo];
        pilha->topo--;
        return val;
    }else{
        //printf("Erro desempilhar\n");
        return -1;
    }
}

void empilhaF(PilhaEx* pilha, float info){
    (pilha->topo)++;
    if(pilha->topo < MAX_NOS){
        //printf("Sucesso Empilhar 2\n");
        pilha->info[pilha->topo] = info;
    }else{
        //printf("Erro empilhar 2\n");
    }
}

float desempilhaF(PilhaEx* pilha){
    if(pilha->topo >= 0){
        //printf("Sucesso Desempilhar 2\n");
        float val = pilha->info[pilha->topo];
        pilha->topo--;
        return val;
    }else{
        //printf("Erro desempilhar 2\n");
        return -1;
    }
}
