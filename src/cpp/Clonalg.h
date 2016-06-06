#include "BaseClonalg.h"
using namespace std;

#ifndef CLONALGC_H_
#define CLONALGC_H_

class Clonalg : public BaseClonalg{
public:
	Clonalg(int generations,
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

	float Search();
	virtual ~Clonalg();

protected:
	void InitPopulation(unsigned ** pop, float **fitness, float **fitnessNorm);
	float Evaluate(unsigned *individual);
	inline void EvaluatePop(unsigned *pop);
	void Mutate(unsigned * clone, float mutationRate);
	void CloneAndHypermutate(unsigned *pop, float * fitness, float *fitnessNorm);
	void RandomInsertion(unsigned *pop, float *fitness);
	void CalculateAffinity(unsigned *pop, float *fitness, float *fitnessNorm);
	void FindBestAndWorst();
	void Statistics(unsigned *pop, float *fitness, int iterationNumber);
	void Decode(unsigned int *pop, float *v);

private:
	unsigned *m_clone,
			 *m_bestClone;
	float *m_v;
	int *m_aux_binary;
};

#endif /* CLONALGC_H_ */
