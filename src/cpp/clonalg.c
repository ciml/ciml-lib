#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "clonalg.h"
#include "utils.h"
#include "parametros.h"

void inicializaPopulacao(anticorpo * pop){

	int i,k,l;

	for(i=0; i < TAMANHO_POPULACAO; i++){

		pop[i].afinidade = 0.0f;

		for(l=0;l<TAMANHO_ANTICORPO;l++){

			unsigned val = pop[i].representacao[l];
			val = 0;

			for(k=0;k<32;k++){

				if(rand() % 2){
					BIT_SET(val, k);
				}
			}

			pop[i].representacao[l] = val;
		}

		avalia(&pop[i]);
	}
}

void imprime_individuo(anticorpo *p){

	int i,j,val;

	for(i=0;i<TAMANHO_ANTICORPO; i++){
		val = p->representacao[i];

		for(j=0;j<32;j++){
			printf("%d ", BIT_CHECK(val, j)/(int)pow(2.0,j));
		}
	}
}

/*
 * Dado um anticorpo com representação bit-a-bit,
 * obtém o valor decimal correspondente.
*/
int obtem_valor_decimal(anticorpo *p){

	int i,j,k, val;
	int soma = 0;

	for(i=0;i<TAMANHO_ANTICORPO; i++){
		val = p->representacao[i];

		for(j=0;j<32;j+=BITS_POR_PARAMETRO){

			soma=0;

			for(k=j; k<j+BITS_POR_PARAMETRO; k++){
				soma += (int)BIT_CHECK(val,j);
			}

			printf("%d ", soma);

		}
		printf("\n");
	}

	return soma;
}

/*
 Função utilizada pelo qsort para ordenar a população segundo a afinidade.
*/
int compara_individuo(const void* a, const void* b){

    anticorpo* a1 = (anticorpo*)a;
    anticorpo* a2 = (anticorpo*)b;

    return a1->afinidade < a2->afinidade;
}

void sort(anticorpo * pop){
	//qsort(pop, TAMANHO_POPULACAO, sizeof(anticorpo), (int(*)(const void*, const void*))compara_individuo);
	//double start = getRealTime();
	qsort(pop, TAMANHO_POPULACAO, sizeof(anticorpo), (int(*)(const void*, const void*))compara_individuo);
	//printf("tempo qsort:%f\n", getRealTime()-start);
}

int isSet(unsigned b, unsigned n) { return (b & ( 1 << n)) != 0; }

float funcaoObjetivoOneMax(unsigned * binarios){

	float soma = 0;
	int i,j;
	unsigned val;

	for(i=0; i < TAMANHO_ANTICORPO; i++){

		val = binarios[i];

		for(j=0; j < 32; j++){

			//soma += BIT_CHECK(val, j)/(int)pow(2.0,j);
			soma += isSet(val, j);
		}
	}
	
	return -soma;
}

float funcaoObjetivoElipsoidal(unsigned * binarios){

	float soma = 0;

	int i;

	for(i=0; i < DIMENSOES; i++){

		int comeco = i*BITS_POR_DIMENSAO;
		int fim;

		fim = comeco + BITS_POR_DIMENSAO;

		//Obtém valor inteiro
		float valorDecimal = 1; //binarioParaDecimal(binarios, comeco, fim);

		//Obtém o valor real correspondente, no intervalo definido nos parâmetros
		valorDecimal = LIMITE_INFERIOR + (LIMITE_SUPERIOR - LIMITE_INFERIOR)/((pow(2, BITS_POR_DIMENSAO)-1)) * valorDecimal;

		soma += (i+1)*(valorDecimal*valorDecimal);
	}

	return -soma;
}

void avalia(anticorpo * pop){

	//short binarios[TAMANHO_ANTICORPO];
    //grayParaBinario(pop->representacao, binarios);

	if(FUNCAO_OBJETIVO == ONE_MAX){
		pop->afinidade = funcaoObjetivoOneMax(pop->representacao);
	}
	else if(FUNCAO_OBJETIVO == ELIPSOIDAL){
		pop->afinidade = funcaoObjetivoElipsoidal(pop->representacao);
	}
	else if(FUNCAO_OBJETIVO == ROSENBROCK){
		pop->afinidade = funcaoObjetivoElipsoidal(pop->representacao);
	}
}

void avaliaPopulacao(anticorpo * pop){

	int i;

	for(i=0; i< TAMANHO_POPULACAO; i++){
		avalia(&pop[i]);
	}	
}

void ImprimePopulacao(anticorpo * pop){

	int i;

	for(i=0; i< TAMANHO_POPULACAO; i++){
		printf("Anticorpo #%d, afinidade: %f\n", i, pop[i].afinidade);
		/*for(j=0; j< TAMANHO_ANTICORPO; j++){
			printf("%d ", pop[i].representacao[j]);
		}*/
		//imprime_individuo(&pop[i]);
		printf("\n");
	}
}

void normalizaAfinidades(anticorpo * pop){

	float max = pop[0].afinidade, 
	      min = pop[TAMANHO_POPULACAO-1].afinidade;

	int i;
	for(i=0; i< TAMANHO_POPULACAO; i++){

		if(max-min == 0){
			pop[i].afinidadeNormalizada = 0;
		}
		else{
			float n = (pop[i].afinidade - min) / (max-min);
			n = (MAXIMIZACAO == 0) ? 1-n : n;
			pop[i].afinidadeNormalizada = n;
		}
	}
}

float calculaTaxaHiperMutacao(anticorpo * anticorpo){
	return exp(-FATOR_MUTACAO * anticorpo->afinidadeNormalizada);
}

int numClones(int popSize, float taxaClonagem, int indice){
	return (int)ceil( (float)(taxaClonagem * popSize) );
}

void clonaHipermuta(anticorpo * pop){

	int i,j,num_clones;
	float taxaMutacao;
	anticorpo clone, bestClone;

	for(i=0; i < TAMANHO_POPULACAO; i++){

		bestClone = pop[i];

		num_clones = NCLONES; //numClones(TAMANHO_POPULACAO, FATOR_CLONAGEM, i+1);

		taxaMutacao = calculaTaxaHiperMutacao(&pop[i]);

		//printf("Taxa de mutação: %f, afinidade: %f\n", taxaMutacao, pop[i].afinidade);

		for(j=0; j < num_clones; j++){

			clone = pop[i];

		    hiperMutacao(&clone, taxaMutacao);
			avalia(&clone);

			if(clone.afinidade > bestClone.afinidade){
				bestClone = clone;
			}
		}

		if(bestClone.afinidade > pop[i].afinidade){
			pop[i] = bestClone;
		}
	}
}

void hiperMutacao(anticorpo * clone, float taxaHiperMutacao){

	int i,j;
	unsigned val;

	for(i=0; i< TAMANHO_ANTICORPO; i++){

		val = clone->representacao[i];

		int count = 0;

		for(j=0;j<32;j++){

			float aleatorio = (float)rand()/RAND_MAX;

			if(aleatorio < taxaHiperMutacao){
				BIT_FLIP(val, j);
				count++;
			}
		}
		clone->representacao[i] = val;
	}
}

void substituicaoAleatoria(anticorpo * pop){

	int nAleatorio = NALEATORIO;
	int i,j;

	for(i=TAMANHO_POPULACAO - nAleatorio; i < TAMANHO_POPULACAO; i++){

	    for(j=0;j< TAMANHO_ANTICORPO ;j++){
			pop[i].representacao[j] = rand() % 2;
		}

		avalia(&pop[i]);
	}
}

void estatisticas(anticorpo * populacao, int geracao){
    
    int j;
    
    printf("Melhor da geração #%d: %.10f\n",geracao, populacao[0].afinidade);
	/*for(j=0; j<TAMANHO_ANTICORPO; j++){
		printf("%d ", populacao[0].representacao[j]);
	}
	printf("\nBinário:\n");

	short binario[TAMANHO_ANTICORPO];
    grayParaBinario(populacao[0].representacao, binario);

	for(j=0;j<TAMANHO_ANTICORPO;j++){
	    printf("%d ", binario[j]);
	}
	printf("\n");*/
}

void clonalg(){

	int i;
	anticorpo populacao[TAMANHO_POPULACAO];
	inicializaPopulacao(populacao);
	avaliaPopulacao(populacao);

	for(i=0; i< GERACOES; i++){

		sort(populacao);
    	estatisticas(populacao, i);
		normalizaAfinidades(populacao);		
		clonaHipermuta(populacao);		

        if(NALEATORIO > 0){
            sort(populacao);
		    substituicaoAleatoria(populacao);						
		}
	}
	//ImprimePopulacao(populacao);
}
