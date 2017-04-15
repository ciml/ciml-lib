#define __CL_ENABLE_EXCEPTIONS

#include <CL/cl.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <utility>
#include <cstdlib>
#include <iomanip>
#include <ctime>
#include <chrono>

#include <windows.h>

#ifdef __cplusplus
extern "C"
{
#endif
#include "genetica.h"
#ifdef __cplusplus
}
#endif
#include <omp.h>

#define MAX_TEXT_LINE_LENGTH 10000

const char *getErrorString(cl_int error) {
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

//TODO:fazer receber os dados também+labels.
void leIndividuo(char *fileName, Arvore* individuo) {
    FILE* arquivo = fopen(fileName, "r");
    char cp[100000];
    int posicao = 0;
    int informacao;

//    fseek(arquivo, 0, SEEK_END);
//    long fsize = ftell(arquivo);
//    fseek(arquivo, 0, SEEK_SET);


    if (arquivo == NULL) {
        fprintf(stderr, "Error opening data file.");
        return;
    }
    const char delimiters[] = " ()";

    while (fgets(cp, 100000, arquivo)) {

        char *token;

        token = strtok (cp, delimiters);      /* token => "words" */
            printf("%s\n", token);

        while(token != NULL){
            printf("%s\n", token);
            if(strcmp(token, "+") == 0){
                //printf("aaa\n");
                informacao = packInt(FBIN, PLUS);
                individuo->informacao[posicao] = informacao;
                individuo->numeroFilhos[posicao] = 2;
            }
            else if(strcmp(token, "-") == 0){
                informacao = packInt(FBIN, MIN);
                individuo->informacao[posicao] = informacao;
                individuo->numeroFilhos[posicao] = 2;
            }
            else if(strcmp(token, "*") == 0){
                informacao = packInt(FBIN, MULT);
                individuo->informacao[posicao] = informacao;
                individuo->numeroFilhos[posicao] = 2;
            }
            else if(strcmp(token, "/") == 0){
                informacao = packInt(FBIN, DIV);
                individuo->informacao[posicao] = informacao;
                individuo->numeroFilhos[posicao] = 2;
            }
            else if(strcmp(token, "sen") == 0){
                informacao = packInt(FUN, SIN);
                individuo->informacao[posicao] = informacao;
                individuo->numeroFilhos[posicao] = 1;
            }
            else if(strcmp(token, "cos") == 0){
                informacao = packInt(FUN, COS);
                individuo->informacao[posicao] = informacao;
                individuo->numeroFilhos[posicao] = 1;
            }
            else if(strcmp(token, "sqrt") == 0){
                informacao = packInt(FUN, SQR);
                individuo->informacao[posicao] = informacao;
                individuo->numeroFilhos[posicao] = 1;
            }
            else if(token[0] == 'x'){
                token++;
                informacao = packInt(VAR, atoll(token)-1);
                individuo->informacao[posicao] = informacao;
                individuo->numeroFilhos[posicao] = 0;
            } else {
                informacao = packFloat(CONST, atof(token));
                individuo->informacao[posicao] = informacao;
                individuo->numeroFilhos[posicao] = 0;
            }
            posicao++;
            token = strtok (NULL, delimiters);
        }
        individuo->numNos = posicao-1;
        //int i;
       // for(i= 0; i < posicao-1; i++){
            //printf("%d ", individuo->informacao[posicao]);
       // }

//        token = strtok (NULL, delimiters);    /* token => "separated" */
//        printf("%s", token);
//        token = strtok (NULL, delimiters);    /* token => "by" */
//        printf("%s", token);
//        token = strtok (NULL, delimiters);    /* token => "spaces" */
//        printf("%s", token);
//        token = strtok (NULL, delimiters);    /* token => "and" */
//        printf("%s", token);
//        token = strtok (NULL, delimiters);    /* token => "punctuation" */
//        printf("%s", token);
//        token = strtok (NULL, delimiters);    /* token => NULL */
//        printf("%s", token);
    }

}

void imprimeParametros(){
    printf("/*-----------------------------------------------------------------\n");
    printf("* NUMERO INDIVIDUOS: %d \t CROSS-OVER: %.2f\n", NUM_INDIV, PROB_CROSS);
    printf("* NUMERO GERACOES  : %d \t MUTACAO   : %.2f\n", NUM_GERACOES, PROB_MUT);
    printf("* NUM MAXIMO NOS   : %d \t ELITISMO  : %.2f\n", MAX_NOS, ELITISMO);
    printf("* PROFUNDIDADE MAX : %d \t \n", MAX_DEPTH);
    printf("*------------------------------------------------------------------\n");
}


bool IsPowerOf2( int n ){
   return (n & -n) == n;
}

unsigned NextPowerOf2( unsigned n ){
   n--;
   n |= n >> 1;  // handle  2 bit numbers
   n |= n >> 2;  // handle  4 bit numbers
   n |= n >> 4;  // handle  8 bit numbers
   n |= n >> 8;  // handle 16 bit numbers
   n |= n >> 16; // handle 32 bit numbers
   n++;

   return n;
}

std::string ToString( int t ){
      std::stringstream ss; ss << t; return ss.str();
}



int main(){
    std::cout << std::setprecision(16) << std::fixed;
    int i, indice1, indice2, novosIndividuos;
    int iteracoes = 0;


    ///variaveis lidas de arquivo
    int M, N;
    char** LABELS;
    int* conjuntoOpTerm;
    float* ctes; //TODO:ainda nao le constantes...
    int NUM_OPBIN, NUM_OPUN, NUM_CTES;

    ///leituras de dados
    float** dadosTreinamento = readTrainingData(&M, &N, &NUM_CTES, &NUM_OPBIN, &NUM_OPUN, &LABELS, &conjuntoOpTerm);

    float* dadosTranspostos = new float [M*N];
    unsigned pos = 0;
    for( unsigned j = 0; j < N; ++j )
        for( unsigned i = 0; i < M; ++i )
            dadosTranspostos[pos++] = dadosTreinamento[i][j];

            for(int i = 0; i < M*N; i++){
                std::cout << dadosTranspostos[i] <<" ";
            }
            std::cout << std::endl;

    imprimeParametros();
    atribuiSemente(SEED);
    printf("Seed: %d \n", SEED);
    printf("M: %d \n", M);
    printf("N: %d \n", N);
    printf("NUM_CTES: %d \n", NUM_CTES);
    printf("NUM_OPBIN: %d \n", NUM_OPBIN);
    printf("NUM_OPUN: %d \n", NUM_OPUN);

    Arvore* popAtual = new Arvore[NUM_INDIV];
    Arvore* popFutura = new Arvore[NUM_INDIV];

    //Arvore popAtual[NUM_INDIV], popFutura[NUM_INDIV];
            cl_int result; //Variavel para verificar erros
                ///TODO: Colocar conferencias de erros pelo código
                std::vector<cl::Platform> platforms;
                std::vector<cl::Device> devices;

                ///Encontrando as plataformas disponiveis
                result = cl::Platform::get(&platforms);
                if(result != CL_SUCCESS){
                    std::cout << "Erro ao encontrar plataformas." << std::endl;
                    exit(1);
                }

                ///Encontrando os devices disponiveis na plataforma 0
                platforms[1].getDevices(CL_DEVICE_TYPE_ALL, &devices);
                if(result != CL_SUCCESS){
                    std::cout << "Erro ao encontrar devices." << std::endl;
                    exit(1);
                }

                std::cout << "Available Platforms: \n";
                for(cl_uint i = 0; i < platforms.size(); ++i) {
                    std::cout <<"\t[" << i << "]"<<platforms[i].getInfo<CL_PLATFORM_NAME>() << std::endl;
                }
                std::cout << std::endl;

                std::cout << "Available Devices for Platform " << platforms[0].getInfo<CL_PLATFORM_NAME>()<< ":\n";
                for(cl_uint i = 0; i < devices.size(); ++i) {
                    std::cout<<"\t[" << i << "]"<<devices[i].getInfo<CL_DEVICE_NAME>() << std::endl;
                }
                std::cout << std::endl;

                ///Estabelecendo o contexto com os devices
                cl::Context contexto(devices, NULL, NULL, NULL, &result);
                if(result != CL_SUCCESS){
                    std::cout << "Erro ao criar um contexto OpenCL" << std::endl;
                    exit(1);
                }

                ///Criando a fila de comando para o device 0
                cl_command_queue_properties commandQueueProperties = CL_QUEUE_PROFILING_ENABLE;
                ///AQUI
                //cl::CommandQueue cmdQueueCPU(contexto, devices[2], commandQueueProperties, &result);
                cl::CommandQueue cmdQueueGPU(contexto, devices[0], commandQueueProperties, &result);
                if(result != CL_SUCCESS){
                    std::cout << "Erro ao criar a Command Queue" << std::endl;
                    exit(1);
                }

                cl::Buffer bufferPop(contexto, CL_MEM_READ_WRITE/*|CL_MEM_USE_HOST_PTR*/, NUM_INDIV * sizeof(Arvore)/* * 2 * MAX_NOS * sizeof(int)/*, popFutura*/);
                cl::Buffer dados(contexto, CL_MEM_READ_ONLY, M*N * sizeof(float));

                //cmdQueueGPU.enqueueWriteBuffer(bufferPop, CL_TRUE, 0, NUM_INDIV * sizeof(Arvore), popFutura);
                cmdQueueGPU.enqueueWriteBuffer(dados, CL_TRUE, 0, M*N * sizeof(float), dadosTranspostos);

                      size_t globalSize, localSize;
                      size_t numPoints = M;
                      std::string compileFlags;

                      size_t maxLocalSize = devices[0].getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();

                      ///FOR GPU
                      /**
                      if( numPoints < maxLocalSize)
                          localSize = numPoints;
                       else
                          localSize = maxLocalSize;

                       // One individual per work-group
                       globalSize = localSize * NUM_INDIV;

                        stringstream ss;
                        ss << NextPowerOf2( localSize );
                        string str = ss.str();

                       compileFlags += " -D LOCAL_SIZE_ROUNDED_UP_TO_POWER_OF_2="
                                    + ToString( NextPowerOf2(localSize) );

                       if( MAX_NOS > localSize ) //MaximumTreeSize() > m_local_size )
                          compileFlags += " -D PROGRAM_TREE_DOES_NOT_FIT_IN_LOCAL_SIZE";

                       if( IsPowerOf2( localSize ) )
                          compileFlags += " -D LOCAL_SIZE_IS_NOT_POWER_OF_2";

                       if( numPoints % localSize != 0 )
                          compileFlags += " -D NUM_POINTS_IS_NOT_DIVISIBLE_BY_LOCAL_SIZE";
                        */

                        ///FOR CPU
                        localSize = 1;//m_num_points;
                        globalSize = NUM_INDIV;
                      //size_t num_groups = global_work_size / local_work_size;

                ///Leitura do arquivo com o programa em C++
                std::ifstream sourceFileName("kernelAvalia.cl");
                std::string sourceFile(std::istreambuf_iterator<char>(sourceFileName),(std::istreambuf_iterator<char>()));

                std::string program_src =
                    "#define TIPO "  + ToString( TIPO ) + "\n"
                    "#define SEED "  + ToString( SEED ) + "\n"
                    "#define VAR     "+ ToString( VAR ) + "\n" +
                    "#define CONST   "+ ToString( CONST ) + "\n" +
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
                    "#define MAX_NOS     "+ ToString( MAX_NOS ) + "\n" +
                    "#define MAX_DEPTH   "+ ToString( MAX_DEPTH ) + "\n" +
                    "#define MAX_FILHOS  "+ ToString( MAX_FILHOS ) + "\n" +
                    "#define NUM_INDIV   "+ ToString( NUM_INDIV ) + "\n" +
                    "#define PROB_CROSS  "+ ToString( PROB_CROSS ) + "\n" +
                    "#define PROB_MUT    "+ ToString( PROB_MUT ) + "\n" +
                    "#define NUM_TORNEIO     "+ ToString( NUM_TORNEIO ) + "\n" +
                    "#define ELITISMO        "+ ToString( ELITISMO ) + "\n" +
                    "#define NUM_GERACOES    "+ ToString( NUM_GERACOES) + "\n" +
                    "#define LOCAL_SIZE " + ToString( localSize ) + "\n" +
                     sourceFile;

                ///Criar programa por Source
                //cl::Program::Sources fonte(1, std::make_pair(kernel_str, std::strlen(kernel_str)));
                cl::Program::Sources source(1, std::make_pair(program_src.c_str(), program_src.length()+1));
                cl::Program programa(contexto, source);

                //const char options[] = "-I.";
                //int error;
                try {
                    programa.build(devices/*, options*/);
                } catch(cl::Error& e){
                    std::cerr << getErrorString(e.err()) << std::endl;
                    std::cout << e.what() << " : " << e.err() << std::endl;
                    exit(1);
                }

                 cl::Kernel krnl(programa, "avaliaIndividuos");

                cl_int aaa = krnl.getWorkGroupInfo<CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE>(devices[0]);
                std::cout << "preferred size = " << aaa << std::endl;



    inicializaPopulacao(popAtual, conjuntoOpTerm, NUM_OPBIN, NUM_OPUN, N);
    //imprimePopulacao(popAtual, LABELS);
    avaliaIndividuos(popAtual, dadosTreinamento, M, N);
    imprimePopulacao(popAtual, LABELS);

    while(criterioDeParada(iteracoes) /*qual o criterio de parada?*/){
        printf("GERACAO %d: \n\n", iteracoes);

        novosIndividuos = 0;
        //Arvore popFutura[NUM_INDIV];
       //novosIndividuos = novosIndividuos + selecionaElite(popAtual, popFutura);
        //printf("novos = %d\n\n", novosIndividuos);

        //while(novosIndividuos < NUM_INDIV){
        int num = 0;
        LARGE_INTEGER start, eNd, frequency;
        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&start);

        //std::clock_t start = std::clock();
        //std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        #pragma omp parallel for
        for(novosIndividuos = selecionaElite(popAtual, popFutura);novosIndividuos < NUM_INDIV; novosIndividuos +=2){
            num = omp_get_num_threads();
            //printf("aaaa");
            indice1 = torneio(popAtual);
            indice2 = torneio(popAtual);

            popFutura[novosIndividuos] = popAtual[indice1];
            popFutura[novosIndividuos+1] = popAtual[indice2];

            ///testar imprimir o que está retornando na parte randomica
            float cross = randomProb();
            float mut = randomProb();

            //printf("cross: %f\n", cross);
            //printf("mut: %f\n", mut);

            if(cross <= PROB_CROSS){
                crossOver(&popFutura[novosIndividuos+1/*-2*/], &popFutura[novosIndividuos/*-1*/]);
            }
            if(mut <= PROB_MUT){
                mutacao(&popFutura[novosIndividuos+1], conjuntoOpTerm, NUM_OPBIN, NUM_OPUN, N);
                mutacao(&popFutura[novosIndividuos], conjuntoOpTerm, NUM_OPBIN, NUM_OPUN, N);
            }
            printf("num threads = %d\n\n", num);

        }
//        std::chrono::steady_clock::time_point end= std::chrono::steady_clock::now();
//        std::cout <<std::setprecision(16) << "Time difference = " << (double)std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() <<std::endl;
//        std::cout <<std::setprecision(16) << "Time difference = " << (double)std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() <<std::endl;

        QueryPerformanceCounter(&eNd);
        //saida << "Guloso: " << r[1] << " | " << (double)((fim - inicio)/CLOCKS_PER_SEC)*1000 << " s " << endl;
        std::cout << (double)(eNd.QuadPart - start.QuadPart)/frequency.QuadPart  << std::endl;
        //std::cout << "Tempo = " << (std::clock() - start) / double(CLOCKS_PER_SEC) << std::endl;

        cmdQueueGPU.enqueueWriteBuffer(bufferPop, CL_TRUE, 0, NUM_INDIV * sizeof(Arvore), popFutura);

        //avaliaIndividuos(popFutura, dadosTreinamento, M, N);
//        for(i = 0; i< NUM_INDIV; i++){
//            popAtual[i] = popFutura[i];
//        }

//



                    ///Dispondo argumentos para o kernel + executar
                    krnl.setArg(0, bufferPop);
                    krnl.setArg(1, dados);
                    krnl.setArg(2, NUM_INDIV * sizeof(float), NULL);
                    krnl.setArg(3, sizeof(int), &M);
                    krnl.setArg(4, sizeof(int), &N);

                    try {
                        result = cmdQueueGPU.enqueueNDRangeKernel(krnl, cl::NullRange, cl::NDRange(globalSize), cl::NDRange(localSize), NULL);
                    } catch(cl::Error& e){
                        std::cerr << getErrorString(e.err()) << std::endl;
                        exit(1);
                    }

        cmdQueueGPU.enqueueReadBuffer(bufferPop, CL_TRUE, 0, NUM_INDIV * sizeof(Arvore), popFutura);
        //popFutura = (Arvore*)cmdQueueGPU.enqueueMapBuffer(bufferPop,CL_FALSE, CL_MAP_READ, 0, NUM_INDIV * sizeof(Arvore));
        cmdQueueGPU.finish();

        for(i = 0; i< NUM_INDIV; i++){
            popAtual[i] = popFutura[i];
        }
        //imprimePopulacao(popAtual);
        imprimeMelhor(popAtual, LABELS);
        iteracoes++;
    }
    printf("\nPOPULACAO FINAL \n");
    imprimePopulacao(popAtual, LABELS);
    printf("*");
    imprimeMelhor(popAtual, LABELS);
    printf("\n");

    return 0;
}
