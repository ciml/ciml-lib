#include <omp.h>        //OpenMP
#include "genetica.h"

#include "GPTime.h"

//#include "OCLHelper.h"
#include "oclConfig.h"  //outras configuracoes e bibliotecas

bool compareSeeds(int* ocl_seeds, int* seq_seeds){
    bool difFlag = false;
    for(int i = 0; i < NUM_INDIV * MAX_NOS; i++){
        if(ocl_seeds[i] != seq_seeds[i]){
            std::cout << i << " " << ocl_seeds[i] << "!=" << seq_seeds[i] << std::endl;
            difFlag = true;
        }
    }
    return difFlag;
}

void checkRandomGenerator(int* seeds, int platform, int device){
    int* seedsCopy;
    seedsCopy = new int [NUM_INDIV * MAX_NOS];
    for(int i = 0; i < NUM_INDIV * MAX_NOS; i++){
        seedsCopy[i] = seeds[i];
    }

    cl_int result; //Variavel para verificar erros
    std::vector<cl::Platform> platforms; /// ok
    std::vector<cl::Device> devices; /// ok

    cl::Platform::get(&platforms);
    platforms[platform].getDevices(CL_DEVICE_TYPE_ALL, &devices);

    printPlatformsDevices(platforms, devices); /// ok

    ///Estabelecendo o contexto com os devices
    cl::Context contexto(devices, nullptr, nullptr, nullptr, &result); /// ok
    if(result != CL_SUCCESS){
        std::cout << "Erro ao criar um contexto OpenCL" << std::endl;
        exit(1);
    }
    ///Criando a fila de comando
    cl_command_queue_properties commandQueueProperties = CL_QUEUE_PROFILING_ENABLE;
    cl::CommandQueue* cmdQueueRand;
    cmdQueueRand = new cl::CommandQueue(contexto, devices[device], commandQueueProperties, &result);

    cl::Buffer bufferSeeds(contexto, CL_MEM_ALLOC_HOST_PTR | CL_MEM_READ_WRITE, NUM_INDIV * MAX_NOS * sizeof(int));


    size_t globalSize = NUM_INDIV * MAX_NOS; /// ok
    size_t localSize = 1;
    std::string compileFlags; /// ok
    (compileFlags) += " -D LOCAL_SIZE_ROUNDED_UP_TO_POWER_OF_2="
                       + ToString( NextPowerOf2(localSize) );

    std::cout << "Global Size = " << globalSize << std::endl << "Local size = " << localSize << std::endl << std::endl;

    ///Leitura do arquivo com o programa em C++
    std::ifstream sourceFileName("kernel.cl");
    std::string sourceFile(std::istreambuf_iterator<char>(sourceFileName),(std::istreambuf_iterator<char>()));
    std::string program_src = setProgramSource(-1, -1, -1, -1, (int)localSize, -1, -1) + sourceFile;

    ///Criar programa por Source
    //cl::Program::Sources source(1, std::make_pair(program_src.c_str(), program_src.length()+1));
    cl::Program programa(contexto, program_src);

    //compileFlags+=" -cl-opt-disable";
    compileFlags+= R"( -I C:\Users\bruno\Desktop\ciml-lib\src\C\bruno\GeneticOpenCL)";
    try {
        programa.build(devices, compileFlags.c_str());
    } catch(cl::Error& e){
        std::cerr << getErrorString(e.err()) << std::endl
                  << programa.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0]);
        std::cout << e.what() << " : " << e.err() << std::endl;
        exit(1);
    }
    cl::Kernel krnlRand(programa, "testRandomGenerator");

    while(!compareSeeds(seeds, seedsCopy)){
        std::cout << "Pass" << std::endl;
        for(int i = 0; i < NUM_INDIV * MAX_NOS; i++){
            rand2(&seedsCopy[i]);
        }

        cmdQueueRand->enqueueWriteBuffer(bufferSeeds, CL_FALSE, 0, NUM_INDIV * MAX_NOS * sizeof(int), seeds);
        cmdQueueRand->finish();

        krnlRand.setArg(0, bufferSeeds);

        try {
            result = cmdQueueRand->enqueueNDRangeKernel(krnlRand, cl::NullRange, cl::NDRange(globalSize), cl::NDRange(localSize), nullptr);
        } catch(cl::Error& e){
            std::cerr << getErrorString(e.err()) << std::endl;
            exit(1);
        }

        try {
            cmdQueueRand->finish();
        } catch(cl::Error& e){
            std::cerr << getErrorString(e.err()) << std::endl;
            exit(1);
        }
        cmdQueueRand->enqueueReadBuffer(bufferSeeds, CL_TRUE, 0, NUM_INDIV * MAX_NOS * sizeof(int), seeds);
        cmdQueueRand->finish();

    }

}

//TODO: conferir questão de multiplas definições com o Opencl 2
#if !DIF_CONTEXT
int main(int argc, char** argv){

    GPTime timeManager(4);
    timeManager.getStartTime(Total_T);

    double tempoAvaliacaoOCL = 0;
    double tempoTotalAvaliacaoOCL = 0;

    double tempoEvolucaoOCL = 0;
    double tempoTotalEvolucaoOCL = 0;

    std::cout << std::setprecision(32) << std::fixed;

    float maxDados, minDados;
    minDados = std::numeric_limits<float>::max();
    maxDados = (-1) * minDados;

    int iteracoes = 0;

    Arvore* popAtual;
    popAtual = new Arvore[NUM_INDIV];

    Arvore* popFutura;
    popFutura = new Arvore[NUM_INDIV];

    int* seeds;
    seeds = new int [NUM_INDIV * MAX_NOS];

    ///variaveis lidas de arquivo
    int M, N;
    char** LABELS;
    int* conjuntoOpTerm;
    float* ctes; //TODO: ler constantes
    int NUM_OPBIN, NUM_OPUN, NUM_CTES;

    ///leituras de dados
    float** dadosTreinamento = readTrainingData(&M, &N, &NUM_CTES, &NUM_OPBIN, &NUM_OPUN, &LABELS, &conjuntoOpTerm, &maxDados, &minDados, argv[1]);

    atribuiSemente(SEED);

    ///transposicao de dados
    float* dadosTranspostos;
    dadosTranspostos = new float [M * N];

    //transposição necessária para otimizar a execução no opencl com acessos sequenciais à memória
    unsigned int pos = 0;
    std::cout << "Transpondo dados..." << std::endl;
    for(int j = 0; j < N; ++j ){
        for(int i = 0; i < M; ++i ){
            dadosTranspostos[pos++] = dadosTreinamento[i][j];
        }
    }

    ///vetor de seeds a ser utilizado no codigo paralelo
    for(int i = 0; i < NUM_INDIV*MAX_NOS; i++){
        seeds[i] = rand();
    }

    //checkRandomGenerator(seeds, GPU_PLATFORM, GPU_DEVICE);

    imprimeParametros(M, N, NUM_CTES, NUM_OPBIN, NUM_OPUN);
    inicializaPopulacao(popAtual, conjuntoOpTerm, NUM_OPBIN, NUM_OPUN, N, &seeds[0], maxDados, minDados);
    avaliaIndividuos(popAtual, /*dadosTranspostos*/dadosTreinamento, M, N);



    #if AVALOCL || EVOLOCL
        cl_ulong inicio, fim;
        ///Evento para controlar tempo gasto
        cl::Event e_tempo;

        cl_int result; //Variavel para verificar erros
        ///TODO: Colocar conferencias de erros pelo codigo
        std::vector<cl::Platform> platforms; /// ok
        std::vector<cl::Device> devices; /// ok

        setupOpenCLOnePlatform(platforms, devices); /// ok
        printPlatformsDevices(platforms, devices); /// ok

        ///Estabelecendo o contexto com os devices
        cl::Context contexto(devices, nullptr, nullptr, nullptr, &result); /// ok
        if(result != CL_SUCCESS){
            std::cout << "Erro ao criar um contexto OpenCL" << std::endl;
            exit(1);
        }

        ///Criando a fila de comando
        cl_command_queue_properties commandQueueProperties = CL_QUEUE_PROFILING_ENABLE;

        cl::CommandQueue* cmdQueueEvol; /// ok
        cl::CommandQueue* cmdQueueAval; /// ok

        setupCmdQueuesOnePlatform(cmdQueueAval, cmdQueueEvol, commandQueueProperties, devices, contexto); /// ok

        cl::Buffer bufferPopA  (contexto, CL_MEM_ALLOC_HOST_PTR | CL_MEM_READ_WRITE, NUM_INDIV * sizeof(Arvore));
        cl::Buffer bufferPopF  (contexto, CL_MEM_ALLOC_HOST_PTR | CL_MEM_READ_WRITE, NUM_INDIV * sizeof(Arvore));
        cl::Buffer bufferOpTerm(contexto, CL_MEM_ALLOC_HOST_PTR | CL_MEM_READ_ONLY, (NUM_OPBIN + NUM_OPUN + NUM_CTES + N - 1) * sizeof(int));
        cl::Buffer bufferSeeds (contexto, CL_MEM_ALLOC_HOST_PTR | CL_MEM_READ_WRITE, NUM_INDIV * MAX_NOS * sizeof(int));
        cl::Buffer dados       (contexto, CL_MEM_ALLOC_HOST_PTR | CL_MEM_READ_ONLY, M * N * sizeof(float));

        cmdQueueEvol->enqueueWriteBuffer(bufferSeeds, CL_FALSE, 0, NUM_INDIV * MAX_NOS * sizeof(int), seeds);
        cmdQueueAval->enqueueWriteBuffer(dados, CL_TRUE, 0, M * N * sizeof(float), dadosTranspostos);
        cmdQueueEvol->enqueueWriteBuffer(bufferOpTerm, CL_FALSE, 0, (NUM_OPBIN + NUM_OPUN + NUM_CTES + N - 1) * sizeof(int), conjuntoOpTerm);

        cmdQueueAval->finish();
        cmdQueueEvol->finish();

        size_t globalSize; /// ok
        size_t localSize; /// ok
        size_t numPoints;
        std::string compileFlags; /// ok

        numPoints = (size_t)M;

        size_t maxLocalSize = cmdQueueAval->getInfo<CL_QUEUE_DEVICE>().getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>(); /// ok
        setNDRanges(&globalSize, &localSize, &compileFlags, maxLocalSize, numPoints, cmdQueueAval->getInfo<CL_QUEUE_DEVICE>().getInfo<CL_DEVICE_TYPE>()); /// ok

        std::cout << "Global Size = " <<globalSize << std::endl << "Local size = " << localSize << std::endl << std::endl;

    /// ok
        ///Leitura do arquivo com o programa em C++
        std::ifstream sourceFileName("kernel.cl");
        std::string sourceFile(std::istreambuf_iterator<char>(sourceFileName),(std::istreambuf_iterator<char>()));
        std::string program_src = setProgramSource(NUM_OPBIN, NUM_OPUN, M, N, (int)localSize, maxDados, minDados) + sourceFile;
        //std::cout << program_src << std::endl;

        ///Criar programa por Source
        //cl::Program::Sources source(1, std::make_pair(program_src.c_str(), program_src.length()+1));
        cl::Program programa(contexto, program_src);

        compileFlags+=" -cl-opt-disable";
        compileFlags+= R"( -I C:\Users\bruno\Desktop\ciml-lib\src\C\bruno\GeneticOpenCL)";
        //std::cout << "Compile Flags = " << compileFlags << std::endl;
        try {
            programa.build(devices, compileFlags.c_str());
        } catch(cl::Error& e){
            std::cerr << getErrorString(e.err()) << std::endl
            << programa.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0]);
            std::cout << e.what() << " : " << e.err() << std::endl;
            exit(1);
        }
        //std::cout << programa.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0]) << std::endl;

        #if AVALGPU
            cl::Kernel krnlAvalia(programa, "avaliaIndividuos");
        #elif AVALOCL && !AVALOCL_SEQ
            cl::Kernel krnlAvalia(programa, "avaliaIndividuosCPU");
        #else
            cl::Kernel krnlAvalia(programa, "avaliaIndividuosSequencial");
        #endif // AVALGPU

        #if EVOLOCL && !EVOLOCL_SEQ
            cl::Kernel krnlEvolucao(programa, "evolucao");
        #else
            cl::Kernel krnlEvolucao(programa, "evolucaoSequencial");
        #endif // EVOLOCL

            //cl::Kernel krnlReplace(programa, "replacePopulation");
/// ok
/*
        OCLHelper teste;
        teste.printPlatformsDevices();
        teste.setupContexts();
        teste.setupCommandQueues(commandQueueProperties);
        teste.setWorkSizesAval(numPoints);
        teste.setupProgramSource("kernel.cl", NUM_OPBIN, NUM_OPUN, M, N, maxDados, minDados);
        teste.allocateBuffers(NUM_OPBIN, NUM_OPUN, NUM_CTES, M, N);
*/
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
            cmdQueueAval->enqueueReadBuffer(bufferPopF, CL_TRUE, 0, NUM_INDIV * sizeof(Arvore), popFutura);
            std::cout << popFutura[0].aptidao << std::endl;

            std::cin >> c;
        }
        #endif // TESTA_INDIV

    #endif // AVALOCL

    //imprimePopulacao(popAtual, LABELS);

    while(criterioDeParada(iteracoes) /*qual o criterio de parada?*/){
        //imprimePopulacao(popAtual, LABELS);
        printf("\n-----------\nGERACAO %d: \n", iteracoes);

        timeManager.getStartTime(Iteracao_T);

        timeManager.getStartTime(Evolucao_T);
        int novosIndividuos = selecionaElite(popAtual, popFutura);

        #if EVOLOCL
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
                #if EVOLOCL_SEQ
                result = cmdQueueEvol->enqueueNDRangeKernel(krnlEvolucao, cl::NullRange, cl::NDRange(1), cl::NDRange(1), NULL, &e_tempo);
                #else
                result = cmdQueueEvol->enqueueNDRangeKernel(krnlEvolucao, cl::NullRange, cl::NDRange((size_t)(NUM_INDIV-novosIndividuos)/2), cl::NDRange(1), nullptr, &e_tempo);
                #endif
            } catch(cl::Error& e){
                std::cerr << getErrorString(e.err()) << std::endl;
                exit(1);
            }

            cmdQueueEvol->finish();

            #if OCL_TIME
            e_tempo.getProfilingInfo(CL_PROFILING_COMMAND_START, &inicio);
            e_tempo.getProfilingInfo(CL_PROFILING_COMMAND_END, &fim);
            tempoEvolucaoOCL = (fim-inicio)/1.0E9;
            tempoTotalEvolucaoOCL += tempoEvolucaoOCL;
            #endif

            ///se esta em um so dispositivo nao precisa disso
            cmdQueueEvol->enqueueReadBuffer(bufferPopA, CL_TRUE, 0, NUM_INDIV * sizeof(Arvore), popAtual);
            //nao preciso dessa copia se estou evoluindo dado que faremos a reposicao de populacoes no opencl
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

                ///testar imprimir o que esta retornando na parte randomica
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

            for(int j = novosIndividuos; j < NUM_INDIV; j +=2){
                int id = (j/2)-(novosIndividuos/2);

                int indice1 = torneio(popAtual, &seeds[id]);
                int indice2 = torneio(popAtual, &seeds[id]);


                popFutura[j] = popAtual[indice1];
                popFutura[j+1] = popAtual[indice2];

                ///testar imprimir o que esta retornando na parte randomica
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
            krnlAvalia.setArg(2, localSize * sizeof(float), nullptr);

            try {
                #if AVALOCL_SEQ
                result = cmdQueueAval->enqueueNDRangeKernel(krnlAvalia, cl::NullRange, cl::NDRange(1), cl::NDRange(1), NULL, &e_tempo);
                #else
                result = cmdQueueAval->enqueueNDRangeKernel(krnlAvalia, cl::NullRange, cl::NDRange(globalSize), cl::NDRange(localSize), nullptr, &e_tempo);
                #endif
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

            #if OCL_TIME
            e_tempo.getProfilingInfo(CL_PROFILING_COMMAND_START, &inicio);
            e_tempo.getProfilingInfo(CL_PROFILING_COMMAND_END, &fim);
            tempoAvaliacaoOCL = (fim-inicio)/1.0E9;
            tempoTotalAvaliacaoOCL += tempoAvaliacaoOCL;
            #endif

            cmdQueueAval->enqueueReadBuffer(bufferPopF, CL_TRUE, 0, NUM_INDIV * sizeof(Arvore), popFutura);

            cmdQueueAval->finish();

        #else // AVALOCL

            avaliaIndividuos(popFutura, dadosTreinamento, M, N);

        #endif // AVALOCL

        timeManager.getEndTime(Avaliacao_T);

        for(int i = 0; i< NUM_INDIV; i++){
            popAtual[i] = popFutura[i];
        }

        timeManager.getEndTime(Iteracao_T);
        double iterTime = timeManager.getElapsedTime(Iteracao_T);
        double avalTime = timeManager.getElapsedTime(Avaliacao_T);
        double evolTime = timeManager.getElapsedTime(Evolucao_T);

        #if PROFILING_TIME
        #if OCL_TIME
        std::cout << "Tempo evolucao OCL = " << tempoEvolucaoOCL << std::endl;
        std::cout << "Tempo avaliacao OCL = " << tempoAvaliacaoOCL << std::endl;
        #endif
        std::cout << "Evol time = " << evolTime << std::endl;
        std::cout << "Aval time = " << avalTime << std::endl;
        std::cout << "Iter time = " << iterTime << std::endl << std::endl;
        #endif

        #if PROFILING_POP
        //imprimePopulacao(popAtual, LABELS);
        imprimeMelhor(popAtual, LABELS);
        #endif
        iteracoes++;
    }
    timeManager.getEndTime(Total_T);

    std::cout << "\n\nPOPULACAO FINAL" << std::endl;
    imprimePopulacao(popAtual, LABELS);
    std::cout << "\n*";
    imprimeMelhor(popAtual, LABELS);
    std::cout << std::endl << std::endl;

    std::cout << "T_Evol time = " << timeManager.getTotalTime(Evolucao_T) << std::endl;
    std::cout << "T_Aval time = " << timeManager.getTotalTime(Avaliacao_T) << std::endl;
    std::cout << "T_Iter time = " << timeManager.getTotalTime(Iteracao_T) << std::endl;
    std::cout << "Total time  = " << timeManager.getElapsedTime(Total_T) << std::endl;
    std::cout << std::endl;
    #if OCL_TIME
    std::cout << "T_EOcl time = " << tempoTotalEvolucaoOCL << std::endl;
    std::cout << "T_AOcl time = " << tempoTotalAvaliacaoOCL << std::endl << std::endl;
    std::cout << "T_TOcl time = " << tempoTotalEvolucaoOCL+tempoTotalAvaliacaoOCL << std::endl;
    #endif

    std::cout << "FIM" << std::endl;
    std::cout << timeManager.getTotalTime(Evolucao_T) << " ";
    std::cout << timeManager.getTotalTime(Avaliacao_T) << " ";
    std::cout << timeManager.getTotalTime(Iteracao_T) << " ";
    std::cout << timeManager.getElapsedTime(Total_T) << std::endl;
    #if OCL_TIME
    std::cout << "OCLFlag" << std::endl;
    std::cout << tempoTotalEvolucaoOCL << " ";
    std::cout << tempoTotalAvaliacaoOCL << " ";
    std::cout << tempoTotalEvolucaoOCL+tempoTotalAvaliacaoOCL << std::endl;
    #endif
    imprimeMelhor(popAtual, LABELS);
    std::cout << std::endl << std::endl;

    delete [] popAtual;
    delete [] popFutura;
    return 0;
}

#else
int main(int argc, char** argv){
    GPTime timeManager(4);
    timeManager.getStartTime(Total_T);

    double tempoAvaliacaoOCL = 0;
    double tempoTotalAvaliacaoOCL = 0;

    double tempoEvolucaoOCL = 0;
    double tempoTotalEvolucaoOCL = 0;

    std::cout << std::setprecision(32) << std::fixed;

    float maxDados, minDados;
    minDados = std::numeric_limits<float>::max();
    maxDados = (-1) * minDados;

    int iteracoes = 0;

    Arvore* popAtual;
    popAtual = new Arvore[NUM_INDIV];

    Arvore* popFutura;
    popFutura = new Arvore[NUM_INDIV];

    int* seeds;
    seeds = new int [NUM_INDIV * MAX_NOS];

    ///variaveis lidas de arquivo
    int M, N;
    char** LABELS;
    int* conjuntoOpTerm;
    float* ctes; //TODO: ler constantes
    int NUM_OPBIN, NUM_OPUN, NUM_CTES;

    ///leituras de dados
    float** dadosTreinamento = readTrainingData(&M, &N, &NUM_CTES, &NUM_OPBIN, &NUM_OPUN, &LABELS, &conjuntoOpTerm, &maxDados, &minDados, argv[1]);

    atribuiSemente(SEED);

    ///transposicao de dados
    float* dadosTranspostos;
    dadosTranspostos = new float [M * N];

    //transposição necessária para otimizar a execução no opencl com acessos sequenciais à memória
    unsigned int pos = 0;
    std::cout << "Transpondo dados..." << std::endl;
    for(int j = 0; j < N; ++j ){
        for(int i = 0; i < M; ++i ){
            dadosTranspostos[pos++] = dadosTreinamento[i][j];
        }
    }

    ///vetor de seeds a ser utilizado no codigo paralelo
    for(int i = 0; i < NUM_INDIV*MAX_NOS; i++){
        seeds[i] = rand();
    }

    ///checkRandomGenerator(seeds, CPU_PLATFORM, CPU_DEVICE);

    imprimeParametros(M, N, NUM_CTES, NUM_OPBIN, NUM_OPUN);
    inicializaPopulacao(popAtual, conjuntoOpTerm, NUM_OPBIN, NUM_OPUN, N, &seeds[0], maxDados, minDados);
    avaliaIndividuos(popAtual, /*dadosTranspostos*/dadosTreinamento, M, N);

        cl_ulong inicio, fim;
        ///Evento para controlar tempo gasto
        cl::Event e_tempo;

        cl_int result; //Variavel para verificar erros
        ///TODO: Colocar conferencias de erros pelo c�digo

        std::vector<cl::Platform> platforms;
        std::vector<cl::Device> devicesCPU, devicesGPU;
        setupOpenCLTwoPlatform(platforms,devicesCPU, devicesGPU);

        cl::Context contextoCPU(devicesCPU, nullptr, nullptr, nullptr, &result);
        if(result != CL_SUCCESS){
            std::cout << "Erro ao criar um contexto OpenCL CPU" << std::endl;
            exit(1);
        }

        cl::Context contextoGPU(devicesGPU, nullptr, nullptr, nullptr, &result);
        if(result != CL_SUCCESS){
            std::cout << "Erro ao criar um contexto OpenCL GPU" << std::endl;
            exit(1);
        }

        //printPlatformsDevices(platforms, devicesCPU);
        //printPlatformsDevices(platforms, devicesGPU);

        ///Criando a fila de comando
        cl_command_queue_properties commandQueueProperties = CL_QUEUE_PROFILING_ENABLE;

        ///4 op��es: GPU-GPU, GPU-CPU, CPU-CPU, CPU-GPU
        ///NUMDEVICES;FIRST

        cl::CommandQueue* cmdQueueEvol;
        cl::CommandQueue* cmdQueueAval;

        setupCmdQueuesTwoPlatform(cmdQueueAval, cmdQueueEvol, commandQueueProperties, devicesCPU, devicesGPU, contextoCPU, contextoGPU);

        #if AVALGPU
            cl::Buffer bufferPopA(contextoCPU, CL_MEM_READ_WRITE, NUM_INDIV * sizeof(Arvore)/*, popAtual*/);
            cl::Buffer bufferPopFCPU(contextoCPU, CL_MEM_READ_WRITE/*|CL_MEM_USE_HOST_PTR*/, NUM_INDIV * sizeof(Arvore)/*, popFutura*/);

            cl::Buffer bufferPopFGPU(contextoGPU, CL_MEM_READ_WRITE/*|CL_MEM_USE_HOST_PTR*/, NUM_INDIV * sizeof(Arvore)/*, popFutura*/);
            cl::Buffer dados(contextoGPU, CL_MEM_READ_ONLY, M*N * sizeof(float));

            cl::Buffer bufferOpTerm(contextoCPU, CL_MEM_READ_ONLY, (NUM_OPBIN+NUM_OPUN+NUM_CTES+N-1)*sizeof(int));
            cl::Buffer bufferSeeds(contextoCPU, CL_MEM_READ_WRITE, NUM_INDIV*MAX_NOS*sizeof(int));
        #endif

        cmdQueueEvol->enqueueWriteBuffer(bufferSeeds, CL_FALSE, 0, NUM_INDIV*MAX_NOS*sizeof(int), seeds);
        cmdQueueAval->enqueueWriteBuffer(dados, CL_FALSE, 0, M*N * sizeof(float), dadosTranspostos);

        cmdQueueEvol->enqueueWriteBuffer(bufferOpTerm, CL_TRUE, 0, (NUM_OPBIN+NUM_OPUN+NUM_CTES+N-1)*sizeof(int), conjuntoOpTerm);

        //cmdQueueAval->enqueueWriteBuffer(bufferOpTerm, CL_TRUE, 0, (NUM_OPBIN+NUM_OPUN+NUM_CTES+N-1)*sizeof(int), conjuntoOpTerm);


        size_t globalSize;
        size_t localSize;
        size_t numPoints;
        std::string compileFlags;

        numPoints = (size_t)M;

        size_t maxLocalSize = cmdQueueAval->getInfo<CL_QUEUE_DEVICE>().getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();
        setNDRanges(&globalSize, &localSize, &compileFlags, maxLocalSize, numPoints, cmdQueueAval->getInfo<CL_QUEUE_DEVICE>().getInfo<CL_DEVICE_TYPE>());

        std::cout << "Global Size = " << globalSize << std::endl << "Local size = " << localSize << std::endl << std::endl;

        ///Leitura do arquivo com o programa em C++
        std::ifstream sourceFileName("kernel.cl");
        std::string sourceFile(std::istreambuf_iterator<char>(sourceFileName),(std::istreambuf_iterator<char>()));
        std::string program_src = setProgramSource(NUM_OPBIN, NUM_OPUN, M, N, localSize, maxDados, minDados) + sourceFile;
        //std::cout << program_src << std::endl;

        ///Criar programa por Source
        //cl::Program::Sources source(1, std::make_pair(program_src.c_str(), program_src.length()+1));
        cl::Program programaCPU(contextoCPU, program_src);
        cl::Program programaGPU(contextoGPU, program_src);

        //compileFlags+=" -cl-opt-disable";
        compileFlags+= R"( -I C:\Users\bruno\Desktop\ciml-lib\src\C\bruno\GeneticOpenCL)";
        //std::cout << "Compile Flags = " << compileFlags << std::endl;
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

    //imprimePopulacao(popAtual, LABELS);

    while(criterioDeParada(iteracoes) /*qual o criterio de parada?*/){
        printf("\n-----------\nGERACAO %d: \n", iteracoes);

        timeManager.getStartTime(Iteracao_T);
        timeManager.getStartTime(Evolucao_T);

        int novosIndividuos = selecionaElite(popAtual, popFutura);

        cmdQueueEvol->enqueueWriteBuffer(bufferPopA, CL_TRUE, 0, NUM_INDIV * sizeof(Arvore), popAtual);
        cmdQueueEvol->enqueueWriteBuffer(bufferPopFCPU, CL_TRUE, 0, NUM_INDIV * sizeof(Arvore), popFutura);
        cmdQueueEvol->finish();

        ///Evento para controlar tempo gasto
        ///Dispondo argumentos para o kernel + executar
        krnlEvolucao.setArg(0, bufferPopA);
        krnlEvolucao.setArg(1, bufferPopFCPU);
        krnlEvolucao.setArg(2, novosIndividuos);
        krnlEvolucao.setArg(3, bufferOpTerm);
        krnlEvolucao.setArg(4, bufferSeeds);


        try {
            result = cmdQueueEvol->enqueueNDRangeKernel(krnlEvolucao, cl::NullRange, cl::NDRange((NUM_INDIV-novosIndividuos)/2), cl::NDRange(1), NULL, &e_tempo);
        } catch(cl::Error& e){
            std::cerr << getErrorString(e.err()) << std::endl;
            exit(1);
        }

        cmdQueueEvol->finish();

        #if OCL_TIME
        e_tempo.getProfilingInfo(CL_PROFILING_COMMAND_START, &inicio);
        e_tempo.getProfilingInfo(CL_PROFILING_COMMAND_END, &fim);
        tempoEvolucaoOCL = (fim-inicio)/1.0E9;
        tempoTotalEvolucaoOCL += tempoEvolucaoOCL;
        #endif

        cmdQueueEvol->enqueueReadBuffer(bufferPopA, CL_TRUE, 0, NUM_INDIV * sizeof(Arvore), popAtual);
        cmdQueueEvol->enqueueReadBuffer(bufferPopFCPU, CL_TRUE, 0, NUM_INDIV * sizeof(Arvore), popFutura);
        cmdQueueEvol->finish();

        timeManager.getEndTime(Evolucao_T);


        timeManager.getStartTime(Avaliacao_T);

        cmdQueueAval->enqueueWriteBuffer(bufferPopFGPU, CL_TRUE, 0, NUM_INDIV * sizeof(Arvore), popFutura);


        ///Dispondo argumentos para o kernel + executar
        krnlAvalia.setArg(0, bufferPopFGPU);
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

        #if OCL_TIME
        e_tempo.getProfilingInfo(CL_PROFILING_COMMAND_START, &inicio);
        e_tempo.getProfilingInfo(CL_PROFILING_COMMAND_END, &fim);
        tempoAvaliacaoOCL = (fim-inicio)/1.0E9;
        tempoTotalAvaliacaoOCL += tempoAvaliacaoOCL;
        #endif

        cmdQueueAval->enqueueReadBuffer(bufferPopFGPU, CL_TRUE, 0, NUM_INDIV * sizeof(Arvore), popFutura);

        //popFutura = (Arvore*)cmdQueueGPU.enqueueMapBuffer(bufferPop,CL_FALSE, CL_MAP_READ, 0, NUM_INDIV * sizeof(Arvore));

        cmdQueueAval->finish();

        timeManager.getEndTime(Avaliacao_T);

        for(int i = 0; i< NUM_INDIV; i++){
            popAtual[i] = popFutura[i];
        }

        timeManager.getEndTime(Iteracao_T);

        double iterTime = timeManager.getElapsedTime(Iteracao_T);
        double avalTime = timeManager.getElapsedTime(Avaliacao_T);
        double evolTime = timeManager.getElapsedTime(Evolucao_T);

        #if PROFILING_TIME
        #if OCL_TIME
        std::cout << "Tempo evolucao OCL = " << tempoEvolucaoOCL << std::endl;
        std::cout << "Tempo avaliacao OCL = " << tempoAvaliacaoOCL << std::endl;
        #endif
        std::cout << "Evol time = " << evolTime << std::endl;
        std::cout << "Aval time = " << avalTime << std::endl;
        std::cout << "Iter time = " << iterTime << std::endl << std::endl;
        #endif

        #if PROFILING_POP
        //imprimePopulacao(popAtual, LABELS);
        imprimeMelhor(popAtual, LABELS);
        #endif
        iteracoes++;
    }
    timeManager.getEndTime(Total_T);

    std::cout << "\n\nPOPULACAO FINAL" << std::endl;
    imprimePopulacao(popAtual, LABELS);
    std::cout << "\n*";
    imprimeMelhor(popAtual, LABELS);
    std::cout << std::endl << std::endl;

    std::cout << "T_Evol time = " << timeManager.getTotalTime(Evolucao_T) << std::endl;
    std::cout << "T_Aval time = " << timeManager.getTotalTime(Avaliacao_T) << std::endl;
    std::cout << "T_Iter time = " << timeManager.getTotalTime(Iteracao_T) << std::endl;
    std::cout << "Total time  = " << timeManager.getElapsedTime(Total_T) << std::endl;
    std::cout << std::endl;
    #if OCL_TIME
    std::cout << "T_EOcl time = " << tempoTotalEvolucaoOCL << std::endl;
    std::cout << "T_AOcl time = " << tempoTotalAvaliacaoOCL << std::endl << std::endl;
    std::cout << "T_TOcl time = " << tempoTotalEvolucaoOCL+tempoTotalAvaliacaoOCL << std::endl;
    #endif

    std::cout << "FIM" << std::endl;
    std::cout << timeManager.getTotalTime(Evolucao_T) << " ";
    std::cout << timeManager.getTotalTime(Avaliacao_T) << " ";
    std::cout << timeManager.getTotalTime(Iteracao_T) << " ";
    std::cout << timeManager.getElapsedTime(Total_T) << std::endl;
    #if OCL_TIME
    std::cout << "OCLFlag" << std::endl;
    std::cout << tempoTotalEvolucaoOCL << " ";
    std::cout << tempoTotalAvaliacaoOCL << " ";
    std::cout << tempoTotalEvolucaoOCL+tempoTotalAvaliacaoOCL << std::endl;
    #endif

    imprimeMelhor(popAtual, LABELS);
    std::cout << std::endl << std::endl;

    delete [] popAtual;
    delete [] popFutura;
    return 0;
}
#endif // DIF_CONTEXT
