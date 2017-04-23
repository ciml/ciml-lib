#include <stdio.h>
#include "representacao.h"
#include "parametros_ag.h"
#include "operadores_geneticos.h"
#include "ag.h"
#include <math.h>
#include <stdlib.h>

void CriaPopulacaoInicial(individuo * pop){

    int i,j;

    for(i=0; i < TAMANHO_POPULACAO; i++){

        pop[i].matrizDeConfusao = malloc(sizeof(int*)*3);
        for(j=0;j<3;j++){
            pop[i].matrizDeConfusao[j] = malloc(sizeof(int)*3);
        }

        for(j=0; j< TAMANHO_INDIVIDUO; j++){
            //pop[i].genotipo[j] = rand() % 256;
            pop[i].genotipo[j] = rand() % 2;
        }
   }
}

void Torneio(int indice_participante, individuo *populacao, individuo *retorno) {

    individuo vencedor = populacao[indice_participante];
	int i, aleatorio = 0;

    for(i=0; i < TAMANHO_TORNEIO; i++) {

        aleatorio = rand() % TAMANHO_POPULACAO;

        if(populacao[aleatorio].aptidao > vencedor.aptidao){
            vencedor = populacao[aleatorio];
        }
    }

    for(i=0; i< TAMANHO_INDIVIDUO; i++){
        retorno->genotipo[i] = vencedor.genotipo[i];
    }
}

/*
    Adiciona o i-ésimo indivíduo na população
*/
void AdicionaIndividuo(individuo *p, individuo * pop, int indice){

    int j;
    for(j=0;j<TAMANHO_INDIVIDUO;j++){
        pop[indice].genotipo[j] = p->genotipo[j];
    }

    pop[indice].aptidao = p->aptidao;
}

/*
 Função utilizada pelo qsort para ordenar a população segundo a aptidão.
*/
int ComparaIndividuo(const void* a, const void* b){

    individuo* p1 = (individuo*)a;
    individuo* p2 = (individuo*)b;

    return p1->aptidao < p2->aptidao;
}

void Sort(individuo * pop){
	qsort(pop, TAMANHO_POPULACAO, sizeof(individuo), (int(*)(const void*, const void*))ComparaIndividuo);
}

void CriaNovaPopulacao(individuo * pop, individuo * newPop){

	int i;

	for(i=0;i<TAMANHO_POPULACAO-1;i++) {

		individuo parents[2], offspring[2];

        //Seleção
		Torneio(i,   pop, &parents[0]);
		Torneio(i+1, pop, &parents[1]);

	 	//Recombinação
    	Recombinacao(&parents[0],  &parents[1], &offspring[0], &offspring[1], TAXA_DE_RECOMBINACAO);

	 	//Mutação
		Mutacao(&offspring[0], TAXA_DE_MUTACAO);
		Mutacao(&offspring[1], TAXA_DE_MUTACAO);

	  	AdicionaIndividuo(&offspring[0],newPop, i);
    	AdicionaIndividuo(&offspring[1],newPop, i+1);

		i++;
	 }
}

void SubstituiPopulacao(individuo *pop, individuo * newPop){

	Sort(pop);
	Sort(newPop);

	/* Mantém elite */
	int j = 0, l, i;
	for(i = ELITE; i < TAMANHO_POPULACAO;i++,j++){

		for(l=0;l<TAMANHO_INDIVIDUO;l++){
			pop[i].genotipo[l] = newPop[j].genotipo[l];
		}
		pop[i].aptidao = newPop[j].aptidao;

		for(l=0;l<3;l++){
            pop[i].matrizDeConfusao[l] = newPop[i].matrizDeConfusao[l];
		}

		pop[i].profundidade = newPop[i].profundidade;
		pop[i].tamanho = newPop[i].tamanho;
	 }
}
