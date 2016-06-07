#ifndef BASECLONALG_H_
#define BASECLONALG_H_

#include <algorithm>
#include <cmath>
#include <iostream>
#include "ProblemFactory.h"
#include "representation.h"

using namespace std;

class BaseClonalg {

public:
	BaseClonalg(int generations,
			int popsize,
			int optimizationProblem,
			int dimensions,
			int bitsperdimension,
			float mutationfactor,
			float cloningfactor,
			int numclones,
			int randominsertion,
			float upperlim,
			float lowerlim);

	float virtual Search() = 0;

	virtual ~BaseClonalg();

protected:
	int m_generations;
	int m_popsize;
	int m_optimizationProblem;
	int m_dimensions;
	int m_bitsperdimension;
	int m_bitsperparameter;
	float m_mutationfactor;
	float m_cloningfactor;
	int m_numclones;
	int m_randominsertion;
	float m_upperlim;
	float m_lowerlim;
	int m_cromLen;
	int m_realLen;

	unsigned * m_pop;
	float * m_fitness;
	float * m_fitnessNorm;
	t_stats m_statistics;

	ObjectiveFunction *m_objective;

	/*virtual void InitPopulation(unsigned ** pop, float **affinity, float ** affinityNorm);
	virtual float Evaluate(unsigned * individual);
	virtual void Mutate(unsigned int * clone, float mutationRate);
	virtual void CloneAndHypermutate(unsigned * pop, float * affinity, float * affinityNorm);
	virtual void Statistics(unsigned * pop, float * affinity, int iterationNumber);
	*/

	/* Returns the jth word from ith individual*/
	inline unsigned int GetWord(unsigned int * pop, int id, int j){
		return 	pop[id*m_realLen + j];
	}

	float HipermutationRate(float affinity);
	void Sort(unsigned int * pop);

	static int CompareIndividuals(const void* a, const void* b);

	int BinaryToDecimal(int *binary, int begin, int end);

	void PrintPop();
};

#endif /* BASECLONALG_H_ */
