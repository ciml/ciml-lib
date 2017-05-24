#ifndef ARVORE_H_INCLUDED
#define ARVORE_H_INCLUDED

#include "pilha.h"
#include "utilitarios.h"
#include "bitwise.h"


typedef struct{
    int numeroFilhos[MAX_NOS];
    int informacao[MAX_NOS];
    int numNos;
    float aptidao;
}Arvore;


void inicializaArvore(Arvore* arv);///Inicializar todos os nós da arvore com um valor flag (verificar necessidade)

int calculaTamanhoSubArvore(Arvore* arv, int indice);///dado o indice de um nó, retorna o tamanho da sub-árvore a partir dele

void geradorArvore(Arvore* arv, int maxTam, int* conjuntoOpTerm, int NUM_OPBIN, int NUM_OPUN, int N, int* seed);///preenche uma árvore com um numero máximo de nós 'maxTam'
void criaCheia(Arvore* arv, int maxDepth, int* conjuntoOpTerm, int NUM_OPBIN, int NUM_OPUN, int N, int* seed);///dada uma profundidade maxima, preenche a arvore ate essa profundidade

void imprimeArvoreNivel(Arvore* arv);///Metodo para imprimir a árvore na estrutura de níveis
void imprimeArvorePre(Arvore* arv, char** LABELS);///Imprime a árvore em pré ordem no modelo: 1filho= (1(0)), 2 filhos=(2(0)(0))
void imprimeArvorePos(Arvore* arv,char** LABELS);///Imprime a árvore em pós ordem no modelo: 1filho= ((0)1), 2 filhos=((0)(0)2)

void shift(Arvore* arv, int tam, int indice); ///Desloca os elementos da árvore a partir de um 'indice' em determinado tamanho
void mutacao(Arvore* arvore, int* conjuntoOpTerm, int NUM_OPBIN, int NUM_OPUN, int N, int* seed); ///Sorteia uma sub-arvore aleatória e troca por outra
void crossOver(Arvore* arvore1, Arvore* arvore2, int* seed); ///Troca informações entre duas arvores

//TODO: conferir a melhor forma de fazer
void trocaSubArv(Arvore* arvMaior,Arvore* arvMenor,int ind1,int ind2,int tamanhoSubMenor, int tamanhoSubMaior);

void imprimeSinxate(Arvore* arv, int j, char** LABELS);///
//void imprimeSinxate(int info, char** LABELS);///


float executa(Arvore* individuo, float dados[], int N);///Interpreta um programa sob determinado conjunto de dados.

float opBinaria(int operador, float valor1, float valor2);
float opUnaria(int k, float valor1);

void generate(Arvore* arv,int min, int max);


void criaArvTeste(Arvore* arvore1);//Cria uma árvore de forma conhecida para testes.
void testaPrint(Arvore* arvore); //Testa as três funções de impressão
void realizaTestes();//testa algumas funções da árvore
void testaExecuta(float* dados[]);//testa a função de executar

#endif // ARVORE_H_INCLUDED
