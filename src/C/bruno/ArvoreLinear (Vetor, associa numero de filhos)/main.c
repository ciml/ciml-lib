#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"

void inicializaCheia(Arvore pop[], int num, int indice){
    int i;
    for(i = indice; i < num; i++){
        criaCheia(&pop[i], MAX_DEPTH);
    }
}

void inicializaAleatorio(Arvore pop[], int num, int indice){
    int i;
    for(i = indice; i < num; i++){
        geradorArvore(&pop[i], MAX_NOS);
    }
}

//TODO: criar metodos pra criar a pop toda cheia, toda aleatoria (passando o numero de indiv) e metade/metade
//TODO: tem alguma maneira melhor de controlar isso??
void inicializaPopulacao(Arvore pop[]){

    inicializaAleatorio(pop, NUM_INDIV/2, 0);
    inicializaCheia(pop, NUM_INDIV, NUM_INDIV/2);
//    int i;
//    for(i = 0; i < NUM_INDIV/2; i++){
//        geradorArvore(&pop[i], MAX_NOS); //sortear a quantidade de nós em determinado intervalo
//    }
//    for( ; i<NUM_INDIV; i++){
//        criaCheia(&pop[i], MAX_DEPTH); // sortear a profundidade máxima em determinado intervalo
//    }
}


//TODO: passar matriz como parametro "com os dados p/ avaliar o individuo" float
//TODO: criar operação na arvore 'executa' que recebe como parametro uma linha da matriz (vetor).
//a aprtir dessas infos, a arvore executa e retorna uma valor
void avaliaIndividuos(Arvore pop[]){
    //A DEFINIR
    int i;
    for(i = 0; i < NUM_INDIV; i++){
        //executa(&pop[i], dados[0]);
        pop[i].aptidao = pop[i].informacao[0];
    }
}

void imprimePopulacao(Arvore pop[]){
    int i;
    for(i = 0; i < NUM_INDIV; i++){
        imprimeArvorePre(&pop[i]);
        printf("| Numero de nos: %d | Aptidao = %.3f  \n",pop[i].numNos, pop[i].aptidao);
    }
    printf("\n");
}

void imprimeMelhor(Arvore pop[]){
    int i;
    int indiceMelhor = 0;
    float melhor = pop[0].aptidao;
    for(i = 1; i < NUM_INDIV; i++){
        if(pop[i].aptidao > melhor){
            indiceMelhor = i;
            melhor = pop[i].aptidao;
        }
    }
    printf("MELHOR: \n");
    imprimeArvorePre(&pop[indiceMelhor]);
    printf("| Numero de nos: %d | Aptidao = %.3f  \n\n",pop[indiceMelhor].numNos, pop[indiceMelhor].aptidao);
}

//TODO: melhorar algoritmo de 'ordenação'
void selection(Arvore pop[], int k){
    int inicio = 0, fim = NUM_INDIV-1;

    while(inicio < fim){
        int r = inicio, w = fim;
        float meio = pop[(r+w)/2].aptidao;

        while(r < w){
            if(pop[r].aptidao <= meio){
                Arvore tmp = pop[w];
                pop[w] = pop[r];
                pop[r] = tmp;
                w--;
            } else {
                r++;
            }
        }
            if(pop[r].aptidao < meio)
                r--;

            if(k<=r){
                fim = r;
            }else{
                inicio = r+1;
            }
    }
    return;
}

int selecionaElite(Arvore popAtual[], Arvore popFutura[]){
    int i, cont = (int) ELITISMO * NUM_INDIV;
    if(cont%2 != 0)
        cont++;
    selection(popAtual, cont);
    for(i = 0; i < cont; i++){
        popFutura[i] = popAtual[i];
    }
    return cont;
}

void testaSelection(Arvore pop[], int k){
    imprimePopulacao(pop);
    selection(pop, k);
    imprimePopulacao(pop);
}


int torneio(Arvore pop[]){
    int indiceMelhor = rand()%NUM_INDIV;
    int indice;
    int i;
    for(i = 0; i < NUM_TORNEIO-1; i++){
        indice = rand()%NUM_INDIV;
        if(pop[indice].aptidao > pop[indiceMelhor].aptidao)
            indiceMelhor = indice;
    }
    return indiceMelhor;
}

int criterioDeParada(int iteracoes){
    return iteracoes < NUM_GERACOES;
}

//TODO: criar 'utilitarios' para gerar numeros aleatorios
//TODO: alterar semente para constante
int main(){
    srand(7);

    int i, indice1, indice2, novosIndividuos,iteracoes = 0;
    //float dados[4][4];
    Arvore popAtual[NUM_INDIV], popFutura[NUM_INDIV];

    inicializaPopulacao(popAtual);
    avaliaIndividuos(popAtual);

    imprimePopulacao(popAtual);

    //criar operação 'criterio de parada' - done
    while(criterioDeParada(iteracoes) /*qual o criterio de parada?*/){
        printf("GERAÇÃO %d: \n\n", iteracoes);

        novosIndividuos = 0;
        Arvore popFutura[NUM_INDIV];
        novosIndividuos += selecionaElite(popAtual, popFutura);
        while(novosIndividuos < NUM_INDIV){
            indice1 = torneio(popAtual);
            indice2 = torneio(popAtual);

            popFutura[novosIndividuos++] = popAtual[indice1];
            popFutura[novosIndividuos++] = popAtual[indice2];

            float cross = (float)(rand())/(float)(RAND_MAX);
            float mut = (float)(rand())/(float)(RAND_MAX);

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
        avaliaIndividuos(popFutura);
        for(i = 0; i< NUM_INDIV; i++){
            popAtual[i] = popFutura[i];
        }

        imprimeMelhor(popAtual);

        iteracoes++;
    }
    imprimePopulacao(popAtual);
    return 0;
}
