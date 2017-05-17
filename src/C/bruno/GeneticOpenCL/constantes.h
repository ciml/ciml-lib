#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED

#define TIPO    4

#ifndef SEED
#define SEED    10
#endif // SEED

#define VAR     7
#define CTE     8
#define FBIN    2
#define FUN     1

#define PLUS    0
#define MIN     1
#define MULT    2
#define DIV     3

#define SIN     4
#define COS     5
#define SQR     6
#define EXP     12

#define LOG10   13

///MUDEI AQUI

#define MAX_NOS     1030
#define MAX_DEPTH   9
#define MAX_FILHOS  2

#define NUM_INDIV   500

#ifndef PROB_CROSS
#define PROB_CROSS  0.9
#endif // PROB_CROSS

#ifndef PROB_MUT
#define PROB_MUT    0.1
#endif // PROB_MUT

#define NUM_TORNEIO     4

#define ELITISMO        0.05//0.002

#define NUM_GERACOES    50


//tem como mudar a constantate antes


#endif // CONSTANTES_H_INCLUDED