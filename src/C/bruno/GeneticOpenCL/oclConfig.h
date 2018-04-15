#ifndef OCLCONFIG_H_INCLUDED
#define OCLCONFIG_H_INCLUDED

#include <CL/cl.hpp>

#include <iostream>
#include <string>
#include <vector>

#include "constantes.h"
#include "utilitarios.h"

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
