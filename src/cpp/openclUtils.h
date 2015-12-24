/*
 * openclUtils.h
 *
 *  Created on: 29/01/2015
 *      Author: igorsr
 */

#ifndef OPENCLUTILS_H_
#define OPENCLUTILS_H_

#include <CL/cl.h>
#include <stdlib.h>
#include <stdio.h>


/* Macros para verificação e tratamento de erros */
#define log_arquivo() printf("Arquivo: %s, linha: %d\n",__FILE__, __LINE__)
#define log_error(M) printf("%s\n", M)
#define log_error_code(CODE) printf("Codigo: (%d)\n", CODE)
#define check_cl(STATUS,M) if(STATUS!=CL_SUCCESS){ log_error(M); log_error_code(STATUS); log_arquivo(); exit(EXIT_FAILURE); }

/*---------------------------------------------------------------------
 * Function:  clObtemPlataformas
 * Purpose:   Descoberta e inicialização das plataformas OpenCL
 * In args:   --
 * Out arg:   Array de cl_platform_id, contendo as plataformas disponíveis
 */
cl_platform_id * clObtemPlataformas();

/*---------------------------------------------------------------------
 * Function:  clObtemDispositivos
 * Purpose:   Descoberta e inicialização dos dispositivos de uma plataforma
 * In args:   plataformas: array de plataformas
 * Out arg:   Array de cl_device_id, contendo os dispositivos da plataforma
*/
cl_device_id * clObtemDispositivos(cl_platform_id * plataformas);

/*---------------------------------------------------------------------
 * Function:  clCompilaPrograma
 * Purpose:   Compila um programa OpenCL para os dispositivos informados.
 * In args:   programa: programa para compilação
              devices: array de dispositivos para os quais o programa 
              deve ser compilado 
 * Out arg:   --
*/
void clCompilaPrograma(cl_program * programa,                        
                       cl_device_id * devices);

#endif /* OPENCLUTILS_H_ */
