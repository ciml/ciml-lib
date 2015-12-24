#include "BaseClonalg.h"
using namespace std;

#ifndef CLONALGC_H_
#define CLONALGC_H_

class Clonalg : public BaseClonalg{
public:
	Clonalg(int generations,
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

	float Search();
	virtual ~Clonalg();

protected:
	/*void InitPopulation(anticorpo ** pop);
	void Evaluate(anticorpo * individual);
	inline void EvaluatePop(anticorpo * individual);
	void Mutate(anticorpo * clone, float mutationRate);
	void CloneAndHypermutate(anticorpo * pop);
	void RandomInsertion(anticorpo * pop);
	void CalculateAffinity(anticorpo * pop);
	void Statistics(anticorpo * pop, int iterationNumber);*/

	void InitPopulation(unsigned ** pop, float **fitness, float ** fitnessNorm);
	float Evaluate(unsigned * individual);
	inline void EvaluatePop(unsigned * pop);
	void Mutate(unsigned * clone, float mutationRate);
	void CloneAndHypermutate(unsigned * pop, float * fitness, float * fitnessNorm);
	void RandomInsertion(unsigned * pop, float * fitness);
	void CalculateAffinity(unsigned * pop, float * fitness, float * fitnessNorm);
	void FindBestAndWorst();
	void Statistics(unsigned * pop, float * fitness, int iterationNumber);

	int *m_v;
};

#endif /* CLONALGC_H_ */
