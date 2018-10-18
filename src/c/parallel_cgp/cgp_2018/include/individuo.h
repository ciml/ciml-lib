#define NPOPULACAO 5
#define NLIN 1
#define NCOL 100
#define LB 50
#define PORTAS 5
#define NMUTACOES 1
#define MUTACAO 2 /// 1-Point Mutatation 2-Single Active Mutation
#define MAX_NOS 10000
#define PLATFORM 0 /// 0 GPU - 1 CPU

#define LIGANTE1 0
#define LIGANTE2 1

#define TRUE 1
#define FALSE 0

#include "operacaoTxt.h"

#ifndef CGP_REFATORADO_INDIVIDUO_H
#define CGP_REFATORADO_INDIVIDUO_H

typedef struct
{
    int lig1;
    int lig2;
    int porta;
    int *saida;
    int computado;

} Gene;

typedef struct
{
    int numeroDeSaidas;
    int numeroDeLinhasDaTabela;
    int numeroDeColunasDaTabela;
    int numeroDeEntradas;
    int nlin;
    int ncol;
    int npopulacao;
} Infos;

typedef struct
{
    int topo;
    int info[MAX_NOS];
} Pilha;

extern Infos infos;

void empilha(Pilha *pilha, int info);
void desempilha(Pilha *pilha);
int topo(Pilha *pilha);
int retornaEDesempilha(Pilha *pilha);
int retornaPosicaoDoGene(int individuo, int linha, int coluna);
void inicializaPopulacao(Gene populacao[]);
int geraNumeroAleatorio(int menorValor, int maiorValor);
void aleatorizaLBMenorColuna(int individuo, int linha, int coluna, Gene populacao[], int ligante);
void aleatorizaLigantes(int individuo, int linha, int coluna, Gene populacao[], int ligante);
void criaPrimeiroIndividuo(Gene populacao[], int portas[PORTAS]);
int geneEhSaida(int gene);
void mutaSaida(int gene, int individuo, int saidas[]);
void mutacaoSinglePoint(Gene populacao[], int individuo, int portas[PORTAS], int gene, int saidas[]);
int ativoEmCadaSaida(Gene populacao[], int individuo, int geneAtual, int posicaoSorteada);
int ehAtivo(int saidas[], Gene populacao[], int individuo, int gene);
void mutacao(Gene populacao[], int individuo, int portas[], int saidas[]);
void igualaIndividuosAoPai(Gene populacao[], int saidas[]);
int portaAND(int entrada1, int entrada2);
int portaOR(int entrada1, int entrada2);
int portaNAND(int entrada1, int entrada2);
int portaNOR(int entrada1, int entrada2);
int portaXOR(int entrada1, int entrada2);
int portaXNOR(int entrada1, int entrada2);
int portaAAndNotB(int entrada1, int entrada2);
int portaBAndNotA(int entrada1, int entrada2);
int portaNotA(int entrada1);
int portaNotB(int entrada2);
int portaAOrNotB(int entrada1, int entrada2);
int portaBOrNotA(int entrada1, int entrada2);
int calculaSaidaDoGene(int entrada1, int entrada2, int portaLogica);
void calculaPosicao(int *linha, int *coluna, int valor);
int calculaNumeroDePortasAtivas(Gene populacao[], int individuo, int saida);
void tornaPaiDaProximaGeracao(Gene populacao[], int individuoDeMaiorPontuacao, int saidas[]);
void criaVetorPosOrdemStack(Gene populacao[], int individuo, int saida, Pilha *posOrdem);
int avaliaPontuacao(Gene populacao[], int individuo, int saida, int tabelaVerdade[], int colunaDaSaida);
#endif //CGP_REFATORADO_INDIVIDUO_H