#include <CL/cl.h>
#include <stdio.h>
#include <string.h>
#include "individuo.h"

cl_device_id criaDevice();
cl_context criaContext(cl_device_id dev);
cl_program montaProgram(cl_context ctx, cl_device_id dev, const char *filename);
cl_command_queue criaCommandQueue(cl_context ctx, cl_device_id dev);
cl_kernel *criaKernels(cl_program program, cl_uint *numKernels);
cl_kernel encontraKernel(cl_kernel *kernels, cl_uint numKernels, char *kernelName);
void setArgumentsAvaliacao(cl_mem buffer_populacao, cl_mem buffer_tabela, cl_mem buffer_saidas,
    cl_mem buffer_pontos, cl_mem buffer_infos, cl_kernel *kernels);
void enqueueAvaliacao(cl_command_queue cmq, cl_kernel *kernels, size_t localSize,
    size_t globalSize);
void enqueueLeituraResultado(cl_command_queue cmq, cl_mem buffer_pontos, int* pontos,
cl_uint tamPontos);
void setArgumentsReducao(cl_mem buffer_pontos, cl_kernel *kernels);
void enqueueReducao(cl_command_queue cmq, cl_kernel *kernels, size_t localSize,
    size_t globalSize);
void printErrorString(cl_int error);