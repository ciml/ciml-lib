#ifndef ANTICORPO_H_
#define ANTICORPO_H_

#define BIT_SET(a,b) ((a) |= (1<<(b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1<<(b)))
#define BIT_FLIP(a,b) ((a) ^= (1<<(b)))
#define BIT_CHECK(a,b) ((a) & (1<<(b)))

#include "parametros.h"

typedef struct{
	float afinidade;
	float afinidadeNormalizada;
	unsigned representacao[TAMANHO_ANTICORPO];
} anticorpo;

typedef struct {
    int indiceMelhor,
        indicePior;
    float afinidadeMelhor,
          afinidadePior;
} t_stats;

typedef struct KernelParameters{
	int POP_SIZE;
	int NCLON;
	int CHROM_LEN;
	int REAL_LEN;
	float MUTATION_FACTOR;
	float CLONING_FACTOR;
	int BITS_PER_DIMENSION;
	int DIMENSIONS;
	float UPPER_LIM;
	float LOWER_LIM;
} KernelParameters;

#endif /* ANTICORPO_H_ */
