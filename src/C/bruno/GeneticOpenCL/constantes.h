#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED

#define TIPO    4

#ifndef SEED
#define SEED    25
#endif // SEED

#define VAR     4
#define CTE     3
#define FBIN    2
#define FUN     1

#define PLUS    10
#define MIN     11
#define MULT    12
#define DIV     13

#define SIN     14
#define COS     15
#define SQR     16
#define EXP     17

#define LOG10   18


#define MAX_NOS     128
#define MAX_DEPTH   3
#define MAX_FILHOS  2

#ifndef NUM_INDIV
#define NUM_INDIV   (128)
#endif // NUM_INDIV

#ifndef PROB_CROSS
#define PROB_CROSS  0.9
#endif // PROB_CROSS

#ifndef PROB_MUT
#define PROB_MUT    0.3
#endif // PROB_MUT

#define NUM_TORNEIO     3

#define ELITISMO        0.05//0.002

#define NUM_GERACOES    50

#define CARGA 200000

#define MAX_TEXT_LINE_LENGTH 100000


#ifndef TWODEVICES
    #define TWODEVICES 0
#endif // TWODEVICES

#ifndef EVOLOCL
    #define EVOLOCL 0
#endif // EVOLOCL

#ifndef AVALOCL
    #define AVALOCL 0
#endif // AVALOCL

#ifndef EVOLOMP
    #define EVOLOMP 0
#endif // EVOLOMP

#ifndef AVALGPU
    #define AVALGPU 0
#endif // AVALGPU

#ifndef DIF_CONTEXT
    #define DIF_CONTEXT 0
#endif // DIF_CONTEXT




#endif // CONSTANTES_H_INCLUDED
