#ifndef BASECLONALG_H_
#define BASECLONALG_H_

#include <algorithm>
#include <cmath>
#include <iostream>
#include "representacao.h"
#include "OneMaxProblem.h"

using namespace std;

class BaseClonalg {

public:
	BaseClonalg(int generations,
			int popsize,
			int dimensions,
			int bitsperdimension,
			int bitsperparameter,
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

	//anticorpo * m_pop;
	unsigned * m_pop;
	float * m_fitness;
	float * m_fitnessNorm;
	t_stats m_statistics;

	ObjectiveFunction *m_objective;

	/*virtual void InitPopulation(anticorpo ** pop) = 0;
	virtual void Evaluate(anticorpo * individual) = 0;
	virtual void Mutate(anticorpo * clone, float mutationRate) = 0;
	virtual void CloneAndHypermutate(anticorpo * pop) = 0;
	virtual void RandomInsertion(anticorpo * pop) = 0;
	virtual void Statistics(anticorpo * pop, int iterationNumber) = 0;*/

	virtual void InitPopulation(unsigned ** pop, float **fitness, float ** fitnessNorm) = 0;
	virtual float Evaluate(unsigned * individual) = 0;
	virtual void Mutate(unsigned int * clone, float mutationRate) = 0;
	virtual void CloneAndHypermutate(unsigned * pop, float * fitness, float * fitnessNorm) = 0;
	virtual void RandomInsertion(unsigned int * pop, float *fitness) = 0;
	virtual void Statistics(unsigned * pop, float * fitness, int iterationNumber) = 0;

	/* Returns the jth word from ith individual*/
	inline unsigned int GetWord(unsigned int * pop, int id, int j){
		return 	pop[id*m_realLen + j];
	}

	float HipermutationRate(float affinity);
	void Sort(unsigned int * pop);
	//void Sort(anticorpo * pop);
	static int CompareIndividuals(const void* a, const void* b);
	void Decode(unsigned int * pop, int *v);


	void PrintPop();
};

#endif /* BASECLONALG_H_ */
