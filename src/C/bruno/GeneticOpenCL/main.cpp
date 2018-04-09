#define __CL_ENABLE_EXCEPTIONS

#include <omp.h>        //OpenMP

#include <iostream>
#include <iomanip>

#include "genetica.h"

#include "GPTime.h"

#include "oclConfig.h"  //Outras configurações e bibliotecas

#define TESTA_INDIV 0

#if !DIF_CONTEXT
int main(int argc, char** argv){

    std::cout << std::setprecision(32) << std::fixed;

    float maxDados, minDados;
    minDados = std::numeric_limits<float>::max();
    maxDados = (-1) * minDados;

    GPTime timeManager(4);
    timeManager.getStartTime(Total_T);

    float tempoAvaliacao = 0;
    float tempoTotalAvaliacao = 0;

    float tempoEvolucao = 0;
    float tempoTotalEvolucao = 0;

    int iteracoes = 0;

    Arvore* popAtual;
    popAtual = new Arvore[NUM_INDIV];

    Arvore* popFutura;
    popFutura = new Arvore[NUM_INDIV];

    int* seeds;
    int* seeds2;
    seeds = new int [NUM_INDIV * MAX_NOS];

    ///variaveis lidas de arquivo
    int M, N;
    char** LABELS;
    int* conjuntoOpTerm;
    int* conjuntoOpTerm2;
    float* ctes; //TODO:ainda nao le constantes...
    int NUM_OPBIN, NUM_OPUN, NUM_CTES;

    ///leituras de dados
    float** dadosTreinamento = readTrainingData(&M, &N, &NUM_CTES, &NUM_OPBIN, &NUM_OPUN, &LABELS, &conjuntoOpTerm, &maxDados, &minDados, argv[1]);

    atribuiSemente(SEED);

    ///transposicao de dados
    float* dadosTranspostos;
    dadosTranspostos = new float [M * N];

    unsigned pos = 0;
    std::cout << "Transpondo dados..." << std::endl;
    for(int j = 0; j < N; ++j ){
        for(int i = 0; i < M; ++i ){
            dadosTranspostos[pos++] = dadosTreinamento[i][j];
        }
    }

    for(int i = 0; i < NUM_INDIV*MAX_NOS; i++){
        seeds[i] = rand();
    }

    imprimeParametros(M, N, NUM_CTES, NUM_OPBIN, NUM_OPUN);
    inicializaPopulacao(popAtual, conjuntoOpTerm, NUM_OPBIN, NUM_OPUN, N, &seeds[0], maxDados, minDados);
    avaliaIndividuos(popAtual, /*dadosTranspostos*/dadosTreinamento, M, N);


    #if AVALOCL || EVOLOCL

        cl_ulong inicio, fim;

        ///Evento para controlar tempo gasto
        cl::Event e_tempo;

        cl_int result; //Variavel para verificar erros
        ///TODO: Colocar conferencias de erros pelo código
        std::vector<cl::Platform> platforms;
        std::vector<cl::Device> devices;

        setupOpenCLOnePlatform(platforms,devices);
        printPlatformsDevices(platforms, devices);

        ///Estabelecendo o contexto com os devices
        cl::Context contexto(devices, NULL, NULL, NULL
                , &result);
        if(result != CL_SUCCESS){
            std::cout << "Erro ao criar um contexto OpenCL" << std::endl;
            exit(1);
        }


        ///Criando a fila de comando
        cl_command_queue_properties commandQueueProperties = CL_QUEUE_PROFILING_ENABLE;

        ///4 opções: GPU-GPU, GPU-CPU, CPU-CPU, CPU-GPU
        ///NUMDEVICES;FIRST

        cl::CommandQueue* cmdQueueEvol;
        cl::CommandQueue* cmdQueueAval;

        #if TWODEVICES
            #if AVALGPU
                cmdQueueEvol = new cl::CommandQueue(contexto, devices[0] /** SELECIONAR CPU */, commandQueueProperties, &result);
                cmdQueueAval = new cl::CommandQueue(contexto, devices[2] /** SELECIONAR GPU*/, commandQueueProperties, &result);
            #else
                cmdQueueEvol = new cl::CommandQueue(contexto, devices[2] /** SELECIONAR GPU*/, commandQueueProperties, &result);
                cmdQueueAval = new cl::CommandQueue(contexto, devices[0] /** SELECIONAR CPU*/, commandQueueProperties, &result);
            #endif // AVALGPU
        #else
            #if AVALGPU
                cmdQueueAval = new cl::CommandQueue(contexto, devices[0] /** SELECIONAR GPU */, commandQueueProperties, &result);
            #else
                cmdQueueAval = new cl::CommandQueue(contexto, devices[0] /** SELECIONAR CPU*/, commandQueueProperties, &result);
            #endif // AVALGPU
            cmdQueueEvol = cmdQueueAval;
        #endif // TWODEVICES

        if(result != CL_SUCCESS){
            std::cout << "Erro ao criar a Command Queue" << std::endl;
            exit(1);
        }


        cl::Buffer bufferPopA(contexto, CL_MEM_ALLOC_HOST_PTR | CL_MEM_READ_WRITE, NUM_INDIV * sizeof(Arvore));
        cl::Buffer bufferPopF(contexto, CL_MEM_READ_WRITE, NUM_INDIV * sizeof(Arvore));
        cl::Buffer bufferOpTerm(contexto, CL_MEM_ALLOC_HOST_PTR | CL_MEM_READ_ONLY, (NUM_OPBIN + NUM_OPUN + NUM_CTES + N - 1) * sizeof(int));
        cl::Buffer bufferSeeds(contexto, CL_MEM_ALLOC_HOST_PTR | CL_MEM_READ_WRITE, NUM_INDIV * MAX_NOS * sizeof(int));
        cl::Buffer dados(contexto, CL_MEM_READ_ONLY, M * N * sizeof(float));

        cmdQueueEvol->enqueueWriteBuffer(bufferSeeds, CL_FALSE, 0, NUM_INDIV * MAX_NOS * sizeof(int), seeds);
        cmdQueueAval->enqueueWriteBuffer(dados, CL_TRUE, 0, M * N * sizeof(float), dadosTranspostos);
        cmdQueueEvol->enqueueWriteBuffer(bufferOpTerm, CL_FALSE, 0, (NUM_OPBIN + NUM_OPUN + NUM_CTES + N - 1) * sizeof(int), conjuntoOpTerm);

        //seeds2 = (int*)cmdQueueEvol->enqueueMapBuffer(bufferSeeds, CL_TRUE, CL_MAP_WRITE, 0, NUM_INDIV * MAX_NOS * sizeof(int));
        //conjuntoOpTerm2 = (int*)cmdQueueEvol->enqueueMapBuffer(bufferOpTerm, CL_TRUE, CL_MAP_WRITE, 0, (NUM_OPBIN + NUM_OPUN + NUM_CTES + N - 1) * sizeof(int));
        //popAtual = (Arvore*)cmdQueueEvol->enqueueMapBuffer(bufferPopA, CL_TRUE, CL_MAP_WRITE, 0, NUM_INDIV * sizeof(Arvore));

//        for(int i = 0; i < NUM_INDIV*MAX_NOS; i++){
//            seeds2[i] = seeds[i];
//        }
//
//        for(int i = 0; i < (NUM_OPBIN+NUM_OPUN+NUM_CTES + N); i++){
//            conjuntoOpTerm2[i] = conjuntoOpTerm[i];
//        }

//        inicializaPopulacao(popAtual, conjuntoOpTerm, NUM_OPBIN, NUM_OPUN, N, &seeds[0], maxDados, minDados);
//        avaliaIndividuos(popAtual, /*dadosTranspostos*/dadosTreinamento, M, N);

//        cmdQueueEvol->enqueueUnmapMemObject(bufferSeeds, seeds2);
//        cmdQueueEvol->enqueueUnmapMemObject(bufferOpTerm, conjuntoOpTerm2);

        cmdQueueAval->finish();
        cmdQueueEvol->finish();

        size_t globalSize;
        size_t localSize;
        size_t numPoints;
        std::string compileFlags;

        numPoints = (size_t)M;

        size_t maxLocalSize = cmdQueueAval->getInfo<CL_QUEUE_DEVICE>().getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();
        setNDRanges(&globalSize, &localSize, &compileFlags, maxLocalSize, numPoints, cmdQueueAval->getInfo<CL_QUEUE_DEVICE>().getInfo<CL_DEVICE_TYPE>());

        std::cout << "Global Size = " <<globalSize << std::endl << "Local size = " << localSize << std::endl;

        ///Leitura do arquivo com o programa em C++
        std::ifstream sourceFileName("kernelAvalia.cl");
        std::string sourceFile(std::istreambuf_iterator<char>(sourceFileName),(std::istreambuf_iterator<char>()));
        std::string program_src = setProgramSource(NUM_OPBIN, NUM_OPUN, M, N, (int)localSize, maxDados, minDados) + sourceFile;
        //std::cout << program_src << std::endl;

        ///Criar programa por Source
        cl::Program::Sources source(1, std::make_pair(program_src.c_str(), program_src.length()+1));
        cl::Program programa(contexto, source);

        //const char options[] = "-cl-opt-disable";
        //int error;
        //compileFlags+=" -cl-opt-disable";
        std::cout << compileFlags << std::endl;
        try {
            programa.build(devices, compileFlags.c_str());
        } catch(cl::Error& e){
            std::cerr << getErrorString(e.err()) << std::endl
            << programa.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0]);
            std::cout << e.what() << " : " << e.err() << std::endl;
            exit(1);
        }
//        std::cout << programa.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0]) << std::endl;

        #if AVALGPU
            cl::Kernel krnlAvalia(programa, "avaliaIndividuos");
        #elif AVALOCL
            cl::Kernel krnlAvalia(programa, "avaliaIndividuosCPU");
        #else
            cl::Kernel krnlAvalia(programa, "avaliaIndividuosSequencial");
        #endif // AVALGPU

        #if EVOLOCL
            cl::Kernel krnlEvolucao(programa, "evolucao");
        #else
            cl::Kernel krnlEvolucao(programa, "evolucaoSequencial");
        #endif // EVOLOCL



        /**/
        #if TESTA_INDIV
        {
        Arvore arv;
        leIndividuo("individual.txt",&arv, LABELS, dadosTreinamento, M, N);
        char c;
        std::cin >> c;

        popFutura[0] = arv;
        popFutura[0].aptidao = -1;
        cmdQueueAval->enqueueWriteBuffer(bufferPopF, CL_TRUE, 0, NUM_INDIV * sizeof(Arvore), popFutura);
        krnlAvalia.setArg(0, bufferPopF);
        krnlAvalia.setArg(1, dados);
        krnlAvalia.setArg(2, localSize*sizeof(float), NULL);
        //std::cout << "aqui" << std::endl;
        try {
                result = cmdQueueAval->enqueueNDRangeKernel(krnlAvalia, cl::NullRange, cl::NDRange(globalSize), cl::NDRange(localSize), NULL, &e_tempo);
        } catch(cl::Error& e){
            std::cerr << getErrorString(e.err()) << std::endl;
            exit(1);
        }
        //std::cout << "aqui" << std::endl;
        try {
        cmdQueueAval->finish();
        } catch(cl::Error& e){
            std::cerr << getErrorString(e.err()) << std::endl;
            exit(1);
        }
        cmdQueueAval->enqueueReadBuffer(bufferPopF, CL_TRUE, 0, NUM_INDIV * sizeof(Arvore), popFutura);
        std::cout << popFutura[0].aptidao << std::endl;
        std::cin >> c;
        }
        #endif // TESTA_INDIV
        /**/

    #endif // AVALOCL

    imprimePopulacao(popAtual, LABELS);

    while(criterioDeParada(iteracoes) /*qual o criterio de parada?*/){
       // imprimePopulacao(popAtual, LABELS);
        timeManager.getStartTime(Iteracao_T);
        printf("\n-----------\nGERACAO %d: \n", iteracoes);

        timeManager.getStartTime(Evolucao_T);
        int novosIndividuos = selecionaElite(popAtual, popFutura);

        #if EVOLOCL
            //cmdQueueEvol->enqueueUnmapMemObject(bufferPopA, popAtual);
            cmdQueueEvol->enqueueWriteBuffer(bufferPopA, CL_TRUE, 0, NUM_INDIV * sizeof(Arvore), popAtual);
            cmdQueueEvol->enqueueWriteBuffer(bufferPopF, CL_TRUE, 0, NUM_INDIV * sizeof(Arvore), popFutura);
            cmdQueueEvol->finish();

            ///Dispondo argumentos para o kernel + executar
            krnlEvolucao.setArg(0, bufferPopA);
            krnlEvolucao.setArg(1, bufferPopF);
            krnlEvolucao.setArg(2, novosIndividuos);
            krnlEvolucao.setArg(3, bufferOpTerm);
            krnlEvolucao.setArg(4, bufferSeeds);


            try {
                result = cmdQueueEvol->enqueueNDRangeKernel(krnlEvolucao, cl::NullRange, cl::NDRange((NUM_INDIV-novosIndividuos)/2), cl::NDRange(1),NULL, &e_tempo);
            } catch(cl::Error& e){
                std::cerr << getErrorString(e.err()) << std::endl;
                exit(1);
            }

            cmdQueueEvol->finish();

            //e_tempo.getProfilingInfo(CL_PROFILING_COMMAND_START, &inicio);
            //e_tempo.getProfilingInfo(CL_PROFILING_COMMAND_END, &fim);
            //tempoEvolucao = (fim-inicio)/1.0E9;

            //tempoTotalEvolucao += tempoEvolucao;
            //std::cout <<"Tempo evolucao OCL = " << tempoEvolucao << std::endl;

            ///se está em um só dispositivo nao precisa disso
            cmdQueueEvol->enqueueReadBuffer(bufferPopA, CL_TRUE, 0, NUM_INDIV * sizeof(Arvore), popAtual);
            //popAtual = (Arvore*) cmdQueueEvol->enqueueMapBuffer(bufferPopA, CL_FALSE, CL_MAP_READ, 0, NUM_INDIV * sizeof(Arvore));
            #if TWODEVICES || !AVALOCL
            cmdQueueEvol->enqueueReadBuffer(bufferPopF, CL_TRUE, 0, NUM_INDIV * sizeof(Arvore), popFutura);
            #endif // TWODEVICES
            cmdQueueEvol->finish();

        #else // EVOLOCL
            #if  EVOLOMP
            //omp_set_num_threads((NUM_INDIV)-novosIndividuos/2);
            #pragma omp parallel for //num_threads((NUM_INDIV)-novosIndividuos/2) ordered
            for(int j = novosIndividuos; j < NUM_INDIV; j += 2){

                //int num = omp_get_num_threads();
                //printf("num threads = %d\n\n", num);

                int id = (j/2)-(novosIndividuos/2);

                //#pragma omp ordered
                //printf("id = %d\n", id);

                int indice1 = torneio(popAtual, &seeds[id]);
                int indice2 = torneio(popAtual, &seeds[id]);

                popFutura[novosIndividuos+2*id] = popAtual[indice1];
                popFutura[novosIndividuos+2*id+1] = popAtual[indice2];

                ///testar imprimir o que está retornando na parte randomica
                float cross = randomProb(&seeds[id]);
                float mut = randomProb(&seeds[id]);


                if(cross <= PROB_CROSS){
                    crossOver(&popFutura[novosIndividuos+2*id+1/*-2*/], &popFutura[novosIndividuos+2*id/*-1*/], &seeds[id], id);
                }
                if(mut <= PROB_MUT){
                    mutacao(&popFutura[novosIndividuos+2*id+1], conjuntoOpTerm, NUM_OPBIN, NUM_OPUN, N, &seeds[id], maxDados, minDados);
                    mutacao(&popFutura[novosIndividuos+2*id], conjuntoOpTerm, NUM_OPBIN, NUM_OPUN, N, &seeds[id], maxDados, minDados);
                }
            }
            #else
/*
            cmdQueueEvol->enqueueWriteBuffer(bufferPopA, CL_TRUE, 0, NUM_INDIV * sizeof(Arvore), popAtual);
            cmdQueueEvol->enqueueWriteBuffer(bufferPopF, CL_TRUE, 0, NUM_INDIV * sizeof(Arvore), popFutura);
            cmdQueueEvol->finish();

            ///Dispondo argumentos para o kernel + executar
            krnlEvolucao.setArg(0, bufferPopA);
            krnlEvolucao.setArg(1, bufferPopF);
            krnlEvolucao.setArg(2, novosIndividuos);
            krnlEvolucao.setArg(3, bufferOpTerm);
            krnlEvolucao.setArg(4, bufferSeeds);


            try {
                result = cmdQueueEvol->enqueueNDRangeKernel(krnlEvolucao, cl::NullRange, cl::NDRange(1), cl::NDRange(1),NULL, &e_tempo);
            } catch(cl::Error& e){
                std::cerr << getErrorString(e.err()) << std::endl;
                exit(1);
            }

            cmdQueueEvol->finish();

            e_tempo.getProfilingInfo(CL_PROFILING_COMMAND_START, &inicio);
            e_tempo.getProfilingInfo(CL_PROFILING_COMMAND_END, &fim);
            tempoEvolucao = (fim-inicio)/1.0E9;

            tempoTotalEvolucao += tempoEvolucao;
            std::cout <<"Tempo evolucao 1 = " << tempoEvolucao << std::endl;

            ///se está em um só dispositivo nao precisa disso
            cmdQueueEvol->enqueueReadBuffer(bufferPopA, CL_TRUE, 0, NUM_INDIV * sizeof(Arvore), popAtual);
            #if TWODEVICES || !AVALOCL
            cmdQueueEvol->enqueueReadBuffer(bufferPopF, CL_TRUE, 0, NUM_INDIV * sizeof(Arvore), popFutura);
            #endif // TWODEVICES
            cmdQueueEvol->finish();

*/
            for(int j = novosIndividuos; j < NUM_INDIV; j +=2){
                int id = (j/2)-(novosIndividuos/2);

                int indice1 = torneio(popAtual, &seeds[id]);
                int indice2 = torneio(popAtual, &seeds[id]);


                popFutura[j] = popAtual[indice1];
                popFutura[j+1] = popAtual[indice2];

                ///testar imprimir o que está retornando na parte randomica
                float cross = randomProb(&seeds[id]);
                float mut = randomProb(&seeds[id]);

                if(cross <= PROB_CROSS){
                    crossOver(&popFutura[j+1], &popFutura[j], &seeds[id], id);
                }

                if(mut <= PROB_MUT){
                    mutacao(&popFutura[j+1], conjuntoOpTerm, NUM_OPBIN, NUM_OPUN, N, &seeds[id], maxDados, minDados);
                    mutacao(&popFutura[j], conjuntoOpTerm, NUM_OPBIN, NUM_OPUN, N, &seeds[id], maxDados, minDados);
                }

            }
            #endif // EVOLOMP

        #endif // EVOLOCL

        timeManager.getEndTime(Evolucao_T);


        timeManager.getStartTime(Avaliacao_T);
        #if AVALOCL
            #if TWODEVICES || !EVOLOCL
                cmdQueueAval->enqueueWriteBuffer(bufferPopF, CL_TRUE, 0, NUM_INDIV * sizeof(Arvore), popFutura);
            #endif // TWODEVICES
            ///Dispondo argumentos para o kernel + executar
            krnlAvalia.setArg(0, bufferPopF);
            krnlAvalia.setArg(1, dados);
            krnlAvalia.setArg(2, localSize * sizeof(float), NULL);

            try {
                result = cmdQueueAval->enqueueNDRangeKernel(krnlAvalia, cl::NullRange, cl::NDRange(globalSize), cl::NDRange(localSize), NULL, &e_tempo);
            } catch(cl::Error& e){
                std::cerr << getErrorString(e.err()) << std::endl;
                exit(1);
            }

            try {
            cmdQueueAval->finish();
            } catch(cl::Error& e){
                std::cerr << getErrorString(e.err()) << std::endl;
                exit(1);
            }

            ///medir tempos
            //e_tempo.getProfilingInfo(CL_PROFILING_COMMAND_START, &inicio);
            //e_tempo.getProfilingInfo(CL_PROFILING_COMMAND_END, &fim);
            //tempoAvaliacao = (fim-inicio)/1.0E9;

            //tempoTotalAvaliacao += tempoAvaliacao;
            //std::cout  << "Tempo de avaliacao OCL = " << tempoAvaliacao << std::endl;

            cmdQueueAval->enqueueReadBuffer(bufferPopF, CL_TRUE, 0, NUM_INDIV * sizeof(Arvore), popFutura);
            //popFutura = (Arvore*)cmdQueueGPU.enqueueMapBuffer(bufferPop,CL_FALSE, CL_MAP_READ, 0, NUM_INDIV * sizeof(Arvore));

            cmdQueueAval->finish();


        #else // AVALOCL
/*
            #if TWODEVICES || !EVOLOCL
                cmdQueueAval->enqueueWriteBuffer(bufferPopF, CL_TRUE, 0, NUM_INDIV * sizeof(Arvore), popFutura);
            #endif // TWODEVICES
            ///Dispondo argumentos para o kernel + executar
            krnlAvalia.setArg(0, bufferPopF);
            krnlAvalia.setArg(1, dados);
            krnlAvalia.setArg(2, localSize * sizeof(float), NULL);

            try {
                result = cmdQueueAval->enqueueTask(krnlAvalia, NULL, &e_tempo);
                //enqueueNDRangeKernel(krnlAvalia, cl::NullRange, cl::NDRange(1), cl::NDRange(1), NULL, &e_tempo);
            } catch(cl::Error& e){
                std::cerr << getErrorString(e.err()) << std::endl;
                exit(1);
            }

            try {
            cmdQueueAval->finish();

            } catch(cl::Error& e){
                std::cerr << getErrorString(e.err()) << std::endl;
                exit(1);
            }


            cmdQueueAval->enqueueReadBuffer(bufferPopF, CL_TRUE, 0, NUM_INDIV * sizeof(Arvore), popFutura);
            //popFutura = (Arvore*)cmdQueueGPU.enqueueMapBuffer(bufferPop,CL_FALSE, CL_MAP_READ, 0, NUM_INDIV * sizeof(Arvore));

            cmdQueueAval->finish();

             ///medir tempos
            e_tempo.getProfilingInfo(CL_PROFILING_COMMAND_START, &inicio);
            e_tempo.getProfilingInfo(CL_PROFILING_COMMAND_END, &fim);

            std::cout  << "Aval time = " << (fim-inicio)/1.0E9 << std::endl;
*/

            avaliaIndividuos(popFutura, dadosTreinamento, M, N);

        #endif // AVALOCL

        timeManager.getEndTime(Avaliacao_T);



        for(int i = 0; i< NUM_INDIV; i++){
            popAtual[i] = popFutura[i];
        }

        timeManager.getEndTime(Iteracao_T);
        double iterTime = timeManager.getElapsedTime(Iteracao_T);
        double avalTime = timeManager.getElapsedTime(Avaliacao_T);
        double evolTime = iterTime - avalTime;
        tempoTotalEvolucao += evolTime;

        std::cout << "Evol time = " << evolTime << std::endl;
        //std::cout << "Evol time = " << timeManager.getElapsedTime(Evolucao_T) << std::endl;
        std::cout << "Aval time = " << avalTime << std::endl;
        std::cout << "Iter time = " << iterTime << std::endl;

        //imprimePopulacao(popAtual, LABELS);
        imprimeMelhor(popAtual, LABELS);
        iteracoes++;
    }
    timeManager.getEndTime(Total_T);

    std::cout << "\n\nPOPULACAO FINAL" << std::endl;
    imprimePopulacao(popAtual, LABELS);
    std::cout << "\n*";
    imprimeMelhor(popAtual, LABELS);
    std::cout << std::endl << std::endl;

    std::cout << "T_Evol time = " << tempoTotalEvolucao << std::endl;
    std::cout << "T_Aval time = " << timeManager.getTotalTime(Avaliacao_T) << std::endl;
    std::cout << "T_Iter time = " << timeManager.getTotalTime(Iteracao_T) << std::endl;

    std::cout << "Total time  = " << timeManager.getElapsedTime(Total_T) << std::endl;

    std::cout << "FIM" << std::endl;
    imprimeMelhor(popAtual, LABELS);
    std::cout << std::endl << std::endl;
    //std::cout << tempoEvolucao << " " << tempoTotalAvaliacao << " " << tempoTotal << std::endl;

    delete [] popAtual;
    delete [] popFutura;
    return 0;
}

#else
int main(int argc, char** argv){

    std::cout << std::setprecision(32) << std::fixed;
    //comparar tudo sequencial com openmp+opencl com tudo opencl
    float maxDados, minDados;
    minDados = std::numeric_limits<float>::max();
    maxDados = (-1) * minDados;

    double tempoTotalAvaliacao = 0;
    double tempoTotalEvolucao = 0;
    double tempoTotal = 0;
    double tempoGeracao;
    //, start, startIt, endIt, tempoEvolucao,
    double startIt, tempoEvolucao, tempoAvaliacao;

    std::chrono::steady_clock::time_point start, end;

    int i, indice1, indice2;
    int iteracoes = 0;

    ///variaveis lidas de arquivo
    int M, N;
    char** LABELS;
    int* conjuntoOpTerm;
    float* ctes; //TODO:ainda nao le constantes...
    int NUM_OPBIN, NUM_OPUN, NUM_CTES;

    ///leituras de dados
    float** dadosTreinamento = readTrainingData(&M, &N, &NUM_CTES, &NUM_OPBIN, &NUM_OPUN, &LABELS, &conjuntoOpTerm, &maxDados, &minDados, argv[1]);
/*
    Arvore arv;
    leIndividuo("6sencos.txt",&arv, LABELS, dadosTreinamento, M, N);
    char c; std::cin >> c;

    for(int i = 0; i < M; i++){
        for(int j = 0; j < N; j++){
            std::cout << dadosTreinamento[i][j] << " ";
        }
        std::cout << std::endl;
    }
*/

    ///transposicao de dados
    float* dadosTranspostos = new float [M*N];
    unsigned pos = 0;
    for( unsigned j = 0; j < N; ++j ){
        for( unsigned i = 0; i < M; ++i ){
            dadosTranspostos[pos++] = dadosTreinamento[i][j];
        }
    }

    imprimeParametros(M, N, NUM_CTES, NUM_OPBIN, NUM_OPUN);
    atribuiSemente(SEED);

    Arvore* popAtual = new Arvore[NUM_INDIV];
    Arvore* popFutura = new Arvore[NUM_INDIV];
    int* seeds = new int [NUM_INDIV*MAX_NOS];
    for(int i = 0; i < NUM_INDIV*MAX_NOS; i++){
        seeds[i] = rand();
    }
//    int t, n;
//    float f, f2;
//    for (t = n = 0; n < 1000000; n++){
//        f = rand2(&seeds[0])/(float)2147483647;
//        if (mysqr(f) != f*f) t++;
//    }
//    printf("%d of %d squares differ\n", t, n);
    //return 0;

    inicializaPopulacao(popAtual, conjuntoOpTerm, NUM_OPBIN, NUM_OPUN, N, &seeds[0], maxDados, minDados);
    avaliaIndividuos(popAtual, dadosTranspostos /*dadosTreinamento*/, M, N);
    //std::cout << seeds[0] <<std::endl;



        cl_ulong inicio, fim;
        ///Evento para controlar tempo gasto
        cl::Event e_tempo;


        cl_int result; //Variavel para verificar erros
        ///TODO: Colocar conferencias de erros pelo código

    std::vector<cl::Platform> platforms;
        std::vector<cl::Device> devicesCPU, devicesGPU;
        setupOpenCLTwoPlatform(platforms,devicesCPU, devicesGPU);

        cl::Context contextoCPU(devicesCPU, NULL, NULL, NULL, &result);
        if(result != CL_SUCCESS){
            std::cout << "Erro ao criar um contexto OpenCL" << std::endl;
            exit(1);
        }

        cl::Context contextoGPU(devicesGPU, NULL, NULL, NULL, &result);
        if(result != CL_SUCCESS){
            std::cout << "Erro ao criar um contexto OpenCL" << std::endl;
            exit(1);
        }

        //printPlatformsDevices(platforms, devices);



        ///Criando a fila de comando
        cl_command_queue_properties commandQueueProperties = CL_QUEUE_PROFILING_ENABLE;

        ///4 opções: GPU-GPU, GPU-CPU, CPU-CPU, CPU-GPU
        ///NUMDEVICES;FIRST

        cl::CommandQueue* cmdQueueEvol;
        cl::CommandQueue* cmdQueueAval;


        #if AVALGPU
            cmdQueueEvol = new cl::CommandQueue(contextoCPU, devicesCPU[0] /** SELECIONAR CPU */, commandQueueProperties, &result);
            cmdQueueAval = new cl::CommandQueue(contextoGPU, devicesGPU[0] /** SELECIONAR GPU*/, commandQueueProperties, &result);
        #else
            cmdQueueEvol = new cl::CommandQueue(contextoGPU, devicesGPU[0] /** SELECIONAR GPU*/, commandQueueProperties, &result);
            cmdQueueAval = new cl::CommandQueue(contextoCPU,devicesCPU[0] /** SELECIONAR CPU*/, commandQueueProperties, &result);
        #endif // AVALGPU


        if(result != CL_SUCCESS){
            std::cout << "Erro ao criar a Command Queue" << std::endl;
            exit(1);
        }

        #if AVALGPU
            cl::Buffer bufferPopA(contextoCPU, CL_MEM_READ_WRITE, NUM_INDIV * sizeof(Arvore)/*, popAtual*/);
            cl::Buffer bufferPopFCPU(contextoCPU, CL_MEM_READ_WRITE/*|CL_MEM_USE_HOST_PTR*/, NUM_INDIV * sizeof(Arvore)/*, popFutura*/);

            cl::Buffer bufferPopFGPU(contextoGPU, CL_MEM_READ_WRITE/*|CL_MEM_USE_HOST_PTR*/, NUM_INDIV * sizeof(Arvore)/*, popFutura*/);
            cl::Buffer dados(contextoGPU, CL_MEM_READ_ONLY, M*N * sizeof(float));

            cl::Buffer bufferOpTerm(contextoCPU, CL_MEM_READ_ONLY, (NUM_OPBIN+NUM_OPUN+NUM_CTES+N-1)*sizeof(int));
            cl::Buffer bufferSeeds(contextoCPU, CL_MEM_READ_WRITE, NUM_INDIV*MAX_NOS*sizeof(int));
        #endif // 1

        cmdQueueEvol->enqueueWriteBuffer(bufferSeeds, CL_FALSE, 0, NUM_INDIV*MAX_NOS*sizeof(int), seeds);
        cmdQueueAval->enqueueWriteBuffer(dados, CL_FALSE, 0, M*N * sizeof(float), dadosTranspostos);

        cmdQueueEvol->enqueueWriteBuffer(bufferOpTerm, CL_TRUE, 0, (NUM_OPBIN+NUM_OPUN+NUM_CTES+N-1)*sizeof(int), conjuntoOpTerm);

        //cmdQueueAval->enqueueWriteBuffer(bufferOpTerm, CL_TRUE, 0, (NUM_OPBIN+NUM_OPUN+NUM_CTES+N-1)*sizeof(int), conjuntoOpTerm);


        size_t globalSize, localSize;
        size_t numPoints = M;
        std::string compileFlags;

        size_t maxLocalSize = cmdQueueAval->getInfo<CL_QUEUE_DEVICE>().getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();
        setNDRanges(&globalSize, &localSize, &compileFlags, maxLocalSize, numPoints, cmdQueueAval->getInfo<CL_QUEUE_DEVICE>().getInfo<CL_DEVICE_TYPE>());
        std::cout << globalSize << std::endl << localSize << std::endl;

        ///Leitura do arquivo com o programa em C++
        std::ifstream sourceFileName("kernelAvalia.cl");
        std::string sourceFile(std::istreambuf_iterator<char>(sourceFileName),(std::istreambuf_iterator<char>()));
        std::string program_src = setProgramSource(NUM_OPBIN, NUM_OPUN, M, N, localSize, maxDados, minDados) + sourceFile;
        //std::cout << program_src << std::endl;

        ///Criar programa por Source
        cl::Program::Sources source(1, std::make_pair(program_src.c_str(), program_src.length()+1));
        cl::Program programaCPU(contextoCPU, source);
        cl::Program programaGPU(contextoGPU, source);

        //const char options[] = "-cl-opt-disable";
        //int error;
        try {
            programaCPU.build(devicesCPU, compileFlags.c_str());
        } catch(cl::Error& e){
            std::cerr << getErrorString(e.err()) << std::endl
            << programaCPU.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devicesCPU[0]);
            std::cout << e.what() << " : " << e.err() << std::endl;
            exit(1);
        }

        try {
            programaGPU.build(devicesGPU, compileFlags.c_str());
        } catch(cl::Error& e){
            std::cerr << getErrorString(e.err()) << std::endl
            << programaGPU.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devicesGPU[0]);
            std::cout << e.what() << " : " << e.err() << std::endl;
            exit(1);
        }
//        std::cout << programa.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0]) << std::endl;

        #if AVALGPU
            cl::Kernel krnlAvalia(programaGPU, "avaliaIndividuos");
            cl::Kernel krnlEvolucao(programaCPU, "evolucao");
        #else
            cl::Kernel krnlAvalia(programaCPU, "avaliaIndividuosCPU");
            cl::Kernel krnlEvolucao(programaGPU, "evolucao");
        #endif // AVALGPU


    imprimePopulacao(popAtual, LABELS);

    while(criterioDeParada(iteracoes) /*qual o criterio de parada?*/){
        printf("\n-----------\nGERACAO %d: \n", iteracoes);
        startIt = getTime();


            int novosIndividuos = selecionaElite(popAtual, popFutura);

            cmdQueueEvol->enqueueWriteBuffer(bufferPopA, CL_TRUE, 0, NUM_INDIV * sizeof(Arvore), popAtual);
            cmdQueueEvol->enqueueWriteBuffer(bufferPopFCPU, CL_TRUE, 0, NUM_INDIV * sizeof(Arvore), popFutura);
            cmdQueueEvol->finish();

            //std::cout << "novos = " << novosIndividuos << std::endl;
            ///Evento para controlar tempo gasto
            //cl::Event e_tempo;
            ///Dispondo argumentos para o kernel + executar
            krnlEvolucao.setArg(0, bufferPopA);
            krnlEvolucao.setArg(1, bufferPopFCPU);
            krnlEvolucao.setArg(2, novosIndividuos);
            krnlEvolucao.setArg(3, bufferOpTerm);
            krnlEvolucao.setArg(4, bufferSeeds);


            try {
                result = cmdQueueEvol->enqueueNDRangeKernel(krnlEvolucao, cl::NullRange, cl::NDRange((NUM_INDIV-novosIndividuos)/2), cl::NDRange(1),NULL, &e_tempo);
            } catch(cl::Error& e){
                std::cerr << getErrorString(e.err()) << std::endl;
                exit(1);
            }

            cmdQueueEvol->finish();

            e_tempo.getProfilingInfo(CL_PROFILING_COMMAND_START, &inicio);
            e_tempo.getProfilingInfo(CL_PROFILING_COMMAND_END, &fim);
            tempoEvolucao = (fim-inicio)/1.0E9;

            tempoTotalEvolucao += tempoEvolucao;
            std::cout <<"Tempo evolucao = " << tempoEvolucao << std::endl;

            cmdQueueEvol->enqueueReadBuffer(bufferPopA, CL_TRUE, 0, NUM_INDIV * sizeof(Arvore), popAtual);
            cmdQueueEvol->enqueueReadBuffer(bufferPopFCPU, CL_TRUE, 0, NUM_INDIV * sizeof(Arvore), popFutura);
            cmdQueueEvol->finish();




            cmdQueueAval->enqueueWriteBuffer(bufferPopFGPU, CL_TRUE, 0, NUM_INDIV * sizeof(Arvore), popFutura);

            //std::cout << "aqui" << std::endl;
            ///Dispondo argumentos para o kernel + executar
            krnlAvalia.setArg(0, bufferPopFGPU);
            krnlAvalia.setArg(1, dados);
            krnlAvalia.setArg(2, localSize * sizeof(float), NULL);
            //std::cout << "aqui" << std::endl;
            try {
                result = cmdQueueAval->enqueueNDRangeKernel(krnlAvalia, cl::NullRange, cl::NDRange(globalSize), cl::NDRange(localSize), NULL, &e_tempo);
            } catch(cl::Error& e){
                std::cerr << getErrorString(e.err()) << std::endl;
                exit(1);
            }

            //std::cout << "aqui" << std::endl;

            try {
                cmdQueueAval->finish();
            } catch(cl::Error& e){
                std::cerr << getErrorString(e.err()) << std::endl;
                exit(1);
            }
            //std::cout << "aqui" << std::endl;
            ///medir tempos
            e_tempo.getProfilingInfo(CL_PROFILING_COMMAND_START, &inicio);
            e_tempo.getProfilingInfo(CL_PROFILING_COMMAND_END, &fim);
            tempoAvaliacao = (fim-inicio)/1.0E9;

            tempoTotalAvaliacao += tempoAvaliacao;
            std::cout  << "Tempo de avaliacao = " << tempoAvaliacao << std::endl;

            cmdQueueAval->enqueueReadBuffer(bufferPopFGPU, CL_TRUE, 0, NUM_INDIV * sizeof(Arvore), popFutura);

            //popFutura = (Arvore*)cmdQueueGPU.enqueueMapBuffer(bufferPop,CL_FALSE, CL_MAP_READ, 0, NUM_INDIV * sizeof(Arvore));

            cmdQueueAval->finish();



        for(i = 0; i< NUM_INDIV; i++){
            popAtual[i] = popFutura[i];
        }

        tempoGeracao = getTime() - startIt;

        tempoTotal += tempoGeracao;
        std::cout <<"Tempo de geracao = " << tempoGeracao << std::endl;

        //imprimePopulacao(popAtual, LABELS);
        imprimeMelhor(popAtual, LABELS);
        iteracoes++;
        //std::cout << "seed8 = " << seeds[8] <<std::endl;
    }

    printf("\nPOPULACAO FINAL \n");
    imprimePopulacao(popAtual, LABELS);
    printf("*");
    imprimeMelhor(popAtual, LABELS);
    printf("\n\n");

    std::cout << "Tempo total Evolucao = " << tempoTotalEvolucao << std::endl;
    std::cout << "Tempo total Avaliacao = " << tempoTotalAvaliacao << std::endl;
    std::cout << "Tempo total = " << tempoTotal << std::endl;

    return 0;
}
#endif // DIF_CONTEXT
