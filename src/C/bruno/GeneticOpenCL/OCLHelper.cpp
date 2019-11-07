//
// Created by BrunoM on 29/08/2018.
//

#include "OCLHelper.h"

OCLHelper::OCLHelper()
{
    cl::Platform::get(&platforms);
    for(unsigned int i = 0; i < platforms.size(); i++)
    {
        devices.emplace_back(std::vector<cl::Device>());
        platforms[i].getDevices(CL_DEVICE_TYPE_ALL, &(devices[i]));
    }

    #if AVALGPU
        activeContext = GPUPlatformIndex;
    #else
        activeContext = CPUPlatformIndex;
    #endif
}

void OCLHelper::printPlatformsDevices()
{
    std::cout << "Available Platforms:" << std::endl;

    for(unsigned int i = 0; i < platforms.size(); ++i)
    {
        std::cout <<"\t[" << i << "]"<<platforms[i].getInfo<CL_PLATFORM_NAME>() << std::endl;
        for(unsigned int j = 0; j < devices[i].size(); j++)
        {
            std::cout <<"\t\t[" << j << "]"<< devices[i][j].getInfo<CL_DEVICE_NAME>() << std::endl;
        }
    }
    std::cout << std::endl;

    std::cout << "Metodo de execucao: " << std::endl;

    #if TWODEVICES
        #if AVALGPU
            std::cout << "\tEvolucao  CPU: " << devices[CPUPlatformIndex][CPUDeviceIndex].getInfo<CL_DEVICE_NAME>() << std::endl;
            std::cout << "\tAvaliacao GPU: " << devices[GPUPlatformIndex][GPUDeviceIndex].getInfo<CL_DEVICE_NAME>() << std::endl;
        #else
            std::cout << "\tEvolucao  GPU: " << devices[GPUPlatformIndex][GPUDeviceIndex].getInfo<CL_DEVICE_NAME>() << std::endl;
            std::cout << "\tAvaliacao CPU: " << devices[CPUPlatformIndex][CPUDeviceIndex].getInfo<CL_DEVICE_NAME>() << std::endl;
        #endif
    #else
        #if EVOLOCL
            std::cout << "\tEvolução " << ((AVALGPU)? "GPU: " + devices[GPUPlatformIndex][GPUDeviceIndex].getInfo<CL_DEVICE_NAME>()
                                                    : "CPU: " + devices[CPUPlatformIndex][CPUDeviceIndex].getInfo<CL_DEVICE_NAME>()) << std::endl;
        #else
            std::cout << "\tEvolução Sequencial" << std::endl;
        #endif

        #if AVALOCL
            std::cout << "\tAvaliação " << ((AVALGPU)? "GPU: " + devices[GPUPlatformIndex][GPUDeviceIndex].getInfo<CL_DEVICE_NAME>()
                                                     : "CPU: " + devices[CPUPlatformIndex][CPUDeviceIndex].getInfo<CL_DEVICE_NAME>()) << std::endl;
        #else
            std::cout << "\tAvaliação Sequencial" << std::endl;
        #endif
    #endif
    std::cout << std::endl;
}

void OCLHelper::setupContexts()
{
    for(unsigned int i = 0; i < devices.size(); i++)
    {
        contexts.emplace_back(cl::Context(devices[i], nullptr, nullptr, nullptr, &error));
        if(error != CL_SUCCESS){
            std::cout << "Erro ao criar um contexto OpenCL" << std::endl;
        }
    }
}

void OCLHelper::setupCommandQueues(cl_command_queue_properties commandQueueProperties)
{
    #if TWODEVICES
        #if AVALGPU
            cmdQueueEvol = cl::CommandQueue(contexts[CPUPlatformIndex], devices[CPUPlatformIndex][CPUDeviceIndex], commandQueueProperties, &error); /** SELECIONAR CPU */
            cmdQueueAval = cl::CommandQueue(contexts[GPUPlatformIndex], devices[GPUPlatformIndex][GPUDeviceIndex], commandQueueProperties, &error); /** SELECIONAR GPU*/
        #else
            cmdQueueEvol = cl::CommandQueue(contexts[GPUPlatformIndex], devices[GPUPlatformIndex][GPUDeviceIndex], commandQueueProperties, &error); /** SELECIONAR GPU*/
            cmdQueueAval = cl::CommandQueue(contexts[CPUPlatformIndex], devices[CPUPlatformIndex][CPUDeviceIndex], commandQueueProperties, &error); /** SELECIONAR CPU*/
        #endif // AVALGPU
    #else
        #if AVALGPU
            cmdQueue = cl::CommandQueue(contexts[GPUPlatformIndex], devices[GPUPlatformIndex][GPUDeviceIndex], commandQueueProperties, &error); /** SELECIONAR GPU */
        #else
            cmdQueue = cl::CommandQueue(contexts[CPUPlatformIndex], devices[CPUPlatformIndex][CPUDeviceIndex], commandQueueProperties, &error); /** SELECIONAR CPU*/
        #endif // AVALGPU
    #endif // TWODEVICES

}

void OCLHelper::setupProgramSource(std::string filename, int NUM_OPBIN, int NUM_OPUN, int M, int N, float maxDados, float minDados)
{
    ///Leitura do arquivo com o programa em C++
    std::ifstream sourceFileName(filename);
    std::string sourceFile(std::istreambuf_iterator<char>(sourceFileName),(std::istreambuf_iterator<char>()));
    program_src = setProgramSource(NUM_OPBIN, NUM_OPUN, M, N, localSizeAval, maxDados, minDados) + sourceFile;

    ///Criar programa por Source
    source = cl::Program::Sources(1, std::make_pair(program_src.c_str(), program_src.length()+1));
    program = cl::Program(contexts[activeContext], source);

    //compileFlags+=" -cl-opt-disable";
    compileFlags+= R"( -I C:\Users\bruno\Desktop\ciml-lib\src\C\bruno\GeneticOpenCL)";
    std::cout << "Compile Flags = " << compileFlags << std::endl;

    try {
        program.build(devices[activeContext], compileFlags.c_str());
    } catch(cl::Error& e) {
        std::cerr << getErrorString(e.err()) << std::endl
                  << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[activeContext][0]);
        std::cout << e.what() << " : " << e.err() << std::endl;
    }

    #if AVALGPU
    krnlAvalia = cl::Kernel(program, "avaliaIndividuos");
    #elif AVALOCL && !AVALOCL_SEQ
    krnlAvalia = cl::Kernel(program, "avaliaIndividuosCPU");
    #else
    krnlAvalia = cl::Kernel(program, "avaliaIndividuosSequencial");
    #endif

    #if EVOLOCL && !EVOLOCL_SEQ
    krnlEvolucao = cl::Kernel(program, "evolucao");
    #else
    krnlEvolucao = cl::Kernel(program, "evolucaoSequencial");
    #endif
}

void OCLHelper::setWorkSizesAval(size_t numPoints)
{
#if TWODEVICES
    cl_device_type deviceType = cmdQueueAval.getInfo<CL_QUEUE_DEVICE>().getInfo<CL_DEVICE_TYPE>();
    size_t maxLocalSize = cmdQueueAval.getInfo<CL_QUEUE_DEVICE>().getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();
#else
    cl_device_type deviceType = cmdQueue.getInfo<CL_QUEUE_DEVICE>().getInfo<CL_DEVICE_TYPE>();
    size_t maxLocalSize = cmdQueue.getInfo<CL_QUEUE_DEVICE>().getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();
#endif

    //FOR GPU
    if(deviceType == CL_DEVICE_TYPE_GPU){
        std::cout << "Definindo NDRanges para avaliacao em GPU...";
        if(numPoints < maxLocalSize){
            localSizeAval = numPoints;
        }else{
            localSizeAval = maxLocalSize;
        }

        // One individual per work-group
        globalSizeAval = (localSizeAval) * NUM_INDIV;

        if( MAX_NOS > (localSizeAval) ) //MaximumTreeSize() > m_local_size )
            (compileFlags) += " -D PROGRAM_TREE_DOES_NOT_FIT_IN_LOCAL_SIZE";

        if( IsPowerOf2( localSizeAval ) )
            (compileFlags) += " -D LOCAL_SIZE_IS_NOT_POWER_OF_2";

        if( numPoints % (localSizeAval) != 0 )
            (compileFlags) += " -D NUM_POINTS_IS_NOT_DIVISIBLE_BY_LOCAL_SIZE";

        ///FOR CPU
    } else if (deviceType == CL_DEVICE_TYPE_CPU){
        std::cout << "Definindo NDRanges para avaliacao em CPU...";
        localSizeAval = 1;//m_num_points;
        globalSizeAval = NUM_INDIV;
    }

    (compileFlags) += " -D LOCAL_SIZE_ROUNDED_UP_TO_POWER_OF_2="
                       + ToString( NextPowerOf2(localSizeAval) );

    std::cout << "...fim." << std::endl;
}

std::string OCLHelper::setProgramSource(int NUM_OPBIN, int NUM_OPUN, int M, int N, int localSize, float maxDados, float minDados)
{
    std::string program_src =
            "#define TIPO "  + ToString( TIPO ) + "\n" +
            "#define SEED "  + ToString( SEED ) + "\n" +
            "#define VAR  "  + ToString( VAR ) + "\n" +
            "#define CTE  "  + ToString( CTE ) + "\n" +
            "#define FBIN    "+ ToString( FBIN ) + "\n" +
            "#define FUN     "+ ToString( FUN ) + "\n" +
            "#define PLUS    "+ ToString( PLUS ) + "\n" +
            "#define MIN     "+ ToString( MIN ) + "\n" +
            "#define MULT    "+ ToString( MULT ) + "\n" +
            "#define DIV     "+ ToString( DIV ) + "\n" +
            "#define SIN     "+ ToString( SIN ) + "\n" +
            "#define COS     "+ ToString( COS ) + "\n" +
            "#define SQR     "+ ToString( SQR ) + "\n" +
            "#define EXP     "+ ToString( EXP ) + "\n" +
            "#define LOG10   "+ ToString( LOG10 ) + "\n" +
            "#define NUM_OPBIN   "+ ToString( NUM_OPBIN ) + "\n" +
            "#define NUM_OPUN   "+ ToString( NUM_OPUN ) + "\n" +
            "#define M   "+ ToString( M ) + "\n" +
            "#define N   "+ ToString( N ) + "\n" +
            "#define maxDados    "+ ToString( maxDados ) + "\n" +
            "#define minDados    "+ ToString( minDados) + "\n" +
            "#define MAX_NOS     "+ ToString( MAX_NOS ) + "\n" +
            "#define MAX_DEPTH   "+ ToString( MAX_DEPTH ) + "\n" +
            "#define MAX_FILHOS  "+ ToString( MAX_FILHOS ) + "\n" +
            "#define NUM_INDIV   "+ ToString( NUM_INDIV ) + "\n" +
            "#define PROB_CROSS  "+ ToString( PROB_CROSS ) + "\n" +
            "#define PROB_MUT    "+ ToString( PROB_MUT ) + "\n" +
            "#define NUM_TORNEIO     "+ ToString( NUM_TORNEIO ) + "\n" +
            "#define ELITISMO        "+ ToString( ELITISMO ) + "\n" +
            "#define NUM_GERACOES    "+ ToString( NUM_GERACOES) + "\n" +
            "#define LOCAL_SIZE " + ToString( localSize ) + "\n";

    return program_src;
}

const char* OCLHelper::getErrorString(cl_int error)
{
    switch(error){
        // run-time and JIT compiler errors
        case 0: return "CL_SUCCESS";
        case -1: return "CL_DEVICE_NOT_FOUND";
        case -2: return "CL_DEVICE_NOT_AVAILABLE";
        case -3: return "CL_COMPILER_NOT_AVAILABLE";
        case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
        case -5: return "CL_OUT_OF_RESOURCES";
        case -6: return "CL_OUT_OF_HOST_MEMORY";
        case -7: return "CL_PROFILING_INFO_NOT_AVAILABLE";
        case -8: return "CL_MEM_COPY_OVERLAP";
        case -9: return "CL_IMAGE_FORMAT_MISMATCH";
        case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
        case -11: return "CL_BUILD_PROGRAM_FAILURE";
        case -12: return "CL_MAP_FAILURE";
        case -13: return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
        case -14: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
        case -15: return "CL_COMPILE_PROGRAM_FAILURE";
        case -16: return "CL_LINKER_NOT_AVAILABLE";
        case -17: return "CL_LINK_PROGRAM_FAILURE";
        case -18: return "CL_DEVICE_PARTITION_FAILED";
        case -19: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";

            // compile-time errors
        case -30: return "CL_INVALID_VALUE";
        case -31: return "CL_INVALID_DEVICE_TYPE";
        case -32: return "CL_INVALID_PLATFORM";
        case -33: return "CL_INVALID_DEVICE";
        case -34: return "CL_INVALID_CONTEXT";
        case -35: return "CL_INVALID_QUEUE_PROPERTIES";
        case -36: return "CL_INVALID_COMMAND_QUEUE";
        case -37: return "CL_INVALID_HOST_PTR";
        case -38: return "CL_INVALID_MEM_OBJECT";
        case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
        case -40: return "CL_INVALID_IMAGE_SIZE";
        case -41: return "CL_INVALID_SAMPLER";
        case -42: return "CL_INVALID_BINARY";
        case -43: return "CL_INVALID_BUILD_OPTIONS";
        case -44: return "CL_INVALID_PROGRAM";
        case -45: return "CL_INVALID_PROGRAM_EXECUTABLE";
        case -46: return "CL_INVALID_KERNEL_NAME";
        case -47: return "CL_INVALID_KERNEL_DEFINITION";
        case -48: return "CL_INVALID_KERNEL";
        case -49: return "CL_INVALID_ARG_INDEX";
        case -50: return "CL_INVALID_ARG_VALUE";
        case -51: return "CL_INVALID_ARG_SIZE";
        case -52: return "CL_INVALID_KERNEL_ARGS";
        case -53: return "CL_INVALID_WORK_DIMENSION";
        case -54: return "CL_INVALID_WORK_GROUP_SIZE";
        case -55: return "CL_INVALID_WORK_ITEM_SIZE";
        case -56: return "CL_INVALID_GLOBAL_OFFSET";
        case -57: return "CL_INVALID_EVENT_WAIT_LIST";
        case -58: return "CL_INVALID_EVENT";
        case -59: return "CL_INVALID_OPERATION";
        case -60: return "CL_INVALID_GL_OBJECT";
        case -61: return "CL_INVALID_BUFFER_SIZE";
        case -62: return "CL_INVALID_MIP_LEVEL";
        case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
        case -64: return "CL_INVALID_PROPERTY";
        case -65: return "CL_INVALID_IMAGE_DESCRIPTOR";
        case -66: return "CL_INVALID_COMPILER_OPTIONS";
        case -67: return "CL_INVALID_LINKER_OPTIONS";
        case -68: return "CL_INVALID_DEVICE_PARTITION_COUNT";

            // extension errors
        case -1000: return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
        case -1001: return "CL_PLATFORM_NOT_FOUND_KHR";
        case -1002: return "CL_INVALID_D3D10_DEVICE_KHR";
        case -1003: return "CL_INVALID_D3D10_RESOURCE_KHR";
        case -1004: return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR";
        case -1005: return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR";
        default: return "Unknown OpenCL error";
    }
}

void OCLHelper::allocateBuffers(int NUM_OPBIN, int NUM_OPUN, int NUM_CTES, int M, int N)
{
    bufferPopA   = cl::Buffer(contexts[activeContext], CL_MEM_ALLOC_HOST_PTR | CL_MEM_READ_WRITE, NUM_INDIV * sizeof(Arvore));
    bufferPopF   = cl::Buffer(contexts[activeContext], CL_MEM_ALLOC_HOST_PTR | CL_MEM_READ_WRITE, NUM_INDIV * sizeof(Arvore));
    bufferOpTerm = cl::Buffer(contexts[activeContext], CL_MEM_ALLOC_HOST_PTR | CL_MEM_READ_ONLY, (NUM_OPBIN + NUM_OPUN + NUM_CTES + N - 1) * sizeof(int));
    bufferSeeds  = cl::Buffer(contexts[activeContext], CL_MEM_ALLOC_HOST_PTR | CL_MEM_READ_WRITE, NUM_INDIV * MAX_NOS * sizeof(int));
    data         = cl::Buffer(contexts[activeContext], CL_MEM_ALLOC_HOST_PTR | CL_MEM_READ_ONLY, M * N * sizeof(float));
}

