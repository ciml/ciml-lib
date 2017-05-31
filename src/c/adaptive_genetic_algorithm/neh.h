#ifndef NEH_H
#define NEH_H

//struct criada para aramzenar informações dos jobs
typedef struct jobsInformation{
int processTime;
int Id;
}JobsInformation;

JobsInformation *jobsInfo;
ind *tempIndividual;

void evaluateProcessTime();
int sortDescendingJobProcessTime(const void *a, const void *b);
void NEHfitnessEvaluation(int w, int y);

void executeNEH(ind NEHindividual);

#endif //NEH_H
