#include<stdio.h>
#include "representacao.h"
#include "parametros_ag.h"
#include "operadores_geneticos.h"
#include "ag.h"
#include <math.h>
#include <stdlib.h>

int n = 1;

int binario_para_decimal(short *binarios, int inicio, int fim){

    int i,n=1; int valorNumerico=0;

    for(i=fim-1; i>=inicio; i--, n=n<<1){

        valorNumerico += n*(binarios[i]);
    }

    return valorNumerico;
}

void binario_para_inteiro(short *binarios, short *inteiros){

    int start,n,i, j;

    int end = 0;
    for (j = 0; j < TAMANHO_INDIVIDUO; j++) {
        inteiros[j] = 0;
        start = end;
        end += TAMANHO_VALOR;
        n=1;
        for (i = start; i < end; i++, n=n<<1) {
            inteiros[j] += binarios[i] * n;
        }
    }
}

void gray_para_binario(short *gray, short*binarios){

    int i,j;

    for(i=0; i< TAMANHO_INDIVIDUO; i++){
        binarios[i] = gray[i];
    }

    int start;
    int end = 0;
    for (j = 0; j < DIMENSOES_PROBLEMA; j++) {
        start = end;
        end += TAMANHO_VALOR;
        for (i = start + 1; i < end; i++) {
            binarios[i] = binarios[i - 1] ^ binarios[i];
        }
    }
}

short genotipo_binario[TAMANHO_INDIVIDUO], fenotipo[DIMENSOES_PROBLEMA];

void obtem_fenotipo_individuo(individuo *p){

    int i, j=0;

    gray_para_binario(p->genotipo, genotipo_binario);
    //binario_para_inteiro(p->genotipo_binario, p->fenotipo);

    for(i=0; i<DIMENSOES_PROBLEMA; i++, j+=TAMANHO_VALOR){

       fenotipo[i] = binario_para_decimal(genotipo_binario, j, j+TAMANHO_VALOR);

    }
}

int funcao_de_avaliacao(individuo *p){

    obtem_fenotipo_individuo(p);

    int soma = 0;

    int i;

    for(i=0;i < DIMENSOES_PROBLEMA; i++){
        soma += (int)pow(fenotipo[i]-10, 2);
    }

    return soma * (-1);
}

void cria_populacao_inicial(individuo * pop){

    int i,j;

    for(i=0; i < TAMANHO_POPULACAO; i++){

         for(j=0; j< TAMANHO_INDIVIDUO; j++){
            pop[i].genotipo[j] = rand() % 2;
        }
       // printf("\n");
    }
}

void avalia_populacao(individuo * pop){

    int i;

    for(i=0; i < TAMANHO_POPULACAO; i++){

        //Avaliação do indivíduo
        pop[i].aptidao = funcao_de_avaliacao(&pop[i]);

    }
}

int soma_avaliacoes(individuo * pop){

    int aux = 0;
    int i;
    for(i=0;i<TAMANHO_POPULACAO;i++){
        aux += pop[i].aptidao;
    }

    return aux;
}

void torneio(int indice_participante, individuo *populacao, individuo *retorno) {

    individuo vencedor = populacao[indice_participante];
	int i, aleatorio = 0;

    for(i=0; i < TAMANHO_TORNEIO; i++) {

        aleatorio = rand() % TAMANHO_POPULACAO;

        if(populacao[aleatorio].aptidao > vencedor.aptidao){
            vencedor = populacao[aleatorio];
        }
	}

	for(i=0;i< TAMANHO_INDIVIDUO;i++){
        retorno->genotipo[i] = vencedor.genotipo[i];
	}

}

/*
    Adiciona o i-ésimo indivíduo na população
*/
void adiciona_individuo(individuo *p, individuo * pop, int indice){

    int j;
    for(j=0;j<TAMANHO_INDIVIDUO;j++){
        pop[indice].genotipo[j] = p->genotipo[j];
    }

    pop[indice].aptidao = p->aptidao;
}

/*
 Função utilizada pelo qsort para ordenar a população segundo a aptidão.
*/
int compara_individuo(const void* a, const void* b){

    individuo* p1 = (individuo*)a;
    individuo* p2 = (individuo*)b;

    return p1->aptidao < p2->aptidao;
}

/*
    Cria uma nova geração, através dos passos:
    1)Seleção;
    2)Recombinação;
    3)Mutação;

    até se obter uma população de tamanho TAMANHO_POPULACAO
*/

individuo pai1,pai2,filho1,filho2;

void cria_nova_geracao(individuo * pop, individuo * pop_copia){

     int i;

     for(i=0;i<TAMANHO_POPULACAO-1;i++) {

        //Seleção
		torneio(i,   pop, &pai1);
        torneio(i+1, pop, &pai2);

        //Recombinação
        recombinacao(&pai1, &pai2, &filho1, &filho2, TAXA_DE_RECOMBINACAO);

        //Mutação
		mutacao(&filho1, TAXA_DE_MUTACAO);
		mutacao(&filho2, TAXA_DE_MUTACAO);

        filho1.aptidao = funcao_de_avaliacao(&filho1);
        filho2.aptidao = funcao_de_avaliacao(&filho2);

        adiciona_individuo(&filho1,pop_copia, i);
        adiciona_individuo(&filho2,pop_copia, ++i);
    }

	 //Ordena a geração atual
     qsort(pop, TAMANHO_POPULACAO, sizeof(individuo), (int(*)(const void*, const void*))compara_individuo);

     //Ordena a nova geração
     qsort(pop_copia, TAMANHO_POPULACAO, sizeof(individuo), (int(*)(const void*, const void*))compara_individuo);

     //Mantém a elite e substitui o restante pelos melhores da nova geração
     int j = 0, l;
     for(i = ELITE; i < TAMANHO_POPULACAO;i++,j++){

        for(l=0;l<TAMANHO_INDIVIDUO;l++){
            pop[i].genotipo[l] = pop_copia[j].genotipo[l];
        }

     }
}


int obtem_mais_apto(individuo * pop){

    int mais_apto = pop[0].aptidao;
    int i;

    for(i=1;i<TAMANHO_POPULACAO;i++){

        if(pop[i].aptidao > mais_apto){
            mais_apto = pop[i].aptidao;
        }
    }

    return mais_apto;
}

void exibe_dados_geracao(individuo * pop){

    printf("---------------------------------");
    printf("\nGeracao %d: \n", n);

    int mais_apto = obtem_mais_apto(pop);

    printf("\nMelhor da geracao: %d: %d\n", n, mais_apto);
    printf("---------------------------------");
}


void AG(){

    individuo populacao[TAMANHO_POPULACAO];
    individuo copia_populacao[TAMANHO_POPULACAO];

    srand(3);
    cria_populacao_inicial(populacao);

    for(;;){

        avalia_populacao(populacao);
        exibe_dados_geracao(populacao);

        if(CRITERIO_DE_PARADA){
            return;
        }
        else{
            cria_nova_geracao(populacao, copia_populacao);
        }

        n++;
    }
}
