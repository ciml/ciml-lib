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

/** \brief Class for executing CLONALG algorithm in OpenCL devices.
 *
 *  This class performs OpenCL initialization and executes the CLONALG algorithm in the available OpenCL devices,
 *  using the OpenMP API to create a dedicated thread for each device.
 */
class ClonalgCL: public BaseClonalg {
public:

	/// Default constructor
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

	/// Destructor
	virtual ~ClonalgCL();

	/// Performs the optimization using the OpenCL devices.
	float Search();

protected:

	Kernel *initKernel;  		     /**< Kernel used to create the initial population */
	Kernel *hipermutationKernel;     /**< Kernel used to clone and hypermutate */
	Kernel *randomInsertionKernel;   /**< Kernel used to replace the worst antibodies with randomly generated ones */
	Kernel *statisticsKernel; 	     /**< Kernel used to collect the best and worst affinity values */
	Kernel *evaluationKernel; 		     /**< Kernel used to evaluate a population */
	Kernel *normalizeAffinityKernel; /**< Kernel used to compute each antibody's normalized affinity */

	Buffer *popBuffer;  		/**< Buffer for storing the population */
	Buffer *seedBuffer; 		/**< Buffer for storing the seeds */
	Buffer *statisticsBuffer; 	/**< Buffer for storing the best and worst affinity values */
	Buffer *affinityBuffer; 	/**< Buffer used to store the affinity values */
	Buffer *affinityNormBuffer; /**< Buffer used to store the normalized affinity values */
	Buffer *parametersBuffer;   /**< Buffer used to store the values of the parameters in constant memory */

	/**  Stores how many individuals will be evolved within each sub population */
	int *m_pop_size_per_queue;

	/**  Number of work_items to be used in the evaluation kernel */
	static const int m_workGroupSize_hypermutation = 128;

	/**  Seeds created by the host at the beginning of the process */
	int host_Seeds[64*16384];

	/** Command queues for gpus */
	CommandQueue *m_gpu_queues;

	/** Command queues for cpus */
	CommandQueue *m_cpu_queues;

	/** Number of gpus being used */
	int m_gpu_count;

	/** Number of cpus being used */
	int m_cpu_count;

	/** GPU computing ratio (in respect to CPU) */
	float m_gpu_ratio;

	/** Used for printing statistics, such as the best solution found so far */
	t_stats *m_stats;

	/** Used for compiling the kernels */
	OpenCLUtils clUtils;

	/// Create the command queues, contexts, kernels and memory buffers
	void OpenCLInit();

	/// Create and copy the seeds for the Park-miller pseudo-random generators to the devices
	void LoadSeeds(int threadID);

	/// Copy the parameters to the devices
	void LoadParameters(int threadID);

	/// Calls a kernel that creates the initial population at the devices
	void InitPopulation(unsigned **pop, float **affinities, float **affinitiesNorm, int threadID);

	/// Calls a kernel that evaluates a population of antibodies
	void EvaluatePop(unsigned *pop, float *affinities, int threadID);

	/// Calls a kernel that performs the cloning and hypermutation
	void CloneAndHypermutate(unsigned * pop, float * affinities, float * affinitiesNorm, int threadID);

	/// Calls a kernel that replaces pRandom antibodies with new ones, randomly generated
	void RandomInsertion(unsigned *pop, float *affinities, int threadID);

	/// Calls a kernel that normalize the affinities in [0,1]
	void NormalizeAffinity(unsigned *pop, float *affinities, float *affinitiesNorm, int threadID);

	/// Calls a kernel that finds the best and worst affinity values
	void FindBestAndWorst(int threadID);

	/// Prints the best affinity value found so far
	void Statistics(unsigned *pop, float *affinities, int iterationNumber);
};

#endif /* CLONALGCL_H_ */
