#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

extern int popLenght;  // Número de indivíduos do AG interno
extern int numGeneration;
extern double probCrossover;
extern double probMutation;

//Indivíduo
typedef struct ind{
  int *jobsOrder;
  int fitMakespan;
  int fitTardiness;
  int iRank;
  double iDistance;
}ind;

//Atividade para mecanismo de avaliação do fitness
typedef struct activity{
  int startTime;
  int endTime;
  int jobId;
}act;

//Lista para avaliação do fitness
typedef struct Node{
  struct activity elem;
  struct Node *prox;
}node;

//Struct que controla a probabilidade das TAXAS dos operadores
typedef struct probabilitiescontrol{
	int index[2];
	int successApplication;
	double *success;
	double prob; //Probabilidade de utilizar determinada TAXA
	double quality;
	double reward;
}ProbabilitiesControl;

ProbabilitiesControl crossOperatorControl[3];
ProbabilitiesControl mut1OperatorControl[2];
ProbabilitiesControl mut2OperatorControl[2];

ProbabilitiesControl crossRateControl[3];
ProbabilitiesControl mut1RateControl[3];
ProbabilitiesControl mut2RateControl[3];

// indíviduos padrão do algoritmo genético
ind *individuals;

// indivíduos temporários criados para a função de seleção
ind *selectedIndividuals;

// Variável temporária para avaliar o fitness
node ***tmp;

extern node ***Gantt;            // Lista encabeçada para as informações
extern node **operation;			 // Armazena info de inicio e fim dos jobs

extern int nJobs;                // Número de Jobs
extern int nMachines;            // Número de Máquinas
extern int nGenes;              // Número de genes que o filho terá, depeendendo da representação adotada
extern int flowMakespan;         // Auxiliar que armazena o máximo de makespan
extern int ***jobMachine;        // Relaciona o tempo dos jobs nas máquinas
extern int *qMachines;           // Vetor que armazena o número de máquinas semelhantes em cada estágio
extern int *dueDate;             // Data de vencimento dos jobs
extern double z[2];              //Valores utópicos para o hypervolume
extern double HV;               // variável para calcular o hypervolume

void GeneticAlgorithm(char *fileName, char *nRepeat, char pop[]);  //main_loop
void initializeIndividuals();
void fitnessEvaluationOBR(int w);
void fitnessEvaluationJBR(int w);
int fitnessMakespan(int numJobs, int jobFinishTime[numJobs]);
int sortFitMakespan(const void *a, const void *b);
void tournament(int *father1, int *father2);
void initializeGeneration();
int sortFitMakespanDescending(const void *a, const void *b);
void selection();
void selectionWithTournament();
int selectionTournament();
void printProbabilities(int countInd, char *fileName, char *nRepeat, char pop[]);
void printBestIndividual(char *fileName, char *nRepeat, int countGen);
void startList();

// Funções que escolhem os OPERADORES a serem aplicados
void crossoverOperatorSelection(int countInd, int father1, int father2);
void mutaOperatorSelection(int countInd, int size, ProbabilitiesControl probControl[size]);

// Funções que controlam a probabilidade as TAXAS de crossover e mutação a serem aplicadas
double crossoverRateSelection();
double mutaRateSelection(int size, ProbabilitiesControl probControl[size]);

#endif //GENETICALGORITHM_H
