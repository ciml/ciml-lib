#include <stdio.h>
#include <stdlib.h>
#include "genetica.h"

#define MAX_TEXT_LINE_LENGTH 10000


void testaIndividuo(){

}


void leIndividuo(char *fileName, Arvore* individuo) {
    FILE* arquivo = fopen(fileName, "r");
    char cp[100000];
    int posicao = 0;
    int informacao;


    if (arquivo == NULL) {
        fprintf(stderr, "Error opening data file.");
        return;
    }
    const char delimiters[] = " ()";

    while (fgets(cp, 100000, arquivo)) {

        char *token;

        token = strtok (cp, delimiters);      /* token => "words" */
            printf("%s\n", token);

        while(token != NULL){
            printf("%s\n", token);
            if(strcmp(token, "+") == 0){
                //printf("aaa\n");
                informacao = packInt(FBIN, PLUS);
                individuo->informacao[posicao] = informacao;
                individuo->numeroFilhos[posicao] = 2;
            }
            else if(strcmp(token, "-") == 0){
                informacao = packInt(FBIN, MIN);
                individuo->informacao[posicao] = informacao;
                individuo->numeroFilhos[posicao] = 2;
            }
            else if(strcmp(token, "*") == 0){
                informacao = packInt(FBIN, MULT);
                individuo->informacao[posicao] = informacao;
                individuo->numeroFilhos[posicao] = 2;
            }
            else if(strcmp(token, "/") == 0){
                informacao = packInt(FBIN, DIV);
                individuo->informacao[posicao] = informacao;
                individuo->numeroFilhos[posicao] = 2;
            }
            else if(strcmp(token, "sen") == 0){
                informacao = packInt(FUN, SIN);
                individuo->informacao[posicao] = informacao;
                individuo->numeroFilhos[posicao] = 1;
            }
            else if(strcmp(token, "cos") == 0){
                informacao = packInt(FUN, COS);
                individuo->informacao[posicao] = informacao;
                individuo->numeroFilhos[posicao] = 1;
            }
            else if(strcmp(token, "sqrt") == 0){
                informacao = packInt(FUN, SQR);
                individuo->informacao[posicao] = informacao;
                individuo->numeroFilhos[posicao] = 1;
            }
            else if(token[0] == 'x'){
                token++;
                informacao = packInt(VAR, atoll(token)-1);
                individuo->informacao[posicao] = informacao;
                individuo->numeroFilhos[posicao] = 0;
            } else {
                informacao = packFloat(CONST, atof(token));
                individuo->informacao[posicao] = informacao;
                individuo->numeroFilhos[posicao] = 0;
            }
            posicao++;
            token = strtok (NULL, delimiters);
        }
        individuo->numNos = posicao-1;
        //int i;
       // for(i= 0; i < posicao-1; i++){
            //printf("%d ", individuo->informacao[posicao]);
       // }
        imprimeArvorePre(individuo);

//        token = strtok (NULL, delimiters);    /* token => "separated" */
//        printf("%s", token);
//        token = strtok (NULL, delimiters);    /* token => "by" */
//        printf("%s", token);
//        token = strtok (NULL, delimiters);    /* token => "spaces" */
//        printf("%s", token);
//        token = strtok (NULL, delimiters);    /* token => "and" */
//        printf("%s", token);
//        token = strtok (NULL, delimiters);    /* token => "punctuation" */
//        printf("%s", token);
//        token = strtok (NULL, delimiters);    /* token => NULL */
//        printf("%s", token);
    }

}


void imprimeParametros(){
    printf("/*-----------------------------------------------------------------\n");
    printf("* NUMERO INDIVIDUOS: %d \t CROSS-OVER: %.2f\n", NUM_INDIV, PROB_CROSS);
    printf("* NUMERO GERACOES  : %d \t MUTACAO   : %.2f\n", NUM_GERACOES, PROB_MUT);
    printf("* NUM MAXIMO NOS   : %d \t ELITISMO  : %.2f\n", MAX_NOS, ELITISMO);
    printf("* PROFUNDIDADE MAX : %d \t \n", MAX_DEPTH);
    printf("*-----------------------------------------------------------------*/\n");
}


int main(){
    int seed = 10;
    int i, indice1, indice2, novosIndividuos;
    int iteracoes = 0;

    float** dadosTreinamento = readTrainingData();

//        for(i = 0; i < M; i++){
//            for(j = 0; j < N; j++){
//                printf("%.2f ", dadosTreinamento[i][j]);
//            }
//            printf("\n");
//        }
//    Arvore arv;
//    leIndividuo("indiv.txt", &arv);
    imprimeParametros();
    atribuiSemente(seed);
    printf("Seed %d: \n", seed);
    //realizaTestes();
//    float** dadosTreinamento = readTrainingData();
//    float dadosTreinamento[M][N];
//    readTrainingData(dadosTreinamento);

    Arvore popAtual[NUM_INDIV], popFutura[NUM_INDIV];
    inicializaPopulacao(popAtual);
    avaliaIndividuos(popAtual, dadosTreinamento);

    //testaSelection(popAtual, 5);
    printf("\nPOPULACAO INICIAL \n");
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

        //imprimePopulacao(popAtual);
        imprimeMelhor(popAtual);
        iteracoes++;
    }
    printf("\nPOPULACAO FINAL \n");
    imprimePopulacao(popAtual);
    printf("*");
    imprimeMelhor(popAtual);
    printf("\n");

    return 0;
}
