#ifndef ARVORE_H_INCLUDED
#define ARVORE_H_INCLUDED

#include "pilha.h"

typedef struct{
    int numeroFilhos[MAX_NOS];
    int informacao[MAX_NOS];
    int numNos;
    float aptidao;
}Arvore;

void inicializaArvore(Arvore* arv);//Inicializar todos os nós da arvore com um valor flag (verificar necessidade)
void geradorArvore(Arvore* arv, int maxTam);//preenche uma árvore com um numero máximo de nós 'maxTam'
void imprimeArvoreNivel(Arvore* arv);//Metodo para imprimir a árvore na estrutura de níveis
void imprimeArvorePre(Arvore* arv);//Imprime a árvore em pré ordem no modelo: 1filho= (1(0)), 2 filhos=(2(0)(0))
void imprimeArvorePos(Arvore* arv);//Imprime a árvore em pós ordem no modelo: 1filho= ((0)1), 2 filhos=((0)(0)2)
void criaCheia(Arvore* arv, int maxDepth);//dada uma profundidade maxima, preenche a arvore ate essa profundidade.
int calculaTamanhoSubArvore(Arvore* arv, int indice);//dado o indice de um nó, retorna o tamanho da subárvore a partir dele
void shift(Arvore* arv, int tam, int indice); //Desloca os elementos da árvore a partir de um 'indice' em determinado tamanho
void mutacao(Arvore* arvore); //Sorteia uma sub-arvore aleatória e troca por outra
void crossOver(Arvore* arvore1, Arvore* arvore2); //Troca informações entre duas arvores
void teste(Arvore* arvore1);//Cria uma árvore de forma conhecida para testes.
void testaPrint(Arvore* arvore); //Testa as três funções de impressão
void realizaTestes();//testa algumas funções da árvore
void executa(Arvore* individuo, float dados[]);
#endif // ARVORE_H_INCLUDED
