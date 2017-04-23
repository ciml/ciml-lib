#include<stdlib.h>
#include "representacao.h"
#include "parametros_ag.h"
#include <stdio.h>

void mutacao(individuo * p, float chance){

    int i;

    for(i=0;i<TAMANHO_INDIVIDUO;i++) {

        //gera um número entre 0 e 1
        float aleatorio = (float)rand()/RAND_MAX;

        if (aleatorio<chance) {
           p->genotipo[i] = (p->genotipo[i] +1)%2;
        }
    }
}


void crossover_um_ponto(individuo *pai1, individuo *pai2, individuo *filho1, individuo *filho2){

    int i;

    //gera número entre 0 e TAMANHO_INDIVIDUO-1
    int ponto = rand() % (TAMANHO_INDIVIDUO);

    for(i=0;i<=ponto;i++){
       filho1->genotipo[i] = pai1->genotipo[i];
       filho2->genotipo[i] = pai2->genotipo[i];
    }

     for(i=ponto;i<TAMANHO_INDIVIDUO;i++){
       filho1->genotipo[i] = pai2->genotipo[i];
       filho2->genotipo[i] = pai1->genotipo [i];
    }
}

void recombinacao(individuo *pai1, individuo*pai2, individuo*filho1, individuo* filho2, float chance){

    //gera um número entre 0 e 1
    float aleatorio = (float)rand()/RAND_MAX;

    if (aleatorio<chance) {
        crossover_um_ponto(pai1, pai2,filho1, filho2);
    }
    else{
        int j;

        for(j=0;j<TAMANHO_INDIVIDUO;j++){
           filho1->genotipo[j] = pai1->genotipo[j];
           filho2->genotipo[j] = pai2->genotipo[j];
        }

    }
}
