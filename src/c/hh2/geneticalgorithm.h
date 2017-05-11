#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

extern int popLenght;  // Número de indivíduos do AG interno

// struct AG interno
typedef struct ind{
  int *jobsOrder;
  int fitMakespan;
  int fitTardiness;
  int iRank;
  double iDistance;
}ind;

ind *individuals;

extern int nJobs;                // Número de Jobs
extern int nMachines;            // Número de Máquinas
extern int flowMakespan;         // Auxiliar que armazena o máximo de makespan
extern int ***Gantt;                  // Matriz que alocará informações
extern int ***jobMachine;            // Relaciona o tempo dos jobs nas máquinas
extern int *qMachines;
extern int *dueDate;                 // Data de vencimento dos jobs
extern double z[2];               //Valores utópicos para o hypervolume


void initializeIndividuals();
void createGantt(int w);
int fitnessMakespan();
// int fitnessMakespanold();
int fitnessTardiness();
// int fitnessTardinessold();
void ranking (int w);
void crowdingDistanceMakespan(int w, int z);
void crowdingDistanceTardiness(int w, int z);
int sortFitMakespan(const void *a, const void *b);
int sortFitTardiness(const void *a, const void *b);
void tournament(int *father1, int *father2);
void initializeGeneration();
int NSGAIIselection(const void * a, const void * b);
double hypervolume();
int sortFitMakespanDescending(const void *a, const void *b);

#endif // GENETICALGORITHM_H
