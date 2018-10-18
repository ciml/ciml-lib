#include "../include/paraleloGPU.h"

cl_device_id criaDevice()
{
	cl_platform_id platforms[2];
	cl_device_id device;
	cl_int err;

	/*Identify a platform*/
	err = clGetPlatformIDs(2, platforms, NULL);
	if (err < 0)
	{
		printErrorString(err);
		perror("Nenhuma plataforma encontrada!");
		exit(1);
	}

	/*Access a device type gpu if available, else access a device type cpu*/
	if(PLATFORM == 0)
	{
		err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, 1, &device, NULL);
	}
	else if(PLATFORM == 1)
	{
		err = clGetDeviceIDs(platforms[1], CL_DEVICE_TYPE_CPU, 1, &device, NULL);
	}
	if (err < 0)
	{
		printErrorString(err);
		perror("Nenhum device encontrado!");
		exit(1);
	}

	/*
	if (err == CL_DEVICE_NOT_FOUND)
	{
		err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, 1, &device, NULL);
	}
	*/

	return device;
}

cl_program montaProgram(cl_context ctx, cl_device_id dev, const char *filename)
{
	cl_program program;
	FILE *programHandle;
	char *programBuffer, *programLog;
	size_t programSize, logSize;
	cl_int err;

	/*open file*/
	programHandle = fopen(filename, "r");
	if (programHandle == NULL)
	{
		perror("Nao foi possivel abrir o arquivo!");
		exit(1);
	}
	
	/*goes to the end of file, get the size and returns to the beginning*/
	fseek(programHandle, 0, SEEK_END);
	programSize = ftell(programHandle);
	rewind(programHandle);

	/*read everything from file to programBuffer*/
	programBuffer = (char *)malloc(programSize + 1);
	programBuffer[programSize + 1] = '\0';
	if (fread(programBuffer, sizeof(char), programSize, programHandle))
	{
	};
	fclose(programHandle);

	/*Create program file*/
	program = clCreateProgramWithSource(ctx, 1, (const char **)&programBuffer,
										&programSize, &err);
	if (err < 0)
	{
		printErrorString(err);
		perror("Nao foi possivel criar o programa!");
	}
	free(programBuffer);

	/*Build program*/
	err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	if (err < 0)
	{
		printErrorString(err);
		/*get the program build log and print it*/
		clGetProgramBuildInfo(program, dev, CL_PROGRAM_BUILD_LOG, 0, NULL,
							  &logSize);
		programLog = (char *)malloc(logSize + 1);
		programLog[logSize + 1] = '\0';
		clGetProgramBuildInfo(program, dev, CL_PROGRAM_BUILD_LOG, logSize,
							  programLog, NULL);

		printf("%s\n", programLog);
		free(programLog);
		exit(1);
	}

	return program;
}

cl_kernel *criaKernels(cl_program program, cl_uint *numKernels)
{
	cl_kernel *kernels;
	cl_int err;


	err = clCreateKernelsInProgram(program, 0, NULL, numKernels);
	if (err < 0)
	{
		printErrorString(err);
		perror("Nenhum kernel encontrado!");
		exit(1);
	}
	kernels = (cl_kernel*)malloc(*numKernels * sizeof(cl_kernel));
	clCreateKernelsInProgram(program, *numKernels, kernels, NULL);
	/*
	char kernelName[100];
	for(int i = 0; i < *numKernels; i++)
	{
		clGetKernelInfo(kernels[i], CL_KERNEL_FUNCTION_NAME, sizeof(kernelName), kernelName, NULL);
		printf("%s\n", kernelName);
	}
	*/
	return kernels;
}

cl_kernel encontraKernel(cl_kernel *kernels, cl_uint numKernels, char *kernelName)
{
	char nameTemp[50];
	for (int i = 0; i < numKernels; i++)
	{
		clGetKernelInfo(kernels[i], CL_KERNEL_FUNCTION_NAME, sizeof(nameTemp),
						&nameTemp, 0);
		if (strcmp(nameTemp, kernelName) == 0)
		{
			return kernels[i];
		}
	}

	perror("Kernel desejado nao foi encontrado!");
	exit(1);

	return NULL;
}

cl_context criaContext(cl_device_id dev)
{
	cl_int err;
	cl_context context;

	context = clCreateContext(NULL, 1, &dev, NULL, NULL, &err);
	if(err < 0)
	{
		printErrorString(err);
		perror("Nao foi possivel criar context!");
		exit(1);
	}

	return context;
}

cl_command_queue criaCommandQueue(cl_context ctx, cl_device_id dev)
{
	cl_int err;
	cl_command_queue cmq;

	cmq = clCreateCommandQueueWithProperties(ctx, dev, 0, &err);
	if(err < 0)
	{
		printErrorString(err);
		perror("Nao foi possivel criar commandQueue!");
		exit(1);
	}

	return cmq;
}

void setArgumentsAvaliacao(cl_mem buffer_populacao, cl_mem buffer_tabela, cl_mem buffer_saidas,
    cl_mem buffer_pontos, cl_mem buffer_infos, cl_kernel *kernels)
{
	cl_int err;

	err = clSetKernelArg(kernels[0], 0, sizeof(cl_mem), &buffer_populacao);
	err |= clSetKernelArg(kernels[0], 1, sizeof(cl_mem), &buffer_tabela);
	err |= clSetKernelArg(kernels[0], 2, sizeof(cl_mem), &buffer_saidas);
	err |= clSetKernelArg(kernels[0], 3, sizeof(cl_mem), &buffer_pontos);
	err |= clSetKernelArg(kernels[0], 4, sizeof(cl_mem), &buffer_infos);
	if(err < 0)
	{
		printErrorString(err);
		perror("Nao foi possivel setar os argumentos!");
		exit(1);
	}
}

void enqueueAvaliacao(cl_command_queue cmq, cl_kernel *kernels, size_t localSize,
	size_t globalSize)
{
	cl_int err;

	err = clEnqueueNDRangeKernel(cmq, kernels[0], 1, NULL, &globalSize,
		&localSize, 0, NULL, NULL);
	if(err < 0)
	{
		printErrorString(err);
		perror("Nao foi possivel empilhar o kernel 1!");
		exit(1);
	}
}

void setArgumentsReducao(cl_mem buffer_pontos, cl_kernel *kernels)
{
	cl_int err;
	
	err = clSetKernelArg(kernels[1], 0, sizeof(cl_mem), &buffer_pontos);
	if (err < 0)
	{
		printErrorString(err);
		perror("Nao foi possivel setar os argumentos!");
		exit(1);
	}
}

void enqueueReducao(cl_command_queue cmq, cl_kernel *kernels, size_t localSize,
	size_t globalSize)
{
	cl_int err;

	err = clEnqueueNDRangeKernel(cmq, kernels[1], 1, NULL, &globalSize,
								 &localSize, 0, NULL, NULL);
	if (err < 0)
	{
		printErrorString(err);
		perror("Nao foi possivel empilhar o kernel 2!");
		exit(1);
	}
}
void enqueueLeituraResultado(cl_command_queue cmq, cl_mem buffer_pontos, int* pontos, cl_uint tamPontos)
{
	cl_int err;

	err = clEnqueueReadBuffer(cmq, buffer_pontos, CL_TRUE, 0, sizeof(int) * tamPontos,
		pontos, 0, NULL, NULL);
	if(err < 0)
	{
		printErrorString(err);
		perror("Nao foi possivel ler do buffer!");
		exit(1);
	}
}

void printErrorString(cl_int error)
{
    switch(error)
	{
        // run-time and JIT compiler errors
        case 0: printf("CL_SUCCESS\n");
        case -1: printf("CL_DEVICE_NOT_FOUND\n");
        case -2: printf("CL_DEVICE_NOT_AVAILABLE\n");
        case -3: printf("CL_COMPILER_NOT_AVAILABLE\n");
        case -4: printf("CL_MEM_OBJECT_ALLOCATION_FAILURE\n");
        case -5: printf("CL_OUT_OF_RESOURCES\n");
        case -6: printf("CL_OUT_OF_HOST_MEMORY\n");
        case -7: printf("CL_PROFILING_INFO_NOT_AVAILABLE\n");
        case -8: printf("CL_MEM_COPY_OVERLAP\n");
        case -9: printf("CL_IMAGE_FORMAT_MISMATCH\n");
        case -10: printf("CL_IMAGE_FORMAT_NOT_SUPPORTED\n");
        case -11: printf("CL_BUILD_PROGRAM_FAILURE\n");
        case -12: printf("CL_MAP_FAILURE\n");
        case -13: printf("CL_MISALIGNED_SUB_BUFFER_OFFSET\n");
        case -14: printf("CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST\n");
        case -15: printf("CL_COMPILE_PROGRAM_FAILURE\n");
        case -16: printf("CL_LINKER_NOT_AVAILABLE\n");
        case -17: printf("CL_LINK_PROGRAM_FAILURE\n");
        case -18: printf("CL_DEVICE_PARTITION_FAILED\n");
        case -19: printf("CL_KERNEL_ARG_INFO_NOT_AVAILABLE\n");

        // compile-time errors
        case -30: printf("CL_INVALID_VALUE\n");
        case -31: printf("CL_INVALID_DEVICE_TYPE\n");
        case -32: printf("CL_INVALID_PLATFORM\n");
        case -33: printf("CL_INVALID_DEVICE\n");
        case -34: printf("CL_INVALID_CONTEXT\n");
        case -35: printf("CL_INVALID_QUEUE_PROPERTIES\n");
        case -36: printf("CL_INVALID_COMMAND_QUEUE\n");
        case -37: printf("CL_INVALID_HOST_PTR\n");
        case -38: printf("CL_INVALID_MEM_OBJECT\n");
        case -39: printf("CL_INVALID_IMAGE_FORMAT_DESCRIPTOR\n");
        case -40: printf("CL_INVALID_IMAGE_SIZE\n");
        case -41: printf("CL_INVALID_SAMPLER\n");
        case -42: printf("CL_INVALID_BINARY\n");
        case -43: printf("CL_INVALID_BUILD_OPTIONS\n");
        case -44: printf("CL_INVALID_PROGRAM\n");
        case -45: printf("CL_INVALID_PROGRAM_EXECUTABLE\n");
        case -46: printf("CL_INVALID_KERNEL_NAME\n");
        case -47: printf("CL_INVALID_KERNEL_DEFINITION\n");
        case -48: printf("CL_INVALID_KERNEL\n");
        case -49: printf("CL_INVALID_ARG_INDEX\n");
        case -50: printf("CL_INVALID_ARG_VALUE\n");
        case -51: printf("CL_INVALID_ARG_SIZE\n");
        case -52: printf("CL_INVALID_KERNEL_ARGS\n");
        case -53: printf("CL_INVALID_WORK_DIMENSION\n");
        case -54: printf("CL_INVALID_WORK_GROUP_SIZE\n");
        case -55: printf("CL_INVALID_WORK_ITEM_SIZE\n");
        case -56: printf("CL_INVALID_GLOBAL_OFFSET\n");
        case -57: printf("CL_INVALID_EVENT_WAIT_LIST\n");
        case -58: printf("CL_INVALID_EVENT\n");
        case -59: printf("CL_INVALID_OPERATION\n");
        case -60: printf("CL_INVALID_GL_OBJECT\n");
        case -61: printf("CL_INVALID_BUFFER_SIZE\n");
        case -62: printf("CL_INVALID_MIP_LEVEL\n");
        case -63: printf("CL_INVALID_GLOBAL_WORK_SIZE\n");
        case -64: printf("CL_INVALID_PROPERTY\n");
        case -65: printf("CL_INVALID_IMAGE_DESCRIPTOR\n");
        case -66: printf("CL_INVALID_COMPILER_OPTIONS\n");
        case -67: printf("CL_INVALID_LINKER_OPTIONS\n");
        case -68: printf("CL_INVALID_DEVICE_PARTITION_COUNT\n");

        // extension errors
        case -1000: printf("CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR\n");
        case -1001: printf("CL_PLATFORM_NOT_FOUND_KHR\n");
        case -1002: printf("CL_INVALID_D3D10_DEVICE_KHR\n");
        case -1003: printf("CL_INVALID_D3D10_RESOURCE_KHR\n");
        case -1004: printf("CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR\n");
        case -1005: printf("CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR\n");
        default: printf("Unknown OpenCL error\n");
    }
}