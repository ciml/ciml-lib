#include "ClonalgCL.h"
#include "utils.h"
#include <omp.h>

ClonalgCL::ClonalgCL(int generations,
				 int popsize,
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
				 int dimensions,
				 int bitsperdimension,
				 int bitsperparameter,
				 float mutationfactor,
				 float cloningfactor,
				 int numclones,
				 int randominsertion,
				 float upperlim,
				 float lowerlim,
				 int gpucount)
				 : BaseClonalg(generations,
						 popsize,
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

	GPUCount = gpucount;

	OpenCLInit();
}

ClonalgCL::~ClonalgCL() {

	delete[] m_pop;
	delete[] m_fitness;
	delete[] m_fitnessNorm;
	delete[] m_stats;
	if(GPUQueues != NULL){
		delete[] GPUQueues;
	}
	if(CPUQueues != NULL){
		delete[] CPUQueues;
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

		omp_set_num_threads(GPUCount);

		#pragma omp parallel
		{
			int threadID = omp_get_thread_num();

			LoadParameters(threadID);
			LoadSeeds(threadID);

			InitPopulation(&m_pop, &m_fitness, &m_fitnessNorm, threadID);
			EvaluatePop(m_pop, m_fitness, threadID);
			FindBestAndWorst(threadID);

			int i=0;
			//while(m_statistics.afinidadeMelhor !=  0.0){
			for(i=0; i< m_generations; i++){
				#pragma omp barrier

				/*GPUQueues[threadID].enqueueReadBuffer(popBuffer[threadID], CL_FALSE,0, m_popsize*m_realLen*sizeof(unsigned)/GPUCount, m_pop);
				GPUQueues[threadID].enqueueReadBuffer(fitnessBuffer[threadID], CL_FALSE,0, m_popsize*sizeof(float)/GPUCount, m_fitness);
				GPUQueues[threadID].finish();
				cout << "População da fila " << threadID << ":" <<endl;
				PrintPop();*/

				//#pragma omp single
				//Statistics(m_pop, m_fitness, i);

				CalculateAffinity(m_pop, m_fitness, m_fitnessNorm, threadID);
				CloneAndHypermutate(m_pop, m_fitness, m_fitnessNorm, threadID);

				if(m_randominsertion > 0){
					//RandomInsertion(m_pop, m_fitness);
				}

				FindBestAndWorst(threadID);

				//#pragma omp barrier
				//cout << "Hello from thread " << threadID << " at iteration " << i << endl;

			}

			//for(i=0; i<GPUCount; i++){
			GPUQueues[threadID].finish();
			//}
		}

		double elapsedTime =  getRealTime()-start;

		//Statistics(m_pop, m_fitness, m_generations);

		return elapsedTime;

	} catch(Error error) {
	   std::cout << error.what() << "(" << error.err() << ")" << std::endl;
	}

	return 0;
}

Program CreateProgramFromSouce(Context context, vector<Device> devices, const char* fileName){

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
		program.build(devices);
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

		GPUQueues = NULL;
		CPUQueues = NULL;

		//setenv("CUDA_CACHE_DISABLE", "1", 1);

		// Get available platforms
		vector<Platform> platforms;
		Platform::get(&platforms);

		//cout << "Plaftorms found: " << platforms.size() << endl;

		// Select the default platform and create a context using this platform and the 1st device
		cl_context_properties cps[3] = {
			CL_CONTEXT_PLATFORM,
			(cl_context_properties)(platforms[0])(),
			0
		};
		Context context(CL_DEVICE_TYPE_ALL, cps);

		// Get a list of devices on this platform
		vector<Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();

		GPUCount = std::min(GPUCount, (int)devices.size());
		CPUCount = 0;

		//cout <<  "Devices found: " << devices.size() << endl;
		//cout <<  "Utilizando: " << GPUCount  << endl;

		device = devices[0];

		//cout << "Attempt to create program from binary" << endl;
		try{
			program = CreateProgramFromBinary(context, devices, "./file.bin");
		}
		catch (Error error) {

			//cout << "Failed to create from binary. Compiling from source..." << endl;

			program = CreateProgramFromSouce(context, devices, "./src/kernels/kernel.cl");
			//SaveProgramBinary(program, devices, "file.bin");
		}

		//Create a command queue and use the first device
		//queue = CommandQueue(context, devices[0], CL_QUEUE_PROFILING_ENABLE);

		GPUQueues = new CommandQueue[GPUCount];
		m_stats = new t_stats[GPUCount];

		popBuffer = new Buffer[GPUCount];
	    seedBuffer = new Buffer[GPUCount];
	    statisticsBuffer = new Buffer[GPUCount];
	    parameterBuffer = new Buffer[GPUCount];
	    fitnessBuffer = new Buffer[GPUCount] ;
	    fitnessNormBuffer = new Buffer[GPUCount];
	    fitnessCloneBuffer = new Buffer[GPUCount];

	    initKernel = new Kernel[GPUCount];
	    hipermutationKernel = new Kernel[GPUCount];
	    randomInsertionKernel = new Kernel[GPUCount];
	    statisticsKernel = new Kernel[GPUCount];
	    fitnessKernel = new Kernel[GPUCount];
	    calculateAffinityKernel = new Kernel[GPUCount];

		for(int i=0; i<GPUCount; i++){

			GPUQueues[i] = CommandQueue(context, devices[i], CL_QUEUE_PROFILING_ENABLE);

			//ShowDeviceInfo(devices[i]);

			// Make the kernels
			/*initKernel.push_back(Kernel(program, "inicializaPopulacao"));
			hipermutationKernel.push_back(Kernel(program, "iteracaoClonalg"));
			fitnessKernel.push_back(Kernel(program, "Fitness"));
			randomInsertionKernel.push_back(Kernel(program, "substituicaoAleatoria"));
			statisticsKernel.push_back(Kernel(program, "StatisticsReduction1"));
			calculateAffinityKernel.push_back(Kernel(program, "CalculateAffinity"));
*/
			initKernel[i] = Kernel(program, "inicializaPopulacao");
			hipermutationKernel[i] = Kernel(program, "iteracaoClonalg");
			fitnessKernel[i] = Kernel(program, "Fitness");
			randomInsertionKernel[i] = Kernel(program, "substituicaoAleatoria");
			statisticsKernel[i] = Kernel(program, "StatisticsReduction1");
			calculateAffinityKernel[i] = Kernel(program, "CalculateAffinity");


			// Create memory buffers
			popBuffer[i] = Buffer(context, CL_MEM_READ_WRITE, m_popsize*m_realLen*sizeof(unsigned)/GPUCount);
			fitnessBuffer[i] = Buffer(context, CL_MEM_READ_WRITE, m_popsize * sizeof(float)/GPUCount);
			fitnessNormBuffer[i] = Buffer(context, CL_MEM_READ_WRITE, m_popsize * sizeof(float)/GPUCount);
			fitnessCloneBuffer[i] = Buffer(context, CL_MEM_READ_WRITE, m_popsize * sizeof(float)/GPUCount);

			seedBuffer[i] = Buffer(context, CL_MEM_READ_WRITE, m_popsize*64*sizeof(int));
			statisticsBuffer[i] = Buffer(context, CL_MEM_READ_WRITE, sizeof(t_stats));
			parameterBuffer[i] = Buffer(context, CL_MEM_READ_ONLY,  sizeof(KernelParameters));



			/*popBuffer.push_back(Buffer(context, CL_MEM_READ_WRITE, m_popsize*m_realLen*sizeof(unsigned)/GPUCount));
			fitnessBuffer.push_back(Buffer(context, CL_MEM_READ_WRITE, m_popsize * sizeof(float)/GPUCount));
			fitnessNormBuffer.push_back(Buffer(context, CL_MEM_READ_WRITE, m_popsize * sizeof(float)/GPUCount));
			fitnessCloneBuffer.push_back(Buffer(context, CL_MEM_READ_WRITE, m_popsize * sizeof(float)/GPUCount));

			seedBuffer.push_back(Buffer(context, CL_MEM_READ_WRITE, m_popsize*64*sizeof(int)));
			statisticsBuffer.push_back(Buffer(context, CL_MEM_READ_WRITE, sizeof(t_stats)));
			parameterBuffer.push_back(Buffer(context, CL_MEM_READ_ONLY,  sizeof(KernelParameters)));*/
		}

	} catch(Error error) {
		std::cout << error.what() << "(" << error.err() << ")" << std::endl;

		if(error.err() == CL_BUILD_PROGRAM_FAILURE) {
			std::cout << "Build log:" << std::endl
			<< program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << std::endl;
		}
	}
}


void ClonalgCL::InitPopulation(unsigned ** pop, float **fitness, float ** fitnessNorm, int threadID)
{
	Event evt;

	int i = threadID;

	//for(int i=0; i< GPUCount; i++){

		initKernel[i].setArg(0, popBuffer[i]);
		initKernel[i].setArg(1, seedBuffer[i]);
		initKernel[i].setArg(2, parameterBuffer[i]);

		/*NDRange local(m_realLen);
		NDRange global(m_realLen * m_popsize);
		NDRange local(32, std::min(m_realLen, 32)); */
		NDRange local(64);

		int gsize = ceil(((m_popsize*m_realLen/(float)local[0])/GPUCount))*local[0];
		NDRange global(gsize);

		//queue.enqueueNDRangeKernel(initKernel[0], NullRange, global, local, NULL, &evt);

		GPUQueues[i].enqueueNDRangeKernel(initKernel[i], NullRange, global, local, NULL, &evt);
	//}

	//queue.finish();

	/*queue.enqueueReadBuffer(popBuffer, CL_TRUE, 0, m_popsize * m_realLen * sizeof(unsigned int), *pop);
	queue.finish();

	cout << "Elapsed time (InitPopulation): " << getElapsedTime(evt) << endl;*/
	//PrintPop();
	//exit(0);32
}

void ClonalgCL::CloneAndHypermutate(unsigned * pop, float * fitness, float * fitnessNorm, int threadID)
{
	Event evt;

	int i = threadID;

	//for(int i=0; i< GPUCount; i++){

		hipermutationKernel[i].setArg(0, popBuffer[i]);
		hipermutationKernel[i].setArg(1, fitnessBuffer[i]);
		hipermutationKernel[i].setArg(2, fitnessNormBuffer[i]);
		hipermutationKernel[i].setArg(3, seedBuffer[i]);
		hipermutationKernel[i].setArg(4, statisticsBuffer[i]);
		hipermutationKernel[i].setArg(5, parameterBuffer[i]);

		NDRange local(32);
		NDRange global(m_popsize * local[0] / GPUCount);
		//queue.enqueueNDRangeKernel(hipermutationKernel[i], NullRange, global, local, NULL, &evt);
		GPUQueues[i].enqueueNDRangeKernel(hipermutationKernel[i], NullRange, global, local, NULL, &evt);
	//}

	//queue.finish();
	//cout << "Elapsed time (hipermutationKernel): " << getElapsedTime(evt) << endl;
	//exit(0);

	/*unsigned *clones = new unsigned[m_popsize*m_realLen*m_numclones];
	queue.enqueueReadBuffer(cloneBuffer, CL_TRUE, 0, m_popsize * m_realLen *m_numclones * sizeof(unsigned int), clones);
	queue.finish();

	for(int i=0; i< m_popsize; i++){
		cout << "Antibody #" << i << endl;
		for(int j=0; j<m_numclones; j++){
			cout << "\t Clone #" << j << endl;
			cout << "\t\t";
			for(int k=0; k < m_realLen; k++){
				//cout << clones[i*m_realLen*m_numclones + j*m_realLen + k];

				unsigned val = clones[i*m_realLen*m_numclones + j*m_realLen + k];

				for(int l = 0; l < BITS_PER_WORD; l++){
					cout << BIT_CHECK(val,l)/ pow(2,l);
				}

			}
			cout << endl;
		}
	}
	delete[] clones;
	*/
	//exit(0);
}

void ClonalgCL::RandomInsertion(unsigned * pop, float * fitness){

	Event evt;

	randomInsertionKernel[0].setArg(0, popBuffer[0]);
	randomInsertionKernel[0].setArg(1, seedBuffer[0]);

	NDRange local(min(m_popsize, 128));
	NDRange global(local[0] * m_randominsertion);

	GPUQueues[0].enqueueNDRangeKernel(randomInsertionKernel[0], NullRange, global, local, NULL, &evt);
	//queue.finish();
}

void ClonalgCL::Statistics(unsigned * pop, float * fitness, int iterationNumber){
	cout << "Best (#"<< iterationNumber << ")\t" << m_stats[0].afinidadeMelhor << "\t" <<  m_stats[0].afinidadePior << endl;
}

void ClonalgCL::LoadSeeds(int threadID)
{
	//cout << "wow " << threadID << endl;

	int host_Seeds[64*4096];// = new int[m_popsize*64/GPUCount];

	//unsigned seed = time(NULL) ^ threadID;
	unsigned seed = 456*(threadID+1);

	for(int i=0; i < m_popsize*64; i++){
		host_Seeds[i] = rand_r(&seed);
	}

	//cout << "wow 2 - " << threadID << endl;

	Event loadEvent;

	int dataSize = m_popsize* 64* sizeof(int)/GPUCount;

	//for(int i=0; i< GPUCount;i++){
		GPUQueues[threadID].enqueueWriteBuffer(seedBuffer[threadID], CL_TRUE, 0, dataSize,
				host_Seeds, NULL, &loadEvent);
		GPUQueues[threadID].finish();

	//}

	//cout << "wow 3 - " << threadID << endl;
	//for(int i=0; i< GPUCount;i++){
		//GPUQueues[threadID].finish();
	//}

	//cout << "wow 4 - " << threadID << endl;

	//delete[] host_Seeds;
}

void ClonalgCL::LoadParameters(int threadID){

	KernelParameters par;
	Event loadEvent;

	par.CHROM_LEN = m_cromLen;
	par.CLONING_FACTOR = m_cloningfactor;
	par.LOWER_LIM = m_lowerlim;
	par.MUTATION_FACTOR = m_mutationfactor;
	par.NCLON = m_numclones;
	par.POP_SIZE = m_popsize/GPUCount;
	par.REAL_LEN = m_realLen;
	par.UPPER_LIM = m_upperlim;

    //int i = threadID;

	//for(int i = 0; i < GPUCount; i++){
	GPUQueues[threadID].enqueueWriteBuffer(parameterBuffer[threadID], CL_TRUE, 0, sizeof(KernelParameters), &par, NULL, &loadEvent);
	GPUQueues[threadID].finish();
	//}
}

void ClonalgCL::FindBestAndWorst(int threadID)
{
	Event evt;
	//int i  = threadID;
	int stat;

	//for(int i=0; i< GPUCount; i++){

	    stat  = statisticsKernel[threadID].setArg(0, popBuffer[threadID]);
	    stat |= statisticsKernel[threadID].setArg(1, fitnessBuffer[threadID]);
	    stat |= statisticsKernel[threadID].setArg(2, statisticsBuffer[threadID]);
	    stat |= statisticsKernel[threadID].setArg(3, parameterBuffer[threadID]);

	    if(stat != CL_SUCCESS){
	    	cout << "Erro no setArg do kernel statistics." << endl;
	    	exit(0);
	    }

		NDRange local(min(32, m_popsize/GPUCount));
		NDRange global(2*local[0]);

		stat = GPUQueues[threadID].enqueueNDRangeKernel(statisticsKernel[threadID], NullRange, global, local, NULL, &evt);

		stat |= GPUQueues[threadID].enqueueReadBuffer(statisticsBuffer[threadID], CL_FALSE, 0, sizeof(t_stats), &m_stats[threadID]);
		stat |= GPUQueues[threadID].finish();

		if(stat != CL_SUCCESS){
			cout << "Erro no statisticsKernel ou statisticsBuffer." << endl;
			exit(0);
		}

	//Sincronismo
	GPUQueues[threadID].finish();

    #pragma omp barrier

	float melhor, pior;

    #pragma omp single
	{
		melhor = m_stats[0].afinidadeMelhor;
		pior   = m_stats[0].afinidadePior;

		for(int i=1; i < GPUCount; i++){
			if(m_stats[i].afinidadeMelhor > melhor){
				melhor = m_stats[i].afinidadeMelhor;
			}
			if(m_stats[i].afinidadePior < pior){
				pior = m_stats[i].afinidadePior;
			}
		}

		for(int i=0; i< GPUCount; i++){
			m_stats[threadID].afinidadeMelhor = melhor;
			m_stats[threadID].afinidadePior   = pior;
		}
	}

	#pragma omp barrier

	//for(int i=0; i< GPUCount; i++){
		stat = GPUQueues[threadID].enqueueWriteBuffer(statisticsBuffer[threadID], CL_FALSE, 0,
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

	int i = threadID;

	//for(int i=0; i< GPUCount; i++){
		fitnessKernel[i].setArg(0, popBuffer[i]);
		fitnessKernel[i].setArg(1, fitnessBuffer[i]);
		fitnessKernel[i].setArg(2, parameterBuffer[i]);

		//NDRange local(std::min(64, m_realLen));
		NDRange local(32);

		//int gsize = ceil((m_popsize*m_realLen/(float)local[0]))*local[0];

		NDRange global(m_popsize*local[0]/GPUCount);

		//double start = getRealTime();

		GPUQueues[i].enqueueNDRangeKernel(fitnessKernel[i], NullRange, global, local, NULL, &evt);
		//queue.finish();
	//}

	/*queue.enqueueReadBuffer(popBuffer, CL_FALSE, 0, m_popsize * m_realLen * sizeof(unsigned), pop);
	queue.enqueueReadBuffer(fitnessBuffer, CL_FALSE, 0, m_popsize * sizeof(float), fitness);
	queue.finish(); */

	//cout << "Elapsed time evt (fitnessKernel): " << getElapsedTime(evt) << endl;
	//PrintPop();
	//cout << "Elapsed time (fitnessKernel): " << getRealTime()-start << endl;
	//exit(0);
}


void ClonalgCL::Mutate(unsigned  * clone, float mutationRate)
{
}

void ClonalgCL::CalculateAffinity(unsigned * pop, float * fitness, float * fitnessNorm, int threadID){

	Event evt;

	int i = threadID;

	//for(int i=0; i<GPUCount; i++){

		calculateAffinityKernel[i].setArg(0, fitnessBuffer[i]);
		calculateAffinityKernel[i].setArg(1, fitnessNormBuffer[i]);
		calculateAffinityKernel[i].setArg(2, statisticsBuffer[i]);
		calculateAffinityKernel[i].setArg(3, parameterBuffer[i]);

		NDRange local(32);
		int gsize = ceil((m_popsize/(float)local[0]/GPUCount))*local[0];
		NDRange global(gsize);

		GPUQueues[i].enqueueNDRangeKernel(calculateAffinityKernel[i], NullRange, global, local, NULL, &evt);
	//}

	//cout << "Elapsed time (calculateAffinity): " << getElapsedTime(evt) << endl;

	/*queue.enqueueReadBuffer(fitnessNormBuffer, CL_FALSE, 0, m_popsize * sizeof(float), m_fitnessNorm);
	queue.finish();

	for(int i=0; i < m_popsize; i++){
		cout << "#" << i << " :" << m_fitnessNorm[i]<<endl;
	}
	exit(0);
	*/
}

void ClonalgCL::InitPopulation(unsigned ** pop, float **fitness, float ** fitnessNorm){}
void ClonalgCL::CloneAndHypermutate(unsigned * pop, float * fitness, float * fitnessNorm){}
