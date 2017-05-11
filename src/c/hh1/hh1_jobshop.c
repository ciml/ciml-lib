#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "geneticalgorithm.h"
#include "crossover.h"
#include "mutation.h"

#define HNINDIVIDUALS 10  // Número de indivíduos da hiperheurística
#define NEXPERIMENTS 5  //Número de execuções do AG interno para cada AG externo

#define HNUMGENERATION 5 //Número de Gerações da hyper-heurística
#define NUMGENERATION 10 //Número de Gerações do AG interno

#define PCROSSOVER 0.8    // Probabilidade de realizar Crossover
#define PMUTATION  0.15    // Probabilidade de realizar Mutação

//DECLARAÇÕES HIPERHEURISTICA

//Se mais técnicas forem implantadas, deve-se alterar o valor também na struct
int HnumCrossover = 3;  //Quantidade de técnicas de crossover implantadas
int HnumMutation = 2;   // Quantidade de técnias de mutação implantadas

//Struct Hiperheuristica
typedef struct Hind{
  int HgCrossover[3]; // Sequência de genes para o crossover
  int HgProbCrossover[9];
  double probCrossover;
  int HgMutation[2];   // Sequência de genes para a mutação
  int HgProbMutation[9];
  double probMutation;
  double Hfitness[5];    // Fitness calculada através do hypervolume
}Hind;

//Variável hiperheuristica
Hind Hindividuals[2*HNINDIVIDUALS];
static int a[9], b[9];
double gray, dec;

//Funções hiperheuristica
void hyperheuristic(); //main loop
void HinitializeIndividuals();
int HsortFitness(const void *a, const void *b);
void Htournament(int *Hfather1, int *Hfather2);
void Hcrossover(int HcountInd, int Hfather1, int Hfather2);
void HonePointRecomb(int HcountInd, int Hfather1, int Hfather2);
void Hmutation(int i);
void Hselection();
int HsortHyperheuristicAscending(const void *a, const void *b);
int HsortHyperheuristicDescending(const void *a, const void *b);
void intToGray(int n1);
double grayToInt(int n);

// DECLARAÇÕES AG INTERNO

//Variáveis do AG interno

int ***jobMachine;            // Relaciona o tempo dos jobs nas máquinas
int nJobs = 0;                // Número de Jobs
int nMachines = 0;            // Número de Máquinas
int *dueDate;                 // Data de vencimento dos jobs
int **Gantt;                  // Matriz que alocará informações
int flowMakespan = 0;         // Auxiliar que armazena o máximo de makespan
double z[2];               //Valores utópicos para o hypervolume

//funções AG interno
void AGinterno(int g, int h); //Main loop
void readDataFile(char fileName[]);
void jobDueDate();
void maxMakespan();
void printDataFile(char *fileName, char *nRepeat);
void printDataFile2(char *fileName, char *nRepeat);

int main(int argc, char *argv[])
{
  int i, j;

  char fileName[10];
  char nRepeat[4];
  if( argc < 3){
    printf("Está faltando argumentos\n" );
    exit(1);
  }
  strcpy(fileName, argv[1]);
  strcpy(nRepeat, argv[2]);


  srand((unsigned)time(NULL));

  readDataFile(fileName);

  jobDueDate();

  //Aloca a memória
  for(i = 0; i < (2*NINDIVIDUALS); i++)
  	individuals[i].jobsOrder = (int*)malloc(nJobs * sizeof(int));

  //Inicialização para calcular o máximo do Makespan
  // initializeIndividuals();

  //Calcula o makespan para essa solução inicial
  // maxMakespan();

  //Aloca a memória do Gantt
  Gantt = (int**)malloc(nMachines*sizeof(int*));
  for(i = 0; i < nMachines; i++)
    Gantt[i] = (int*)malloc(flowMakespan*sizeof(int));

  // { //Cálculo do Z utópico
  //   z[0] = (double)flowMakespan * 1.5;
  //   z[1] = 0;
  //   for(i = 0; i < NINDIVIDUALS; i++)
  //   {
  //     createGantt(i);
  //     individuals[i].fitTardiness = fitnessTardiness();
  //     if(individuals[i].fitTardiness > z[1])
  //       z[1] = individuals[i].fitTardiness;
  //   }
  //   z[1] = z[1] * 2.0;
  // }


  //Inicializa a hiper-heurística
	hyperheuristic();

  qsort(Hindividuals, (HNINDIVIDUALS), sizeof(Hind), HsortHyperheuristicDescending);

  // for(i = 0; i < HNINDIVIDUALS; i++)
  //   printf("%d %.2lf\n", i, Hindividuals[i].Hfitness[2]);

  printDataFile(fileName, nRepeat);
  printDataFile2(fileName, nRepeat);

  //Desaloca a memória
  for(i = 0; i < nJobs; i++)
  {
    for(j = 0; j < nMachines; j++)
      free(jobMachine[i][j]);
    free(jobMachine[i]);
  }
  free(jobMachine);
  for(i = 0; i < (2*NINDIVIDUALS); i++)
  	free(individuals[i].jobsOrder);
  for(i = 0; i < nMachines; i++)
      free(Gantt[i]);
  free(Gantt);
  free(dueDate);
  return 0;
}

// Loops principais
void hyperheuristic()
{
  int i, j;
  int HcountGen = 0, HcountInd, Hfather1, Hfather2;
  double x;

  HinitializeIndividuals();
  for(i = 0; i < HNINDIVIDUALS; i++)
  {
    for(j = 0; j < NEXPERIMENTS; j++)
    {
      AGinterno(i, j);
    }
    qsort(Hindividuals[i].Hfitness, NEXPERIMENTS, sizeof(double), HsortFitness);
  }

  while(HcountGen < HNUMGENERATION)
  {
    HcountInd = HNINDIVIDUALS;
    while(HcountInd < (2*HNINDIVIDUALS))
    {
      Htournament(&Hfather1, &Hfather2);
      //Probabilidade de realizar o crossover
      x = (double)((double)rand() / (double)RAND_MAX);
      if(x < PCROSSOVER)
        Hcrossover(HcountInd, Hfather1, Hfather2);
      else{
        Hindividuals[HcountInd] = Hindividuals[Hfather1];
        Hindividuals[HcountInd + 1] = Hindividuals[Hfather2];
      }
      //Probabilidade de realizar a mutação
      x = (double)((double)rand() / (double)RAND_MAX);
      if(x < PMUTATION)
        Hmutation(HcountInd);
      x = (double)((double)rand() / (double)RAND_MAX);
      if(x < PMUTATION)
        Hmutation(HcountInd + 1);

      { //Atualiza os valores de probCrossover e probMutation
        for(i = 0; i < 9; i++)
          b[i] = Hindividuals[HcountInd].HgProbCrossover[i];
        gray = 0;
        for(i = 0; i < 9; i++)
          gray += b[i]*pow(10, 8 - i);
        Hindividuals[HcountInd].probCrossover =
                                            (50.0 + grayToInt(gray)/10.0)/100.0;

        for(i = 0; i < 9; i++)
          b[i] = Hindividuals[HcountInd + 1].HgProbCrossover[i];
        gray = 0;
        for(i = 0; i < 9; i++)
        {
          gray += b[i]*pow(10, 8 - i);
        }
        Hindividuals[HcountInd + 1].probCrossover =
                                            (50.0 + grayToInt(gray)/10.0)/100.0;

        for(i = 0; i < 9; i++)
          b[i] = Hindividuals[HcountInd].HgProbMutation[i];
        gray = 0;
        for(i = 0; i < 9; i++)
        {
          gray += b[i]*pow(10, 8 - i);
        }
        Hindividuals[HcountInd].probMutation =
                                            (50.0 + grayToInt(gray)/10.0)/100.0;

        for(i = 0; i < 9; i++)
          b[i] = Hindividuals[HcountInd + 1].HgProbMutation[i];
        gray = 0;
        for(i = 0; i < 9; i++)
        {
          gray += b[i]*pow(10, 8 - i);
        }
        Hindividuals[HcountInd + 1].probMutation =
                                            (50.0 + grayToInt(gray)/10.0)/100.0;
      }//Fim da atualização dos valores de probCrossover e probMutation

      HcountInd = HcountInd + 2;
    }

    for(i = HNINDIVIDUALS; i < (2*HNINDIVIDUALS); i++)
    {
      for(j = 0; j < NEXPERIMENTS; j++)
      {
        AGinterno(i, j);
      }
      qsort(Hindividuals[i].Hfitness, NEXPERIMENTS, sizeof(double),
                                                                  HsortFitness);
    }
    Hselection();
    HcountGen++;
  }
} //hyperheuristic

void AGinterno(int g, int h)
{
  int i, qFront[2*NINDIVIDUALS] = {0}, interval[2] = {0}, countGen = 0,
                                            father1 = 0, father2 = 0, countInd;
  double x; //Número para o sorteio

  initializeIndividuals();

  // Laço que calcula o fitness do pai para as duas funções objetivo
  for(i = 0; i < NINDIVIDUALS; i++)
  {
    createGantt(i);
    individuals[i].fitMakespan = fitnessMakespan(Gantt, i);
    individuals[i].fitTardiness = fitnessTardiness();
  }
  //Reiniciar o iDistance e iRank previamente
  for(i = 0; i < (2*NINDIVIDUALS); i++)
  {
    individuals[i].iRank = 0;
    individuals[i].iDistance = 0;
  }
  ranking(NINDIVIDUALS);
  for(i = 0; i < NINDIVIDUALS; i++)
    qFront[individuals[i].iRank - 1] ++;
    interval[0] = 0;
    interval[1] = 0;

  qsort(individuals, NINDIVIDUALS, sizeof(ind), sortFitMakespan);
  for(i = 0; i < 2*NINDIVIDUALS; i++){
    interval[0] += qFront[i];
    if(qFront[i] != 0){
      crowdingDistanceMakespan(interval[0], interval[1]);
    }
    else
      break;
    interval[1] += qFront[i];
  }

  interval[0] = 0;
  interval[1] = 0;
  qsort(individuals, NINDIVIDUALS, sizeof(ind), sortFitTardiness);
  for(i = 0; i < 2*NINDIVIDUALS; i++){
    interval[0] += qFront[i];
    if(qFront[i] != 0){
      crowdingDistanceTardiness(interval[0], interval[1]);
    }
    else
      break;
    interval[1] += qFront[i];
  }

  //Laço principal do AG
  while(countGen < NUMGENERATION)
  {
    initializeGeneration();
    countInd = NINDIVIDUALS;
    while(countInd < 2*NINDIVIDUALS)
    {
      // Seleção dos pais
      tournament(&father1, &father2);

      // Cálculo da chance de realizar crossover
      x = (double)((double)rand() / (double)RAND_MAX);
      if(x < Hindividuals[g].probCrossover)
      {
        if(Hindividuals[g].HgCrossover[0] == 1)
          opCrossover(countInd, father1, father2);
        if(Hindividuals[g].HgCrossover[1] == 1)
          LOXcrossover(countInd, father1, father2);
        if(Hindividuals[g].HgCrossover[2] == 1)
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
      if(x < Hindividuals[g].probMutation)
      {
        if(Hindividuals[g].HgMutation[0] == 1)
          shiftMutation(countInd);
        if(Hindividuals[g].HgMutation[1] == 1)
          interchangeMutation(countInd);
      }

      // Chance de realizar mutação
      x = (double)((double)rand() / (double)RAND_MAX);
      if(x < Hindividuals[g].probMutation)
      {
        if(Hindividuals[g].HgMutation[0] == 1)
          shiftMutation(countInd + 1);
        if(Hindividuals[g].HgMutation[1] == 1)
          interchangeMutation(countInd + 1);
      }

      countInd = countInd + 2;
    }

    for(i = NINDIVIDUALS; i < (2*NINDIVIDUALS); i++)
    {
      createGantt(i);
      individuals[i].fitMakespan = fitnessMakespan(Gantt, i);
      individuals[i].fitTardiness = fitnessTardiness();
    }

    //Reiniciar o iDistance e iRank previamente e as variáveis auxiliares
    for(i = 0; i < (2*NINDIVIDUALS); i++)
    {
      individuals[i].iRank = 0;
      individuals[i].iDistance = 0;
      qFront[i] = 0;
    }
    ranking(2*NINDIVIDUALS);
    for(i = 0; i < 2*NINDIVIDUALS; i++)
      qFront[individuals[i].iRank - 1] ++;

    interval[0] = 0;
    interval[1] = 0;
    qsort(individuals, 2*NINDIVIDUALS, sizeof(ind), sortFitMakespan);
    for(i = 0; i < 2*NINDIVIDUALS; i++){
      interval[0] += qFront[i];
      if(qFront[i] != 0){
        crowdingDistanceMakespan(interval[0], interval[1]);
      }
      else
        break;
      interval[1] += qFront[i];
    }

    interval[0] = 0;
    interval[1] = 0;
    qsort(individuals, 2*NINDIVIDUALS, sizeof(ind), sortFitTardiness);
    for(i = 0; i < 2*NINDIVIDUALS; i++){
      interval[0] += qFront[i];
      if(qFront[i] != 0){
        crowdingDistanceTardiness(interval[0], interval[1]);
      }
      else
        break;
      interval[1] += qFront[i];
    }

    qsort(individuals, (2*NINDIVIDUALS), sizeof(ind), NSGAIIselection);
    countGen++;
  }
  Hindividuals[g].Hfitness[h] = hypervolume();
} // AGinterno

// Funções da hyperheurística
void HinitializeIndividuals()
{
  int i, j, x;
  // int y;
  for(i = 0; i < HNINDIVIDUALS; i++)
  {
    x = rand() % HnumCrossover;
    for(j = 0; j < HnumCrossover; j++)
    {
      if(j == x)
        Hindividuals[i].HgCrossover[j] = 1;
      else
        Hindividuals[i].HgCrossover[j] = 0;
    }

    do
    {
      dec = (rand() % 511);
    } while(dec > 500);
    intToGray(dec);
    for(j = 0; j < 9; j++)
      Hindividuals[i].HgProbCrossover[j] = b[j];
    Hindividuals[i].probCrossover = ((dec / 10.0) + 50.0)/100.0;

    x = rand() % HnumMutation;
    for(j = 0; j < HnumMutation; j++)
    {
      if(j == x)
        Hindividuals[i].HgMutation[j] = 1;
      else
        Hindividuals[i].HgMutation[j] = 0;
    }

    do
    {
      dec = (rand() % 511);
    } while(dec > 500);
    intToGray(dec);
    for(j = 0; j < 9; j++)
      Hindividuals[i].HgProbMutation[j] = b[j];
    Hindividuals[i].probMutation = ((dec / 10.0) + 50.0)/100.0;
  }
} //HinitializeIndividuals

//Função para organizar os fitness dos individuos
int HsortFitness(const void *a, const void *b)
{
  double a2 = *(double *)a;
  double b2 = *(double *)b;
  if(a2 > b2)
    return 1;
  else
    return -1;
} //HsortFitness

void Htournament(int *Hfather1, int *Hfather2)
{
  int i, nCandidate = 4, candidateFather[nCandidate];

  //Seleciona o primeiro pai
  for(i = 0; i < nCandidate; i++)
  {
    candidateFather[i] = rand() % HNINDIVIDUALS;
  }

  (*Hfather1) = candidateFather[0];
  for(i = 1; i < nCandidate; i++)
  {
    if(Hindividuals[(*Hfather1)].Hfitness[2] < Hindividuals[(i)].Hfitness[2])
      (*Hfather1) = candidateFather[i];
  }

  //Seleciona o segundo pai
  for(i = 0; i < nCandidate; i++)
  {
    candidateFather[i] = rand() % HNINDIVIDUALS;
  }
  (*Hfather2) = candidateFather[0];
  for(i = 1; i < nCandidate; i++)
  {
    if(Hindividuals[(*Hfather2)].Hfitness[2] < Hindividuals[(i)].Hfitness[2])
      (*Hfather2) = candidateFather[i];
  }
} //Htournament

void Hcrossover(int HcountInd, int Hfather1, int Hfather2)
{
  int i, x;
  x = rand() % 2;
  //Copia a técnica de crossover que será utilizada do pai1 ou pai2
  if(x == 0)
  {
    for(i = 0; i < HnumCrossover; i++)
    {
      Hindividuals[HcountInd].HgCrossover[i] =
                                          Hindividuals[Hfather1].HgCrossover[i];
      Hindividuals[HcountInd + 1].HgCrossover[i] =
                                          Hindividuals[Hfather2].HgCrossover[i];
    }
  }
  else
  {
    for(i = 0; i < HnumCrossover; i++)
    {
      Hindividuals[HcountInd].HgCrossover[i] =
                                          Hindividuals[Hfather2].HgCrossover[i];
      Hindividuals[HcountInd + 1].HgCrossover[i] =
                                          Hindividuals[Hfather1].HgCrossover[i];
    }
  }

  HonePointRecomb(HcountInd, Hfather1, Hfather2);

  x = rand() % 2;
  //Copia a técnica de mutação que será utilizada do pai1 ou pai2
  if(x == 0)
  {
    for(i = 0; i < HnumCrossover; i++)
    {
      Hindividuals[HcountInd].HgMutation[i] =
                                          Hindividuals[Hfather1].HgMutation[i];
      Hindividuals[HcountInd + 1].HgMutation[i] =
                                          Hindividuals[Hfather2].HgMutation[i];
    }
  }
  else
  {
    for(i = 0; i < HnumCrossover; i++)
    {
      Hindividuals[HcountInd].HgMutation[i] =
                                          Hindividuals[Hfather2].HgMutation[i];
      Hindividuals[HcountInd + 1].HgMutation[i] =
                                          Hindividuals[Hfather1].HgMutation[i];
    }
  }
} //Hcrossover

void HonePointRecomb(int HcountInd, int Hfather1, int Hfather2)
{
  int i, k;
  k = rand() % 9;
  for(i = 0; i < k; i++)
  {
    Hindividuals[HcountInd].HgProbCrossover[i] =
                                      Hindividuals[Hfather1].HgProbCrossover[i];
    Hindividuals[HcountInd + 1].HgProbCrossover[i] =
                                      Hindividuals[Hfather2].HgProbCrossover[i];
  }
  for (i = k; i < 9; i++) {
    Hindividuals[HcountInd].HgProbCrossover[i] =
                                      Hindividuals[Hfather2].HgProbCrossover[i];
    Hindividuals[HcountInd + 1].HgProbCrossover[i] =
                                      Hindividuals[Hfather1].HgProbCrossover[i];
  }

  k = rand() % 9;
  for(i = 0; i < k; i++)
  {
    Hindividuals[HcountInd].HgProbMutation[i] =
                                      Hindividuals[Hfather1].HgProbMutation[i];
    Hindividuals[HcountInd + 1].HgProbMutation[i] =
                                      Hindividuals[Hfather2].HgProbMutation[i];
  }
  for (i = k; i < 9; i++)
  {
    Hindividuals[HcountInd].HgProbMutation[i] =
                                      Hindividuals[Hfather2].HgProbMutation[i];
    Hindividuals[HcountInd + 1].HgProbMutation[i] =
                                      Hindividuals[Hfather1].HgProbMutation[i];
  }
} //HonePointRecomb

void Hmutation(int i)
{
  int gene, place, temp;
  //Mutação nas técnicas de crossover
  gene = rand() % HnumCrossover;
  place = rand() % HnumCrossover;
  temp = Hindividuals[i].HgCrossover[gene];
  Hindividuals[i].HgCrossover[gene] = Hindividuals[i].HgCrossover[place];
  Hindividuals[i].HgCrossover[place] = temp;

  //Mutação na probabilidade de crossover do ag interno
  gene = rand() % 9;
  if(Hindividuals[i].HgProbCrossover[gene] == 1)
    Hindividuals[i].HgProbCrossover[gene] = 0;
  else
    Hindividuals[i].HgProbCrossover[gene] = 1;

  //Mutação na probabilidade de mutação do ag interno
  gene = rand() % 9;
  if(Hindividuals[i].HgProbMutation[gene] == 1)
    Hindividuals[i].HgProbMutation[gene] = 0;
  else
    Hindividuals[i].HgProbMutation[gene] = 1;


  //Mutação nas técnicas de mutação
  gene = rand() % HnumMutation;
  place = rand() % HnumMutation;
  temp = Hindividuals[i].HgMutation[gene];
  Hindividuals[i].HgMutation[gene] = Hindividuals[i].HgMutation[place];
  Hindividuals[i].HgMutation[place] = temp;
} //Hmutation

void Hselection()
{
  int i, j;
  double x, proportion[2*HNINDIVIDUALS], s = 1.5, sumProportion;
  Hind tempInd[HNINDIVIDUALS];

  qsort(Hindividuals, (2*HNINDIVIDUALS), sizeof(Hind), HsortHyperheuristicAscending);
  for(i = 0; i < (2*HNINDIVIDUALS); i++)
    proportion[i] = (double)((2.0 - (double)s)/((double)(2*HNINDIVIDUALS)) +
      (2.0 * ((double)i) * ((double)s - 1.0))/
                ((double)(2*HNINDIVIDUALS)*((double)(2*HNINDIVIDUALS) - 1.0)));

  for(i = 0; i < HNINDIVIDUALS; i++)
  {
    sumProportion = 0.0;
    x = (double)((double)rand() / (double)RAND_MAX);
    for(j = 0; j < (2*HNINDIVIDUALS); j++)
    {
      sumProportion += proportion[j];
      if(x < sumProportion)
      {
        tempInd[i] = Hindividuals[j];
        break;
      }
    }
  }

  for(i = 0; i < HNINDIVIDUALS; i++)
    Hindividuals[i] = tempInd[i];
} //Hselection

//Função utilizada para o ordenamento na etapa de seleção
int HsortHyperheuristicAscending(const void *a, const void *b)
{
  Hind *a2 = (Hind *)a;
  Hind *b2 = (Hind *)b;
  return (a2->Hfitness[2] - b2->Hfitness[2]);
} //HsortHyperheuristicAscending

//Função utilizada para o ordenamento na etapa de seleção
int HsortHyperheuristicDescending(const void *a, const void *b)
{
  Hind *a2 = (Hind *)a;
  Hind *b2 = (Hind *)b;
  return (b2->Hfitness[2] - a2->Hfitness[2]);
} //HsortHyperheuristicDescending

double grayToInt(int n)
{
  int a[9], i = 0, c = 0, bin = 0, rest;
  dec = 0;

  //Converte em binario
  while(n != 0)
  {
    a[i] = (int)n % 10;
    n = n / 10;
    i++;
    c++;
  }
  for(i = c - 1; i >= 0; i--)
  {
    if(a[i] == 1)
    {
      if(a[i - 1] == 1)
        a[i - 1] = 0;
      else
        a[i - 1] = 1;
    }
  }

  //Converte em decimal
  for(i = c; i < 9; i++)
  {
    a[i] = 0;
  }
  for(i = 0; i < 9; i++)
  {
    bin += a[i]*pow(10, i);
  }
  // printf("bin == %d", bin);
  i = 0;
  while (bin != 0)
  {
    rest = bin % 10;
    bin = bin / 10;
    dec += rest * pow(2,i);
    ++i;
  }

  return dec;
} //grayToInt

void intToGray(int n1)
{
  int i = 0, k = 1;
  /* converting number to its binary equivalent */
  while(n1 != 0)
  {
    a[i] = n1 % 2;
    n1 = n1 / 2;
    i++;
  }

  /* gray code conversion */
  b[0] = a[8];
  for(i = 8; i >= 0; i--)
  {
    if(a[i] == 0 && a[i - 1] == 0)
      b[k] = 0;
    if(a[i] == 1 && a[i - 1] == 1)
      b[k] = 0;
    if(a[i] == 0 && a[i - 1] == 1)
      b[k] = 1;
    if(a[i] == 1 && a[i - 1] == 0)
      b[k] = 1;
    k++;
  }

}//intToGray

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

  for(k = 0; k < NINDIVIDUALS; k++)
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

void printDataFile(char *fileName, char *nRepeat)
 {
  int i;
  char *outputName =
        (char*)malloc((strlen(fileName) + strlen(nRepeat) + 13)*sizeof(char));
  strcpy(outputName, fileName);
  strcat(outputName, "_");
  strcat(outputName, nRepeat);
  strcat(outputName, "_output.txt");
  FILE *output;
  output = fopen(outputName , "a");
  if (output == NULL)
  {
    printf("Arquivo não encontrado!\n");
    exit(1);
  }
  AGinterno(0, 1);

  for(i = 0; i < NINDIVIDUALS; i++){
    if(individuals[i].iRank == 1){
      fprintf(output, "%d \t\t %d \t\t %3.lf \t\t %d \t\t %d\n", i,
              individuals[i].iRank, individuals[i].iDistance,
                      individuals[i].fitMakespan, individuals[i].fitTardiness);
    }
  }

  fclose(output);
} //printDataFile

void printDataFile2(char *fileName, char *nRepeat)
{
  int i;
  char *outputName =
        (char*)malloc((strlen(fileName) + strlen(nRepeat) + 12)*sizeof(char));
  strcpy(outputName, fileName);
  strcat(outputName, "_");
  strcat(outputName, nRepeat);
  strcat(outputName, "output.txt");
  FILE *output;
  output = fopen(outputName , "a");
  if (output == NULL)
  {
    printf("Arquivo não encontrado!\n");
    exit(1);
  }

  fprintf(output, "\n\n\n");
  for(i = 0; i < HnumCrossover; i++)
     fprintf(output, "%d ", Hindividuals[0].HgCrossover[i]);
  fprintf(output, " ");
  fprintf(output, "%.2lf ", Hindividuals[0].probCrossover);
  fprintf(output, " ");
  for(i = 0; i < HnumMutation; i++)
    fprintf(output, "%d ", Hindividuals[0].HgMutation[i]);
  fprintf(output, " ");
  fprintf(output, "%.2lf ", Hindividuals[0].probMutation);
  fprintf(output, " %.0lf\n", Hindividuals[0].Hfitness[2]);

  fclose(output);
} //printDataFile2
