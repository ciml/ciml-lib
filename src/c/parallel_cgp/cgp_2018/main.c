#include <stdio.h>
#include <time.h>
#include "include/individuo.h"
#include "include/paraleloGPU.h"
#define FILENAME "paraleloGPU.cl"
#define MAXGERACOES 5000000

/*
PORTAS:
0  - and
1  - or
2  - nand
3  - nor
4  - xor
5  - xnor
6  - A and Not B
7  - A
8  - B and Not A
9  - B
10 - Not B
11 - A or Not B
12 - Not A
13 - B or Not A
*/

unsigned int semente = 0;

void imprimeTabelaVerdade(int tabelaVerdade[])
{
    for (int i = 0; i < infos.numeroDeLinhasDaTabela * infos.numeroDeColunasDaTabela; i++)
    {
        if (i != 0 && i % infos.numeroDeColunasDaTabela == 0)
            printf("\n");
        printf("%d ", tabelaVerdade[i]);
    }
    printf("\n");
}

void sorteiaSaidasDoCircuito(int saidas[])
{
    int individuo;
    int saida;

    for (saida = 0; saida < infos.numeroDeSaidas; saida++)
    {
        saidas[saida] = geraNumeroAleatorio(0, (NLIN * NCOL) + (infos.numeroDeEntradas) - 1);
    }
    for (individuo = 1; individuo < NPOPULACAO; individuo++)
    {
        for (saida = 0; saida < infos.numeroDeSaidas; saida++)
        {
            saidas[individuo * infos.numeroDeSaidas + saida] = saidas[saida];
        }
    }
}

void zeraPontuacaoDosIndividuos(int *pontuacaoDosIndividuos)
{
    int individuo;
    for (individuo = 1; individuo < NPOPULACAO; individuo++)
    {
        pontuacaoDosIndividuos[individuo] = 0;
    }
}

void imprimeSaidasInicias(int saidas[])
{
    for (int saida = 0; saida < infos.numeroDeSaidas; saida++)
    {
        printf("%d\t", saidas[saida]);
    }
    printf("\n");
}

void imprimeIndividuoSolucao(int individuo, int pontuacaoMaxima, int saidas[], Gene populacao[])
{
    printf("ACHOU!!!  INDIVIDUO %d\n", individuo);
    printf("PONTUAÇÃO: %d\n", pontuacaoMaxima);
    int posicao;
    for (int coluna = 0; coluna < NCOL; coluna++) ///criei esse for apenas pra analisar se os dados estavam entrando na matriz de forma correta
    {
        for (int linha = 0; linha < NLIN; linha++)
        {
            posicao = retornaPosicaoDoGene(0, linha, coluna);
            printf("Número do gene: %d.\n", (infos.numeroDeEntradas) + coluna * NLIN + linha);
            printf("Lig 1: %d \t Lig2: %d \t Porta: %d \n\n", populacao[posicao].lig1,
                   populacao[posicao].lig2, populacao[posicao].porta);
        }
    }
    for (int saida = 0; saida < infos.numeroDeSaidas; saida++)
    {
        printf("Saida: %d\n", saidas[individuo * infos.numeroDeSaidas + saida]);
        printf("Numero de portas ativas: %d\n", calculaNumeroDePortasAtivas(populacao, 0, saidas[saida]));
    }
}


void evoluiPopulacao(int portas[], Gene populacao[], int inicializa, char nomeDoArquivo[], long int *geracao)
{
    leituraDadosDaTabelaVerdade(&infos.numeroDeLinhasDaTabela, &infos.numeroDeColunasDaTabela, &infos.numeroDeEntradas, &infos.numeroDeSaidas, nomeDoArquivo);
    inicializaPopulacao(populacao);

    /*Datas from evolution*/
    int *tabelaVerdade = (int*)malloc(sizeof(int) * infos.numeroDeLinhasDaTabela * infos.numeroDeColunasDaTabela);
    int saidas[NPOPULACAO * infos.numeroDeSaidas];
    int pontuacaoMaxima = infos.numeroDeLinhasDaTabela * infos.numeroDeSaidas;
    int pontuacaoDosIndividuos[NPOPULACAO] = {0};
    int maiorPont = 0;
    int pontuacao = 0;

    /*Datas from OpenCL*/
    cl_device_id device;
	cl_context context;
	cl_program program;
	cl_command_queue cmq;
	cl_kernel *kernels;
	cl_int err;
    cl_uint numKernels;

    /*Datas from evaluation*/
    int pontos[(NPOPULACAO - 1) * infos.numeroDeLinhasDaTabela];
	cl_uint tamPopulacao = NPOPULACAO * NLIN * NCOL;
	cl_uint tamTabela = infos.numeroDeLinhasDaTabela * infos.numeroDeColunasDaTabela;
	cl_uint tamSaidas = NPOPULACAO * infos.numeroDeSaidas;
	cl_uint tamPontos = infos.numeroDeLinhasDaTabela * (NPOPULACAO - 1);
	size_t globalSize = (NPOPULACAO - 1) * infos.numeroDeLinhasDaTabela;
	size_t localSize = infos.numeroDeLinhasDaTabela;

    /*Buffers from evaluation*/
    cl_mem buffer_populacao;
	cl_mem buffer_tabela;
	cl_mem buffer_saidas;
	cl_mem buffer_pontos;
	cl_mem buffer_infos;

    /*Read truth table from file and prints*/
    printf("%d %d %d %d\n", infos.numeroDeLinhasDaTabela, infos.numeroDeColunasDaTabela, infos.numeroDeEntradas, infos.numeroDeSaidas);
    extraiTabelaVerdadeDoTxt(infos.numeroDeLinhasDaTabela, infos.numeroDeColunasDaTabela, tabelaVerdade, nomeDoArquivo);
    imprimeTabelaVerdade(tabelaVerdade);

    /*Randomize the outputs and prints*/
    sorteiaSaidasDoCircuito(saidas);
    imprimeSaidasInicias(saidas);
    
    /*Set OpenCL configs*/
    device = criaDevice();
    context = criaContext(device);
    program = montaProgram(context, device, FILENAME);
    cmq = criaCommandQueue(context, device);
    kernels = criaKernels(program, &numKernels);

    /*Set buffers*/
    buffer_populacao = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
        sizeof(Gene) * tamPopulacao, populacao, &err);
    buffer_tabela = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
        sizeof(int) * tamTabela, tabelaVerdade, &err);
    buffer_saidas = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
        sizeof(int) * tamSaidas, saidas, &err);
    buffer_pontos = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
        sizeof(int) * tamPontos, pontos, &err);
    buffer_infos = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
        sizeof(int) * 7, &infos, &err);
    if(err < 0)
    {
        printErrorString(err);
        perror("Nao foi possivel criar buffer!");
        exit(1);
    }

    /*Initializes the parent and evaluate*/
    if (inicializa == TRUE)
    {
        criaPrimeiroIndividuo(populacao, portas); /// iniciando primeiro indivíduo
        for (int saidaDoIndividuo = 0; saidaDoIndividuo < infos.numeroDeSaidas; saidaDoIndividuo++)
        {
            //imprimeIndividuoSolucao(0,pontuacaoMaxima, numeroDeEntradas, numeroDeSaidas, saidas[0],populacao[0]);
            pontuacao += avaliaPontuacao(populacao, 0, saidas[saidaDoIndividuo],
                tabelaVerdade, saidaDoIndividuo);
        }
        pontuacaoDosIndividuos[0] = pontuacao;
    }

    /*Evolution*/
    while (pontuacaoDosIndividuos[0] != pontuacaoMaxima && (*geracao) < MAXGERACOES)
    {

        zeraPontuacaoDosIndividuos(pontuacaoDosIndividuos);
        igualaIndividuosAoPai(populacao, saidas);
        (*geracao)++;
        for (int individuo = 1; individuo < NPOPULACAO; individuo++)
        {
            mutacao(populacao, individuo, portas, saidas);
        }

        /*Enqueue WriteBuffer*/
        err = clEnqueueWriteBuffer(cmq, buffer_populacao, CL_TRUE, 0, sizeof(Gene) * tamPopulacao, populacao, 0, NULL, NULL);
        err = clEnqueueWriteBuffer(cmq, buffer_tabela, CL_TRUE, 0, sizeof(int) * tamTabela, tabelaVerdade, 0, NULL, NULL);
        err = clEnqueueWriteBuffer(cmq, buffer_saidas, CL_TRUE, 0, sizeof(int) * tamSaidas, saidas, 0, NULL, NULL);
        err = clEnqueueWriteBuffer(cmq, buffer_pontos, CL_TRUE, 0, sizeof(int) * tamPontos, pontos, 0, NULL, NULL);
        err = clEnqueueWriteBuffer(cmq, buffer_infos, CL_TRUE, 0, sizeof(int) * 7, &infos, 0, NULL, NULL);
        if(err < 0)
        {
            printErrorString(err);
            perror("Nao foi possivel empilhar WriteBuffer!");
            exit(1);
        }
        clFinish(cmq);
        
        /*Set evaluation kernel arguments*/
        setArgumentsAvaliacao(buffer_populacao, buffer_tabela, buffer_saidas, buffer_pontos,
            buffer_infos, kernels);

        /*Enqueue the evalution kernel*/
        enqueueAvaliacao(cmq, kernels, localSize, globalSize);
        clFinish(cmq);

        /*Read from buffer the resul*/
        enqueueLeituraResultado(cmq, buffer_pontos, pontos, tamPontos);
        clFinish(cmq);

        for(int i = 0; i < NPOPULACAO - 1; i++)
        {
            pontuacaoDosIndividuos[i + 1] = pontos[i * infos.numeroDeLinhasDaTabela];
        }

        maiorPont = 0; /// guarda o indice do individuo com maior pontuacao
        for (int individuo = 1; individuo < NPOPULACAO; individuo++)
        {
            
            if (pontuacaoDosIndividuos[individuo] >= pontuacaoDosIndividuos[maiorPont])
            {
                maiorPont = individuo;
            }
        }

        pontuacaoDosIndividuos[0] = pontuacaoDosIndividuos[maiorPont];
        if (*geracao % 50000 == 0)
            printf("Individuo de maior pontuacao:%d PONTUACAO:%d PONTUACAO DESEJADA:%d GERACAO:%ld\n", maiorPont, pontuacaoDosIndividuos[0], pontuacaoMaxima, *geracao);

        tornaPaiDaProximaGeracao(populacao, maiorPont, saidas);
    }
    printf("pontuacao final: %d \n", pontuacaoDosIndividuos[0]);
    imprimeIndividuoSolucao(maiorPont, pontuacaoMaxima, saidas, populacao);
    imprimePontuacaoEmEstatisticas(pontuacaoDosIndividuos[0]);


    /*Release all the objects used*/ 
    clReleaseDevice(device);
	clReleaseProgram(program);
	clReleaseContext(context);
	clReleaseMemObject(buffer_pontos);
	clReleaseMemObject(buffer_populacao);
	clReleaseMemObject(buffer_saidas);
	clReleaseMemObject(buffer_tabela);
	clReleaseMemObject(buffer_infos);
	clReleaseCommandQueue(cmq);
	clReleaseKernel(kernels[0]);
}

void chamadaEvolucao(long int *geracaoInicial, long int *geracaoFinal, char txt[], int portas[PORTAS], Gene populacao[], int inicializa)
{
    clock_t inicio, fim;
    double tempo;

    inicio = clock();
    evoluiPopulacao(portas, populacao, inicializa, txt, geracaoInicial);
    fim = clock();
    tempo = (fim - inicio) / (double)CLOCKS_PER_SEC;

    imprimeEstatisticasNoTxt(tempo, geracaoInicial, geracaoFinal, semente);

    *geracaoFinal = *geracaoInicial;
}

void evoluacaoSequencial(long int *geracao, long int *geracaoAnterior, int portas[], Gene populacao[])
{
    chamadaEvolucao(geracao, geracaoAnterior, "multiplicador2x2.txt", portas, populacao, 1);
    if (*geracao < MAXGERACOES)
        chamadaEvolucao(geracao, geracaoAnterior, "multiplicador3x3.txt", portas, populacao, 0);
    if (*geracao < MAXGERACOES)
        chamadaEvolucao(geracao, geracaoAnterior, "multiplicador4x4.txt", portas, populacao, 0);
}

void evolucaoUnica(long int *geracao, long int *geracaoAnterior, int portas[], Gene populacao[])
{
    chamadaEvolucao(geracao, geracaoAnterior, "multiplicador3x3.txt", portas, populacao, 1);
}

int main(int argc, char **argv)
{
    char *ptr;
    semente = (unsigned int)strtol(argv[1], &ptr, 10);
    srand(semente);
    //int arg = (unsigned int)strtol(argv[2], &ptr, 10);

    long int *geracao = malloc(sizeof(long int));
    long int *geracaoAnterior = malloc(sizeof(long int));
    *geracao = 0;
    *geracaoAnterior = 0;
    Gene *populacao = malloc(sizeof(Gene) * NPOPULACAO * NLIN * NCOL);

    infos.nlin = NLIN;
    infos.ncol = NCOL;
    infos.npopulacao = NPOPULACAO;

    int portas[PORTAS] = {0, 1, 2, 3, 4};

    /*
    if(arg == 1)
        evoluacaoSequencial(geracao,geracaoAnterior,portas,populacao);
    else if(arg == 2)
        evolucaoUnica(geracao,geracaoAnterior,portas,populacao);
    else{
        printf("Argumento 2 de entradasTabela inválido!\n");
    }
    */
    evolucaoUnica(geracao, geracaoAnterior, portas, populacao);

    free(geracao);
    free(geracaoAnterior);
    return 0;
}
