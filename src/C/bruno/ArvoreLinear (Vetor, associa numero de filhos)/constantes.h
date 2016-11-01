#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED

#define TIPO 4

#define VAR     4
#define CONST   3
#define FBIN    2
#define FUN     1

#define PLUS    0
#define MIN     1
#define MULT    2
#define DIV     3
#define SIN     4
#define COS     5
#define SQR     6

#define MAX_NOS     15
#define MAX_DEPTH   3
#define MAX_FILHOS  2

#define NUM_INDIV   100
#define PROB_CROSS  0.9
#define PROB_MUT    0.3

#define NUM_TORNEIO     2
#define ELITISMO        0.05
#define NUM_GERACOES    1000
#define NUM_OPBIN       4
#define NUM_OPUN        3

#define TAM_DADOS 10

#define SEED 7

//#define M 10 // numero de conjunto de teste
//#define N 5 //numero de variaveis de teste+1

int M;
int N;
char** LABELS;

#endif // CONSTANTES_H_INCLUDED
