#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define HNINDIVIDUALS 10  // Número de indivíduos da hiperheurística
#define NINDIVIDUALS 20  // Número de indivíduos do AG interno
#define NEXPERIMENTS 5  //Número de execuções do AG interno para cada AG externo

#define PCROSSOVER 0.8    // Probabilidade de realizar Crossover
#define PMUTATION  0.15    // Probabilidade de realizar Mutação

//DECLARAÇÕES HIPERHEURISTICA

//Se mais técnicas forem implantadas, deve-se alterar o valor também na struct
int HnumCrossover = 3;  //Quantidade de técnicas de crossover implantadas
int HnumMutation = 2;   // Quantidade de técnias de mutação implantadas

//Struct Hiperheuristica
typedef struct Hind{
  int HgCrossover[3]; // Sequência de gene para o crossover
  int HgProbCrossover[9];
  double probCrossover;
  int HgMutation[2];   // Sequência de gene para a mutação
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
void Hcrossover(int HnumGeneration, int Hfather1, int Hfather2);
void HonePointRecomb(int HnumGeneration, int Hfather1, int Hfather2);
void Htournament(int *Hfather1, int *Hfather2);
void Hmutation(int i);
void Hselection();
int Hcompare(const void *a, const void *b);
int HcompFitness(const void *a, const void *b);
void intToGray(int n1);
double grayToInt(int n);

// DECLARAÇÕES AG INTERNO  0 1 0  0.96  0 1  0.29  25244594

// struct AG interno
typedef struct newGeneration{
  int *newIndividuals;
  int fitM;
  int fitT;
  int fitRank;
  double fitDistance;
}nG;

//Variáveis do AG interno
int **individuals;        // matriz indivíduo para os pais
int **generation;         // matriz de indivíduos para os filhos
int **fitnessIndividuals; // Matriz que atribui os fitness dos indivíduos
int ***jobMachine;        // Matriz que relaciona o tempo do Job em cada Máquina
int nJobs = 0;            // Número de Jobs
int nMachines = 0;        // Número de Máquinas
int **Gantt;              // Matriz que alocará informações
int *dueDate;             // Vetor que gauarda as informações da data de termino dos Jobs
int flowMakespan = 0;     // Realiza um cálculo prévio para avaliar o máximo de Makespan
int *iRank;               // Vetor que armazena o ranking dos indivíduos
double *iDistance;        // Vetor que armazena a crowding distance das soluções
int **front;              // Array auxiliar para o cálculo da crowding distance
double z[2];              //Valores utópicos das contas


//funções AG interno
void AGinterno(int g, int h); //Main loop
void readDataFile(char fileName[]);
void jobDueDate();
void printJobMachine();
void initializeIndividuals();
void initializeGeneration();
void maxMakespan();
void createGantt(int **population, int w);
void printGantt();
int fitnessMakespan(int **graph, int w);
int fitnessTardiness(int **graph, int w);
void opCrossover(int nGeneration, int *geneFather1, int *geneFather2);
void LOXcrossover(int nGeneration, int *geneFather1, int *geneFather2);
void PMXcrossover(int nGeneneration, int *geneFather1, int *geneFather2);
void permutation(int firstCromossome, int i, int nGeneration, int *geneFather1, int *geneFather2, int j);
void shiftMutation(int nGeneration);
void interchangeMutation(int nGeneration);
void NSGAIIselection();
void tournament(int *father1, int *father2);
void ranking(int w);
int selectiRank(int w, int x);
void crowdingDistance(int w);
void quickSort(int w, int x, int low, int high);
int compare(const void *a, const void *b);
int compare2(const void *a, const void *b);
double hypervolume();
void printDataFile();

int main()
{
  int i, j;

  char fileName[] = "input.txt";

  srand((unsigned)time(NULL));

  readDataFile(fileName);

  jobDueDate();

  // Aloca a memória
  individuals = (int**)malloc(NINDIVIDUALS* sizeof(int*));
  generation = (int**)malloc(NINDIVIDUALS*sizeof(int*));
  for(i = 0; i < NINDIVIDUALS; i++)
  {
    individuals[i] = (int*)malloc(nJobs* sizeof(int));
    generation[i] = (int*)malloc(nJobs*sizeof(int));
  }
  fitnessIndividuals = (int**)malloc(2 * NINDIVIDUALS*sizeof(int*));
  for(i = 0; i < (2 * NINDIVIDUALS); i++)
    fitnessIndividuals[i] = (int*)malloc(2*sizeof(int));
  iRank = (int*)malloc((NINDIVIDUALS)*(2*sizeof(int)));
  iDistance = (double*)malloc((NINDIVIDUALS)*(2*sizeof(double)));

  // 0 = fitnessIndividuals; 1 = fitnessTardiness; 2 = índice;
  front = (int**)malloc(2*NINDIVIDUALS*sizeof(int*));
  for(i = 0; i < (2*NINDIVIDUALS); i++)
    front[i] = (int*)malloc(3*sizeof(int));

  // Essa inicialização é necessária para calcular o máximo de Makespan
  initializeIndividuals();

  // Calcula o tamanho máximo dos indivíduos para o Makespan
  maxMakespan();

  //Aloca a memória do Gantt
  Gantt = (int**)malloc(nMachines*sizeof(int*));
  for(i = 0; i < nMachines; i++)
    Gantt[i] = (int*)malloc(flowMakespan*sizeof(int));


  { //Cálculo do Z utópico
    z[0] = (double)flowMakespan * 2.0 / 1.5;
    z[1] = 0;
	for(i = 0; i < NINDIVIDUALS; i++)
	{
		createGantt(individuals, i);
		fitnessIndividuals[i][1] = fitnessTardiness(Gantt, i);
		if(fitnessIndividuals[i][1] > z[1])
			z[1] = fitnessIndividuals[i][1];
	}
    z[1] = z[1] * 2.0;
  }
  
  //Inicializa a hiperheuristica
  hyperheuristic();

  qsort(Hindividuals, (HNINDIVIDUALS), sizeof(Hind), (Hcompare));
  
  
  printDataFile();
  
  // Libera a memória
  for(i = 0; i < nJobs; i++)
  {
    for(j = 0; j < nMachines; j++)
      free(jobMachine[i][j]);
    free(jobMachine[i]);
  }
  free(jobMachine);
  for(i = 0; i < NINDIVIDUALS; i++)
  {
    free(individuals[i]);
    free(generation[i]);
  }
  free(individuals);
  free(generation);
  for(i = 0; i < 2* NINDIVIDUALS; i++)
  {
    free(fitnessIndividuals[i]);
    free(front[i]);
  }
  free(front);
  free(fitnessIndividuals);
  for(i = 0; i < nMachines; i++)
      free(Gantt[i]);
  free(Gantt);
  free(dueDate);
  free(iRank);
  free(iDistance);

  return 0;
} //main

// /* Loops principais*/
void AGinterno(int g, int h)
{
  int i, qFront, count = 0, father1 = 0, father2 = 0, nGeneration;
  double x; //Número para o sorteio
  int *geneFather1 = (int*)malloc(nJobs*sizeof(int));
  int *geneFather2 = (int*)malloc(nJobs*sizeof(int));
  
  initializeIndividuals();

  // Laço que calcula o fitness do pai para as duas funções objetivo
  for(i = 0; i < NINDIVIDUALS; i++)
  {
    createGantt(individuals, i);
    fitnessIndividuals[i][0] = fitnessMakespan(Gantt, i);
    fitnessIndividuals[i][1] = fitnessTardiness(Gantt, i);
  }

  //Reiniciar o iDistance e iRank previamente
  for(i = 0; i < (2*NINDIVIDUALS); i++)
  {
    iRank[i] = 0;
    iDistance[i] = 0;
  }
  ranking(NINDIVIDUALS);
  for(i = 0; i < NINDIVIDUALS; i++)
  {
    qFront = selectiRank(NINDIVIDUALS, i+1);
    if(qFront != 0)
      crowdingDistance(qFront);
  }

  //Laço principal do AG
  while(count < 10)
  {
    //~ initializeGeneration();
    nGeneration = 0;
    while(nGeneration < NINDIVIDUALS)
    {
      // Seleção dos pais
      tournament(&father1, &father2);

      // Cálculo da chance de realizar crossover
      x = (double)((double)rand() / (double)RAND_MAX);
      if(x < Hindividuals[g].probCrossover)
      {
	    for(i = 0; i < nJobs; i++)
		{
		  geneFather1[i] = individuals[father1][i];
		  geneFather2[i] = individuals[father2][i];
		}
        if(Hindividuals[g].HgCrossover[0] == 1)
        {
		  for( i = 0; i < nJobs; i++)
		  {
			 generation[nGeneration][i] = -1;
			 generation[nGeneration + 1][i] = -1;
		  }
          opCrossover(nGeneration, geneFather1, geneFather2);
		  for(i = 0; i < nJobs; i++)
		  {
			 geneFather1[i] = generation[nGeneration][i];
			 geneFather2[i] = generation[nGeneration + 1][i];
		  }    
	    }
        if(Hindividuals[g].HgCrossover[1] == 1)
        {
		  for( i = 0; i < nJobs; i++)
		  {
			 generation[nGeneration][i] = -1;
			 generation[nGeneration + 1][i] = -1;
		  }
          LOXcrossover(nGeneration, geneFather1, geneFather2);
          for(i = 0; i < nJobs; i++)
		  {
			 geneFather1[i] = generation[nGeneration][i];
			 geneFather2[i] = generation[nGeneration + 1][i];
		  }  
	    }
        if(Hindividuals[g].HgCrossover[2] == 1)
        {
		  for( i = 0; i < nJobs; i++)
		  {
			 generation[nGeneration][i] = -1;
			 generation[nGeneration + 1][i] = -1;
		  }
          PMXcrossover(nGeneration, geneFather1, geneFather2);
		}  
      }
      else
      {
        for(i = 0; i < nJobs; i++)
        {
          generation[nGeneration][i] = individuals[father1][i];
          generation[nGeneration + 1][i] = individuals[father2][i];
		}
      }
      // Chance de realizar mutação
      x = (double)((double)rand() / (double)RAND_MAX);
      if(x < Hindividuals[g].probMutation)
      {
        if(Hindividuals[g].HgMutation[0] == 1)
          shiftMutation(nGeneration);
        if(Hindividuals[g].HgMutation[1] == 1)
          interchangeMutation(nGeneration);
      }
      
      // Chance de realizar mutação
      x = (double)((double)rand() / (double)RAND_MAX);
      if(x < Hindividuals[g].probMutation)
      {
        if(Hindividuals[g].HgMutation[0] == 1)
          shiftMutation(nGeneration + 1);
        if(Hindividuals[g].HgMutation[1] == 1)
          interchangeMutation(nGeneration + 1);
      }
      
      nGeneration = nGeneration + 2;
    }

    for(i = NINDIVIDUALS; i < (2*NINDIVIDUALS); i++)
    {
      createGantt(generation, (i - NINDIVIDUALS));
      fitnessIndividuals[i][0] = fitnessMakespan(Gantt, i);
      fitnessIndividuals[i][1] = fitnessTardiness(Gantt, i);
    }

    //Reiniciar o iDistance e iRank previamente
    for(i = 0; i < (2*NINDIVIDUALS); i++)
    {
      iRank[i] = 0;
      iDistance[i] = 0;
    }
    ranking(2*NINDIVIDUALS);
    for(i = 0; i < (2*NINDIVIDUALS); i++)
    {
      qFront = selectiRank(2*NINDIVIDUALS, i + 1);
      if(qFront != 0)
        crowdingDistance(qFront);
    }

    NSGAIIselection();

    count++;
  }

  free(geneFather1);
  free(geneFather2);

  Hindividuals[g].Hfitness[h] = hypervolume();
  
} //AGinterno

void hyperheuristic()
{
  int i, j;
  int count = 0, HnumGeneration, Hfather1, Hfather2;
  double x;


  HinitializeIndividuals();
  for(i = 0; i < HNINDIVIDUALS; i++)
  {
    for(j = 0; j < NEXPERIMENTS; j++)
    {
      AGinterno(i, j);    
    }
    qsort(Hindividuals[i].Hfitness, NEXPERIMENTS, sizeof(double), HcompFitness);
  }
 
  while(count < 5)
  {
    HnumGeneration = HNINDIVIDUALS;
    while(HnumGeneration < (2*HNINDIVIDUALS))
    {
      Htournament(&Hfather1, &Hfather2);
      //Probabilidade de realizar o crossover
      x = (double)((double)rand() / (double)RAND_MAX);
      if(x < PCROSSOVER)
        Hcrossover(HnumGeneration, Hfather1, Hfather2);
      else{
        Hindividuals[HnumGeneration] = Hindividuals[Hfather1];
        Hindividuals[HnumGeneration + 1] = Hindividuals[Hfather2];
      }
      //Probabilidade de realizar a mutação
      x = (double)((double)rand() / (double)RAND_MAX);
      if(x < PMUTATION)
        Hmutation(HnumGeneration);
      x = (double)((double)rand() / (double)RAND_MAX);
      if(x < PMUTATION)
        Hmutation(HnumGeneration + 1);

      { //Atualiza os valores de probCrossover e probMutation
        for(i = 0; i < 9; i++)
          b[i] = Hindividuals[HnumGeneration].HgProbCrossover[i];
        gray = 0;
        for(i = 0; i < 9; i++)
          gray += b[i]*pow(10, 8 - i);
        Hindividuals[HnumGeneration].probCrossover = (50.0 + grayToInt(gray)/10.0)/100.0;

        for(i = 0; i < 9; i++)
          b[i] = Hindividuals[HnumGeneration + 1].HgProbCrossover[i];
        gray = 0;
        for(i = 0; i < 9; i++)
        {
          gray += b[i]*pow(10, 8 - i);
        }
        Hindividuals[HnumGeneration + 1].probCrossover = (50.0 + grayToInt(gray)/10.0)/100.0;

        for(i = 0; i < 9; i++)
          b[i] = Hindividuals[HnumGeneration].HgProbMutation[i];
        gray = 0;
        for(i = 0; i < 9; i++)
        {
          gray += b[i]*pow(10, 8 - i);
        }
        Hindividuals[HnumGeneration].probMutation = (grayToInt(gray)/10.0)/100.0;

        for(i = 0; i < 9; i++)
          b[i] = Hindividuals[HnumGeneration + 1].HgProbMutation[i];
        gray = 0;
        for(i = 0; i < 9; i++)
        {
          gray += b[i]*pow(10, 8 - i);
        }
        Hindividuals[HnumGeneration + 1].probMutation = (grayToInt(gray)/10.0)/100.0;
      }//Fim da atualização dos valores de probCrossover e probMutation
      
      HnumGeneration = HnumGeneration + 2;
    }

    for(i = HNINDIVIDUALS; i < (2*HNINDIVIDUALS); i++)
    {
      for(j = 0; j < NEXPERIMENTS; j++)
      {
        AGinterno(i, j);
      }
      qsort(Hindividuals[i].Hfitness, NEXPERIMENTS, sizeof(double), HcompFitness);
    }
    
    Hselection();
    count++;
  }
} //hyperheuristic


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
    Hindividuals[i].probMutation = (dec / 10)/100.0;

  }
} //HinitializeIndividuals

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
}

//Técnica de recombinação aplicada para a probabilidade de crossover e mutação 
void HonePointRecomb(int HnumGeneration, int Hfather1, int Hfather2)
{
  int i, k;
  k = rand() % 9;
  for(i = 0; i < k; i++)
  {
    Hindividuals[HnumGeneration].HgProbCrossover[i] = Hindividuals[Hfather1].HgProbCrossover[i];
    Hindividuals[HnumGeneration + 1].HgProbCrossover[i] = Hindividuals[Hfather2].HgProbCrossover[i];
  }
  for (i = k; i < 9; i++) {
    Hindividuals[HnumGeneration].HgProbCrossover[i] = Hindividuals[Hfather2].HgProbCrossover[i];
    Hindividuals[HnumGeneration + 1].HgProbCrossover[i] = Hindividuals[Hfather1].HgProbCrossover[i];
  }

  k = rand() % 9;
  for(i = 0; i < k; i++)
  {
    Hindividuals[HnumGeneration].HgProbMutation[i] = Hindividuals[Hfather1].HgProbMutation[i];
    Hindividuals[HnumGeneration + 1].HgProbMutation[i] = Hindividuals[Hfather2].HgProbMutation[i];
  }
  for (i = k; i < 9; i++)
  {
    Hindividuals[HnumGeneration].HgProbMutation[i] = Hindividuals[Hfather2].HgProbMutation[i];
    Hindividuals[HnumGeneration + 1].HgProbMutation[i] = Hindividuals[Hfather1].HgProbMutation[i];
  }
} //HonePointRecomb

// O crossover da hiperheuristica implantado, permite que somente um crossover do ag interno esteja ativo
 void Hcrossover(int HnumGeneration, int Hfather1, int Hfather2)
{
   int i, x;
   //Seleção da técnica de crossover
   //Seleciona o ponto que ocorrerá crossover
   x = rand() % HnumCrossover;

   //Primeiro filho
   for(i = 0; i < HnumCrossover; i++)
   {
     if(i != x)
       Hindividuals[HnumGeneration].HgCrossover[i] = Hindividuals[Hfather1].HgCrossover[i];
     else
      Hindividuals[HnumGeneration].HgCrossover[i] = Hindividuals[Hfather2].HgCrossover[i];
   }

   //Segundo filho
   for(i = 0; i < HnumCrossover; i++)
   {
     if(i != x)
       Hindividuals[HnumGeneration + 1].HgCrossover[i] = Hindividuals[Hfather2].HgCrossover[i];
     else
       Hindividuals[HnumGeneration + 1].HgCrossover[i] = Hindividuals[Hfather1].HgCrossover[i];
   }

   //Função que realiza a recombinação para as probabilidades
   HonePointRecomb(HnumGeneration, Hfather1, Hfather2);


   //Seleção das técnicas de mutação
   //Seleciona o ponto que ocorrerá crossover
   x = rand() % HnumMutation;

   //Primeiro filho
   for(i = 0; i < HnumMutation; i++)
   {
     if(i != x)
       Hindividuals[HnumGeneration].HgMutation[i] = Hindividuals[Hfather1].HgMutation[i];
     else
       Hindividuals[HnumGeneration].HgMutation[i] = Hindividuals[Hfather2].HgMutation[i];
   }

   //Segundo filho
   for(i = 0; i < HnumMutation; i++)
   {
     if(i != x)
       Hindividuals[HnumGeneration + 1].HgMutation[i] = Hindividuals[Hfather2].HgMutation[i];
     else
       Hindividuals[HnumGeneration + 1].HgMutation[i] = Hindividuals[Hfather1].HgMutation[i];
   }
 } //Hcrossover

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

  qsort(Hindividuals, (2*HNINDIVIDUALS), sizeof(Hind), Hcompare);
  for(i = 0; i < (2*HNINDIVIDUALS); i++)
    proportion[i] = (double)((2.0 - (double)s)/((double)(2*HNINDIVIDUALS)) + (2.0 * ((double)i) * ((double)s - 1.0))/((double)(2*HNINDIVIDUALS)*((double)(2*HNINDIVIDUALS) - 1.0)));
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

double hypervolume()
{
  int i, count = 0;
  double volume;
  for(i = 0; i < NINDIVIDUALS; i++)
    if(iRank[i] == 1)
      count++;
	if(((z[0] - fitnessIndividuals[0][0]) < 0) || ((z[1] - fitnessIndividuals[0][1]) < 0))
		printf("Ajustar o multiplicadores de Z[0] = %lf Z[1] = %lf", (z[0] - fitnessIndividuals[0][0]), (z[1] - fitnessIndividuals[0][1]));

  volume = ((z[0] - fitnessIndividuals[0][0]) * 
									(z[1] - fitnessIndividuals[0][1]));
  for(i = 1; i < count; i++)
  {
	if(((z[0] - fitnessIndividuals[i][0]) < 0) || ((z[1] - fitnessIndividuals[i][1]) < 0))
		printf("Ajustar o multiplicadores de Z[0] = %lf Z[1] = %lf", (z[0] - fitnessIndividuals[i][0]), (z[1] - fitnessIndividuals[i][1]));
    volume += ((fitnessIndividuals[i - 1][0] - fitnessIndividuals[i][0])
								* (z[1] - fitnessIndividuals[i][1]));
  }
  return volume;
} //hypervolume

//Função utilizada para o ordenamento na etapa de seleção
int Hcompare(const void *a, const void *b)
{
  Hind *a2 = (Hind *)a;
  Hind *b2 = (Hind *)b;
  return (a2->Hfitness[2] - b2->Hfitness[2]);
} //Hcompare

//Função para organizar os fitness dos individuos
int HcompFitness(const void *a, const void *b)
{
  double a2 = *(double *)a;
  double b2 = *(double *)b;
  if(a2 > b2)
    return 1;
  else
    return -1;
} //HcompFitness


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


/* FUNÇÕES DE INICIALIZAÇÃO E RELATIVA AO AG*/
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

void printDataFile()
 {
   int i;
   FILE *output;
   output = fopen("output.txt" , "a");
   if (output == NULL)
{
    printf("Arquivo não encontrado!\n");
    exit(1);
}
	fprintf(output, "\n");
    for(i = 0; i < HnumCrossover; i++)
      fprintf(output, "%d ", Hindividuals[HNINDIVIDUALS - 1].HgCrossover[i]);
	fprintf(output, " ");
    fprintf(output, "%.2lf ", Hindividuals[HNINDIVIDUALS - 1].probCrossover);
	fprintf(output, " ");
    for(i = 0; i < HnumMutation; i++)
      fprintf(output, "%d ", Hindividuals[HNINDIVIDUALS - 1].HgMutation[i]);
    fprintf(output, " ");
    fprintf(output, "%.2lf ", Hindividuals[HNINDIVIDUALS - 1].probMutation);
    fprintf(output, " %.0lf", Hindividuals[HNINDIVIDUALS - 1].Hfitness[2]);
   
   AGinterno(HNINDIVIDUALS - 1, 1);
    
} //printDataFile
    
    
void printJobMachine()
{
  int i, j, k;
  for(i = 0; i < nJobs; i++)
  {
    for(j = 0; j < nMachines; j++)
    {
      for(k = 0; k < 2; k++)
        printf("%d ", jobMachine[i][j][k]);
    }
    printf("\n");
  }
} // printJobMachine

// Função que gera os indivíduos iniciais através de embaralhamento
void initializeIndividuals()
{
  int i, j, r, temp;
  //Realiza o embaralhamento dos indivíduos
  for (i = 0; i < NINDIVIDUALS; i++)
  {
    // Gera o indivíduo
    for (j = 0; j < nJobs; j++)
      individuals[i][j] = j;
    // Realiza o embaralhamento
    for (j = 0; j < nJobs; j++)
    {
      r = rand() % nJobs;
      temp = individuals[i][j];
      individuals[i][j] = individuals[i][r];
      individuals[i][r] = temp;
    }
  }
} //Função initializeIndividuals

void initializeGeneration()
{
  int i, j;
  for(i = 0; i < NINDIVIDUALS; i++)
    for(j = 0; j < nJobs; j++)
      generation[i][j] = (-1);
} //initializeGeneration

// Crossover - Cruzamento Parcialmente Mapeado
void PMXcrossover(int nGeneration, int *geneFather1, int *geneFather2)
{
  int i, j, firstCromossome, interval, vaux1;

  //Seleciona o intervalo que ocorrerá crossover
  firstCromossome = rand() % nJobs; //Estabelece onde ocorrerá o crossover

   //Estabelece o intervalo de genes que realizarão crossover
  if(firstCromossome == (nJobs - 1))
    interval = 0;
  else
    interval = rand() % (nJobs - firstCromossome);

  // Copia os genes do intervalo do father1 para a filho
  for(i = 0; i <= interval; i++)
  {
    if((firstCromossome + i) < nJobs)
    {
      generation[nGeneration][firstCromossome + i] = geneFather1[firstCromossome + i];
      generation[nGeneration + 1][firstCromossome + i] = geneFather2[firstCromossome + i];
    }
  }
  
  //Finaliza primeiro filho
  for(i = 0; i <= interval; i++)
  {
    vaux1 = 0;
    //Verifica se o gene já foi copiado para o filho
    for(j = 0; j <=interval; j++)
    {
      if(geneFather2[firstCromossome + i] == geneFather1[firstCromossome + j])
      {
        vaux1 = 1;
        break;
      }
    }

    // Localiza uma posição para receber o gene diferente do father2
    if(vaux1 == 0)
    {
      for(j = 0; j < nJobs; j++)
      {
        if(geneFather1[firstCromossome + i] == geneFather2[j])
        {
          if(generation[nGeneration][j] == (-1))
          {
            generation[nGeneration][j] = geneFather2[firstCromossome + i];
          } else
          {
            permutation(firstCromossome, i, nGeneration, geneFather1, geneFather2, j);
            break;
          }
        }
      }
    }
  }
  // Copia o restante dos genes do father2
  for(i = 0; i < nJobs; i++)
  {
    if(generation[nGeneration][i] == (-1))
    {
      generation[nGeneration][i] = geneFather2[i];
    }
  }
  
  //Finaliza segundo filho
  for(i = 0; i <= interval; i++)
  {
    vaux1 = 0;
    //Verifica se o gene já foi copiado para o filho
    for(j = 0; j <= interval; j++)
    {
      if(geneFather1[firstCromossome + i] == geneFather2[firstCromossome + j])
      {
        vaux1 = 1;
        break;
      }
    }

    // Localiza uma posição para receber o gene diferente do father2
    if(vaux1 == 0)
    {
      for(j = 0; j < nJobs; j++)
      {
        if(geneFather2[firstCromossome + i] == geneFather1[j])
        {
          if(generation[nGeneration + 1][j] == (-1))
          {
            generation[nGeneration + 1][j] = geneFather1[firstCromossome + i];
          } else
          {
            permutation(firstCromossome, i, nGeneration + 1, geneFather2, geneFather1, j);
            break;
          }
        }
      }
    }
  }
  // Copia o restante dos genes do father2
  for(i = 0; i < nJobs; i++)
  {
    if(generation[nGeneration + 1][i] == (-1))
    {
      generation[nGeneration + 1][i] = geneFather1[i];
    }
  }
} // PMXcrossover

//Crossover que copia um pedaço do Pai1 e completa com os genes do Pai2 mantendo a ordem
void LOXcrossover(int nGeneration, int *geneFather1, int *geneFather2)
{
  int i, j, firstCromossome, interval, id = 0, gver;
  
  //Seleciona o intervalo que ocorrerá crossover
  firstCromossome = rand() % nJobs; //Estabelece onde ocorrerá o crossover

   //Estabelece o intervalo de genes que realizarão crossover
  if(firstCromossome == (nJobs - 1))
    interval = 0;
  else
    interval = rand() % (nJobs - firstCromossome);

  // Copia os genes do intervalo do primeiro pai para a filho
  for(i = 0; i <= interval; i++)
  {
    if((firstCromossome + i) < nJobs)
    {
      generation[nGeneration][firstCromossome + i] = geneFather1[firstCromossome + i];
      generation[nGeneration + 1][firstCromossome + i] = geneFather2[firstCromossome + i];
    }
  }
  
  //Finaliza o primeiro filho
  //Preenche com a segunda metade do pai
  for(i = 0; i < nJobs; i++)
  {
    //Verifica se o gene já existe
    gver = 0;
    for(j = 0; j < nJobs; j++)
    {
      if(geneFather2[i] == generation[nGeneration][j])
      {
        gver++;
      }
    }
    if(gver == 0)
    {
      while(generation[nGeneration][id] != -1)
      {
        id++;
      }
      generation[nGeneration][id] = geneFather2[i];
    }
  }
  
    //Finaliza o segundo filho
  //Preenche com a segunda metade do pai
  id = 0;
  for(i = 0; i < nJobs; i++)
  {
    //Verifica se o gene já existe
    gver = 0;
    for(j = 0; j < nJobs; j++)
    {
      if(geneFather1[i] == generation[nGeneration + 1][j])
      {
        gver++;
      }
    }
    if(gver == 0)
    {
      while(generation[nGeneration + 1][id] != -1)
      {
        id++;
      }
      generation[nGeneration + 1][id] = geneFather1[i];
    }
  }
} //LOXcrossover

// Order Preserving One-Point Crossover
void opCrossover(int nGeneration, int *geneFather1, int *geneFather2)
{
	int i, j, id, id2, gver;
    id = rand() % nJobs;
    id2 = id;
	//Copia metade dos genes do primeiro pai
	for(i = 0; i < id; i++)
	{
		generation[nGeneration][i] = geneFather1[i];
		generation[nGeneration + 1][i] = geneFather2[i];
	}
	
	//Primeiro filho
	//Copia metade dos genes do segundo pai (na ordem que se apresenta sem repetir os genes que o filho já tem)
	for(i = 0; i < nJobs; i++)
  {
		//Verifica se o gene já existe
		gver = 0;
		for (j = 0; j < id; j++)
    {
			if (geneFather2[i] == generation[nGeneration][j])
      {
        gver++;
      }
    }
		if (gver == 0)
    {
			generation[nGeneration][id] = geneFather2[i];
			id++;
		}
	}

//Segundo filho
	//Copia metade dos genes do segundo pai (na ordem que se apresenta sem repetir os genes que o filho já tem)
	for(i = 0; i < nJobs; i++)
  {
		//Verifica se o gene já existe
		gver = 0;
		for (j = 0; j < id2; j++)
    {
			if (geneFather1[i] == generation[nGeneration + 1][j])
      {
        gver++;
      }
    }
		if (gver == 0)
    {
			generation[nGeneration + 1][id2] = geneFather1[i];
			id2++;
		}
	}
	
} //opCrossover

void permutation(int firstCromossome, int i, int nGeneration, int *geneFather1, int *geneFather2, int j)
{
  int k;
  for(k = 0; k < nJobs; k++)
  {
    if(geneFather1[j] == geneFather2[k])
    {
      if(generation[nGeneration][k] == (-1))
      {
        generation[nGeneration][k] = geneFather2[firstCromossome + i];
      }
      else
      {
        permutation(firstCromossome, i, nGeneration, geneFather1, geneFather2, k);
        break;
      }
    }
  }
} //permutation

// Mutação por inserção
void shiftMutation(int nGeneration)
{
  int gene, place, vaux1, i;
  gene = rand() % nJobs;
  place = rand() % nJobs;
  while(gene == place)
    place = rand() % nJobs;
  if(gene < place)
  {
    vaux1 = generation[nGeneration][gene];
    for (i = gene; i < place; i++)
    {
      generation[nGeneration][i] = generation[nGeneration][i+1];
    }
    generation[nGeneration][place] = vaux1;
  }
  else
  {
    vaux1 = generation[nGeneration][gene];
    for(i = gene ; i > place; i--)
    {
      generation[nGeneration][i] = generation[nGeneration][i-1];
    }
    generation[nGeneration][place] = vaux1;
  }
} // shiftMutation

//mutação por troca
void interchangeMutation(int nGeneration)
{
	int gene, place, temp;
    gene = rand() % nJobs;
    place = rand() % nJobs;
	temp = generation[nGeneration][gene];
	generation[nGeneration][gene] = generation[nGeneration][place];
	generation[nGeneration][place] = temp;
} // interchangeMutation

//Seleçao por torneio (4 individuos)
void tournament(int *father1, int *father2)
{
  int i, nCandidate = 4, candidateFather[nCandidate];

  //Seleciona o primeiro pai
  for(i = 0; i < nCandidate; i++)
  {
    candidateFather[i] = rand() % NINDIVIDUALS;
  }

  (*father1) = candidateFather[0];
  for(i = 1; i < nCandidate; i++)
  {
    if(iRank[candidateFather[i]] < iRank[(*father1)])
      (*father1) = candidateFather[i];
    else if((iRank[candidateFather[i]] == iRank[(*father1)]) && (iDistance[candidateFather[i]] > iDistance[(*father1)]))
      (*father1) = candidateFather[i];
  }

  //Seleciona o segundo pai
  for(i = 0; i < nCandidate; i++)
  {
    candidateFather[i] = rand()%NINDIVIDUALS;
  }
  (*father2) = candidateFather[0];
  for(i = 1; i < nCandidate; i++)
  {
    if(iRank[candidateFather[i]] < iRank[(*father1)])
      (*father2) = candidateFather[i];
    else if((iRank[candidateFather[i]] == iRank[(*father2)]) && (iDistance[candidateFather[i]] > iDistance[(*father2)]))
      (*father2) = candidateFather[i];
  }
} //tournament

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
          vetMachines[j] += jobMachine[individuals[k][i]][j][1];
        else
          if(vetMachines[j] < vetMachines[j-1])
            vetMachines[j] = vetMachines[j-1] + jobMachine[individuals[k][i]][j][1];
          else
            vetMachines[j] += jobMachine[individuals[k][i]][j][1];
      }
    }
    if(vetMachines[nMachines - 1] > flowMakespan)
      flowMakespan = vetMachines[nMachines - 1];
  }
  flowMakespan = flowMakespan + (int)(flowMakespan/2);

  // Libera a memória
  free (vetMachines);
} //maxMakespan

void createGantt(int **population, int w)
{
  int i, j, k, aux = 0, count = 0;

  // Reseta o Gráfico de Gantt
  for(i = 0; i < nMachines; i++)
    for(j = 0; j < flowMakespan; j++)
      Gantt[i][j] = 0;

  // Preenche o Gráfico de Gantt
  for(i = 0; i < nJobs; i++)
  {
    for(j = 0; j < nMachines; j++)
    {
      redogantt:
      //Enquanto o count for menor que o tempo de duração da atividade e houver máquinas vazias
      while((count < jobMachine[population[w][i]][j][1]) && (Gantt[jobMachine[population[w][i]][j][0]][aux] == 0))
      {
        count++;
        aux++;
        //Caso ultrapasse o valor máximo do Gantt
        if(aux > (flowMakespan))
        {
          printf("Erro violou variável\n");
          return 1;
        }
      }
      //condição criada para dar prosseguimento, caso a atividade não tenha tempo suficiente para ser alocada
      if(count < jobMachine[population[w][i]][j][1])
      {
        count = 0;
        aux++;
        goto redogantt;
      }
      else //Quando o tamanho da tarefa a ser alocada é encontrado, preenche no Gantt
      {
        k = aux;
        while(count > 0)
        {
          Gantt[jobMachine[population[w][i]][j][0]][aux - 1] = (population[w][i] + 1);
          count--;
          aux--;
        }
      }
      aux = k;
    }
    aux = 0;
  }
}// createGantt

// Calcula o makespan dos indivíduos
int fitnessMakespan(int **graph, int w)
{
  int i = 0, j = (flowMakespan - 1), makespan = 0;
  while(i < nMachines)
  {
    while(j > 0)
    {
      if((Gantt[i][j] != 0) && (j > makespan))
      {
        makespan = j;
        break;
      }
      j--;
    }
    i++;
    j = (flowMakespan - 1);
  }
  return (makespan + 1);
} // fitnessMakespan/

void printGantt()
{
  int i, j;

  for(i = 0; i < nMachines; i++)
  {
    for(j = 0; j < flowMakespan; j++)
    {
      printf("%d ", Gantt[i][j]);
    }
    printf("\n");
  }
} //printGantt

void jobDueDate()
{
  int i, j, aux;
  dueDate = (int*)malloc(nJobs*sizeof(int));
  // A data de término foi calculada de acordo com o proposto por Baker, ver artigo Nguyen(2013)
  for(i = 0; i < nJobs; i++)
  {
    aux = 0;
    for(j = 0; j < nMachines; j++)
    {
      aux += jobMachine[i][j][1];
    }
    //O fator h = 1.3 é o mesmo que Nguyen utilizou em seu artigo
    dueDate[i] = (int)(aux * 1.3);
  }
} //jobDueDate

int fitnessTardiness(int **graph, int w)
{
  int i, j, k, tardiness = 0, aux;
  int *jobFinishTime;

  jobFinishTime = (int*)calloc(nJobs,sizeof(int));

  //Verfica a data de término de cada trabalho
  for(i = 0; i < nJobs; i++)
  {
    aux = 0;
    for(j = 0; j < nMachines; j++)
    {
      k = flowMakespan - 1;
      while((k >= 0) && (aux < k))
      {
        if(Gantt[j][k] == (i + 1))
          aux = k + 1;
        k--;
      }
      if(jobFinishTime[i] < aux)
        jobFinishTime[i] = aux;
    }
    // Verifica se o trabalho terminou com atraso
    if(jobFinishTime[i] < dueDate[i])
    {
      jobFinishTime[i] = 0;
    }
    else
    {
      // O critério de peso é o definido por Pinedo e utilizado em alguns artigos, verificar Nguyen (2013)
      if (i < ((int)(nJobs/5)))
      {
        tardiness += (jobFinishTime[i] - dueDate[i]) * 4;
      } else if (i < ((int)((nJobs*3)/5))) {
        tardiness += (jobFinishTime[i] - dueDate[i]) * 2;
      } else {
        tardiness += (jobFinishTime[i] - dueDate[i]);
      }
    }
  }
  return tardiness;
  free(jobFinishTime);
} // fitnessTardiness

void ranking(int w)
{
  int i, j, k, aux = 1; // contadores
  int *n; // Vetor que identifica quantas soluções dominam a solução 'i'
  int **S; // Lista que diz quais soluções são dominadas pela solução 'i'

  n = (int*)calloc(w, sizeof(int));
  S = (int**)malloc(w*sizeof(int*));
  for(i = 0; i < w; i++)
  {
    S[i] = (int*)malloc((w - 1)*sizeof(int));
  }

  for(i = 0; i < w; i++)
    for(j = 0; j < (w - 1); j++)
      S[i][j] = -1;

  // Calcula os termos n e S, utilizados para criar o ranking
  for(i = 0; i < w; i++)
  {
    k = 0;
    for(j = 0; j < w; j++)
    {
      if(i != j)
      {
        if(((fitnessIndividuals[i][0] > fitnessIndividuals[j][0]) && (fitnessIndividuals[i][1] >= fitnessIndividuals[j][1])) || ((fitnessIndividuals[i][0] >= fitnessIndividuals[j][0]) && (fitnessIndividuals[i][1] > fitnessIndividuals[j][1])))
          n[i]++;
        else if(((fitnessIndividuals[i][0] < fitnessIndividuals[j][0]) && (fitnessIndividuals[i][1] <= fitnessIndividuals[j][1])) || ((fitnessIndividuals[i][0] <= fitnessIndividuals[j][0]) && (fitnessIndividuals[i][1] < fitnessIndividuals[j][1])))
        {
          while(S[i][k] != -1)
          {
            k++;
          }
          S[i][k] = j;
        }
      }
    }
  }

  k = 0;
  // Faz o ranking dos indivíduos
  while(k < w)
  {
    // printf("k = %d\n", k);
    for(i = 0; i < w; i++)
    {
      if(n[i] == 0)
      {
        iRank[i] = aux;
        n[i] = -1;
        k++;
      }
    }
    for(i = 0; i < w; i++)
    {
      if(iRank[i] == aux)
      {
        for(j = 0; j < (w -1); j++)
        {
          if(S[i][j] != -1)
            n[S[i][j]]--;
        }
      }
    }
    aux++;
  }

  for(i = 0; i < w; i++)
    free(S[i]);
  free(S);
  free(n);
} //ranking

int selectiRank(int w, int x)
{
  int i, j = 0;

  for(i = 0; i < w; i++)
  {
    if(iRank[i] == x)
    {
      front[j][0] = fitnessIndividuals[i][0];
      front[j][1] = fitnessIndividuals[i][1];
      front[j][2] = i;
      j++;
    }
  }
  return j;
} //selectiRank

void crowdingDistance(int w)
{
  int i;
  double norma;

  quickSort(0, 1, 0, w - 1);

  norma = (double)(front[w - 1][0] - front [0][0]);

  iDistance[front[0][2]] = (double)INFINITY;
  iDistance[front[w - 1][2]] = (double)INFINITY;
  for(i = 1; i < (w - 1); i++)
  {
    iDistance[front[i][2]] = iDistance[front[i][2]] + (double)((double)((double)front[i + 1][0] - (double)front[i - 1][0])/norma);
  }

  quickSort(1, 0, 0, w - 1);

  norma = (double)(front[w - 1][1] - front [0][1]);

  iDistance[front[0][2]] = (double)INFINITY;
  iDistance[front[w - 1][2]] = (double)INFINITY;
  for(i = 1; i < (w - 1); i++)
  {
    iDistance[front[i][2]] = iDistance[front[i][2]] + (double)((double)((double)front[i + 1][1] - (double)front[i - 1][1])/norma);
  }

} //crowdingDistance

void quickSort(int w, int x, int low, int high)
{
  int i, j, pivot, temp;
  if(low < high)
  {
    pivot = low;
    i = low;
    j = high;
    while(i < j)
    {
      while((front[i][w] <= front[pivot][w]) && (i < high))
        i++;
      while(front[j][w] > front[pivot][w])
        j--;
      if(i < j)
      {
        temp = front[i][w];
        front[i][w] = front[j][w];
        front[j][w] = temp;

        temp = front[i][x];
        front[i][x] = front[j][x];
        front[j][x] = temp;

        temp = front[i][2];
        front[i][2] = front[j][2];
        front[j][2] = temp;
      }
    }
    temp = front[pivot][w];
    front[pivot][w] = front[j][w];
    front[j][w] = temp;

    temp = front[pivot][x];
    front[pivot][x] = front[j][x];
    front[j][x] = temp;

    temp = front[pivot][2];
    front[pivot][2] = front[j][2];
    front[j][2] = temp;

    quickSort(w, x, low, j - 1);
    quickSort(w, x, j + 1, high);
  }
} //quickSort

void NSGAIIselection()
{
  int i, j;
  nG newGen[NINDIVIDUALS*2];
  for(i = 0; i < (2*NINDIVIDUALS); i++)
    newGen[i].newIndividuals = (int*)malloc(nJobs*sizeof(int));

  // Copia os individuos par o novo vetor
  for(i = 0; i < (NINDIVIDUALS*2); i++)
  {
    for(j = 0; j < nJobs; j++)
    {
      if(i < NINDIVIDUALS)
        newGen[i].newIndividuals[j] = individuals[i][j];
      else
        newGen[i].newIndividuals[j] = generation[i - NINDIVIDUALS][j];
    }
    newGen[i].fitM = fitnessIndividuals[i][0];
    newGen[i].fitT = fitnessIndividuals[i][1];
    newGen[i].fitRank = iRank[i];
    newGen[i].fitDistance = iDistance[i];
  }

  //realiza o sorteio
  qsort(newGen, (2*NINDIVIDUALS), sizeof(nG), compare);

  qsort(newGen, (NINDIVIDUALS), sizeof(nG), compare2);

  // copia os indivíduos para a nova geração
  for(i = 0; i < NINDIVIDUALS; i++)
  {
    for(j = 0; j < nJobs; j++)
    {
      individuals[i][j] = newGen[i].newIndividuals[j];
    }
    fitnessIndividuals[i][0] = newGen[i].fitM;
    fitnessIndividuals[i][1] = newGen[i].fitT;
    iRank[i] = newGen[i].fitRank;
    iDistance[i] = newGen[i].fitDistance;
  }

  for(i = 0; i < (2*NINDIVIDUALS); i++)
    free(newGen[i].newIndividuals);
} //NSGAIIselection

int compare (const void * a, const void * b)
{
  nG *a2 = (nG *)a;
  nG *b2 = (nG *)b;
  if( a2->fitRank == b2->fitRank )
  {
    if( a2->fitDistance > b2->fitDistance)
      return -1;
    else if (a2->fitDistance < b2->fitDistance)
      return 1;
    else
      return 0;
  }
  else
    return ( a2->fitRank - b2->fitRank );
} //compare

int compare2 (const void * a, const void * b)
{
  nG *a2 = (nG *)a;
  nG *b2 = (nG *)b;
  if( a2->fitRank == b2->fitRank )
    return ((-1)*( a2->fitM - b2->fitM ));
  else
    return ( a2->fitRank - b2->fitRank );
} //compare
