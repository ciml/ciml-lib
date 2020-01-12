#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED

#define TIPO    4

#ifndef SEED
#define SEED    10
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


#define MAX_NOS     256
#define MAX_DEPTH   7
#define MAX_FILHOS  2

#ifndef NUM_INDIV
#define NUM_INDIV   (32)
#endif // NUM_INDIV

#ifndef PROB_CROSS
#define PROB_CROSS  0.8
#endif // PROB_CROSS

#ifndef PROB_MUT
#define PROB_MUT    0.3
#endif // PROB_MUT

#define NUM_TORNEIO     6

#define ELITISMO        0.05

#define NUM_ELITE (ELITISMO*NUM_INDIV)

#define NUM_GERACOES    500 //nao necessriamente usado

#define CARGA 200000

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


#ifndef EVOLOCL_SEQ
#define EVOLOCL_SEQ 0
#endif // EVOLOCL_SEQ

#ifndef AVALOCL_SEQ
#define AVALOCL_SEQ 0
#endif // AVALOCL_SEQ

#ifndef AMD
#define AMD 0
#endif // AVALOCL_SEQ

#define OCL_TIME 1
#define PROFILING_TIME 1
#define PROFILING_POP 1

#define TESTA_INDIV 0

#endif // CONSTANTES_H_INCLUDED
