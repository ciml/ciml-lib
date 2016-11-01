#include <stdio.h>
#include <stdlib.h>
#include "genetica.h"


int main3(){
    //float dadosTreinamento[M][N];
    float** dadosTreinamento = readTrainingData();

    int i, j;
    for(i = 0; i < M; i++){
        for(j = 0; j < N; j++){
            printf("%.2f ", dadosTreinamento[i][j]);
        }
        printf("\n");
    }
     printf("\nN = %d\n\n", N);

    printf("\nM = %d\n\n", M);
    //readTrainingData(dadosTreinamento);
    testaExecuta(dadosTreinamento);
}

int main2(){
    int i,j, indice1, indice2, novosIndividuos;
    int iteracoes = 0;
    float** dadosTreinamento = readTrainingData();

    for(i = 0; i < M; i++){
        for(j = 0; j < N; j++){
            printf("%.2f ", dadosTreinamento[i][j]);
        }
        printf("\n");
    }

    Arvore arv;
    geradorArvore(&arv, 10);
    testaPrint(&arv);

    return 0;
}

int main(){
    atribuiSemente();
    //realizaTestes();
    int i,j, indice1, indice2, novosIndividuos;
    int iteracoes = 0;

    //float** dadosTreinamento = readTrainingData();

//    float dadosTreinamento[M][N];
//    readTrainingData(dadosTreinamento);

    float** dadosTreinamento = readTrainingData();

    for(i = 0; i < M; i++){
        for(j = 0; j < N; j++){
            printf("%.2f ", dadosTreinamento[i][j]);
        }
        printf("\n");
    }

    Arvore popAtual[NUM_INDIV], popFutura[NUM_INDIV];

    inicializaPopulacao(popAtual);

    avaliaIndividuos(popAtual, dadosTreinamento);

    //testaSelection(popAtual, 5);

    imprimePopulacao(popAtual);



    while(criterioDeParada(iteracoes) /*qual o criterio de parada?*/){
        //printf("GERAÇÃO %d: \n\n", iteracoes);

        novosIndividuos = 0;
        Arvore popFutura[NUM_INDIV];
        novosIndividuos = novosIndividuos + selecionaElite(popAtual, popFutura);
        //printf("novos = %d\n\n", novosIndividuos);
        while(novosIndividuos < NUM_INDIV){
            indice1 = torneio(popAtual);
            indice2 = torneio(popAtual);

            popFutura[novosIndividuos++] = popAtual[indice1];
            popFutura[novosIndividuos++] = popAtual[indice2];

            float cross = randomProb();
            float mut = randomProb();

            //printf("cross: %f\n", cross);
            //printf("mut: %f\n", mut);

            if(cross <= PROB_CROSS){
                crossOver(&popFutura[novosIndividuos-2], &popFutura[novosIndividuos-1]);
            }
            if(mut <= PROB_MUT){
                mutacao(&popFutura[novosIndividuos-2]);
                mutacao(&popFutura[novosIndividuos-1]);
            }
        }

        avaliaIndividuos(popFutura, dadosTreinamento);
        for(i = 0; i< NUM_INDIV; i++){
            popAtual[i] = popFutura[i];
        }
        //testaSelection(popAtual, 5);
        //imprimePopulacao(popAtual);
        //imprimeMelhor(popAtual);
        iteracoes++;
    }
    imprimePopulacao(popAtual);

    return 0;
}
