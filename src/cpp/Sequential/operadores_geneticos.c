#include<stdlib.h>
#include "representacao.h"
#include "parametros_ag.h"

void Mutacao(individuo * p, float chance){

    int i;

    for(i=0;i<TAMANHO_INDIVIDUO;i++) {

        //gera um número entre 0 e 1
        float aleatorio = (float)rand()/RAND_MAX;

        if (aleatorio<chance) {
            //p->genotipo[i] = 1 - p->genotipo[i];
            p->genotipo[i] = rand() % 256;
        }
    }
}

void CrossoverUmPonto(individuo *pai1, individuo *pai2, individuo *filho1, individuo *filho2){

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

void CrossoverDoisPontos(individuo *pai1, individuo *pai2, individuo *filho1, individuo *filho2){

    int i;

    //gera 2 números entre 0 e TAMANHO_INDIVIDUO-1
    int ponto1 = rand() % (TAMANHO_INDIVIDUO);
    int ponto2 = rand() % (TAMANHO_INDIVIDUO);

    if(ponto1 > ponto2){
        int temp = ponto2;
        ponto2 = ponto1;
        ponto1 = temp;
    }

    for(i=0;i<ponto1;i++){
       filho1->genotipo[i] = pai1->genotipo[i];
       filho2->genotipo[i] = pai2->genotipo[i];
    }

    for(i=ponto1;i<ponto2;i++){
       filho1->genotipo[i] = pai2->genotipo[i];
       filho2->genotipo[i] = pai1->genotipo[i];
    }

    for(i=ponto2;i<TAMANHO_INDIVIDUO;i++){
       filho1->genotipo[i] = pai1->genotipo[i];
       filho2->genotipo[i] = pai2->genotipo[i];
    }
}

void Recombinacao(individuo *pai1, individuo*pai2, individuo*filho1, individuo* filho2, float chance){

    //gera um número entre 0 e 1
    float aleatorio = (float)rand()/RAND_MAX;

    if (aleatorio<chance) {
        CrossoverUmPonto(pai1, pai2,filho1, filho2);
            //CrossoverDoisPontos(pai1, pai2,filho1, filho2);
    }
    else{
        int j;

        for(j=0;j<TAMANHO_INDIVIDUO;j++){
           filho1->genotipo[j] = pai1->genotipo[j];
           filho2->genotipo[j] = pai2->genotipo[j];
        }

    }
}
