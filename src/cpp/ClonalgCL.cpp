#include "ClonalgCL.h"
#include "utils.h"
#include <math.h>
#include <omp.h>
#include <string>
#include "TimeUtils.h"

using namespace std;

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

ClonalgCL::~ClonalgCL() {

	delete[] m_pop;
	delete[] m_fitness;
	delete[] m_fitnessNorm;
	delete[] m_stats;

	if(m_gpu_queues != NULL){
		delete[] m_gpu_queues;
	}
	if(m_cpu_queues != NULL){
		delete[] m_cpu_queues;
	}
	if(m_pop_size_per_queue != NULL){
		delete [] m_pop_size_per_queue;
	}

	delete[] initKernel;
	delete[] hipermutationKernel,
	delete[] randomInsertionKernel,
	delete[] statisticsKernel,
	delete[] fitnessKernel,
	delete[] calculateAffinityKernel;

	delete[] popBuffer;
	delete[] seedBuffer;
	delete[] statisticsBuffer;
	delete[] parameterBuffer;
	delete[] fitnessBuffer;
	delete[] fitnessNormBuffer;
	delete[] fitnessCloneBuffer;
}

float ClonalgCL::Search(){

	try{
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

			for(gen=0; gen < m_generations; gen++){
			//while(m_stats[0].afinidadeMelhor < -0.00001){

				#pragma omp single
				Statistics(m_pop, m_fitness, gen);

				CalculateAffinity(m_pop, m_fitness, m_fitnessNorm, threadID);
				CloneAndHypermutate(m_pop, m_fitness, m_fitnessNorm, threadID);

				if(m_randominsertion > 0){
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

	} catch(Error & error) {
	   std::cout << error.what() << "(" << error.err() << ")" << std::endl;
	}

	return 0;
}

void ClonalgCL::OpenCLInit(){

	Program program;
	Device device;

	try {
		m_gpu_queues = NULL;
		m_cpu_queues = NULL;

		// Get available platforms
		vector<Platform> platforms;
		Platform::get(&platforms);

		//cout << platforms.size() << " plataforms available." << endl;

		cl::vector<Device> Devices, Devices1, Devices2, cpu_devices, gpu_devices;

		for(int i=0; i < 1; i++){

			platforms[i].getDevices(CL_DEVICE_TYPE_ALL, &Devices);

			//cout << "Platform #" << i << ": " << Devices.size() << " devices." << endl;

			for(int k=0; k < (int)Devices.size(); k++){

				if(Devices[k].getInfo<CL_DEVICE_TYPE>() == CL_DEVICE_TYPE_GPU){
					gpu_devices.push_back(Devices[k]);
				}
				else{
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

		for(int k=0; k < m_gpu_count; k++){
			contexts.push_back(Context(gpu_devices));
		}

		for(int k=0; k < m_cpu_count; k++){
			contexts.push_back(Context(cpu_devices));
		}

		vector<Program> programs;

		//cout <<"Using " <<  m_gpu_count << " gpus and " << m_cpu_count << " cpus." << endl;

		std::string compilerOptions = "-I ./ -D " + m_objective->getFormula() + " -D " + m_objective->getSumLimit() + " -D " + m_objective->getName();

		for(int k=0; k < m_gpu_count; k++){
			programs.push_back(clUtils.CreateProgramFromSource(contexts[k], gpu_devices, compilerOptions, "./src/kernels/kernel.cl"));
		}

		for(int k=0; k < m_cpu_count; k++){
			programs.push_back(clUtils.CreateProgramFromSource(contexts[m_gpu_count+k], cpu_devices, compilerOptions, "./src/kernels/kernel.cl"));
		}

		m_gpu_queues = new CommandQueue[m_gpu_count+m_cpu_count];

		int total_devices = m_gpu_count + m_cpu_count;

		if(m_cpu_count == 0){
			m_gpu_ratio = 1.0f;
		}
		if(m_gpu_count == 0){
			m_gpu_ratio = 0.0f;
		}

		m_pop_size_per_queue = new int[total_devices];

		m_stats = new t_stats[total_devices];

		popBuffer = new Buffer[total_devices];
	    seedBuffer = new Buffer[total_devices];
	    statisticsBuffer = new Buffer[total_devices];
	    parameterBuffer = new Buffer[total_devices];
	    fitnessBuffer = new Buffer[total_devices] ;
	    fitnessNormBuffer = new Buffer[total_devices];
	    fitnessCloneBuffer = new Buffer[total_devices];

	    initKernel = new Kernel[total_devices];
	    hipermutationKernel = new Kernel[total_devices];
	    randomInsertionKernel = new Kernel[total_devices];
	    statisticsKernel = new Kernel[total_devices];
	    fitnessKernel = new Kernel[total_devices];
	    calculateAffinityKernel = new Kernel[total_devices];

		for(int i=0; i<total_devices; i++){

			if(i < m_gpu_count){
				m_gpu_queues[i] = CommandQueue(contexts[i], gpu_devices[i], CL_QUEUE_PROFILING_ENABLE);

				m_pop_size_per_queue[i] = ceil(m_popsize * m_gpu_ratio /( m_gpu_count ));
				//cout << "m_pop_size_per_queue[" << i << "]:" << m_pop_size_per_queue[i] << endl << endl;
			}
			else{
				m_gpu_queues[i] = CommandQueue(contexts[i], cpu_devices[i-m_gpu_count], CL_QUEUE_PROFILING_ENABLE);
				m_pop_size_per_queue[i] = (int)(m_popsize * (1-m_gpu_ratio) /( m_cpu_count ));
				//cout << "m_pop_size_per_queue[" << i << "]:" << m_pop_size_per_queue[i] << endl;
			}

			// Create kernels
			initKernel[i] = Kernel(programs[i], "initPopulation");
			hipermutationKernel[i] = Kernel(programs[i], "iteracaoClonalg");
			fitnessKernel[i] = Kernel(programs[i], "Fitness");
			randomInsertionKernel[i] = Kernel(programs[i], "randomInsertion");
			statisticsKernel[i] = Kernel(programs[i], "StatisticsReduction1");
			calculateAffinityKernel[i] = Kernel(programs[i], "CalculateAffinity");

			// Create memory buffers
			popBuffer[i] = Buffer(contexts[i], CL_MEM_READ_WRITE, m_realLen*m_pop_size_per_queue[i]*sizeof(unsigned));
			fitnessBuffer[i] = Buffer(contexts[i], CL_MEM_READ_WRITE, m_pop_size_per_queue[i] * sizeof(float));
			fitnessNormBuffer[i] = Buffer(contexts[i], CL_MEM_READ_WRITE, m_pop_size_per_queue[i] * sizeof(float));
			seedBuffer[i] = Buffer(contexts[i], CL_MEM_READ_WRITE, m_pop_size_per_queue[i]*64*sizeof(int));
			statisticsBuffer[i] = Buffer(contexts[i], CL_MEM_READ_WRITE, sizeof(t_stats));
			parameterBuffer[i] = Buffer(contexts[i], CL_MEM_READ_WRITE,  sizeof(KernelParameters));

			// Set kernel arguments
			initKernel[i].setArg(0, popBuffer[i]);
			initKernel[i].setArg(1, seedBuffer[i]);
			initKernel[i].setArg(2, parameterBuffer[i]);

			hipermutationKernel[i].setArg(0, popBuffer[i]);
			hipermutationKernel[i].setArg(1, fitnessBuffer[i]);
			hipermutationKernel[i].setArg(2, fitnessNormBuffer[i]);
			hipermutationKernel[i].setArg(3, seedBuffer[i]);
			hipermutationKernel[i].setArg(4, statisticsBuffer[i]);
			hipermutationKernel[i].setArg(5, parameterBuffer[i]);

			fitnessKernel[i].setArg(0, popBuffer[i]);
			fitnessKernel[i].setArg(1, fitnessBuffer[i]);
			fitnessKernel[i].setArg(2, parameterBuffer[i]);

			statisticsKernel[i].setArg(0, popBuffer[i]);
			statisticsKernel[i].setArg(1, fitnessBuffer[i]);
			statisticsKernel[i].setArg(2, statisticsBuffer[i]);
			statisticsKernel[i].setArg(3, parameterBuffer[i]);

			calculateAffinityKernel[i].setArg(0, fitnessBuffer[i]);
			calculateAffinityKernel[i].setArg(1, fitnessNormBuffer[i]);
			calculateAffinityKernel[i].setArg(2, statisticsBuffer[i]);
			calculateAffinityKernel[i].setArg(3, parameterBuffer[i]);

			randomInsertionKernel[i].setArg(0, popBuffer[i]);
			randomInsertionKernel[i].setArg(1, fitnessBuffer[i]);
			randomInsertionKernel[i].setArg(2, statisticsBuffer[i]);
			randomInsertionKernel[i].setArg(3, seedBuffer[i]);
			randomInsertionKernel[i].setArg(4, parameterBuffer[i]);
		}

	} catch(Error error) {
		std::cout << error.what() << "(" << error.err() << ")" << std::endl;

		if(error.err() == CL_BUILD_PROGRAM_FAILURE) {
			std::cout << "Build log:" << std::endl
			<< program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << std::endl;
		}
		exit(1);
	}
}

void ClonalgCL::InitPopulation(unsigned ** pop, float **fitness, float ** fitnessNorm, int threadID)
{
	Event evt;
	int stat;

	NDRange local(32);
	int gsize = ceil(((m_pop_size_per_queue[threadID]*m_realLen/(float)local[0])))*local[0];

	NDRange global(gsize);
	stat = m_gpu_queues[threadID].enqueueNDRangeKernel(initKernel[threadID], NullRange, global, local, NULL, &evt);

	if(stat != CL_SUCCESS){
		cout << "Error in kernel initKernel." << endl;
		exit(0);
	}
}

void ClonalgCL::CloneAndHypermutate(unsigned * pop, float * fitness, float * fitnessNorm, int threadID)
{
	Event evt;

	// is this thread using a GPU?
	if(threadID < m_gpu_count){
		NDRange local(m_workGroupSize_hypermutation);
		NDRange global(m_pop_size_per_queue[threadID] * local[0]);

		//queue.enqueueNDRangeKernel(hipermutationKernel[i], NullRange, global, local, NULL, &evt);
		m_gpu_queues[threadID].enqueueNDRangeKernel(hipermutationKernel[threadID], NullRange, global, local, NULL, &evt);
	}
	else{
		NDRange local(4);
		NDRange global(m_pop_size_per_queue[threadID] * local[0]);
		//queue.enqueueNDRangeKernel(hipermutationKernel[i], NullRange, global, local, NULL, &evt);
		m_gpu_queues[threadID].enqueueNDRangeKernel(hipermutationKernel[threadID], NullRange, global, local, NULL, &evt);
	}
}

void ClonalgCL::RandomInsertion(unsigned * pop, float * fitness, int threadID){

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

void ClonalgCL::Statistics(unsigned * pop, float * fitness, int iterationNumber){
	cout << "Best (#"<< iterationNumber << ")\t" << m_stats[0].afinidadeMelhor << "\t" <<  m_stats[0].afinidadePior << endl;
}

void ClonalgCL::LoadSeeds(int threadID)
{
	unsigned seed = time(NULL) ^ threadID;

	int my_first_i = threadID * m_pop_size_per_queue[threadID]*64;
	int my_last_i = my_first_i + m_pop_size_per_queue[threadID]*64;

	for(int i=my_first_i; i < my_last_i; i++){
		host_Seeds[i] = rand_r(&seed);
	}

	Event loadEvent;

	int dataSize = m_pop_size_per_queue[threadID]* 64* sizeof(int);

	m_gpu_queues[threadID].enqueueWriteBuffer(seedBuffer[threadID], CL_FALSE, 0, dataSize,
			&host_Seeds[my_first_i], NULL, &loadEvent);

	m_gpu_queues[threadID].finish();
}

void ClonalgCL::LoadParameters(int threadID){

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

	m_gpu_queues[threadID].enqueueWriteBuffer(parameterBuffer[threadID], CL_FALSE, 0,
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

	if(stat != CL_SUCCESS){
		cout << "Erro no statisticsKernel ou statisticsBuffer." << endl;
		exit(0);
	}

    #pragma omp barrier

	float melhor, pior;

    #pragma omp single
	{
		melhor = m_stats[0].afinidadeMelhor;
		pior   = m_stats[0].afinidadePior;

		for(int i=1; i < m_gpu_count+m_cpu_count; i++){

			if(m_stats[i].afinidadeMelhor > melhor){
				melhor = m_stats[i].afinidadeMelhor;
			}
			if(m_stats[i].afinidadePior < pior){
				pior = m_stats[i].afinidadePior;
			}
		}

		for(int i=0; i< m_gpu_count+m_cpu_count; i++){
			m_stats[i].afinidadeMelhor = melhor;
			m_stats[i].afinidadePior   = pior;
		}
	}

	#pragma omp barrier

	stat = m_gpu_queues[threadID].enqueueWriteBuffer(statisticsBuffer[threadID], CL_FALSE, 0,
			sizeof(t_stats), &m_stats[threadID], NULL, NULL);

	if(stat != CL_SUCCESS){
		cout << "Error in statisticsBuffer." << endl;
		exit(1);
	}
}

float ClonalgCL::Evaluate(unsigned  * individual)
{
}

void ClonalgCL::EvaluatePop(unsigned  * pop, float * fitness, int threadID)
{
	Event evt;

	NDRange local(std::min(m_workGroupSize_hypermutation, m_dimensions));
	NDRange global(m_pop_size_per_queue[threadID]*local[0]);
	m_gpu_queues[threadID].enqueueNDRangeKernel(fitnessKernel[threadID], NullRange, global, local, NULL, &evt);
}


void ClonalgCL::Mutate(unsigned  * clone, float mutationRate)
{
}

void ClonalgCL::CalculateAffinity(unsigned * pop, float * fitness, float * fitnessNorm, int threadID){

	Event evt;

	int i = threadID;

	NDRange local(32);
	int gsize = ceil((m_pop_size_per_queue[threadID]/(float)local[0]))*local[0];
	NDRange global(gsize);

	m_gpu_queues[i].enqueueNDRangeKernel(calculateAffinityKernel[i], NullRange, global, local, NULL, &evt);
}

void ClonalgCL::InitPopulation(unsigned ** pop, float **fitness, float ** fitnessNorm){}
void ClonalgCL::CloneAndHypermutate(unsigned * pop, float * fitness, float * fitnessNorm){}
