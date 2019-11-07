#ifndef OCLCONFIG_H_INCLUDED
#define OCLCONFIG_H_INCLUDED

#define CL_HPP_TARGET_OPENCL_VERSION 200
#define CL_HPP_MINIMUM_OPENCL_VERSION 120

#define CL_HPP_ENABLE_EXCEPTIONS

#include <CL/cl2.hpp>

#include "utilitarios.h"

#define GPU_PLATFORM 0
#define CPU_PLATFORM 1

#define GPU_DEVICE 0
#define CPU_DEVICE 1

const char *getErrorString(cl_int error);
void printPlatformsDevices(std::vector<cl::Platform> platforms, std::vector<cl::Device> devices);
void setupOpenCLOnePlatform(std::vector<cl::Platform> &platforms, std::vector<cl::Device> &devices);
void setupOpenCLTwoPlatform(std::vector<cl::Platform> &platforms, std::vector<cl::Device> &devicesCPU, std::vector<cl::Device> &devicesGPU);
void setNDRanges(size_t* globalSize, size_t* localSize, std::string* compileFlags, size_t maxLocalSize, size_t numPoints, cl_device_type deviceType);
std::string setProgramSource(int NUM_OPBIN, int NUM_OPUN, int M, int N, int localSize, float maxDados, float minDados);
void setupCmdQueuesOnePlatform(cl::CommandQueue* &cmdQueueAval, cl::CommandQueue* &cmdQueueEvol, cl_command_queue_properties commandQueueProperties, std::vector<cl::Device> &devices, cl::Context &contexto);
void setupCmdQueuesTwoPlatform(cl::CommandQueue* &cmdQueueAval, cl::CommandQueue* &cmdQueueEvol, cl_command_queue_properties commandQueueProperties,
                                std::vector<cl::Device> &devicesCPU, std::vector<cl::Device> &devicesGPU, cl::Context &contextoCPU, cl::Context &contextoGPU);



#endif // OCLCONFIG_H_INCLUDED
