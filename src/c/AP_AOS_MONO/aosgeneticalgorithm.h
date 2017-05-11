#ifndef AOSGENETICALGORITHM_H
#define AOSGENETICALGORITHM_H

extern int popLenght;  // Número de indivíduos do AG interno
extern int numGeneration;
extern float probCrossover;
extern float probMutation;

// struct AG interno
//Indivíduo
typedef struct ind{
  int *jobsOrder;
  int fitMakespan;
  int fitTardiness;
  int iRank;
  double iDistance;
}ind;

//Atividade
typedef struct activity{
  int startTime;
  int endTime;
  int jobId;
}act;

//Lista
typedef struct Node{
  struct activity elem;
  struct Node *prox;
}node;

ind *individuals;
ind *selectedIndividuals;

node ***tmp;

extern node ***Gantt;            // Lista encabeçada para as informações
extern node **operation;			 // Armazena info de inicio e fim dos jobs

extern int nJobs;                // Número de Jobs
extern int nMachines;            // Número de Máquinas
extern int flowMakespan;         // Auxiliar que armazena o máximo de makespan
extern int ***jobMachine;        // Relaciona o tempo dos jobs nas máquinas
extern int *qMachines;
extern int *dueDate;             // Data de vencimento dos jobs
extern double z[2];              //Valores utópicos para o hypervolume
extern double HV;

void AGinterno(char *fileName, char *nRepeat, char pop[]);  //main_loop
void initializeIndividuals();
void createGantt(int w);
int fitnessMakespan(int jobFinishTime[nJobs]);
// int fitnessTardiness();
// void ranking (int w);
// void crowdingDistanceMakespan(int w, int z);
// void crowdingDistanceTardiness(int w, int z);
int sortFitMakespan(const void *a, const void *b);
// int sortFitTardiness(const void *a, const void *b);
void tournament(int *father1, int *father2);
void initializeGeneration();
// int NSGAIIselection(const void * a, const void * b);
// double hypervolume();
int sortFitMakespanDescending(const void *a, const void *b);
void selection();
int selectionTournament();
void printProbabilities(int countInd, char *fileName, char *nRepeat, char pop[]);
void printBestIndividual(char *fileName, char *nRepeat, int countGen);
void startList();

#endif //AOSGENETICALGORITHM_H
