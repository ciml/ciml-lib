#ifndef OPTIONS_HPP_INCLUDED
#define OPTIONS_HPP_INCLUDED

#define __CL_ENABLE_EXCEPTIONS



#include <CL/cl.hpp>
#include <omp.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <utility>
#include <cstdlib>
#include <iomanip>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif
#include "genetica.h"
#ifdef __cplusplus
}
#endif

const char *getErrorString(cl_int error);
double getTime();
bool IsPowerOf2( int n );
unsigned NextPowerOf2( unsigned n );
std::string ToString( float t );
void printPlatformsDevices(std::vector<cl::Platform> platforms, std::vector<cl::Device> devices);
void setupOpenCL(std::vector<cl::Platform> &platforms, std::vector<cl::Device> &devices);
void setNDRanges(size_t* globalSize, size_t* localSize, std::string* compileFlags, size_t maxLocalSize, size_t numPoints, cl_device_type deviceType);
std::string setProgramSource(int NUM_OPBIN, int NUM_OPUN, int M, int N, int localSize);

int all_sequential(int M, int N, int NUM_CTES, int NUM_OPBIN, int NUM_OPUN, char** LABELS, int* conjuntoOpTerm, Arvore* popAtual, Arvore* popFutura, float** dadosTreinamento, int* seeds);
int opencl_one_device(bool GPU);

#endif // OPTIONS_HPP_INCLUDED
