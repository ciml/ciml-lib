 #include "eg_opencl.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <CL/cl_ext.h>
#include "gramatica.h"
#include "utils.h"

#ifndef INT64_C
    #define INT64_C(c) (c ## LL)
    #define UINT64_C(c) (c ## ULL)
#endif

#define KERNEL_2_POR_WORK_ITEM 0
#define KERNEL_2_POR_WORK_GROUP 1
#define KERNEL_N_POR_WORK_GROUP 2
#define KERNEL_2_POR_WORK_ITEM_GRUPO_FIXO 3
#define KERNEL_1_POR_WORK_ITEM 4
#define KERNEL_2_POR_WORK_GROUP2 5

#include <vector>
#include <utility>
#include <cstdlib>
#include <math.h>
#include <sys/time.h>
#include "representacao.h"
#include "parser.h"

#define check_cl(STATUS,M) if(STATUS!=CL_SUCCESS){ log_error(M); log_error_code(STATUS); log_arquivo(); opencl_dispose(); exit(EXIT_FAILURE); }

using namespace std;

const char * kernel_srt;

std::string LoadKernel(std::string fileName)
{
    std::ifstream file(fileName.c_str());
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

double getRealTime()
{
    struct timeval tv;
    gettimeofday(&tv,0);
    return (double)tv.tv_sec + 1.0e-6*(double)tv.tv_usec;
}

template<class T> inline std::string ToString( const T& t )
{
   try {
      std::stringstream ss; ss << t; return ss.str();
   }
   catch( ... ) { return ""; }
}

int geracao = 1;

//Utilizado para tratamento de erros
cl_int status;

cl_device_id device;
cl_uint max_compute_units;


//Número de cores informado por parâmetro
int pcores;

//Indica se o dispositivo é cpu
short CPU;

//Kernel selecionado para as iterações do AG
int kernelAG;

int geracaoMelhor = 0;

cl_command_queue cmdQueue;
cl_context context = NULL;
cl_device_id * devices = NULL, * subDevices = NULL;
cl_uint numDevices = 0, numSubDevices=0;
cl_kernel kernelIteracao, kernelAvaliacao, kernelInicializacao, 
kernelSubstituicao, kernelInitializeEvaluatedNodes;
cl_kernel kernelSelecao, kernelCrossOverMutacao, kernelSumGPULongInt;
cl_program program;

size_t globalWorkSizeIteracao[1],localWorkSizeIteracao[1];
size_t datasize;

cl_mem bufferA, bufferB, bufferC, bufferCounter, bufferProgramas, 
bufferGramatica, bufferDatabase, bufferDSeeds, bufferNodeEvaluations;

int tamanhoBancoDeDados;

//Eventos utilizados para medir o tempo de execução do kernel
//e das trocas de memória
cl_event event1, event2, event3;

size_t preferred_workgroup_size_multiple, max_local_size;

float tempoTotal = 0;
int seed;

#ifdef PROFILING

float tempoTotalProcessamento=0;
float tempoInicializacaoPopulacao=0;
float tempoTotalAvaliacao=0;
float tempoTotalSubstituicao=0;
float tempoTotalAG=0;
float tempoTotalTransfMemoria=0;
float tempoTransfMemoriaInicial=0;
float tempoTotalSelecao=0;
float tempoTotalCrossOverMutacao=0;

#endif

/*
  Obtém o tempo decorrido entre o início e o fim de um evento (em picosegundos)
*/
float getTempoDecorrido(cl_event event){

    cl_ulong time_start, time_end;

    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);

    return time_end - time_start;
}

void CriaSubDevices(){

    //-----------------------
    // Criação dos subdevices
    //-----------------------
    status = clGetDeviceInfo(devices[0], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &max_compute_units, NULL);

    cl_uint ncores = std::min((int)max_compute_units, (int)pcores);

    //cout << "max comput units:" << ncores << endl;

    const cl_device_partition_property subdevice_properties[] =
    { CL_DEVICE_PARTITION_BY_COUNTS,
        ncores, 0, CL_DEVICE_PARTITION_BY_COUNTS_LIST_END, 0 };

    cl_device_id device_ids[1];
    numSubDevices=1;
    status = clCreateSubDevices(devices[0], subdevice_properties, numSubDevices, devices, NULL);
    device = devices[0];

    check_cl(status, "Erro ao criar os subdevices");
}

int binario_para_decimal2(short *binarios, int inicio, int fim){

    int i,n=1; int valorNumerico=0;

    for(i=fim-1; i>=inicio; i--, n=n<<1){

        valorNumerico += n*(binarios[i]);
    }

    return valorNumerico;
}

void gray_para_binario2(short *gray, short*binarios){

    int i,j;

    for(i=0; i< TAMANHO_INDIVIDUO; i++){
        binarios[i] = gray[i];
    }

    int start;
    int end = 0;
    for (j = 0; j < DIMENSOES_PROBLEMA; j++) {
        start = end;
        end += TAMANHO_VALOR;
        for (i = start + 1; i < end; i++) {
            binarios[i] = binarios[i - 1] ^ binarios[i];
        }
    }
}

void obtem_fenotipo_individuo2(individuo *p, short fenotipo[]){

    int i, j=0;

    short genotipo_binario[TAMANHO_INDIVIDUO];

    gray_para_binario2(p->genotipo, genotipo_binario);
    //binario_para_inteiro(p->genotipo_binario, p->fenotipo);

    for(i=0; i<DIMENSOES_PROBLEMA; i++, j+=TAMANHO_VALOR){

       fenotipo[i] = binario_para_decimal2(genotipo_binario, j, j+TAMANHO_VALOR);

    }
}


void opencl_init(int cores, int kernel, Database *dataBase, Database *testData){

    kernelAG = kernel;
    pcores   = cores;

    #ifdef cpu
        CPU = 1;
    #else
        CPU = 0;
    #endif

  //---------------------------------------------
	// 1: Descoberta e inicialização da(s) plataforma(s)
	//---------------------------------------------

	cl_uint numPlatforms = 0;
	cl_platform_id * platforms = NULL;

	status = clGetPlatformIDs(0, NULL, &numPlatforms);

	//cout << "Número de plataformas:" << numPlatforms << endl;

	//Aloca espaço para cada plataforma
	platforms = (cl_platform_id*) malloc(numPlatforms*sizeof(cl_platform_id));

	//Obtém as plataformas
	status = clGetPlatformIDs(numPlatforms, platforms, NULL);

    check_cl(status, "Erro ao obter as plataformas disponiveis");

   	//---------------------------------------------
	// 2: Descoberta e inicialização do(s) dispositivo(s)
	//---------------------------------------------

	//Obtém o número de dispositivos na plataforma de índice 0
	status = clGetDeviceIDs(platforms[0],
							CL_DEVICE_TYPE_ALL,
							0,
							NULL,
							&numDevices);

    check_cl(status, "Erro ao obter os dispositivos");


    //Aloca espaço para cada dispositivo
	devices = (cl_device_id*) malloc(numDevices*sizeof(cl_device_id));

	//Obtém os dispositivos
	status = clGetDeviceIDs(platforms[0],
							CL_DEVICE_TYPE_ALL,
							numDevices,
							devices,
							NULL);

    check_cl(status, "Erro ao obter os dispositivos");

    //---------------------------------------------
	// 3: Criação do contexto de execução
	//---------------------------------------------

	//Cria o contexto de execução, associando os dispositivos
	context = clCreateContext(NULL,
							 1,
						 	 devices,
						 	 NULL,
						 	 NULL,
						 	 &status);
    check_cl(status, "Erro ao criar o contexto de execução");

    clGetDeviceInfo(devices[0], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &max_local_size, NULL);

    pcores=1;

    //if(pcores>0) CriaSubDevices();

    device = devices[0];

    /* Consulta as propriedades do dispositivo */

    //-----------------------
    // 4: Criação da fila de execução
    //---------------------------------------------

    cmdQueue = clCreateCommandQueue(context,
									device,
                                    CL_QUEUE_PROFILING_ENABLE,
									&status);

    check_cl(status, "Erro ao criar a fila de execução");


    //-----------------------------------=----------
    // 5: Criação e compilação do programa
    //----------------------------------------------

    cl_ulong max_constant_buffer_size;

    clGetDeviceInfo(device, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, sizeof(cl_ulong),
                                 &max_constant_buffer_size, NULL);

    //printf("Max constant buffer size: %ld \n", (long)max_constant_buffer_size);

    std::string header_string = "#define NUMERO_DE_GERACOES " + ToString(NUMERO_DE_GERACOES) + " \n"+
                                "#define DIMENSOES_PROBLEMA " + ToString(DIMENSOES_PROBLEMA)  + "\n" +
                                "#define TAMANHO_POPULACAO " + ToString(TAMANHO_POPULACAO) + "\n" +
                                "#define TAMANHO_VALOR " + ToString(TAMANHO_VALOR) + "\n" +
                                //"#define TAMANHO_INDIVIDUO DIMENSOES_PROBLEMA*TAMANHO_VALOR  " + "\n" +
                                "#define TAMANHO_INDIVIDUO DIMENSOES_PROBLEMA " + "\n" +
                                "#define TAXA_DE_MUTACAO " + ToString(TAXA_DE_MUTACAO) + "\n" +
                                "#define TAXA_DE_RECOMBINACAO " + ToString(TAXA_DE_RECOMBINACAO )+ "\n" +
                                "#define TAMANHO_TORNEIO " + ToString(TAMANHO_TORNEIO) + "\n" +
                                "#define TAMANHO_GRAMATICA " + ToString(10) + "\n" +
                                "#define TAMANHO_DATABASE " + ToString(dataBase->numRegistros) + "\n" +
                                "#define NUM_VARIAVEIS " + ToString(dataBase->numVariaveis) + "\n" +
                                "#define ELITE " + ToString(ELITE) + "\n";


    long constant_size = sizeof(t_regra)*10 + (dataBase->numRegistros * sizeof(float));

    //if(constant_size > max_constant_buffer_size )
        header_string += " #define Y_DOES_NOT_FIT_IN_CONSTANT_BUFFER \n ";
    
    header_string += " #define LOCAL_SIZE_ROUNDED_UP_TO_POWER_OF_2 "
                      + ToString( next_power_of_2( max_local_size) ) + " \n ";

    if(dataBase->numRegistros % max_local_size != 0 )
        header_string += " #define NUM_POINTS_IS_NOT_DIVISIBLE_BY_LOCAL_SIZE \n";

    if( is_power_of_2( max_local_size ) )
        header_string += " #define LOCAL_SIZE_IS_NOT_POWER_OF_2 \n ";

    std::string body_string   = LoadKernel("kernel.cl");
    std::string kernel_string = header_string + body_string;

    kernel_srt = kernel_string.c_str();
 	  size_t programSize = (size_t)strlen(kernel_srt);

	//Cria o programa
	program = clCreateProgramWithSource(context,
							   1,
							   (const char **)&kernel_srt,
							   &programSize,
							   &status);

    check_cl(status, "Erro ao criar o programa");

    double start  = getRealTime();

	//Compilação do programa
	status = clBuildProgram(program,
                            1,
				            devices,
				            "-I ./ -I ./include",
				            NULL,
				            NULL);

    if(status != CL_SUCCESS) {

        // Exibe os erros de compilação
        char buildLog[16384];
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG,
                              sizeof(buildLog), buildLog, NULL);

        printf("Erros no kernel: \n %s \n", buildLog);

        clReleaseProgram(program);
    }  

    //printf("\nTempo de compilação: %lf\n", getRealTime()-start);    
    
    check_cl(status, "Erro ao compilar o programa");

    //---------------------------------------------
	// 6: Criação dos buffers de memória
	//---------------------------------------------

    datasize = sizeof(individuo)*TAMANHO_POPULACAO;

	bufferA = clCreateBuffer(context, CL_MEM_READ_WRITE, datasize, NULL, &status);
    check_cl(status, "Erro ao criar buffer de memoria bufferA");

    bufferB = clCreateBuffer(context, CL_MEM_READ_WRITE , datasize, NULL, &status);
    check_cl(status, "Erro ao criar buffer de memoria bufferB");

	bufferC = clCreateBuffer(context, CL_MEM_READ_WRITE, datasize, NULL, &status);
	check_cl(status, "Erro ao criar buffer de memoria bufferC");

    //bufferProgramas = clCreateBuffer(context, CL_MEM_READ_WRITE, TAMANHO_POPULACAO*sizeof(t_prog), NULL, &status);
    bufferGramatica = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(t_regra)*10, NULL, &status);
    check_cl(status, "Erro ao criar buffer de memoria bufferGramatica");
    
    if(testData == NULL || dataBase->numRegistros > testData->numRegistros)
	    bufferDatabase  = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float)*(dataBase->numRegistros)*(dataBase->numVariaveis), NULL, &status);
    else
    	bufferDatabase  = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float)*(testData->numRegistros)*(testData->numVariaveis), NULL, &status);
    check_cl(status, "Erro ao criar buffer de memoria bufferDatabase");

    /*if(kernelAG == KERNEL_2_POR_WORK_GROUP2){
        bufferCounter = clCreateBuffer(context, CL_MEM_READ_WRITE , TAMANHO_POPULACAO * sizeof(r123array4x32) * 4, NULL, &status);
    }
    else{
        bufferCounter = clCreateBuffer(context, CL_MEM_READ_WRITE , TAMANHO_POPULACAO * TAMANHO_INDIVIDUO * sizeof(r123array4x32), NULL, &status);
    }*/
    
    bufferDSeeds = clCreateBuffer(context, 
                                  CL_MEM_READ_WRITE, 
                                  TAMANHO_POPULACAO*TAMANHO_INDIVIDUO*sizeof(int), 
                                  NULL,
                                  &status);
    check_cl(status, "Erro ao criar buffer de memoria bufferCounter");		
	                                  
                                  
	bufferNodeEvaluations = clCreateBuffer(context, 
						                  CL_MEM_READ_WRITE, 
						                  TAMANHO_POPULACAO*sizeof(long int), 
						                  NULL,
						                  &status);

    check_cl(status, "Erro ao criar buffer de memoria bufferNodeEvaluations");

    //---------------------------------------------
	// 7: Criação do kernel
    //---------------------------------------------
    
    kernelInicializacao = clCreateKernel(program, "inicializa_populacao", &status);

    check_cl(status, "Erro ao criar kernel kernelInicializacao");

    if(CPU){

        status = clGetDeviceInfo(devices[0], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint),
                                 &max_compute_units, NULL);

        preferred_workgroup_size_multiple = (size_t)max_compute_units;
    }
    else{

        //Obtém o tamanho  do qual o groupsize deve ser múltiplo
        status = clGetKernelWorkGroupInfo(kernelInicializacao, device,
                                          CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE,
                                          sizeof(size_t),
                                          &preferred_workgroup_size_multiple,
                                          NULL);
                                          
    }

    //Cria o kernel de avaliação

    if(CPU){
        kernelAvaliacao = clCreateKernel(program, "avaliacao", &status);
    }
    else{
        kernelAvaliacao = clCreateKernel(program, "avaliacao_gpu", &status);
    }

    check_cl(status, "Erro ao criar kernel 'avaliacao'");

    kernelSubstituicao = clCreateKernel(program, "substituicao", &status);
    check_cl(status, "Erro ao criar kernel 'substituicao'");
    
    kernelSelecao = clCreateKernel(program, "selecao", &status);
    check_cl(status, "Erro ao criar kernel 'selecao'");
    
    kernelCrossOverMutacao = clCreateKernel(program, "crossOverEMutacao", &status);
    check_cl(status, "Erro ao criar kernel 'crossOverEMutacao'");    
    
    kernelInitializeEvaluatedNodes = clCreateKernel(program, "initializeEvaluatedNodes", &status);
    check_cl(status, "Erro ao criar kernel 'initializeEvaluatedNodes'");   
    
    kernelSumGPULongInt = clCreateKernel(program, "sumGPULongInt", &status);
	check_cl(status, "Erro ao criar kernel 'sumGPULongInt'");
}

void exibe_melhor(individuo * melhor, t_regra * gramatica){

    check(melhor != 0, "Indivíduo não pode ser nulo");
    check(gramatica != 0, "Gramática não pode ser nula");

    /*if(melhor->aptidao==0){
        printf("Geracoes para encontrar o melhor: \t %d\n", geracao);

        int i;

        short genotipo_binario[TAMANHO_INDIVIDUO];

        exit(EXIT_SUCCESS);
    }    
    */
    
    //printf("---------------------------------");
    //printf("\nGeracao %d: \n", geracao);
    //printf("\nMelhor da geracao: %d: %E\n", geracao, melhor->aptidao);

    t_item_programa programa[TAMANHO_MAX_PROGRAMA];
    //  short fenotipo[DIMENSOES_PROBLEMA];
    //    obtem_fenotipo_individuo2(melhor, fenotipo);

    Decodifica(gramatica, melhor->genotipo, programa);

    ImprimeInfixa(programa);
    //printf("\n");
}

void exibePopulacao(individuo * populacao){

    int i,j;

    printf("\n");

    for(i=0; i < TAMANHO_POPULACAO; i++){

        for(j=0; j < TAMANHO_INDIVIDUO; j++){
               printf(" %d ", populacao[i].genotipo[j]);
        }
        printf("%d \t %f \n",i, populacao[i].aptidao);
    }
}

void carrega_gramatica(t_regra * gramatica){

    cl_event eventoCargaGramatica;
    
    //cout << "Espaço utilizado pela gramática: " << sizeof(t_regra)*5 << endl;
    
    status = clEnqueueWriteBuffer(cmdQueue,
                                      bufferGramatica,
                                      CL_FALSE,
                                      0,
                                      sizeof(t_regra)*5,
                                      gramatica,
                                      0,
                                      NULL,
                                      &eventoCargaGramatica);
    check_cl(status, "Erro ao carregar o buffer da gramatica");

    //Espera o término da fila de execução
    status = clFinish(cmdQueue);
    check_cl(status, "Erro ao carregar o buffer da gramatica");

    #ifdef PROFILING

    float tempoCarga = getTempoDecorrido(eventoCargaGramatica) / 1000000000.0 ;
    tempoTotal += tempoCarga;
    tempoTotalTransfMemoria += tempoCarga;
    tempoTransfMemoriaInicial += tempoCarga;

    #endif
}


void carrega_bancoDeDados(Database *dataBase){

    cl_event eventoCargaBanco;

    status = clEnqueueWriteBuffer(cmdQueue,
                                  bufferDatabase,
                                  CL_FALSE,
                                  0,
                                  sizeof(float)*(dataBase->numRegistros)*(dataBase->numVariaveis),
                                  dataBase->registros,
                                  0,
                                  NULL,
                                  &eventoCargaBanco);

    check_cl(status, "Erro ao carregar o buffer do banco de dados");

    //Espera o término da fila de execução
    status = clFinish(cmdQueue);
    check_cl(status, "Erro ao carregar o buffer do banco de dados");

    #ifdef PROFILING

    float tempoCarga = getTempoDecorrido(eventoCargaBanco) / 1000000000.0 ;
    tempoTotal += tempoCarga;
    tempoTotalTransfMemoria += tempoCarga;
    tempoTransfMemoriaInicial += tempoCarga;

    #endif
}

void carrega_seeds(){
    
    int host_Seeds[TAMANHO_POPULACAO*TAMANHO_INDIVIDUO];    
    int i;
   
    //cout << "Gerando seeds..." << endl;
   
    for(i=0; i < TAMANHO_POPULACAO*TAMANHO_INDIVIDUO; i++){
        host_Seeds[i] = rand();
        //cout << host_Seeds[i] << " " << endl;
    }
    
    //cout << "Copiando seeds para o dispositivo..." << endl;
    
    cl_event eventoCarga;

    status = clEnqueueWriteBuffer(cmdQueue,
                                  bufferDSeeds,
                                  CL_FALSE,
                                  0,
                                  sizeof(host_Seeds),
                                  &host_Seeds,
                                  0,
                                  NULL,
                                  &eventoCarga);

    check_cl(status, "Erro ao carregar o buffer de seeds");

    //Espera o término da fila de execução
    status = clFinish(cmdQueue);
    check_cl(status, "Erro ao carregar o buffer de seeds");

    #ifdef PROFILING

        float tempoCarga = getTempoDecorrido(eventoCarga) / 1000000000.0 ;
        tempoTotal += tempoCarga;
        tempoTotalTransfMemoria += tempoCarga;
        tempoTransfMemoriaInicial += tempoCarga;

    #endif
}

void initializEvaluatedNodes()
{
	cl_event event;

    status = clSetKernelArg(kernelInitializeEvaluatedNodes,
							0,
							sizeof(bufferNodeEvaluations),
							&bufferNodeEvaluations);
    check_cl(status, "Erro ao adicionar 0 argumento ao kernel");

    size_t globalWorkSize[1] = {TAMANHO_POPULACAO};

    status = clEnqueueNDRangeKernel(cmdQueue,
									kernelInitializeEvaluatedNodes,
									1,
                                    NULL,
									globalWorkSize,
									NULL,
									0,
									NULL,
									&event);
    check_cl(status, "Erro ao enfileirar o kernel para execucao");

	clFinish(cmdQueue);	
	
	/*//Verify
	
	long int evaluatedNodes[TAMANHO_POPULACAO];
	
	status = clEnqueueReadBuffer(cmdQueue, bufferNodeEvaluations, CL_TRUE, 0,
						 TAMANHO_POPULACAO*sizeof(long int), evaluatedNodes, 0, NULL, &event);

	 check_cl(status, "Erro ao enfileirar leitura de buffer de memoria");
	 
	 clFinish(cmdQueue);
	 
	 for(int i=0; i < TAMANHO_POPULACAO; i++)
	 {
	 	printf("%ld ", evaluatedNodes[i]);
	 }
     
	printf("\n");
	*/
	
	#ifdef PROFILING
	  float time = getTempoDecorrido(event) / 1000000000.0 ;
	  tempoTotal += time;
	  tempoTotalProcessamento += time;
	#endif
}

long int evaluatedNodesReduction()
{
	cl_event event;
	
	/*long int evaluatedNodes[TAMANHO_POPULACAO];
	
	status = clEnqueueReadBuffer(cmdQueue, bufferNodeEvaluations, CL_TRUE, 0,
						 TAMANHO_POPULACAO*sizeof(long int), evaluatedNodes, 0, NULL, &event);

 	check_cl(status, "Erro ao enfileirar leitura de buffer de memoria");
	 
 	clFinish(cmdQueue);
	 
	for(int i=0; i < TAMANHO_POPULACAO; i++)
	{
		printf("%ld ", evaluatedNodes[i]);
 	}     
	printf("\n");*/	
	
    size_t localWorkSize[1] = {1024};    
	size_t globalWorkSize[1] = {localWorkSize[0]};	

    status = clSetKernelArg(kernelSumGPULongInt,
							0,
							sizeof(bufferNodeEvaluations),
							&bufferNodeEvaluations);
    check_cl(status, "Erro ao adicionar 0 argumento ao kernel");
    
 	status = clSetKernelArg(kernelSumGPULongInt,  1, sizeof(long int)*localWorkSize[0],  NULL);
    check_cl(status, "Erro ao adicionar argumento ao kernel");

    status = clEnqueueNDRangeKernel(cmdQueue,
									kernelSumGPULongInt,
									1,
                                    NULL,
									globalWorkSize,
									localWorkSize,
									0,
									NULL,
									&event);
    check_cl(status, "Erro ao enfileirar o kernel para execucao");
	clFinish(cmdQueue);	
	
	long int evaluatedNodes[1];
	
	status = clEnqueueReadBuffer(cmdQueue, bufferNodeEvaluations, CL_TRUE, 0,
						 	1*sizeof(long int), evaluatedNodes, 0, NULL, &event);

 	check_cl(status, "Erro ao enfileirar leitura de buffer de memoria");
     
 	clFinish(cmdQueue);
     
	//printf("Valor total: %ld\n", evaluatedNodes[0]);
	 
	#ifdef PROFILING
	  float time = getTempoDecorrido(event) / 1000000000.0 ;
	  tempoTotal += time;
	  tempoTotalProcessamento += time;
	#endif
	
	return evaluatedNodes[0];
}


void inicializa_populacao(individuo * pop){

    cl_event eventoInicializacao;

    status = clSetKernelArg(kernelInicializacao,
							0,
							sizeof(bufferA),
							&bufferA);
    check_cl(status, "Erro ao adicionar 0 argumento ao kernel");


     status = clSetKernelArg(kernelInicializacao,
							1,
							sizeof(bufferDSeeds),
							&bufferDSeeds);
    check_cl(status, "Erro ao adicionar 1 argumento ao kernel");

    size_t globalWorkSize[1] = {TAMANHO_POPULACAO*TAMANHO_INDIVIDUO};
    size_t localWorkSize[1]  = {TAMANHO_INDIVIDUO};

    status = clEnqueueNDRangeKernel(cmdQueue,
									kernelInicializacao,
									1,
                                    NULL,
									globalWorkSize,
									localWorkSize,
									0,
									NULL,
									&eventoInicializacao);
    check_cl(status, "Erro ao enfileirar o kernel para execucao");

    //Espera o término da fila de execução
	clFinish(cmdQueue);

	#ifdef PROFILING

      tempoInicializacaoPopulacao = getTempoDecorrido(eventoInicializacao) / 1000000000.0 ;
      tempoTotal += tempoInicializacaoPopulacao;
      tempoTotalProcessamento += tempoInicializacaoPopulacao;

    #endif
}

void avaliacao(individuo *pop, t_regra * gramatica, cl_mem bufferPop, int popCount, size_t offset){

    cl_event eventoAvaliacao;

    status = clSetKernelArg(kernelAvaliacao,  0, sizeof(bufferPop), &bufferPop);
    check_cl(status, "Erro ao adicionar argumento ao kernel");

    status = clSetKernelArg(kernelAvaliacao,  1, sizeof(bufferGramatica), &bufferGramatica);
    check_cl(status, "Erro ao adicionar argumento ao kernel");

    status = clSetKernelArg(kernelAvaliacao,  2, sizeof(bufferDatabase),  &bufferDatabase);
    check_cl(status, "Erro ao adicionar argumento ao kernel");
    
    if(!CPU)
    {
        size_t localWorkSize[1];
        size_t globalWorkSize[1];

        max_local_size = 1024;

        if(tamanhoBancoDeDados < max_local_size)
            localWorkSize[0] = tamanhoBancoDeDados;
        else
            localWorkSize[0] = max_local_size;

        //Um indivíduo por work-group
        globalWorkSize[0] = localWorkSize[0] * popCount;

        status = clSetKernelArg(kernelAvaliacao,  3, sizeof(float)*localWorkSize[0],  NULL);
        check_cl(status, "Erro ao adicionar argumento ao kernel");
        
        status = clSetKernelArg(kernelAvaliacao,
							4, sizeof(bufferNodeEvaluations),
							&bufferNodeEvaluations);
		check_cl(status, "Erro ao adicionar argumento ao kernel");
        

        status = clEnqueueNDRangeKernel(cmdQueue,
                               kernelAvaliacao,
                               1,
                               &offset,
                               globalWorkSize,
                               localWorkSize,
                               0,
                               NULL,
                               &eventoAvaliacao);
     }
     else{
     
        size_t localWorkSize[1] = {1};
        size_t globalWorkSize[1];
        
        globalWorkSize[0] = ceil((float)popCount/localWorkSize[0])*localWorkSize[0];
        
     	status = clSetKernelArg(kernelAvaliacao,
							3, sizeof(bufferNodeEvaluations),
							&bufferNodeEvaluations);
		check_cl(status, "Erro ao adicionar argumento ao kernel");
        

        status = clEnqueueNDRangeKernel(cmdQueue,
                               kernelAvaliacao,
                               1,
                               &offset,
                               globalWorkSize,
                               localWorkSize,
                               0,
                               NULL,
                               &eventoAvaliacao);
     }

     check_cl(status, "Erro ao enfileirar o kernel para execucao");

     /* status = clEnqueueReadBuffer(cmdQueue, bufferPop, CL_TRUE, 0,
						 datasize, pop, 0, NULL, &event3);
     check_cl(status, "Erro ao enfileirar leitura de buffer de memoria");*/

     clFinish(cmdQueue);

     #ifdef PROFILING
     
        float tempoAvaliacao = getTempoDecorrido(eventoAvaliacao) / 1000000000.0 ;
        //printf("Tempo de avaliacao: %f\n", tempoAvaliacao);
        tempoTotal += tempoAvaliacao;
        tempoTotalAvaliacao += tempoAvaliacao;
        tempoTotalProcessamento += tempoAvaliacao;

     #endif     
}

void substituicao(individuo *pop, t_regra * gramatica, individuo * melhor1, individuo * melhor2){
    
    cl_event eventoSubstituicao;

    status = clSetKernelArg(kernelSubstituicao,  0, sizeof(bufferA), &bufferA);
    check_cl(status, "Erro ao adicionar argumento 0 ao kernel");

    status = clSetKernelArg(kernelSubstituicao,  1, sizeof(bufferB), &bufferB);
    check_cl(status, "Erro ao adicionar argumento 1 ao kernel");

    status = clSetKernelArg(kernelSubstituicao,  2, sizeof(bufferC), &bufferC);
    check_cl(status, "Erro ao adicionar argumento 2 ao kernel");

    size_t globalWorkSize[1] = {TAMANHO_POPULACAO*128};
    size_t localWorkSize[1]  = {128};

    clEnqueueNDRangeKernel(cmdQueue,
                               kernelSubstituicao,
                                1,
                               NULL,
                               globalWorkSize,
                               localWorkSize,
                               0,
                               NULL,
                               &eventoSubstituicao);

    check_cl(status, "Erro ao enfileirar o kernel para execucao");

    /*
        Troca os buffers A e C, de forma que a população gerada na etapa de substituição
        passe a ser a população atual
    */

    cl_mem aux = bufferA;
    bufferA = bufferC;
    bufferC = aux;

    size_t offset = {sizeof(individuo) * ELITE};

    clEnqueueReadBuffer(cmdQueue, bufferA, CL_TRUE, 0,
                        datasize, pop,
                        0, NULL, &event3);    

    cl_event eventoTroca1, eventoTroca2;

    check_cl(status, "Erro ao enfileirar leitura de buffer de memória");

    clEnqueueReadBuffer(cmdQueue, bufferA, CL_FALSE, 0,
                        sizeof(individuo), melhor1,
                        0, NULL, &eventoTroca1);
    check_cl(status, "Erro ao enfileirar leitura de buffer de memória");


    clEnqueueReadBuffer(cmdQueue, bufferA, CL_FALSE, offset,
                        sizeof(individuo), melhor2,
                        0, NULL, &eventoTroca2);
    check_cl(status, "Erro ao enfileirar leitura de buffer de memória");

    clFinish(cmdQueue);

    /*if(melhor1[0].aptidao > melhor2[0].aptidao){
       exibe_melhor(melhor1, gramatica);
    }
    else{
       exibe_melhor(melhor2, gramatica);
    }*/

    #ifdef PROFILING

    float tempoSubs = getTempoDecorrido(eventoSubstituicao) / 1000000000.0 ;
    //tempoTotal += tempoSubs;
    tempoTotalProcessamento += tempoSubs;
    tempoTotalSubstituicao += tempoSubs;

    //printf("Tempo de substituicao: \t %.10f\n", tempoSubs);

    float troca1 =  getTempoDecorrido(eventoTroca1) / 1000000000.0;
    float troca2 =  getTempoDecorrido(eventoTroca2) / 1000000000.0;

    tempoTotalTransfMemoria+=troca1+troca2;

    #endif  
}


void iteracao(individuo * populacao, t_regra * gramatica, individuo *melhor1,individuo *melhor2)
{
    cl_event eventoSelecao, eventoCrossOver;


    status = clSetKernelArg(kernelSelecao,
                            0,
                            sizeof(bufferA),
                            &bufferA);
    check_cl(status, "Erro ao adicionar argumento ao kernel");

    /*status = clSetKernelArg(kernelSelecao,
                            1,
                            sizeof(geracao),
                            &geracao);
    check_cl(status, "Erro ao adicionar argumento ao kernel");*/
    
    
   /* status = clSetKernelArg(kernelSelecao,
                            1,
                            sizeof(seed),
                            &seed);
    check_cl(status, "Erro ao adicionar argumento ao kernel");*/
   

    status =  clSetKernelArg(kernelSelecao,
                            1,
                            sizeof(bufferB),
                            &bufferB);
    check_cl(status, "Erro ao adicionar argumento ao kernel");


    status = clSetKernelArg(kernelSelecao,
							2,
							sizeof(bufferDSeeds),
							&bufferDSeeds);
    check_cl(status, "Erro ao adicionar 2 argumento ao kernel");
   

   /* status = clSetKernelArg(kernelSelecao,
                            4,
                            sizeof(bufferCounter),
                            &bufferCounter);
    check_cl(status, "Erro ao adicionar argumento ao kernel"); */   

    
    //localWorkSizeIteracao[0]  = TAMANHO_INDIVIDUO/2;
    localWorkSizeIteracao[0]  = TAMANHO_INDIVIDUO;
    globalWorkSizeIteracao[0] = localWorkSizeIteracao[0]*TAMANHO_POPULACAO;
    

    status = clEnqueueNDRangeKernel(cmdQueue,
                                kernelSelecao,
                                1,
                                NULL,
                                globalWorkSizeIteracao,
                                localWorkSizeIteracao,
                                0,
                                NULL,
                                &eventoSelecao);
    

    check_cl(status, "Erro ao enfileirar o kernel para execucao");

    /*status = clEnqueueReadBuffer(cmdQueue, bufferB, CL_TRUE, 0,
						 datasize, populacao, 0, NULL, &event3);
    check_cl(status, "Erro ao enfileirar leitura de buffer de memoria");
    */

    //Espera o término da fila de execução
    clFinish(cmdQueue);
    
   	/* int i;    
    for(i=0;i<TAMANHO_POPULACAO;i++){
        int j;
        for(j=0;j<TAMANHO_INDIVIDUO/2;j++){
            cout <<  populacao[i].genotipo[j] << " ";
        }
        cout << endl;        
    } */
    
    size_t localWorkSizeCrossOver[1]  = {TAMANHO_INDIVIDUO};
    size_t globalWorkSizeCrossOver[1] = {(TAMANHO_POPULACAO*TAMANHO_INDIVIDUO)/2};    
    

    status = clSetKernelArg(kernelCrossOverMutacao,
                            0,
                            sizeof(bufferA),
                            &bufferA);
    check_cl(status, "Erro ao adicionar argumento ao kernel"); 
    
   /* status = clSetKernelArg(kernelCrossOverMutacao,
                            1,
                            sizeof(geracao),
                            &geracao);
    check_cl(status, "Erro ao adicionar argumento ao kernel");    
    
    
    status = clSetKernelArg(kernelCrossOverMutacao,
                            2,
                            sizeof(seed),
                            &seed);
    check_cl(status, "Erro ao adicionar argumento ao kernel");*/

    status =  clSetKernelArg(kernelCrossOverMutacao,
                            1,
                            sizeof(bufferB),
                            &bufferB);
    check_cl(status, "Erro ao adicionar argumento ao kernel");

    
   /* status = clSetKernelArg(kernelCrossOverMutacao,
                            4,
                            sizeof(bufferCounter),
                            &bufferCounter);
    check_cl(status, "Erro ao adicionar argumento ao kernel");*/
    
    
     status = clSetKernelArg(kernelCrossOverMutacao,
							2,
							sizeof(bufferDSeeds),
							&bufferDSeeds);
    check_cl(status, "Erro ao adicionar 0 argumento ao kernel");
    
    
	status = clEnqueueNDRangeKernel(cmdQueue,
                                kernelCrossOverMutacao,
                                1,
                                NULL,
                                globalWorkSizeCrossOver,
                                localWorkSizeCrossOver,
                                0,
                                NULL,
                                &eventoCrossOver);
    
    status = clEnqueueReadBuffer(cmdQueue, bufferB, CL_FALSE, 0,
						 datasize, populacao, 0, NULL, &event3);
    check_cl(status, "Erro ao enfileirar leitura de buffer de memoria");

    //Espera o término da fila de execução
    clFinish(cmdQueue);
        
    /*  int i;
    for(i=0;i<TAMANHO_POPULACAO;i++){
        int j;
        for(j=0;j<TAMANHO_INDIVIDUO;j++){
            cout << populacao[i].genotipo[j] << " ";           
        }
        cout << endl;        
    } */  
    

    #ifdef PROFILING

    float kernelSelecao = getTempoDecorrido(eventoSelecao) / 1000000000.0 ;
    float tempoCrossOver = getTempoDecorrido(eventoCrossOver) / 1000000000.0 ;
    tempoTotal += kernelSelecao+tempoCrossOver;
    tempoTotalProcessamento += kernelSelecao+tempoCrossOver;
    tempoTotalAG+=kernelSelecao+tempoCrossOver;
    tempoTotalSelecao +=kernelSelecao;
    tempoTotalCrossOverMutacao+=tempoCrossOver;

    #endif        
        
    //Avalia a nova geração
    avaliacao(populacao, gramatica, bufferB, TAMANHO_POPULACAO, 0);  

    //Política de substituição dos indivíduos da geração
    substituicao(populacao, gramatica, melhor1, melhor2);
}

/*
 Função utilizada pelo qsort para ordenar a população segundo a aptidão.
*/
int compara_individuo(const void* a, const void* b){

    individuo* p1 = (individuo*)a;
    individuo* p2 = (individuo*)b;

    return p1->aptidao < p2->aptidao;
}


void eg_paralela(t_regra *gramatica, Database *trainingData, Database *testData,	
char * pop_file, int pop_count, int pcores, int kernelAG){

	individuo * pop = (individuo*) malloc(sizeof(individuo)*TAMANHO_POPULACAO);
	individuo melhor1[1], melhor2[1], *melhor;

    check(pop    != NULL, "A população não pode ser nula");
    check(gramatica != NULL, "A gramática não pode ser nula");
    check(trainingData != NULL, "o banco de dados não pode ser nulo");
    //check(testData != NULL, "o banco de dados não pode ser nulo");
	
    tamanhoBancoDeDados = trainingData->numRegistros;

	printf("opencl init\n");
	
    opencl_init(pcores, kernelAG, trainingData, testData);
    
    double inicio = getTime();        

    carrega_gramatica(gramatica);
    carrega_bancoDeDados(trainingData);
    carrega_seeds();
    
	printf("constantes carregadas\n");
    
    initializEvaluatedNodes();
    
	printf("evaluated nodes initialized\n");
            
    inicializa_populacao(pop);
    
	printf("pop initialized\n");
    
    /*status = clEnqueueReadBuffer(cmdQueue, bufferA, CL_TRUE, 0,
						 datasize, pop, 0, NULL, NULL);
    
    //exibePopulacao(pop);
    
    int i;
    
    for(i=0; i< TAMANHO_POPULACAO; i++)
    {
    	exibe_melhor(&pop[i], gramatica);
    }
    //exit(1);    
    */       
    
    avaliacao(pop, gramatica, bufferA, TAMANHO_POPULACAO, 0);
    
    while(geracao <= NUMERO_DE_GERACOES){
    
	    cout << "Geração" << geracao << endl;

        iteracao(pop, gramatica, melhor1, melhor2);
        geracao++;
        
        /*status = clEnqueueReadBuffer(cmdQueue, bufferA, CL_TRUE, 0,
	 	datasize, pop, 0, NULL, NULL);	 	
	 	qsort(pop, TAMANHO_POPULACAO, sizeof(individuo), (int(*)(const void*, const void*))compara_individuo);        
		//exibePopulacao(pop);		
		/*int i;		
		for(i=0; i< TAMANHO_POPULACAO; i++)
		{
			exibe_melhor(&pop[i], gramatica);
		}*/
    }
    
    float trainingFitness = melhor1[0].aptidao, testFitness;
    
    if(melhor2[0].aptidao > melhor1[0].aptidao)
	{		
		trainingFitness = melhor2[0].aptidao;
	}
	
    if(testData != NULL)
    {    
		size_t offset = {0};
		
		if(melhor2[0].aptidao > melhor1[0].aptidao)
		{		
			offset = {sizeof(individuo) * ELITE};
		}
		
		carrega_bancoDeDados(testData);   
		
		avaliacao(pop, gramatica, bufferA, 1, offset);
		
		cl_event readEvent;
		
	 	clEnqueueReadBuffer(cmdQueue, bufferA, CL_FALSE, offset,
                        sizeof(individuo), melhor2,
                        0, NULL, &readEvent);
    	check_cl(status, "Erro ao enfileirar leitura de buffer de memória");

   	 	clFinish(cmdQueue);
   	 	
   	 	testFitness = melhor2[0].aptidao;
    }
    else
    {
	   testFitness = trainingFitness;
    }
    
    printf("%E\t%E\t", trainingFitness, testFitness);
    
    exibe_melhor(melhor2, gramatica);
    
    /*status = clEnqueueReadBuffer(cmdQueue, bufferA, CL_TRUE, 0, datasize, pop, 0, NULL, NULL);
    
    exibePopulacao(pop);
    
    int i;
    
    for(i=0; i< TAMANHO_POPULACAO; i++)
    {
    	exibe_melhor(&pop[i], gramatica);
    }
    */
	
    long int nodeEvaluations = evaluatedNodesReduction();
    /*if(testData != NULL)
		nodeEvaluations =  nodeEvaluations * (trainingData->numRegistros + testData->numRegistros);    	
	else
		nodeEvaluations =  nodeEvaluations * trainingData->numRegistros;*/		

    printf("\t%f\t", ((float)nodeEvaluations) / 1000000000);
    
    double tempo = getTime()-inicio;
    
    printf("%lf\t%lf\t", tempoTotalAvaliacao, tempo);

    opencl_dispose();
    
    free(pop);

   #ifdef PROFILING

      /*printf("Inicializacao\tAG\tSubstituição\tAvaliação\tProcessamento Total\tTransf memoria\tTransf memoria inicial\tProc+Memoria\n");
      printf("%.10f\t", tempoInicializacaoPopulacao);
      printf("%.10f\t", tempoTotalAG);      
      printf("%.10f\t", tempoTotalSubstituicao);
      printf("%.10f\t", tempoTotalAvaliacao);
      printf("%.10f\t", tempoTotalProcessamento);
      printf("%.10f\t", tempoTotalTransfMemoria);
      printf("%.10f\t", tempoTransfMemoriaInicial);
      printf("%.10f\n", tempoTotalProcessamento + tempoTotalTransfMemoria);      
      */

   #endif
}

void opencl_dispose(){

    clReleaseMemObject(bufferA);
    clReleaseMemObject(bufferB);
    clReleaseMemObject(bufferC);
    clReleaseMemObject(bufferGramatica);
    clReleaseMemObject(bufferDatabase);
    clReleaseKernel(kernelAvaliacao);
    clReleaseKernel(kernelInicializacao);
    clReleaseKernel(kernelIteracao);
    clReleaseProgram(program);
    clReleaseCommandQueue(cmdQueue);
    clReleaseContext(context);
}
