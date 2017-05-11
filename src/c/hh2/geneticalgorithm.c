#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "geneticalgorithm.h"

void initializeIndividuals()
{
  int i, j, r, temp;
  //Realiza o embaralhamento dos indivíduos
  for (i = 0; i < popLenght; i++)
  {
    // Gera o indivíduo
    for (j = 0; j < nJobs; j++)
      individuals[i].jobsOrder[j] = j;
    // Realiza o embaralhamento
    for (j = 0; j < nJobs; j++)
    {
      r = rand() % nJobs;
      temp = individuals[i].jobsOrder[j];
      individuals[i].jobsOrder[j] = individuals[i].jobsOrder[r];
      individuals[i].jobsOrder[r] = temp;
    }
  }
} //Função initializeIndividuals

void createGantt(int w)
{
  int i, j, k, l, aux[3] = {0}, count[3] = {0}, bestAllocation;

  // Reseta o Gráfico de Gantt
  for(i = 0; i < nMachines; i++)
    for(j = 0; j < flowMakespan; j++)
      for(k = 0; k < qMachines[i]; k++)
        Gantt[i][j][k] = 0;

  // Preenche o Gráfico de Gantt
  for(i = 0; i < nJobs; i++)
  {
    for(j = 0; j < nMachines; j++)
    {
      /*Enquanto o count for menor que o tempo de duração da atividade
                                                    e houver máquinas vazias*/
      for(k = 0; k < qMachines[j]; k++)
      {
        redogantt:
        while((count[k] < jobMachine[individuals[w].jobsOrder[i]][j][1]) &&
            (Gantt[jobMachine[individuals[w].jobsOrder[i]][j][0]][aux[k]][k] == 0))
        {
          count[k]++;
          aux[k]++;
          //Caso ultrapasse o valor máximo do Gantt
          if(aux[k] > (flowMakespan))
          {
            printf("Erro, violou tamanho do Gantt! Reajustar flowMakespan\n");
            exit(2);
          }
        }
        /*condição criada para dar prosseguimento, caso a atividade não tenha
                                            tempo suficiente para ser alocada*/
        if(count[k] < jobMachine[individuals[w].jobsOrder[i]][j][1])
        {
          count[k] = 0;
          aux[k]++;
          goto redogantt;
        }
      }

      bestAllocation = 0;
      for(k = 1; k < qMachines[j]; k++)
        if(aux[k] < aux[bestAllocation])
          bestAllocation = k;

      l = aux[bestAllocation];
      while(count[bestAllocation] > 0)
      {
        Gantt[jobMachine[individuals[w].jobsOrder[i]][j][0]][aux[bestAllocation] - 1][bestAllocation] =
                                          (individuals[w].jobsOrder[i] + 1);
        count[bestAllocation]--;
        aux[bestAllocation]--;
      }

      for(k = 0; k < 3; k++)
      {
        aux[k] = l;
        count[k] = 0;
      }
    }
    for(k = 0; k < qMachines[j]; k++)
      aux[k] = 0;
  }
}// createGantt

// Calcula o makespan dos indivíduos
int fitnessMakespan()
{
  int i, j, k, makespan = 0;
  for(i = 0; i < nMachines; i++)
  {
    for(k = 0; k < qMachines[i]; k++)
    {
      for(j = (flowMakespan - 1); j > 0; j--)
      {
        if((Gantt[i][j][k] != 0) && (j > makespan))
        {
          makespan = j;
          break;
        }
        if(j <= makespan)
        {
          break;
        }
      }
    }
  }
  return (makespan + 1);
} // fitnessMakespan

int fitnessTardiness()
{
  int i, j, k[3], l, tardiness = 0;
  int *jobFinishTime;

  jobFinishTime = (int*)calloc(nJobs, sizeof(int));

  for(i = 0; i < nJobs; i++)
  {
    j = jobMachine[i][nMachines - 1][0];
    for(l = 0; l < qMachines[j]; l++)
      k[l] = flowMakespan - 1;

    for(l = 0; l < qMachines[j]; l++)
    {
      while(k[l] > 0)
      {
        if(Gantt[j][k[l]][l] == (i + 1))
        {
          // jobFinishTime[i] = k[l] + 1;
          break;
        }
        if((l > 0) && k[l] < k[l - 1])
            break;
        k[l]--;
      }
    }
    jobFinishTime[i] = k[0] + 1;
    for(l = 1; l < qMachines[l]; l++)
      if((k[l] + 1) > jobFinishTime[i])
        jobFinishTime[i] = k[l] + 1;


    // Verifica se o trabalho terminou com atraso
    if(jobFinishTime[i] < dueDate[i])
    {
      jobFinishTime[i] = 0;
    }
    else
    {
      /*O critério de peso é o definido por Pinedo e utilizado
                                    em alguns artigos, verificar Nguyen (2013)*/
      if (i < ((int)(nJobs/5)))
      {
        tardiness += ((jobFinishTime[i] - dueDate[i]) * 4);
      } else if (i < ((int)((nJobs*3)/5))) {
        tardiness += ((jobFinishTime[i] - dueDate[i]) * 2);
      } else {
        tardiness += (jobFinishTime[i] - dueDate[i]);
      }
    }
  }
  free(jobFinishTime);
  return tardiness;
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
        if(((individuals[i].fitMakespan > individuals[j].fitMakespan) &&
          (individuals[i].fitTardiness >= individuals[j].fitTardiness)) ||
          ((individuals[i].fitMakespan >= individuals[j].fitMakespan) &&
            (individuals[i].fitTardiness > individuals[j].fitTardiness)))
          n[i]++;
        else if(((individuals[i].fitMakespan < individuals[j].fitMakespan) &&
          (individuals[i].fitTardiness <= individuals[j].fitTardiness)) ||
          ((individuals[i].fitMakespan <= individuals[j].fitMakespan) &&
            (individuals[i].fitTardiness < individuals[j].fitTardiness)))
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
    for(i = 0; i < w; i++)
    {
      if(n[i] == 0)
      {
        individuals[i].iRank = aux;
        n[i] = -1;
        k++;
      }
    }
    for(i = 0; i < w; i++)
    {
      if(individuals[i].iRank == aux)
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

void crowdingDistanceMakespan(int w, int z)
{
  int i;
  double norma;

  norma = (double)(individuals[w - 1].fitMakespan - individuals[z].fitMakespan);

  individuals[z].iDistance = (double)INFINITY;
  individuals[w - 1].iDistance = (double)INFINITY;
  for(i = z + 1; i < (w - 1); i++)
  {
    individuals[i].iDistance = individuals[i].iDistance +
                (double)((double)((double)individuals[i + 1].fitMakespan -
                                (double)individuals[i - 1].fitMakespan)/norma);
  }
} //crowdingDistanceMakespan

void crowdingDistanceTardiness(int w, int z)
{
  int i;
  double norma;

  norma = (double)(individuals[w - 1].fitTardiness -
                                                  individuals[z].fitTardiness);

  individuals[z].iDistance = (double)INFINITY;
  individuals[w - 1].iDistance = (double)INFINITY;
  for(i = z + 1; i < (w - 1); i++)
  {
    individuals[i].iDistance = individuals[i].iDistance +
            (double)((double)((double)individuals[i + 1].fitTardiness -
                                (double)individuals[i - 1].fitTardiness)/norma);
  }
} //crowdingDistanceTardiness

int sortFitMakespan(const void *a, const void *b)
{
  ind *a2 = (ind *)a;
  ind *b2 = (ind *)b;
  if( a2->iRank == b2->iRank )
  {
    return (a2->fitMakespan - b2->fitMakespan);
  }
  else
    return (a2->iRank - b2->iRank);
} //sortFitMakespan

int sortFitTardiness(const void *a, const void *b)
{
  ind *a2 = (ind *)a;
  ind *b2 = (ind *)b;
  if( a2->iRank == b2->iRank )
  {
    return (a2->fitTardiness - b2->fitTardiness);
  }
  else
    return (a2->iRank - b2->iRank);
} //sortFitTardiness

void initializeGeneration()
{
  int i, j;
  for(i = popLenght; i < 2*popLenght; i++)
    for(j = 0; j < nJobs; j++)
      individuals[i].jobsOrder[j] = (-1);
} //initializeGeneration

//Seleçao por torneio (4 individuos)
void tournament(int *father1, int *father2)
{
  int i, nCandidate = 2, candidateFather[nCandidate];

  //Seleciona o primeiro pai
  for(i = 0; i < nCandidate; i++)
  {
    candidateFather[i] = rand() % popLenght;
  }

  (*father1) = candidateFather[0];
  for(i = 1; i < nCandidate; i++)
  {
    if(individuals[candidateFather[i]].iRank < individuals[(*father1)].iRank)
      (*father1) = candidateFather[i];
    else if((individuals[candidateFather[i]].iRank ==
                            individuals[(*father1)].iRank) &&
                              (individuals[candidateFather[i]].iDistance >
                                            individuals[(*father1)].iDistance))
      (*father1) = candidateFather[i];
  }

  //Seleciona o segundo pai
  for(i = 0; i < nCandidate; i++)
  {
    candidateFather[i] = rand()%popLenght;
  }
  (*father2) = candidateFather[0];
  for(i = 1; i < nCandidate; i++)
  {
    if(individuals[candidateFather[i]].iRank < individuals[(*father2)].iRank)
      (*father2) = candidateFather[i];
    else if((individuals[candidateFather[i]].iRank ==
                            individuals[(*father2)].iRank) &&
                              (individuals[candidateFather[i]].iDistance >
                                            individuals[(*father2)].iDistance))
      (*father2) = candidateFather[i];
  }
} //tournament

int NSGAIIselection (const void * a, const void * b)
{
  ind *a2 = (ind *)a;
  ind *b2 = (ind *)b;
  if( a2->iRank == b2->iRank )
  {
    if( a2->iDistance > b2->iDistance)
      return -1;
    else if (a2->iDistance < b2->iDistance)
      return 1;
    else
      return 0;
  }
  else
    return ( a2->iRank - b2->iRank );
} //NSGAIIselection

double hypervolume()
{
  int i, count = 0;
  double volume;

  qsort(individuals, popLenght, sizeof(ind), sortFitMakespanDescending);

  for(i = 0; i < popLenght; i++){
    if(individuals[i].iRank == 1)
      count++;
    else
      break;
  }
  if(((z[0] - individuals[0].fitMakespan) < 0) ||
                                    ((z[1] - individuals[0].fitTardiness) < 0))

    printf("Ajustar o multiplicadores de Z[0] = %lf Z[1] = %lf",
     (z[0] - individuals[0].fitMakespan), (z[1] - individuals[0].fitTardiness));

  volume = ((z[0] - individuals[0].fitMakespan) *
                                        (z[1] - individuals[0].fitTardiness));

  for(i = 1; i < count; i++)
  {
  if(((individuals[i - 1].fitMakespan - individuals[i].fitMakespan) < 0) ||
                                    ((z[1] - individuals[i].fitTardiness) < 0))
    printf("Ajustar o multiplicadores de Z[%d] = %d Z[1] = %lf\n", i,
    (individuals[i - 1].fitMakespan - individuals[i].fitMakespan), (z[1] - individuals[i].fitTardiness));

    volume += ((individuals[i - 1].fitMakespan - individuals[i].fitMakespan)
                                        * (z[1] - individuals[i].fitTardiness));
  }
  return volume;
} //hypervolume

int sortFitMakespanDescending(const void *a, const void *b)
{
  ind *a2 = (ind *)a;
  ind *b2 = (ind *)b;
  if( a2->iRank == b2->iRank )
  {
    return (b2->fitMakespan - a2->fitMakespan);
  }
  else
    return (a2->iRank - b2->iRank);
} //sortFitMakespan
