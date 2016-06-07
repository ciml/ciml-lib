#include "ClonalgCL.h"
#include "TimeUtils.h"
#include <cmath>
#include <omp.h>
#include <string>

using namespace std;

/**
 *  Receives the problem and hardware configurations;
 *	Creates the memory buffers;
 *  Compiles the kernels for each device;
 *  Creates the command queues.
*/
ClonalgCL::ClonalgCL(int generations,
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
				 float gpuratio)
				 : BaseClonalg(generations,
						 popsize,
						 optimizationProblem,
						 dimensions,
						 bitsperdimension,
						 mutationfactor,
						 cloningfactor,
						 numclones,
						 randominsertion,
						 upperlim,
						 lowerlim
						 )
{
	m_pop = new unsigned[m_popsize*m_realLen];
	m_fitness = new float[m_popsize];
	m_fitnessNorm = new float[m_popsize];

	m_gpu_count = gpucount;
	m_cpu_count = cpucount;
	m_gpu_ratio = gpuratio;

	OpenCLInit();
}

ClonalgCL::~ClonalgCL()
{
	delete[] m_pop;
	delete[] m_fitness;
	delete[] m_fitnessNorm;
	delete[] m_stats;

	if(m_gpu_queues != NULL)
	{
		delete[] m_gpu_queues;
	}
	if(m_cpu_queues != NULL)
	{
		delete[] m_cpu_queues;
	}
	if(m_pop_size_per_queue != NULL)
	{
		delete [] m_pop_size_per_queue;
	}

	delete[] initKernel;
	delete[] hipermutationKernel,
	delete[] randomInsertionKernel,
	delete[] statisticsKernel,
	delete[] evaluationKernel,
	delete[] normalizeAffinityKernel;

	delete[] popBuffer;
	delete[] seedBuffer;
	delete[] statisticsBuffer;
	delete[] parametersBuffer;
	delete[] affinityBuffer;
	delete[] affinityNormBuffer;
}

/**
 * At first, declares a OpenMP parallel block with (m_gpu_count + m_cpu_count) threads.
 * Then each thread loads the parameters, seeds and initializes their sub populations.
 * The optimization goes on until m_generations are executed.
 * The only synchronization among the threads occurs in FindBestAndWorst,
 *  where is necessary to find the best and worst affinity values of the entire population.
 * @return The total computing time, without overheads.
 */
float ClonalgCL::Search()
{
	try
	{
		double start = TimeUtils::getRealTime();

		omp_set_num_threads(m_gpu_count + m_cpu_count);

		#pragma omp parallel
		{
			int gen = 0;

			int threadID = omp_get_thread_num();

			LoadParameters(threadID);
			LoadSeeds(threadID);
			InitPopulation(&m_pop, &m_fitness, &m_fitnessNorm, threadID);

			EvaluatePop(m_pop, m_fitness, threadID);
			FindBestAndWorst(threadID);

			//while(m_stats[0].afinidadeMelhor < -0.00001){
			for(gen=0; gen < m_generations; gen++)
			{
				#pragma omp single
				Statistics(m_pop, m_fitness, gen);

				NormalizeAffinity(m_pop, m_fitness, m_fitnessNorm, threadID);
				CloneAndHypermutate(m_pop, m_fitness, m_fitnessNorm, threadID);

				if(m_randominsertion > 0)
				{
					RandomInsertion(m_pop, m_fitness, threadID);
				}
				FindBestAndWorst(threadID);
				//gen++;
			}

			m_gpu_queues[threadID].finish();
		}

		double elapsedTime =  TimeUtils::getRealTime()-start;

		Statistics(m_pop, m_fitness, m_generations);

		return elapsedTime;
	}
	catch(Error & error)
	{
	   cout << error.what() << "(" << error.err() << ")" << endl;
	}

	return 0;
}

/**
 * Perform the necessary initialization steps to use the OpenCL devices: \n
 * Discovers the platforms; \n
 * Discovers the gpu and cpu devices available in each platform; \n
 * Creates a context for each device; \n
 * Creates a command queue for each device; \n
 * Compiles the kernels from source for all devices; \n
 * Creates the kernels, replicated for each device; \n
 * Creates the memory buffers.
 */
void ClonalgCL::OpenCLInit()
{
	try
	{
		m_gpu_queues = NULL;
		m_cpu_queues = NULL;

		// Get available platforms
		vector<Platform> platforms;
		Platform::get(&platforms);

		//cout << platforms.size() << " plataforms available." << endl;

		cl::vector<Device> Devices, cpu_devices, gpu_devices;

		for(int i=0; i < 1; i++)
		{
			platforms[i].getDevices(CL_DEVICE_TYPE_ALL, &Devices);

			//cout << "Platform #" << i << ": " << Devices.size() << " devices." << endl;

			for(int k=0; k < (int)Devices.size(); k++)
			{
				if(Devices[k].getInfo<CL_DEVICE_TYPE>() == CL_DEVICE_TYPE_GPU)
				{
					gpu_devices.push_back(Devices[k]);
				}
				else
				{
					cpu_devices.push_back(Devices[k]);
				}
			}
		}

		/*
		cout << "GPUs:" << endl;
		cout << "-------------------------------------" << endl;
		for(int k=0; k < (int)gpu_devices.size(); k++){
			ShowDeviceInfo(gpu_devices[k]);
			cout << "-------------------------------------" << endl;
		}

		cout << "CPUs:" << endl;
		cout << "-------------------------------------" << endl;
		for(int k=0; k < (int)cpu_devices.size(); k++){
			ShowDeviceInfo(cpu_devices[k]);
			cout << "-------------------------------------" << endl;
		}
		*/

		m_gpu_count = std::min(m_gpu_count, (int)gpu_devices.size());
		m_cpu_count = std::min(m_cpu_count, (int)cpu_devices.size());

		vector<Context> contexts;

		for(int k=0; k < m_gpu_count; k++)
		{
			contexts.push_back(Context(gpu_devices));
		}

		for(int k=0; k < m_cpu_count; k++)
		{
			contexts.push_back(Context(cpu_devices));
		}

		vector<Program> programs;

		//cout <<"Using " <<  m_gpu_count << " gpus and " << m_cpu_count << " cpus." << endl;

		std::string compilerOptions = "-I ./ -D " + m_objective->getFormula() + " -D " + m_objective->getSumLimit() + " -D " + m_objective->getName();

		for(int k=0; k < m_gpu_count; k++)
		{
			programs.push_back(clUtils.CreateProgramFromSource(contexts[k], gpu_devices, compilerOptions, "./src/kernels/kernel.cpp"));
		}

		for(int k=0; k < m_cpu_count; k++)
		{
			programs.push_back(clUtils.CreateProgramFromSource(contexts[m_gpu_count+k], cpu_devices, compilerOptions, "./src/kernels/kernel.cpp"));
		}

		m_gpu_queues = new CommandQueue[m_gpu_count+m_cpu_count];

		int total_devices = m_gpu_count + m_cpu_count;

		if(m_cpu_count == 0)
		{
			m_gpu_ratio = 1.0f;
		}
		if(m_gpu_count == 0)
		{
			m_gpu_ratio = 0.0f;
		}

		m_pop_size_per_queue = new int[total_devices];

		m_stats = new t_stats[total_devices];

		popBuffer = new Buffer[total_devices];
	    seedBuffer = new Buffer[total_devices];
	    statisticsBuffer = new Buffer[total_devices];
	    parametersBuffer = new Buffer[total_devices];
	    affinityBuffer = new Buffer[total_devices] ;
	    affinityNormBuffer = new Buffer[total_devices];

	    initKernel = new Kernel[total_devices];
	    hipermutationKernel = new Kernel[total_devices];
	    randomInsertionKernel = new Kernel[total_devices];
	    statisticsKernel = new Kernel[total_devices];
	    evaluationKernel = new Kernel[total_devices];
	    normalizeAffinityKernel = new Kernel[total_devices];

		for(int i=0; i<total_devices; i++)
		{
			if(i < m_gpu_count)
			{
				m_gpu_queues[i] = CommandQueue(contexts[i], gpu_devices[i], CL_QUEUE_PROFILING_ENABLE);
				m_pop_size_per_queue[i] = ceil(m_popsize * m_gpu_ratio /( m_gpu_count ));
				//cout << "m_pop_size_per_queue[" << i << "]:" << m_pop_size_per_queue[i] << endl << endl;
			}
			else
			{
				m_gpu_queues[i] = CommandQueue(contexts[i], cpu_devices[i-m_gpu_count], CL_QUEUE_PROFILING_ENABLE);
				m_pop_size_per_queue[i] = (int)(m_popsize * (1-m_gpu_ratio) /( m_cpu_count ));
				//cout << "m_pop_size_per_queue[" << i << "]:" << m_pop_size_per_queue[i] << endl;
			}

			// Create kernels
			initKernel[i] = Kernel(programs[i], "initPopulation");
			hipermutationKernel[i] = Kernel(programs[i], "cloneAndHypermutation");
			evaluationKernel[i] = Kernel(programs[i], "Evaluation");
			randomInsertionKernel[i] = Kernel(programs[i], "randomInsertion");
			statisticsKernel[i] = Kernel(programs[i], "StatisticsReduction1");
			normalizeAffinityKernel[i] = Kernel(programs[i], "NormalizeAffinity");

			// Create memory buffers
			popBuffer[i] = Buffer(contexts[i], CL_MEM_READ_WRITE, m_realLen*m_pop_size_per_queue[i]*sizeof(unsigned));
			affinityBuffer[i] = Buffer(contexts[i], CL_MEM_READ_WRITE, m_pop_size_per_queue[i] * sizeof(float));
			affinityNormBuffer[i] = Buffer(contexts[i], CL_MEM_READ_WRITE, m_pop_size_per_queue[i] * sizeof(float));
			seedBuffer[i] = Buffer(contexts[i], CL_MEM_READ_WRITE, m_pop_size_per_queue[i]*64*sizeof(int));
			statisticsBuffer[i] = Buffer(contexts[i], CL_MEM_READ_WRITE, sizeof(t_stats));
			parametersBuffer[i] = Buffer(contexts[i], CL_MEM_READ_WRITE,  sizeof(KernelParameters));

			// Set kernel arguments
			initKernel[i].setArg(0, popBuffer[i]);
			initKernel[i].setArg(1, seedBuffer[i]);
			initKernel[i].setArg(2, parametersBuffer[i]);

			hipermutationKernel[i].setArg(0, popBuffer[i]);
			hipermutationKernel[i].setArg(1, affinityBuffer[i]);
			hipermutationKernel[i].setArg(2, affinityNormBuffer[i]);
			hipermutationKernel[i].setArg(3, seedBuffer[i]);
			hipermutationKernel[i].setArg(4, statisticsBuffer[i]);
			hipermutationKernel[i].setArg(5, parametersBuffer[i]);

			evaluationKernel[i].setArg(0, popBuffer[i]);
			evaluationKernel[i].setArg(1, affinityBuffer[i]);
			evaluationKernel[i].setArg(2, parametersBuffer[i]);

			statisticsKernel[i].setArg(0, popBuffer[i]);
			statisticsKernel[i].setArg(1, affinityBuffer[i]);
			statisticsKernel[i].setArg(2, statisticsBuffer[i]);
			statisticsKernel[i].setArg(3, parametersBuffer[i]);

			normalizeAffinityKernel[i].setArg(0, affinityBuffer[i]);
			normalizeAffinityKernel[i].setArg(1, affinityNormBuffer[i]);
			normalizeAffinityKernel[i].setArg(2, statisticsBuffer[i]);
			normalizeAffinityKernel[i].setArg(3, parametersBuffer[i]);

			randomInsertionKernel[i].setArg(0, popBuffer[i]);
			randomInsertionKernel[i].setArg(1, affinityBuffer[i]);
			randomInsertionKernel[i].setArg(2, statisticsBuffer[i]);
			randomInsertionKernel[i].setArg(3, seedBuffer[i]);
			randomInsertionKernel[i].setArg(4, parametersBuffer[i]);
		}
	}
	catch(Error &error)
	{
		cout << error.what() << "(" << error.err() << ")" << endl;
		exit(1);
	}
}

void ClonalgCL::InitPopulation(unsigned **pop, float **affinities, float **affinitiesNorm, int threadID)
{
	Event evt;
	int stat;

	NDRange local(32);
	int gsize = ceil(((m_pop_size_per_queue[threadID]*m_realLen/(float)local[0])))*local[0];

	NDRange global(gsize);
	stat = m_gpu_queues[threadID].enqueueNDRangeKernel(initKernel[threadID], NullRange, global, local, NULL, &evt);

	if(stat != CL_SUCCESS)
	{
		cout << "Error in kernel initKernel." << endl;
		exit(0);
	}
}

void ClonalgCL::CloneAndHypermutate(unsigned * pop, float * affinities, float * affinitiesNorm, int threadID)
{
	Event evt;

	// is this thread using a GPU?
	if(threadID < m_gpu_count)
	{
		NDRange local(m_workGroupSize_hypermutation);
		NDRange global(m_pop_size_per_queue[threadID] * local[0]);

		//queue.enqueueNDRangeKernel(hipermutationKernel[i], NullRange, global, local, NULL, &evt);
		m_gpu_queues[threadID].enqueueNDRangeKernel(hipermutationKernel[threadID], NullRange, global, local, NULL, &evt);
	}
	else
	{
		NDRange local(4);
		NDRange global(m_pop_size_per_queue[threadID] * local[0]);
		//queue.enqueueNDRangeKernel(hipermutationKernel[i], NullRange, global, local, NULL, &evt);
		m_gpu_queues[threadID].enqueueNDRangeKernel(hipermutationKernel[threadID], NullRange, global, local, NULL, &evt);
	}
}

void ClonalgCL::RandomInsertion(unsigned * pop, float * affinities, int threadID)
{
	vector<Event> events;

	for(int i=0; i < m_randominsertion; i++)
	{
		Event evt;

		NDRange local(min(m_popsize, 256));
		NDRange global(local[0] * m_randominsertion);

		m_gpu_queues[threadID].enqueueNDRangeKernel(randomInsertionKernel[threadID], NullRange, global, local, &events, &evt);

		events.push_back(evt);
	}
}

void ClonalgCL::Statistics(unsigned * pop, float * fitness, int iterationNumber)
{
	cout << "Best (#"<< iterationNumber << ")\t" << m_stats[0].affinityBest << "\t" <<  m_stats[0].affinityWorst << endl;
}

void ClonalgCL::LoadSeeds(int threadID)
{
	unsigned seed = time(NULL) ^ threadID;

	int my_first_i = threadID * m_pop_size_per_queue[threadID]*64;
	int my_last_i = my_first_i + m_pop_size_per_queue[threadID]*64;

	for(int i=my_first_i; i < my_last_i; i++)
	{
		host_Seeds[i] = rand_r(&seed);
	}

	Event loadEvent;

	int dataSize = m_pop_size_per_queue[threadID]* 64* sizeof(int);

	m_gpu_queues[threadID].enqueueWriteBuffer(seedBuffer[threadID], CL_FALSE, 0, dataSize,
			&host_Seeds[my_first_i], NULL, &loadEvent);

	m_gpu_queues[threadID].finish();
}

void ClonalgCL::LoadParameters(int threadID)
{
	KernelParameters par;
	Event loadEvent;

	par.CHROM_LEN = m_cromLen;
	par.CLONING_FACTOR = m_cloningfactor;
	par.LOWER_LIM = m_lowerlim;
	par.MUTATION_FACTOR = m_mutationfactor;
	par.NCLON = m_numclones;
	par.POP_SIZE = m_pop_size_per_queue[threadID];
	par.REAL_LEN = m_realLen;
	par.UPPER_LIM = m_upperlim;

	par.BITS_PER_DIMENSION = m_bitsperdimension;
	par.DIMENSIONS = m_dimensions;

	m_gpu_queues[threadID].enqueueWriteBuffer(parametersBuffer[threadID], CL_FALSE, 0,
			sizeof(KernelParameters), &par, NULL, &loadEvent);
	m_gpu_queues[threadID].finish();
}

void ClonalgCL::FindBestAndWorst(int threadID)
{
	Event evt;
	int stat;

	NDRange local(min(64, m_pop_size_per_queue[threadID]));
	NDRange global(2*local[0]);

	stat = m_gpu_queues[threadID].enqueueNDRangeKernel(statisticsKernel[threadID],
			NullRange, global, local, NULL, &evt);

	stat |= m_gpu_queues[threadID].enqueueReadBuffer(statisticsBuffer[threadID], CL_FALSE,
			0, sizeof(t_stats), &m_stats[threadID]);


	stat |= m_gpu_queues[threadID].finish();

	if(stat != CL_SUCCESS)
	{
		cout << "Erro no statisticsKernel ou statisticsBuffer." << endl;
		exit(0);
	}

    #pragma omp barrier

	float best, worst;

    #pragma omp single
	{
		best = m_stats[0].affinityBest;
		worst   = m_stats[0].affinityWorst;

		for(int i=1; i < m_gpu_count+m_cpu_count; i++)
		{
			if(m_stats[i].affinityBest > best)
			{
				best = m_stats[i].affinityBest;
			}
			if(m_stats[i].affinityWorst < worst)
			{
				worst = m_stats[i].affinityWorst;
			}
		}

		for(int i=0; i< m_gpu_count+m_cpu_count; i++)
		{
			m_stats[i].affinityBest = best;
			m_stats[i].affinityWorst   = worst;
		}
	}

	#pragma omp barrier

	stat = m_gpu_queues[threadID].enqueueWriteBuffer(statisticsBuffer[threadID], CL_FALSE, 0,
			sizeof(t_stats), &m_stats[threadID], NULL, NULL);

	if(stat != CL_SUCCESS)
	{
		cout << "Error in statisticsBuffer." << endl;
		exit(1);
	}
}

void ClonalgCL::EvaluatePop(unsigned  *pop, float * affinities, int threadID)
{
	Event evt;

	NDRange local(std::min(m_workGroupSize_hypermutation, m_dimensions));
	NDRange global(m_pop_size_per_queue[threadID]*local[0]);
	m_gpu_queues[threadID].enqueueNDRangeKernel(evaluationKernel[threadID], NullRange, global, local, NULL, &evt);
}

void ClonalgCL::NormalizeAffinity(unsigned *pop, float *affinities, float *affinitiesNorm, int threadID)
{
	Event evt;

	NDRange local(32);
	int gsize = ceil((m_pop_size_per_queue[threadID]/(float)local[0]))*local[0];
	NDRange global(gsize);
	m_gpu_queues[threadID].enqueueNDRangeKernel(normalizeAffinityKernel[threadID], NullRange, global, local, NULL, &evt);
}
