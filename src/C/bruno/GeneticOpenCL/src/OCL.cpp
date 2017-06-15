#include "OCL.h"

OCL::OCL()
{
    //ctor
}

OCL::~OCL()
{
    //dtor
}


 void OCL::startCPU(){

 }

 void OCL::startKernel{
    ///Leitura do arquivo com o programa em C++
    std::ifstream sourceFileName("kernelAvalia.cl");
    std::string sourceFile(std::istreambuf_iterator<char>(sourceFileName),(std::istreambuf_iterator<char>()));
    std::string program_src = setProgramSource(NUM_OPBIN, NUM_OPUN, M, N, localSize) + sourceFile;
    //std::cout << program_src << std::endl;

    ///Criar programa por Source
    cl::Program::Sources source(1, std::make_pair(program_src.c_str(), program_src.length()+1));
    cl::Program programa(contexto, source);

    //const char options[] = "-cl-opt-disable";
    //int error;

    try {
        programa.build(devices/*, options*/);
    } catch(cl::Error& e){
        std::cerr << getErrorString(e.err()) << std::endl
        << programa.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[2]);
        std::cout << e.what() << " : " << e.err() << std::endl;
        exit(1);
    }

     krnlAvalia = new cl::Kernel(programa, "avaliaIndividuosCPU");
     krnlEvolucao = new cl::Kernel(programa, "evolucao");
 }

            setupOpenCL(platforms,devices);
            printPlatformsDevices(platforms, devices);

            ///Estabelecendo o contexto com os devices
            contexco = new cl::Context(devices, NULL, NULL, NULL, &result);
            if(result != CL_SUCCESS){
                std::cout << "Erro ao criar um contexto OpenCL" << std::endl;
                exit(1);
            }

            ///Criando a fila de comando para o device 0
            cl_command_queue_properties commandQueueProperties = CL_QUEUE_PROFILING_ENABLE;

            cmdQueueCPU = new cl::CommandQueue(contexto, devices[0], commandQueueProperties, &result);


            if(result != CL_SUCCESS){
                std::cout << "Erro ao criar a Command Queue" << std::endl;
                exit(1);
            }


            bufferPopA = new cl::Buffer(contexto, CL_MEM_READ_WRITE, NUM_INDIV * sizeof(Arvore)/*, popAtual*/);
            bufferPopF = new cl::Buffer(contexto, CL_MEM_READ_WRITE/*|CL_MEM_USE_HOST_PTR*/, NUM_INDIV * sizeof(Arvore)/*, popFutura*/);
            bufferOpTerm = new cl::Buffer(contexto, CL_MEM_READ_ONLY, (NUM_OPBIN+NUM_OPUN+NUM_CTES+N-1)*sizeof(int));
            bufferSeeds = new cl::Buffer(contexto, CL_MEM_READ_WRITE, NUM_INDIV*MAX_NOS*sizeof(int));
            dados = new cl::Buffer(contexto, CL_MEM_READ_ONLY, M*N * sizeof(float));



            //cmdQueueGPU.enqueueWriteBuffer(bufferPop, CL_TRUE, 0, NUM_INDIV * sizeof(Arvore), popFutura);
            cmdQueueCPU.enqueueWriteBuffer(bufferSeeds, CL_FALSE, 0, NUM_INDIV*MAX_NOS*sizeof(int), seeds);
            cmdQueueCPU.enqueueWriteBuffer(dados, CL_FALSE, 0, M*N * sizeof(float), dadosTranspostos);
            cmdQueueCPU.enqueueWriteBuffer(bufferOpTerm, CL_TRUE, 0, (NUM_OPBIN+NUM_OPUN+NUM_CTES+N-1)*sizeof(int), conjuntoOpTerm);




            size_t globalSize, localSize;
            size_t numPoints = M;
            std::string compileFlags;

            size_t maxLocalSize = devices[0].getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();
            setNDRanges(&globalSize, &localSize, &compileFlags, maxLocalSize, numPoints, devices[0].getInfo<CL_DEVICE_TYPE>());

