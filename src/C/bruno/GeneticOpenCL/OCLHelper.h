#ifndef GENETICOPENCL_OCLHELPER_H
#define GENETICOPENCL_OCLHELPER_H

#define __CL_ENABLE_EXCEPTIONS
#define CL_TARGET_OPENCL_VERSION 120

#include <CL/cl.hpp>

#include "utilitarios.h"

enum platformsIndex
{
    CPUPlatformIndex = 0,
    GPUPlatformIndex = 2
};

enum devicesIndex
{
    CPUDeviceIndex = 0,
    GPUDeviceIndex = 0
};

class OCLHelper {

private:
    unsigned int activeContext;
    std::string setProgramSource(int NUM_OPBIN, int NUM_OPUN, int M, int N, int localSize, float maxDados, float minDados);
    const char* getErrorString(cl_int error);
public:
    OCLHelper();

    cl_int error;

    std::vector<cl::Platform> platforms;
    std::vector<std::vector<cl::Device>> devices;
    std::vector<cl::Context> contexts;

    cl::Kernel krnlAvalia;
    cl::Kernel krnlEvolucao;
    cl::Kernel krnlSubstituicao;

    cl::Program::Sources source;
    cl::Program program;

    cl::CommandQueue cmdQueueEvol;
    cl::CommandQueue cmdQueueAval;
    cl::CommandQueue cmdQueue;

    size_t globalSizeEvol, localSizeEvol;
    size_t globalSizeAval, localSizeAval;

    std::string compileFlags;
    std::string program_src;

    void printPlatformsDevices();


    void setupContexts();
    void setupCommandQueues(cl_command_queue_properties commandQueueProperties);
    void setupProgramSource(std::string filename, int NUM_OPBIN, int NUM_OPUN, int M, int N, float maxDados, float minDados);
    void setWorkSizesAval(size_t numPoints);

};


#endif //GENETICOPENCL_OCLHELPER_H
