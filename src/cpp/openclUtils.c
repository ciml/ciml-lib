/*
 * openclUtils.c
 *
 *  Created on: 29/01/2015
 *      Author: igorsr
 */

#include "openclUtils.h"


cl_platform_id * clObtemPlataformas(){

	cl_int status;

	cl_uint numPlatforms = 0;
	cl_platform_id * platforms = NULL;

	status = clGetPlatformIDs(0, NULL, &numPlatforms);

	//Aloca espaço para cada plataforma
	platforms = (cl_platform_id*) malloc(numPlatforms*sizeof(cl_platform_id));

	//Obtém as plataformas
	status = clGetPlatformIDs(numPlatforms, platforms, NULL);

	check_cl(status, "Erro ao obter as plataformas disponiveis");

	return platforms;
}

cl_device_id * clObtemDispositivos(cl_platform_id * plataformas){

	cl_int status;
	cl_uint numDevices = 0;
	cl_device_id * devices = NULL;

	//Obtém o número de dispositivos na plataforma de índice 0
	status = clGetDeviceIDs(plataformas[0],
							CL_DEVICE_TYPE_DEFAULT,
							0,
							NULL,
							&numDevices);

	check_cl(status, "Erro ao descobrir os dispositivos");

	//Aloca espaço para cada dispositivo
	devices = (cl_device_id*) malloc(numDevices*sizeof(cl_device_id));

	//Obtém os dispositivos
	status = clGetDeviceIDs(plataformas[0],
							CL_DEVICE_TYPE_ALL,
							numDevices,
							devices,
							NULL);

	check_cl(status, "Erro ao obter os dispositivos");

	return devices;
}

void clCompilaPrograma(cl_program * programa, cl_device_id * devices){

    cl_int status;
    
    status = clBuildProgram(*programa,
							1,
							devices,
							"-I ./ ",
							NULL,
							NULL);

	if(status != CL_SUCCESS) {

		// Exibe os erros de compilação
		char buildLog[16384];
		clGetProgramBuildInfo(*programa, devices[0], CL_PROGRAM_BUILD_LOG,
							  sizeof(buildLog), buildLog, NULL);

		printf("Erros no kernel: \n %s \n", buildLog);

		clReleaseProgram(*programa);

		check_cl(status, "Erro ao compilar o programa");
	}	
}

