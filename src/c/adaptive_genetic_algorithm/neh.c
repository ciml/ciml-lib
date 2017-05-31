#include <stdio.h>
#include <stdlib.h>
#include "geneticalgorithm.h"
#include "neh.h"

void evaluateProcessTime()
{
  int i, j;
  jobsInfo = (JobsInformation*)calloc(nJobs, sizeof(JobsInformation));
  for(i = 0; i < nJobs; i++)
  {
    jobsInfo[i].Id = i;
    for(j = 0; j < nMachines; j++)
    {
      jobsInfo[i].processTime += jobMachine[i][j][1];
    }
  }
}

int sortDescendingJobProcessTime(const void *a, const void *b)
{
  JobsInformation *a2 = (JobsInformation *)a;
  JobsInformation *b2 = (JobsInformation *)b;
  return (b2->processTime - a2->processTime);
}

void NEHfitnessEvaluation(int w, int y)
{
  int i, j, k, aux[3], flag[3], timeGap, timeAux, bestAllocation;
  int *jobFinishTime;
  // node ***tmp;

  startList();

  jobFinishTime = (int*)calloc(y, sizeof(int));

  for(i = 0; i < y; i++)
  {
    timeAux = 0;
    for(j = 0; j < nMachines; j++)
    {
      for(k = 0; k < qMachines[jobMachine[tempIndividual[w].jobsOrder[i]][j][0]]; k++)
      {
        if(Gantt[jobMachine[tempIndividual[w].jobsOrder[i]][j][0]][k]->prox == NULL)
        {
          aux[k] = timeAux;
          flag[k] = 1;
        }else
        {
          tmp[k][0] = Gantt[jobMachine[tempIndividual[w].jobsOrder[i]][j][0]][k];
          tmp[k][1] = Gantt[jobMachine[tempIndividual[w].jobsOrder[i]][j][0]][k]->prox;
          if((*tmp[k][1]).elem.startTime >
                      (timeAux + jobMachine[tempIndividual[w].jobsOrder[i]][j][1]))
          {
            aux[k] = timeAux;
            flag[k] = 2;
          }else
          {
            while(1)
            {
              tmp[k][0] = tmp[k][1];
              tmp[k][1] = tmp[k][1]->prox;
              if(tmp[k][1] == NULL)
              {
                if(timeAux > (*tmp[k][0]).elem.endTime)
                {
                  aux[k] = timeAux;
                  flag[k] = 3;
                }
                else
                {
                  aux[k] = (*tmp[k][0]).elem.endTime;
                  flag[k] = 4;
                }
                break;
              }
              else
              {
                if(timeAux > (*tmp[k][0]).elem.endTime)
                {
                  timeGap = (*tmp[k][1]).elem.startTime - timeAux;
                  if(timeGap > jobMachine[tempIndividual[w].jobsOrder[i]][j][1])
                  {
                    aux[k] = timeAux;
                    flag[k] = 5;
                    break;
                  }
                }
                else
                {
                  timeGap = (*tmp[k][1]).elem.startTime - (*tmp[k][0]).elem.endTime;
                  if(timeGap > jobMachine[tempIndividual[w].jobsOrder[i]][j][1])
                  {
                    aux[k] = (*tmp[k][0]).elem.endTime;
                    flag[k] = 6;
                    break;
                  }
                }
              }
            }
          }
        }
      }

      bestAllocation = 0;
      for(k = 0; k < qMachines[jobMachine[tempIndividual[w].jobsOrder[i]][j][0]]; k++)
      {
        if(aux[k] < aux[bestAllocation])
          bestAllocation = k;
      }

      //aloca a tarefa
      switch(flag[bestAllocation])
      {
        case 1:
          (*operation[i*nMachines + j]).elem.startTime = timeAux;
          (*operation[i*nMachines + j]).elem.endTime = (*operation[i*nMachines + j]).elem.startTime
                                + jobMachine[tempIndividual[w].jobsOrder[i]][j][1];
          (*operation[i*nMachines + j]).elem.jobId = tempIndividual[w].jobsOrder[i] + 1;
          operation[i*nMachines + j]->prox = NULL;
          Gantt[jobMachine[tempIndividual[w].jobsOrder[i]][j][0]][bestAllocation]->prox =
                                                      operation[i*nMachines + j];
          timeAux = (*operation[i*nMachines + j]).elem.endTime;
          break;
        case 2:
          (*operation[i*nMachines + j]).elem.startTime = timeAux;
          (*operation[i*nMachines + j]).elem.endTime = (*operation[i*nMachines + j]).elem.startTime
                              + jobMachine[tempIndividual[w].jobsOrder[i]][j][1];
          (*operation[i*nMachines + j]).elem.jobId = tempIndividual[w].jobsOrder[i] + 1;
          operation[i*nMachines + j]->prox = tmp[bestAllocation][1];
          Gantt[jobMachine[tempIndividual[w].jobsOrder[i]][j][0]][bestAllocation]->prox =
                                                    operation[i*nMachines + j];
          timeAux = (*operation[i*nMachines + j]).elem.endTime;
          break;
        case 3:
          (*operation[i*nMachines + j]).elem.startTime = timeAux;
          (*operation[i*nMachines + j]).elem.endTime = (*operation[i*nMachines + j]).elem.startTime
                                 + jobMachine[tempIndividual[w].jobsOrder[i]][j][1];
          (*operation[i*nMachines + j]).elem.jobId = tempIndividual[w].jobsOrder[i] + 1;
          operation[i*nMachines + j]->prox = NULL;
          tmp[bestAllocation][0]->prox = operation[i*nMachines + j];
          timeAux = (*operation[i*nMachines + j]).elem.endTime;
          break;
        case 4:
          (*operation[i*nMachines + j]).elem.startTime = (*tmp[bestAllocation][0]).elem.endTime;
          (*operation[i*nMachines + j]).elem.endTime = (*operation[i*nMachines + j]).elem.startTime
                              + jobMachine[tempIndividual[w].jobsOrder[i]][j][1];
          (*operation[i*nMachines + j]).elem.jobId = tempIndividual[w].jobsOrder[i] + 1;
          operation[i*nMachines + j]->prox = NULL;
          tmp[bestAllocation][0]->prox = operation[i*nMachines + j];
          timeAux = (*operation[i*nMachines + j]).elem.endTime;
          break;
        case 5:
          (*operation[i*nMachines + j]).elem.startTime = timeAux;
          (*operation[i*nMachines + j]).elem.endTime = (*operation[i*nMachines + j]).elem.startTime
                            + jobMachine[tempIndividual[w].jobsOrder[i]][j][1];
          (*operation[i*nMachines + j]).elem.jobId = tempIndividual[w].jobsOrder[i] + 1;
          operation[i*nMachines + j]->prox = tmp[bestAllocation][1];
          tmp[bestAllocation][0]->prox = operation[i*nMachines + j];
          timeAux = (*operation[i*nMachines + j]).elem.endTime;
          break;
        case 6:
          (*operation[i*nMachines + j]).elem.startTime = (*tmp[bestAllocation][0]).elem.endTime;
          (*operation[i*nMachines + j]).elem.endTime = (*operation[i*nMachines + j]).elem.startTime
                            + jobMachine[tempIndividual[w].jobsOrder[i]][j][1];
          (*operation[i*nMachines + j]).elem.jobId = tempIndividual[w].jobsOrder[i] + 1;
          operation[i*nMachines + j]->prox = tmp[bestAllocation][1];
          tmp[bestAllocation][0]->prox = operation[i*nMachines + j];
          timeAux = (*operation[i*nMachines + j]).elem.endTime;
          break;
      }
    }
    //Armazena o término das operações
    jobFinishTime[i] = (*operation[i*nMachines + nMachines - 1]).elem.endTime;
  }

  tempIndividual[w].fitMakespan = fitnessMakespan(y, jobFinishTime);

  free(jobFinishTime);
}//NEHfitnessEvaluation

void executeNEH(ind NEHindividual)
{
  int i, j, k = 2, flag, bestAllocation;
  tempIndividual = (ind*)malloc(nJobs * sizeof(ind));
  for(i = 0; i < nJobs; i++)
    tempIndividual[i].jobsOrder = (int*)malloc(nJobs * sizeof(int));

  evaluateProcessTime();
  qsort(jobsInfo, nJobs, sizeof(JobsInformation), sortDescendingJobProcessTime);

  tempIndividual[0].jobsOrder[0] = jobsInfo[0].Id;
  tempIndividual[0].jobsOrder[1] = jobsInfo[1].Id;
  NEHfitnessEvaluation(0, k);
  tempIndividual[1].jobsOrder[0] = jobsInfo[1].Id;
  tempIndividual[1].jobsOrder[1] = jobsInfo[0].Id;
  NEHfitnessEvaluation(1, k);

  bestAllocation = 0;
  for(i = 1; i < k; i++)
  {
    if(tempIndividual[i].fitMakespan < tempIndividual[bestAllocation].fitMakespan)
      bestAllocation = i;
  }
  for(i = 0; i < k; i++)
    NEHindividual.jobsOrder[i] = tempIndividual[bestAllocation].jobsOrder[i];

  k++; //Contador de quantos trabalhos já foram avaliados na heurística construtiva
  while(k <= nJobs)
  {
    for(i = 0; i < k; i++)
    {
      flag = 0; //flag para verificar se o trabalho 'w' já foi adicionado
      for(j = 0; j < k; j++)
      {
        if(i == j)
        {
          tempIndividual[i].jobsOrder[j] = jobsInfo[k - 1].Id;
          flag++;
        }
        else
        {
          tempIndividual[i].jobsOrder[j] = NEHindividual.jobsOrder[j - flag];
        }
      }
    }

    for(i = 0; i < k; i++)
      NEHfitnessEvaluation(i, k);

    bestAllocation = 0;
    for(i = 1; i < k; i++)
    {
      if(tempIndividual[i].fitMakespan < tempIndividual[bestAllocation].fitMakespan)
        bestAllocation = i;
    }
    for(i = 0; i < k; i++)
      NEHindividual.jobsOrder[i] = tempIndividual[bestAllocation].jobsOrder[i];
    k++;
  }
  NEHindividual.fitMakespan = tempIndividual[bestAllocation].fitMakespan;

  //Libera memória
  for(i = 0; i < nJobs; i++)
    free(tempIndividual[i].jobsOrder);
  free(tempIndividual);
  free(jobsInfo);
}
