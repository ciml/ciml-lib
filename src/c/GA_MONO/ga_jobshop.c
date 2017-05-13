#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "geneticalgorithm.h"
#include "crossover.h"
#include "mutation.h"

int popLenght;
int numGeneration; //Número de Gerações do AG interno

float probCrossover;    // Probabilidade de realizar Crossover
float probMutation;     // Probabilidade de realizar Mutação

// DECLARAÇÕES AG INTERNO

//Variáveis do AG interno

int nJobs = 0;                // Número de Jobs
int nMachines = 0;            // Número de Máquinas
int flowMakespan = 0;         // Auxiliar que armazena o máximo de makespan
int ***jobMachine;            // Relaciona o tempo dos jobs nas máquinas
int *qMachines;               /* Aramazena a quantidade de máquinas
                                                  disponíveis em cada estágio*/
int *dueDate;                 // Data de vencimento dos jobs
double z[2];               //Valores utópicos para o hypervolume
double HV;

node **operation;                 //Info de início e término dos jobs
node ***Gantt;                    // Ponteiro para as operações


//funções AG interno
void AGinterno(); //Main loop
void readDataFile(char fileName[]);
void jobDueDate();
void maxMakespan();
void printDataFile(char *fileName, char *nRepeat,
                                        char pop[], char pcros[], char pmut[]);
void printMakespan(char *fileName, char *nRepeat);
void printBestIndividual(char *fileName, char *nRepeat, int countGen);

int main(int argc, char *argv[])
{
  int i, j, orc;
  char fileName[10];
  char nRepeat[4];
  // char popString[4], probCrossString[4], probMutaString[4];
  char popString[] = "100", probCrossString[] = "0.8", probMutaString[] = "0.9";
  
  srand((unsigned)time(NULL));

  strcpy(fileName, argv[1]);
  strcpy(nRepeat, argv[2]);

  readDataFile(fileName);

  orc = nJobs * nJobs * (800 + 10 * nJobs);
  if (orc > 5000000)
    orc = 5000000;

  if( argc < 6){
    printf("Está faltando argumentos\n" );
    printf("Executando programa default\n" );
    popLenght = 100;
    numGeneration = orc / popLenght;
    probCrossover = 0.8;
    probMutation = 0.9;
  } else {
    popLenght = atoi(argv[3]);
    numGeneration = orc / popLenght;
    printf("%d %d\n", popLenght, numGeneration);
    probCrossover = atof(argv[4]);
    probMutation = atof(argv[5]);
    strcpy(popString, argv[3]);
    strcpy(probCrossString, argv[4]);
    strcpy(probMutaString, argv[5]);
  }

  jobDueDate();

  //Aloca a memória
  individuals = (ind*)malloc(2 * popLenght * sizeof(ind));
  for(i = 0; i < (2 * popLenght); i++)
  	individuals[i].jobsOrder = (int*)malloc(nJobs * sizeof(int));

  selectedIndividuals = (ind*)malloc(popLenght * sizeof(ind));
  for(i = 0; i < (popLenght); i++)
    selectedIndividuals[i].jobsOrder = (int*)malloc(nJobs * sizeof(int));  

  //Aloca a memória do Gantt
  operation = (node**)malloc((nJobs * nMachines) * sizeof(node*));
  for(i = 0; i < (nJobs*nMachines); i++)
    operation[i] = (node*)malloc(sizeof(node));

  Gantt = (node***)malloc(nMachines * sizeof(node**));
  for(i = 0; i < nMachines; i++)
  {
    Gantt[i] = (node**)malloc(qMachines[i] * sizeof(node*));
    for(j = 0; j < qMachines[i]; j++)
      Gantt[i][j] = (node*)malloc(sizeof(node));
  }

  tmp = (node***)malloc(3 * sizeof(node**));
  for(i = 0; i < 3; i++)
  {
    tmp[i] = (node**)malloc(2 * sizeof(node*));
    for(j = 0; j < 2; j++){
      tmp[i][j] = (node*)malloc(sizeof(node));
      tmp[i][j] = NULL;
    }
  }

  AGinterno();

  // printDataFile(fileName, nRepeat, popString, probCrossString, probMutaString);
  printMakespan(fileName, nRepeat);

  //Desaloca a memória
  for(i = 0; i < nMachines; i++)
  {
    for(j = 0; j < qMachines[i]; j++)
      free(Gantt[i][j]);
    free(Gantt[i]);
  }
  free(Gantt);
  for(i = 0; i < (nJobs*nMachines); i++)
    free(operation[i]);
  free(operation);
  for(i = 0; i < nJobs; i++)
  {
    for(j = 0; j < nMachines; j++)
      free(jobMachine[i][j]);
    free(jobMachine[i]);
  }
  free(jobMachine);
  for(i = 0; i < popLenght; i++)
    free(selectedIndividuals[i].jobsOrder);
  free(selectedIndividuals);
  for(i = 0; i < (2*popLenght); i++)
    free(individuals[i].jobsOrder);
  free(individuals);
  free(qMachines);
  free(dueDate);
  for(i = 0; i < 3; i++)
  {
    for(j = 0; j < 2; j++)
    {
      tmp[i][j] = NULL;
      free(tmp[i][j]);
    }
    free(tmp[i]);
  }
  free(tmp);

  return 0;
}

void AGinterno()
{
  int i, countGen = 0, father1 = 0, father2 = 0, countInd;
  double x; //Número para o sorteio

  initializeIndividuals();

  // Laço que calcula o fitness do pai para as duas funções objetivo
  for(i = 0; i < popLenght; i++)
  {
    createGantt(i);
  }

  //Laço principal do AG
  while(countGen < numGeneration)
  {
    initializeGeneration();
    countInd = popLenght;
    while(countInd < 2*popLenght)
    {
      // Seleção dos pais
      tournament(&father1, &father2);

      // Cálculo da chance de realizar crossover
      x = (double)((double)rand() / (double)RAND_MAX);
      if(x < probCrossover)
      {
        PMXcrossover(countInd, father1, father2);
      }
      else
      {
        for(i = 0; i < nJobs; i++)
        {
          individuals[countInd].jobsOrder[i] =
                                              individuals[father1].jobsOrder[i];
          individuals[countInd + 1].jobsOrder[i] =
                                              individuals[father2].jobsOrder[i];
        }
      }
      // Chance de realizar mutação
      x = (double)((double)rand() / (double)RAND_MAX);
      if(x < probMutation)
      {
        shiftMutation(countInd);
      }

      // Chance de realizar mutação
      x = (double)((double)rand() / (double)RAND_MAX);
      if(x < probMutation)
      {
        shiftMutation(countInd + 1);
      }

      countInd = countInd + 2;
    }

    for(i = popLenght; i < (2*popLenght); i++)
    {
      createGantt(i);
    }
    selection();

    qsort(individuals, (2*popLenght), sizeof(ind), sortFitMakespan);
    // printBestIndividual(fileName, nRepeat, countGen);

    countGen++;
  }
} // AGinterno

/* FUNÇÕES DE INICIALIZAÇÃO E DO AG*/
// Realiza a leitura do arquivo
void readDataFile(char fileName[])
{
  int i, j, k;
  FILE *fileInput;
  fileInput = fopen(fileName, "r");

  if (fileInput == NULL)
    printf("\nO arquivo '%s' n%co p%ode ser lido!\n", fileName, 134, 147);
  else
  {
    //recebe o número de Jobs
    fscanf(fileInput, "%d\n", &nJobs);

    //recebe o número de Máquinas
    fscanf(fileInput, "%d\n", &nMachines);

    //recebe o Z[0] e flowMakespan
    fscanf(fileInput, "%lf\n", &z[0]);
    flowMakespan = (int)z[0];

    fscanf(fileInput, "%lf\n", &z[1]);

    qMachines = (int*)malloc(nMachines * sizeof(int));
    for(i = 0; i < nMachines; i++)
      fscanf(fileInput, "%d\n", &qMachines[i]);

    //Cria a matriz que relaciona o tempo dos jobs com as máquinas
    jobMachine = (int***)malloc(nJobs*sizeof(int**));
    for(i = 0; i < nJobs; i++)
      jobMachine[i] = (int**)malloc(nMachines*sizeof(int*));
    for(i = 0; i < nJobs; i++)
      for(j = 0; j < nMachines; j++)
        jobMachine[i][j] = (int*)malloc(2*sizeof(int));

    //Preenche a matriz
    for(i = 0; i < nJobs; i++)
      for(j = 0; j < nMachines; j++)
        for(k = 0; k < 2; k++)
          fscanf(fileInput, "%d", &jobMachine[i][j][k]);
  }
  fclose(fileInput);
} // Função readDataFile

void jobDueDate()
{
  int i, j, aux;
  dueDate = (int*)malloc(nJobs*sizeof(int));
  /* A data de término foi calculada de acordo com o proposto por Baker,
                                                      ver artigo Nguyen(2013)*/
  for(i = 0; i < nJobs; i++)
  {
    aux = 0;
    for(j = 0; j < nMachines; j++)
    {
      aux += jobMachine[i][j][1];
    }
    //O fator h = 1.3 é o fator de pressão utilizado por Nguyen
    dueDate[i] = (int)(aux * 1.3);
  }
}//jobDueDate

// Função que calcula o máximo de Makespan para um caso Flowshop
void maxMakespan()
{
  int i, j, k;
  int *vetMachines; // Variável que calcula o tempo utilizado em cada máquina
  vetMachines = (int*)malloc(nMachines*sizeof(int));

  for(k = 0; k < popLenght; k++)
  {
    //Reinicia o vetMachines após cada interação
    for(i = 0; i < nMachines; i++)
      vetMachines[i] = 0;

    for(i = 0; i < nJobs; i++)
    {
      for(j = 0; j < nMachines; j++)
      {
        if (j == 0)
          vetMachines[j] += jobMachine[individuals[k].jobsOrder[i]][j][1];
        else
          if(vetMachines[j] < vetMachines[j-1])
            vetMachines[j] = vetMachines[j-1] +
                                  jobMachine[individuals[k].jobsOrder[i]][j][1];
          else
            vetMachines[j] += jobMachine[individuals[k].jobsOrder[i]][j][1];
      }
    }
    if(vetMachines[nMachines - 1] > flowMakespan)
      flowMakespan = vetMachines[nMachines - 1];
  }
  flowMakespan = flowMakespan + (int)(flowMakespan/2);

  // Libera a memória
  free (vetMachines);
} //maxMakespan

void printDataFile(char *fileName, char *nRepeat,
                                        char pop[], char pcros[], char pmut[])
 {
  int i;
  char *outputName =
        (char*)malloc((strlen(fileName) + strlen(nRepeat) + strlen(pop) +
              strlen(pcros)+ strlen(pmut) + 16)*sizeof(char));
  strcpy(outputName, fileName);
  strcat(outputName, "_");
  strcat(outputName, nRepeat);
  strcat(outputName, "_");
  strcat(outputName, pop);
  strcat(outputName, "_");
  strcat(outputName, pcros);
  strcat(outputName, "_");
  strcat(outputName, pmut);
  strcat(outputName, "_output.txt");
  FILE *output;
  output = fopen(outputName , "w");
  if (output == NULL)
  {
    printf("Arquivo não encontrado!\n");
    exit(1);
  }

  // AGinterno();

  for(i = 0; i < popLenght; i++){
    if(individuals[i].iRank == 1){
      fprintf(output, "%d \t %d \t %3.2lf \t %d \t %d\n", i,
              individuals[i].iRank, individuals[i].iDistance,
                      individuals[i].fitMakespan, individuals[i].fitTardiness);
    }
  }
  fprintf(output, "\n\n");
  fclose(output);
  free(outputName);
} //printDataFile

void printMakespan(char *fileName, char *nRepeat)
{
  char *outputName =
        (char*)malloc((strlen(fileName) + 8)*sizeof(char));
  strcpy(outputName, fileName);
  strcat(outputName, "_MK.txt");
  FILE *output;
  output = fopen(outputName , "a");
  if (output == NULL)
  {
    printf("Arquivo não encontrado!\n");
    exit(1);
  }

  fprintf(output, "%d \t %d \t %1.1lf \t %1.1lf \t %s \t %d\n",
    popLenght, numGeneration, probCrossover, probMutation, nRepeat, individuals[0].fitMakespan);

  fclose(output);
  free(outputName);

} //printHypervolume

void printBestIndividual(char *fileName, char *nRepeat, int countGen)
{
  char *outputName =
        (char*)malloc((strlen(fileName) + 10)*sizeof(char));
  strcpy(outputName, fileName);
  strcat(outputName, "_c_MK.txt");
  FILE *output;
  output = fopen(outputName , "a");
  if (output == NULL)
  {
    printf("Arquivo não encontrado!\n");
    exit(1);
  }

  fprintf(output, "%d \t %d \t %1.1lf \t %1.1lf \t %s \t %d\n",
    popLenght, numGeneration, probCrossover, probMutation, nRepeat,
                                                    individuals[0].fitMakespan);

  fclose(output);
  free(outputName);

} //printBestIndividual
