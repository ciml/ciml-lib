#ifndef REPRESENTATION_H_
#define REPRESENTATION_H_

#include "parameters.h"

#define BIT_SET(a,b) ((a) |= (1<<(b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1<<(b)))
#define BIT_FLIP(a,b) ((a) ^= (1<<(b)))
#define BIT_CHECK(a,b) ((a) & (1<<(b)))

typedef struct {
    int indexBest,
        indexWorst;
    float affinityBest,
          affinityWorst;
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

#endif /* REPRESENTATION_H_ */
