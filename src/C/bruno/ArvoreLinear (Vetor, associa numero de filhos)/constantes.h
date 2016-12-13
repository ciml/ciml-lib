#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED

#define TIPO    4


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


#define NUM_TESTES 3

#define MAX_NOS     30
#define MAX_DEPTH   3
#define MAX_FILHOS  2

#define NUM_INDIV   10
#define PROB_CROSS  0.9
#define PROB_MUT    0.3

#define NUM_TORNEIO     2
#define ELITISMO        0.05
#define NUM_GERACOES    10

//tem como mudar a contasntate antes


//essas variaveis passa por parametros pelas funções
int M;
int N;
char** LABELS;
int* conjuntoOpTerm;
float* ctes;

int NUM_OPBIN;
int NUM_OPUN;
int NUM_CTES;


#endif // CONSTANTES_H_INCLUDED
