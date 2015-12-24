#ifndef CLONALGCL_H_
#define CLONALGCL_H_

#include <iostream>
#include <fstream>
//#include <vector>
#define __NO_STD_VECTOR // Use cl::vector instead of STL version
#define __CL_ENABLE_EXCEPTIONS
#include "CL/cl.h"
#include "BaseClonalg.h"

using namespace std;
using namespace cl;

class ClonalgCL: public BaseClonalg {
public:
	ClonalgCL(int generations,
				 int popsize,
				 int dimensions,
				 int bitsperdimension,
				 int bitsperparameter,
				 float mutationfactor,
				 float cloningfactor,
				 int numclones,
				 int randominsertion,
				 float upperlim,
				 float lowerlim,
				 int gpucount);

	ClonalgCL(int generations,
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

	virtual ~ClonalgCL();
	float Search();

protected:

	/*vector<Kernel> initKernel,
				   hipermutationKernel,
				   randomInsertionKernel,
				   statisticsKernel,
				   fitnessKernel,
				   calculateAffinityKernel;*/

	Kernel   *initKernel,
		     *hipermutationKernel,
		     *randomInsertionKernel,
		     *statisticsKernel,
		     *fitnessKernel,
		     *calculateAffinityKernel;

	Buffer 		*popBuffer,
				*seedBuffer,
				*statisticsBuffer,
				*parameterBuffer,
				*fitnessBuffer,
				*fitnessNormBuffer,
				*fitnessCloneBuffer;


	/*vector<Buffer> popBuffer,
				   seedBuffer,
				   statisticsBuffer,
				   parameterBuffer,
				   fitnessBuffer,
				   fitnessNormBuffer,
				   fitnessCloneBuffer;*/

	CommandQueue queue;
	CommandQueue *GPUQueues,
				 *CPUQueues;
	int GPUCount,
		CPUCount;

	t_stats *m_stats;

	void InitPopulation(unsigned ** pop, float **fitness, float ** fitnessNorm, int threadID);
	inline void EvaluatePop(unsigned * pop, float * fitness, int threadID);
	void CloneAndHypermutate(unsigned * pop, float * fitness, float * fitnessNorm, int threadID);
	void RandomInsertion(unsigned * pop, float * fitness);
	void CalculateAffinity(unsigned * pop, float * fitness, float * fitnessNorm, int threadID);
	void FindBestAndWorst(int threadID);
	void Statistics(unsigned * pop, float * fitness, int iterationNumber);
	void OpenCLInit();
	void LoadSeeds(int threadID);
	void LoadParameters(int threadID);
	double getElapsedTime(Event evt);

	void InitPopulation(unsigned ** pop, float **fitness, float ** fitnessNorm);
	float Evaluate(unsigned * individual);
	void Mutate(unsigned int * clone, float mutationRate);
	void CloneAndHypermutate(unsigned * pop, float * fitness, float * fitnessNorm);
	//void RandomInsertion(unsigned int * pop, float *fitness) ;

};

#endif /* CLONALGCL_H_ */
