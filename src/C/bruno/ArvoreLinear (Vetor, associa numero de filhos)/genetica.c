#include <stdio.h>
#include <stdlib.h>
#include "genetica.h"

//TODO: criar metodos pra criar a pop toda cheia, toda aleatoria (passando o numero de indiv) e metade/metade
//TODO: tem alguma maneira melhor de controlar isso??
//criar só uma
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

//tabela com dados
//cara que carrega os dados

//fazer tres cargar no futuro: treinamento, validação e reste.
//por enquanto só treinmento e dps ir estendendo
//execução do programa deende das varoaveis dependentes
//usar diferença de quadrados somada como "erro"
//separar o cara que faz a conta (passa operador e os parametros e faz a conta
           //opbin
           //op unária
//no arquivo tem num linhas e num colunas e as infos na sequencia
//na hora de sotear os valores dos nós folhas, considerar o tamanho dos dados de treinamento

//num linhas, num colunas, VouF (1)

//TODO: passar matriz como parametro "com os dados p/ avaliar o individuo" float
//TODO: criar operação na arvore 'executa' que recebe como parametro uma linha da matriz (vetor).
//a aprtir dessas infos, a arvore executa e retorna uma valor
void avaliaIndividuos(Arvore pop[], float* dados[]){

    int i, j = 0;
    float erro = 0;
    for(i = 0; i < NUM_INDIV; i++){
        for(j = 0; j < M; j++){
            erro += executa(&pop[i], dados[j]);
        }
        pop[i].aptidao = erro; // + 2 * pop[i].numNos;
        erro = 0;
    }
}

void imprimePopulacao(Arvore pop[]){
    int i;
    for(i = 0; i < NUM_INDIV; i++){
        imprimeArvorePre(&pop[i]);
        //imprimeArvoreNivel(&pop[i]);
        printf("| Numero de nos: %d | Aptidao = %.25f  \n",pop[i].numNos, pop[i].aptidao);
    }
    printf("\n");
}

void imprimeMelhor(Arvore pop[]){
    int i;
    int indiceMelhor = 0;
    float melhor = pop[0].aptidao;
    for(i = 1; i < NUM_INDIV; i++){
        if(pop[i].aptidao < melhor){
            indiceMelhor = i;
            melhor = pop[i].aptidao;
        }
    }
    printf("MELHOR: \n");
    imprimeArvorePre(&pop[indiceMelhor]);
    printf("| Numero de nos: %d | Aptidao = %.25f  \n\n",pop[indiceMelhor].numNos, pop[indiceMelhor].aptidao);
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

void ordenaElite(Arvore pop[], int k){
    Arvore aux;
    int i, j;
    int indiceMelhor;

    for(i = 0; i < k; i++){
        indiceMelhor = i;
        for(j = i+1; j < NUM_INDIV; j++){
            if(pop[j].aptidao < pop[indiceMelhor].aptidao){
                indiceMelhor = j;
            }
        }
        aux = pop[i];
        pop[i] = pop[indiceMelhor];
        pop[indiceMelhor] = aux;
    }
}

int selecionaElite(Arvore popAtual[], Arvore popFutura[]){
    int i;
    int cont = (int)(ELITISMO*NUM_INDIV);

    if(cont%2 != 0)
        cont++;
    ordenaElite(popAtual, cont);
    for(i = 0; i < cont; i++){
        popFutura[i] = popAtual[i];
    }

    return cont;
}

void testaSelection(Arvore pop[], int k){
    //imprimePopulacao(pop);
    ordenaElite(pop, k);
    imprimePopulacao(pop);
}

int torneio(Arvore pop[]){
    int indiceMelhor = rand() % NUM_INDIV;
    int indice;
    int i;
    for(i = 0; i < NUM_TORNEIO-1; i++){
        indice = rand()%NUM_INDIV;
        if(pop[indice].aptidao < pop[indiceMelhor].aptidao)
            indiceMelhor = indice;
    }
    return indiceMelhor;
}

int criterioDeParada(int iteracoes){
    return iteracoes < NUM_GERACOES;
}
