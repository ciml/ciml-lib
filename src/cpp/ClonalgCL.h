#ifndef CLONALGCL_H_
#define CLONALGCL_H_

#include <iostream>
#include <fstream>
#define __NO_STD_VECTOR // Use cl::vector instead of STL version
#define __CL_ENABLE_EXCEPTIONS
#include "CL/cl.h"
#include "BaseClonalg.h"
#include "OpenCLUtils.h"

using namespace std;
using namespace cl;

class ClonalgCL: public BaseClonalg {
public:
	ClonalgCL(int generations,
			 int popsize,
			 int optimizationProblem,
			 int dimensions,
			 int bitsperdimension,
			 float mutationfactor,
			 float cloningfactor,
			 int numclones,
			 int randominsertion,
			 float upperlim,
			 float lowerlim,
			 int gpucount,
			 int cpucount,
			 float gpuratio);

	virtual ~ClonalgCL();
	float Search();

protected:
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

	// Stores how many individuals will be evolved within each island
	int *m_pop_size_per_queue;

	static const int m_workGroupSize_hypermutation = 128;

	int host_Seeds[64*16384];

	CommandQueue *m_gpu_queues,
				 *m_cpu_queues;
	int m_gpu_count,
		m_cpu_count;

	// GPU computing ratio (in respect to CPU)
	float m_gpu_ratio;

	t_stats *m_stats;
	OpenCLUtils clUtils;

	void OpenCLInit();
	void InitPopulation(unsigned **pop, float **fitness, float **fitnessNorm, int threadID);
	inline void EvaluatePop(unsigned *pop, float *fitness, int threadID);
	void CloneAndHypermutate(unsigned *pop, float *fitness, float *fitnessNorm, int threadID);
	void RandomInsertion(unsigned *pop, float *fitness, int threadID);
	void CalculateAffinity(unsigned *pop, float *fitness, float *fitnessNorm, int threadID);
	void FindBestAndWorst(int threadID);
	void Statistics(unsigned *pop, float *fitness, int iterationNumber);
	void LoadSeeds(int threadID);
	void LoadParameters(int threadID);

	void InitPopulation(unsigned **pop, float **fitness, float **fitnessNorm);
	float Evaluate(unsigned *individual);
	void Mutate(unsigned int *clone, float mutationRate);
	void CloneAndHypermutate(unsigned *pop, float *fitness, float *fitnessNorm);
};

#endif /* CLONALGCL_H_ */
