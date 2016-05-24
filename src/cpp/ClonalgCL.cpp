#include "ClonalgCL.h"
#include "utils.h"
#include <math.h>
#include <omp.h>
#include <string>

using namespace std;

ClonalgCL::ClonalgCL(int generations,
				 int popsize,
				 int optimizationProblem,
				 int dimensions,
				 int bitsperdimension,
				 int bitsperparameter,
				 float mutationfactor,
				 float cloningfactor,
				 int numclones,
				 int randominsertion,
				 float upperlim,
				 float lowerlim)
				 : BaseClonalg(generations,
						 popsize,
						 optimizationProblem,
						 dimensions,
						 bitsperdimension,
						 bitsperparameter,
						 mutationfactor,
						 cloningfactor,
						 numclones,
						 randominsertion,
						 upperlim,
						 lowerlim
						 )

{}

ClonalgCL::ClonalgCL(int generations,
				 int popsize,
				 int optimizationProblem,
				 int dimensions,
				 int bitsperdimension,
				 int bitsperparameter,
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
						 bitsperparameter,
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
		double start = getRealTime();

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

				//#pragma omp single
				//Statistics(m_pop, m_fitness, gen);

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

		double elapsedTime =  getRealTime()-start;

		//Statistics(m_pop, m_fitness, gen);

		/*m_gpu_queues[0].enqueueReadBuffer(popBuffer[0], CL_TRUE, 0, m_realLen*m_pop_size_per_queue[0]*sizeof(unsigned), m_pop);
		m_gpu_queues[0].enqueueReadBuffer(fitnessBuffer[0], CL_TRUE, 0, sizeof(unsigned)*m_popsize, m_fitness);
		_gpu_queues[0].finish();
		for(int j = 0; j< m_realLen; j++){

			//unsigned val = m_pop[m_realLen*i + j];
			unsigned val = GetWord(m_pop, m_stats[0].indiceMelhor,j);
			for(int k = 0; k < BITS_PER_WORD; k++){
				cout << BIT_CHECK(val,k)/ pow(2,k);
			}
		}
		cout << endl;*/

		return elapsedTime;

	} catch(Error & error) {
	   std::cout << error.what() << "(" << error.err() << ")" << std::endl;
	}

	return 0;
}

Program ClonalgCL::CreateProgramFromSouce(Context context, vector<Device> devices, const char* fileName){

	//Read source file
	std::ifstream sourceFile(fileName);
	//std::ifstream sourceFile("./src/kernels/kernel.cl");
	std::string sourceCode(
		std::istreambuf_iterator<char>(sourceFile),
		(std::istreambuf_iterator<char>()));
	Program::Sources source(1, std::make_pair(sourceCode.c_str(), sourceCode.length()+1));

	// Make program of the source code in the context
	Program program = Program(context, source);

	// Build program for these specific devices
	try{

		std::string compileOptions = "-I ./ -D " + m_objective->getFormula() + " -D " + m_objective->getSumLimit() + " -D " + m_objective->getName();
		program.build(devices, compileOptions.c_str());

		/*for(unsigned i=0;i<devices.size(); i++){
			cout << "Build log:" << endl << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[i]) << endl;
		}*/
	}
	catch (Error error) {

		if(error.err() == CL_BUILD_PROGRAM_FAILURE) {

			for(unsigned i=0;i<devices.size(); i++){
				cout << "Build log:" << endl << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[i]) << endl;
			}
		}
	}

	return program;
}

int CLReadBinary(const char *filename, char* &buffer)
{
    try
    {
        std::ifstream is;
        is.open (filename, std::ios::binary);
        is.seekg (0, std::ios::end);
        int length = is.tellg();
        is.seekg (0, std::ios::beg);
        buffer = new char [length];
        is.read (buffer, length);
        is.close();
        return length;
    }
    catch(...) {
    	//std::cout << "Binary read function failure" << std::endl;
    }

    return 0;
}


Program CreateProgramFromBinary(Context context, vector<Device> devices, const char* fileName)
{
	char *bin;
	int bin_length = CLReadBinary(fileName, bin);

	Program::Binaries   binary(1, std::make_pair(bin, bin_length));
	Program             program = Program(context, devices, binary);
	program.build(devices);

	delete [] bin;

	return program;
}

void SaveProgramBinary(Program program, vector<Device> devices, const char* fileName)
{
    //cout << "Attempting to save binary code to file \"" << fileName << "\"" << endl;

    std::ofstream bfile(fileName, std::ios::binary);

    vector<int> sizes;
    vector<char*> binaries;

    program.getInfo(CL_PROGRAM_BINARY_SIZES, &sizes);

    int numDevices = sizes.size();

    unsigned char **programBinaries = new unsigned char*[numDevices];
	for (int i = 0; i < numDevices; i++)
	{
		programBinaries[i] = new unsigned char[sizes[i]];
	}

	//Get all of the program binaries
	int errNum = program.getInfo(CL_PROGRAM_BINARIES, programBinaries);

	if (errNum != CL_SUCCESS)
	{
		std::cerr << "Error querying for program binaries" << std::endl;

		for (int i = 0; i < numDevices; i++)
		{
			delete [] programBinaries[i];
		}
		delete [] programBinaries;
	}

	// Store the binaries for the device to disk
	FILE *fp = fopen(fileName, "ab");
	for (int i = 0; i < numDevices; i++)
	{
		// Store the binary just for the device requested.  In a scenario where
		// multiple devices were being used you would save all of the binaries out here.
		//if (devices[i] == device)
		//{
			fwrite(programBinaries[i], 1, sizes[i], fp);
			fclose(fp);
			break;
		//}
	}
}

void ShowDeviceInfo(Device device){

	std::cout << "Device Name: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;
	std::cout << "Device Type: " << device.getInfo<CL_DEVICE_TYPE>();
	std::cout << " (GPU: " << CL_DEVICE_TYPE_GPU << ", CPU: " << CL_DEVICE_TYPE_CPU << ")" << std::endl;
	std::cout << "Device Vendor: " << device.getInfo<CL_DEVICE_VENDOR>() << std::endl;
	std::cout << "Device Max Compute Units: " << device.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>() << std::endl;
	std::cout << "Device Global Memory: " << device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>() << std::endl;
	std::cout << "Device Max Clock Frequency: " << device.getInfo<CL_DEVICE_MAX_CLOCK_FREQUENCY>() << std::endl;
	std::cout << "Device Max Allocateable Memory: " << device.getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE>() << std::endl;
	std::cout << "Device Local Memory: " << device.getInfo<CL_DEVICE_LOCAL_MEM_SIZE>() << std::endl;
	std::cout << "Device Available: " << device.getInfo< CL_DEVICE_AVAILABLE>() << std::endl;
}

void ClonalgCL::OpenCLInit(){

	Program program;
	Device device;

	try {
		m_gpu_queues = NULL;
		m_cpu_queues = NULL;

		//setenv("CUDA_CACHE_DISABLE", "1", 1);

		// Get available platforms
		vector<Platform> platforms;
		Platform::get(&platforms);

		//cout << platforms.size() << " plataformas" << endl;

		cl::vector<Device> Devices, Devices1, Devices2, cpu_devices, gpu_devices;

		for(int i=0; i < 1; i++){

			platforms[i].getDevices(CL_DEVICE_TYPE_ALL, &Devices);

			//cout << "Platform #" << i << ": " << Devices.size() << " devices" << endl;

			for(int k=0; k < (int)Devices.size(); k++){

				if(Devices[k].getInfo<CL_DEVICE_TYPE>() == CL_DEVICE_TYPE_GPU){
					gpu_devices.push_back(Devices[k]);
				}
				else{
					cpu_devices.push_back(Devices[k]);
				}
			}
		}

		/*cout << "GPUS:" << endl;
		cout << "-------------------------------------" << endl;
		for(int k=0; k < (int)gpu_devices.size(); k++){
			ShowDeviceInfo(gpu_devices[k]);
			cout << "-------------------------------------" << endl;
		}

		cout << "CPUS:" << endl;
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

		//cout << "Attempt to create program from binary" << endl;
		//try{
			//program = CreateProgramFromBinary(context, devices, "./file.bin");
		//}
		//catch (Error error) {

			//cout << "Failed to create from binary. Compiling from source..." << endl;
			//program = CreateProgramFromSouce(context, devices, "./src/kernels/kernel.cl");
			//SaveProgramBinary(program, devices, "file.bin");

			//programas.push_back(CreateProgramFromSouce(contexts[0], Devices1, "./src/kernels/kernel.cl"));
			//programas.push_back(CreateProgramFromSouce(contexts[1], Devices2, "./src/kernels/kernel.cl"));

			std::ifstream sourceFile("./src/kernels/kernel.cl");
				std::string sourceCode(
					std::istreambuf_iterator<char>(sourceFile),
					(std::istreambuf_iterator<char>()));
			Program::Sources source(1, std::make_pair(sourceCode.c_str(), sourceCode.length()+1));

			for(int k=0; k < m_gpu_count; k++){
				programs.push_back(CreateProgramFromSouce(contexts[k], gpu_devices, "./src/kernels/kernel.cl"));
			}

			for(int k=0; k < m_cpu_count; k++){
				programs.push_back(CreateProgramFromSouce(contexts[m_gpu_count+k], cpu_devices, "./src/kernels/kernel.cl"));
			}

		//}

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

			// Make the kernels
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

	//int i = threadID;

	//for(int i=0; i< GPUCount; i++){

		/*initKernel[threadID].setArg(0, popBuffer[threadID]);
		initKernel[threadID].setArg(1, seedBuffer[threadID]);
		initKernel[threadID].setArg(2, parameterBuffer[threadID]);*/

		NDRange local(32);
		int gsize = ceil(((m_pop_size_per_queue[threadID]*m_realLen/(float)local[0])))*local[0];

		//cout << "gsize = " << gsize << endl;

		NDRange global(gsize);
		stat = m_gpu_queues[threadID].enqueueNDRangeKernel(initKernel[threadID], NullRange, global, local, NULL, &evt);

		if(stat != CL_SUCCESS){
			cout << "Error in kernel initKernel." << endl;
			exit(0);
		}

	//}
}

void ClonalgCL::CloneAndHypermutate(unsigned * pop, float * fitness, float * fitnessNorm, int threadID)
{
	Event evt;

	//for(int i=0; i< GPUCount; i++){

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
	//}
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

int host_Seeds[64*16384];

void ClonalgCL::LoadSeeds(int threadID)
{
	// = new int[m_popsize*64/GPUCount];

	unsigned seed = time(NULL) ^ threadID;
	//unsigned seed = 456*(threadID+1);

	int my_first_i = threadID * m_pop_size_per_queue[threadID]*64;
	int my_last_i = my_first_i + m_pop_size_per_queue[threadID]*64;

	for(int i=my_first_i; i < my_last_i; i++){
		host_Seeds[i] = rand_r(&seed);
	}

	Event loadEvent;

	int dataSize = m_pop_size_per_queue[threadID]* 64* sizeof(int);

	//for(int i=0; i< GPUCount;i++){
		m_gpu_queues[threadID].enqueueWriteBuffer(seedBuffer[threadID], CL_FALSE, 0, dataSize,
				&host_Seeds[my_first_i], NULL, &loadEvent);
		//GPUQueues[threadID].finish();
		m_gpu_queues[threadID].finish();
	//}

	//for(int i=0; i< GPUCount;i++){
		//GPUQueues[threadID].finish();
	//}
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
	//int i  = threadID;
	int stat;

	//for(int i=0; i< GPUCount; i++){

	    /*stat  = statisticsKernel[threadID].setArg(0, popBuffer[threadID]);
	    stat |= statisticsKernel[threadID].setArg(1, fitnessBuffer[threadID]);
	    stat |= statisticsKernel[threadID].setArg(2, statisticsBuffer[threadID]);
	    stat |= statisticsKernel[threadID].setArg(3, parameterBuffer[threadID]);

	    if(stat != CL_SUCCESS){
	    	cout << "Erro no setArg do kernel statistics." << endl;
	    	exit(0);
	    }*/

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

	//for(int i=0; i< GPUCount; i++){
		stat = m_gpu_queues[threadID].enqueueWriteBuffer(statisticsBuffer[threadID], CL_FALSE, 0,
				sizeof(t_stats), &m_stats[threadID], NULL, NULL);

		if(stat != CL_SUCCESS){
			cout << "Erro no statisticsBuffer." << endl;
			exit(0);
		}
	//}

	/*cout << "Elapsed time (statisticsKernel): " << getElapsedTime(evt )<< endl;*/
}

double ClonalgCL::getElapsedTime(Event evt){

	double elapsed = evt.getProfilingInfo<CL_PROFILING_COMMAND_END>() -
	            evt.getProfilingInfo<CL_PROFILING_COMMAND_START>();

    return (double)(elapsed)/1000000000.0;
}

float ClonalgCL::Evaluate(unsigned  * individual)
{
}

void ClonalgCL::EvaluatePop(unsigned  * pop, float * fitness, int threadID)
{
	Event evt;

	//for(int i=0; i< GPUCount; i++){
		/*fitnessKernel[threadID].setArg(0, popBuffer[threadID]);
		fitnessKernel[threadID].setArg(1, fitnessBuffer[threadID]);
		fitnessKernel[threadID].setArg(2, parameterBuffer[threadID]);*/

		NDRange local(std::min(m_workGroupSize_hypermutation, m_dimensions));
		NDRange global(m_pop_size_per_queue[threadID]*local[0]);
		m_gpu_queues[threadID].enqueueNDRangeKernel(fitnessKernel[threadID], NullRange, global, local, NULL, &evt);
	//}
}


void ClonalgCL::Mutate(unsigned  * clone, float mutationRate)
{
}

void ClonalgCL::CalculateAffinity(unsigned * pop, float * fitness, float * fitnessNorm, int threadID){

	Event evt;

	int i = threadID;

	//for(int i=0; i<GPUCount; i++){

		/*calculateAffinityKernel[i].setArg(0, fitnessBuffer[i]);
		calculateAffinityKernel[i].setArg(1, fitnessNormBuffer[i]);
		calculateAffinityKernel[i].setArg(2, statisticsBuffer[i]);
		calculateAffinityKernel[i].setArg(3, parameterBuffer[i]);*/

		NDRange local(32);
		int gsize = ceil((m_pop_size_per_queue[threadID]/(float)local[0]))*local[0];
		NDRange global(gsize);

		m_gpu_queues[i].enqueueNDRangeKernel(calculateAffinityKernel[i], NullRange, global, local, NULL, &evt);
	//}

	//cout << "Elapsed time (calculateAffinity): " << getElapsedTime(evt) << endl;
}

void ClonalgCL::InitPopulation(unsigned ** pop, float **fitness, float ** fitnessNorm){}
void ClonalgCL::CloneAndHypermutate(unsigned * pop, float * fitness, float * fitnessNorm){}
