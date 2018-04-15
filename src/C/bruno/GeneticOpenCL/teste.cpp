cl_ulong inicio, fim;
///Evento para controlar tempo gasto
cl::Event e_tempo;

cl_int result; //Variavel para verificar erros
///TODO: Colocar conferencias de erros pelo c�digo

std::vector<cl::Platform> platforms;

#if !DIF_CONTEXT
std::vector<cl::Device> devices;
setupOpenCLOnePlatform(platforms,devices);
cl::Context contexto(devices, NULL, NULL, NULL, &result);
#else
std::vector<cl::Device> devicesCPU, devicesGPU;
setupOpenCLTwoPlatform(platforms,devicesCPU, devicesGPU);
cl::Context contextoCPU(devicesCPU, NULL, NULL, NULL, &result);
cl::Context contextoGPU(devicesGPU, NULL, NULL, NULL, &result);
#endif


if(result != CL_SUCCESS){
    std::cout << "Erro ao criar um contexto OpenCL" << std::endl;
    exit(1);
}

printPlatformsDevices(platforms, devices);

///Criando a fila de comando
cl_command_queue_properties commandQueueProperties = CL_QUEUE_PROFILING_ENABLE;

 ///4 op��es: GPU-GPU, GPU-CPU, CPU-CPU, CPU-GPU
///NUMDEVICES;FIRST

cl::CommandQueue* cmdQueueEvol;
cl::CommandQueue* cmdQueueAval;

#if !DIF_CONTEXT
setupCmdQueuesOnePlatform(cmdQueueAval, cmdQueueEvol, commandQueueProperties, devices, contexto);
#else
setupCmdQueuesTwoPlatform(cmdQueueAval, cmdQueueEvol, commandQueueProperties, &result, contextoCPU, contextoGPU);
#endif



cl::Buffer* bufferPopA, bufferPopF, bufferOpTerm, bufferSeeds, dados;
#if !DIF_CONTEXT
setupBuffersOnePlatform(bufferPopA, bufferPopF, bufferOpTerm, bufferSeeds, dados, contexto);
#else
setupBuffersTwoPlatform(bufferPopA, bufferPopF, bufferOpTerm, bufferSeeds, dados, contexto);
#endif

cmdQueueEvol->enqueueWriteBuffer(bufferSeeds, CL_FALSE, 0, NUM_INDIV * MAX_NOS * sizeof(int), seeds);
cmdQueueAval->enqueueWriteBuffer(dados, CL_TRUE, 0, M * N * sizeof(float), dadosTranspostos);
cmdQueueEvol->enqueueWriteBuffer(bufferOpTerm, CL_FALSE, 0, (NUM_OPBIN + NUM_OPUN + NUM_CTES + N - 1) * sizeof(int), conjuntoOpTerm);

cmdQueueAval->finish();
cmdQueueEvol->finish();

size_t globalSize;
size_t localSize;
size_t numPoints;
std::string compileFlags;

numPoints = (size_t)M;

size_t maxLocalSize = cmdQueueAval->getInfo<CL_QUEUE_DEVICE>().getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();
setNDRanges(&globalSize, &localSize, &compileFlags, maxLocalSize, numPoints, cmdQueueAval->getInfo<CL_QUEUE_DEVICE>().getInfo<CL_DEVICE_TYPE>());

std::cout << "Global Size = " << globalSize << std::endl << "Local size = " << localSize << std::endl;

///Leitura do arquivo com o programa em C++
std::ifstream sourceFileName("kernelAvalia.cl");
std::string sourceFile(std::istreambuf_iterator<char>(sourceFileName),(std::istreambuf_iterator<char>()));
std::string program_src = setProgramSource(NUM_OPBIN, NUM_OPUN, M, N, (int)localSize, maxDados, minDados) + sourceFile;

///Criar programa por Source
cl::Program::Sources source(1, std::make_pair(program_src.c_str(), program_src.length()+1));

#if !DIF_CONTEXT
cl::Program programa(contexto, source);
#else
cl::Program programaCPU(contextoCPU, source);
cl::Program programaGPU(contextoGPU, source);
#endif

//const char options[] = "-cl-opt-disable";
//int error;
//compileFlags+=" -cl-opt-disable";
std::cout << compileFlags << std::endl;
try {
    programa.build(devices, compileFlags.c_str());
} catch(cl::Error& e){
    std::cerr << getErrorString(e.err()) << std::endl
    << programa.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0]);
    std::cout << e.what() << " : " << e.err() << std::endl;
    exit(1);
}

try {
    programaCPU.build(devicesCPU, compileFlags.c_str());
    programaGPU.build(devicesGPU, compileFlags.c_str());
} catch(cl::Error& e){
    std::cerr << getErrorString(e.err()) << std::endl
    << programaCPU.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devicesCPU[0]);
    << programaGPU.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devicesGPU[0]);
    std::cout << e.what() << " : " << e.err() << std::endl;
    exit(1);
}

#if AVALGPU
    cl::Kernel krnlAvalia(programa, "avaliaIndividuos");
#elif AVALOCL
    cl::Kernel krnlAvalia(programa, "avaliaIndividuosCPU");
#else
    cl::Kernel krnlAvalia(programa, "avaliaIndividuosSequencial");
#endif // AVALGPU

#if EVOLOCL
    cl::Kernel krnlEvolucao(programa, "evolucao");
#else
    cl::Kernel krnlEvolucao(programa, "evolucaoSequencial");
#endif // EVOLOCL


/**----------------------------------------------------*/

#if AVALGPU
    cl::Kernel krnlAvalia(programaGPU, "avaliaIndividuos");
    cl::Kernel krnlEvolucao(programaCPU, "evolucao");
#else
    cl::Kernel krnlAvalia(programaCPU, "avaliaIndividuosCPU");
    cl::Kernel krnlEvolucao(programaGPU, "evolucao");
#endif // AVALGPU



#if TESTA_INDIV
        {
        Arvore arv;
        leIndividuo("individual.txt",&arv, LABELS, dadosTreinamento, M, N);
        char c;
        std::cin >> c;

        popFutura[0] = arv;
        popFutura[0].aptidao = -1;
        cmdQueueAval->enqueueWriteBuffer(bufferPopF, CL_TRUE, 0, NUM_INDIV * sizeof(Arvore), popFutura);
        krnlAvalia.setArg(0, bufferPopF);
        krnlAvalia.setArg(1, dados);
        krnlAvalia.setArg(2, localSize*sizeof(float), NULL);
        //std::cout << "aqui" << std::endl;
        try {
                result = cmdQueueAval->enqueueNDRangeKernel(krnlAvalia, cl::NullRange, cl::NDRange(globalSize), cl::NDRange(localSize), NULL, &e_tempo);
        } catch(cl::Error& e){
            std::cerr << getErrorString(e.err()) << std::endl;
            exit(1);
        }
        //std::cout << "aqui" << std::endl;
        try {
        cmdQueueAval->finish();
        } catch(cl::Error& e){
            std::cerr << getErrorString(e.err()) << std::endl;
            exit(1);
        }
        cmdQueueAval->enqueueReadBuffer(bufferPopF, CL_TRUE, 0, NUM_INDIV * sizeof(Arvore), popFutura);
        std::cout << popFutura[0].aptidao << std::endl;
        std::cin >> c;
        }
        #endif // TESTA_INDIV





        //seeds2 = (int*)cmdQueueEvol->enqueueMapBuffer(bufferSeeds, CL_TRUE, CL_MAP_WRITE, 0, NUM_INDIV * MAX_NOS * sizeof(int));
        //conjuntoOpTerm2 = (int*)cmdQueueEvol->enqueueMapBuffer(bufferOpTerm, CL_TRUE, CL_MAP_WRITE, 0, (NUM_OPBIN + NUM_OPUN + NUM_CTES + N - 1) * sizeof(int));
        //popAtual = (Arvore*)cmdQueueEvol->enqueueMapBuffer(bufferPopA, CL_TRUE, CL_MAP_WRITE, 0, NUM_INDIV * sizeof(Arvore));

//        for(int i = 0; i < NUM_INDIV*MAX_NOS; i++){
//            seeds2[i] = seeds[i];
//        }
//
//        for(int i = 0; i < (NUM_OPBIN+NUM_OPUN+NUM_CTES + N); i++){
//            conjuntoOpTerm2[i] = conjuntoOpTerm[i];
//        }

//        inicializaPopulacao(popAtual, conjuntoOpTerm, NUM_OPBIN, NUM_OPUN, N, &seeds[0], maxDados, minDados);
//        avaliaIndividuos(popAtual, /*dadosTranspostos*/dadosTreinamento, M, N);

//        cmdQueueEvol->enqueueUnmapMemObject(bufferSeeds, seeds2);
//        cmdQueueEvol->enqueueUnmapMemObject(bufferOpTerm, conjuntoOpTerm2);