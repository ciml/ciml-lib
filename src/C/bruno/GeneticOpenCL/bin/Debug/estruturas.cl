typedef struct{
    int numeroFilhos[MAX_NOS];
    int informacao[MAX_NOS];
    int numNos;
    float aptidao;
}Arvore;

typedef struct{
    int topo;
    int info[MAX_NOS];
}Pilha;

typedef struct{
    int topo;
    float info[MAX_NOS];
}PilhaEx;

void empilha(Pilha* pilha, int info){
    (pilha->topo)++;
    if(pilha->topo < MAX_NOS){
        pilha->info[pilha->topo] = info;
    }else{
        return;
    }
}

int desempilha(Pilha* pilha){
    if(pilha->topo >= 0){
        int val = pilha->info[pilha->topo];
        pilha->topo--;
        return val;
    }else{
        return -1;
    }
}

void empilhaF(PilhaEx* pilha, float info){
    (pilha->topo)++;
    if(pilha->topo < MAX_NOS){
        pilha->info[pilha->topo] = info;
    }else{
        return;
    }
}

float desempilhaF(PilhaEx* pilha){
    if(pilha->topo >= 0){
        float val = pilha->info[pilha->topo];
        pilha->topo--;
        return val;
    }else{
        return -1;
    }
}