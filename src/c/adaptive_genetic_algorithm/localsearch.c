#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "localsearch.h"
#include "geneticalgorithm.h"
#include "neh.h"

void iteratedGreedy(int countInd, int *countEvaluation)
{
  int i, flag, countLS = 0;
  int nd = nJobs * 0.1; //número de itens na fase de destruição
  double lsNumEvaluation = 0.0;
  d = (int*)malloc(nd * sizeof(int)); // aloca os genes que serão retirados

  IGindividual.jobsOrder = (int*)malloc(nGenes * sizeof(int));

  for(i = 0; i < nd; i++)
    lsNumEvaluation += ((double)(nGenes - i)*(nGenes - i))/nGenes;

  do {
    destructionPhase(countInd, nd);
    constructionPhase(nd);

    (*countEvaluation) += (int)ceil(lsNumEvaluation);
    if((*countEvaluation) > orc)
      break;

    if(IGindividual.fitMakespan < individuals[countInd].fitMakespan)
    {
      for(i = 0; i < nGenes; i++)
        individuals[countInd].jobsOrder[i] = IGindividual.jobsOrder[i];
      individuals[countInd].fitMakespan = IGindividual.fitMakespan;
      flag = 1;
      countLS++;
    }
    else{
      flag = 0;
    }
  } while((flag == 1) && ((*countEvaluation) < orc));

  free(d);
  free(IGindividual.jobsOrder);
} //integratedGreedy

void destructionPhase(int countInd, int nd)
{
  int i, j, aux, flag;
  int wd; //Número para sorteio de qual gene será retirado

  for(i = 0; i < nd; i++)
  {
    flag = 1;
    while(flag == 1)
    {
      wd = rand() % nGenes;
      if(i == 0)
      {
        flag = 0;
      }
      else
      {
        for(j = 0; j < i; j++)
        {
          if(wd == d[j])
          {
            flag = 1;
            break;
          }
          else
            flag = 0;
        }
      }
    }
    d[i] = wd;
  }

  for(i = 0; i < nGenes; i++)
    IGindividual.jobsOrder[i] = individuals[countInd].jobsOrder[i];

  for(i = 0; i < nd; i++)
  {
    aux = IGindividual.jobsOrder[d[i]];
    IGindividual.jobsOrder[d[i]] = (-1);
    d[i] = aux;
  }

  for(i = 0; i < (nGenes - 1); i++)
  {
    if(IGindividual.jobsOrder[i] == -1)
    {
      j = i;
      do {
        j++;
      } while((IGindividual.jobsOrder[j] == -1) && (j < (nGenes -1)));

      IGindividual.jobsOrder[i] = IGindividual.jobsOrder[j];
      IGindividual.jobsOrder[j] = -1;
    }
  }
} //destructionPhase

void constructionPhase(int nd)
{
  int i, j, k = 0, bestAllocation = 0, flag;

  tempIndividual = (ind*)malloc(nGenes * sizeof(ind));
  for(i = 0; i < nGenes; i++)
    tempIndividual[i].jobsOrder = (int*)malloc(nGenes * sizeof(int));

  while(k < nd)
  {
    for(i = 0; i < (nGenes - nd + k + 1); i++)
    {
      flag = 0; //flag para verificar se o trabalho 'w' já foi adicionado
      for(j = 0; j < (nGenes - nd + k + 1); j++)
      {
        if(i == j)
        {
          tempIndividual[i].jobsOrder[j] = d[k];
          flag++;
        }
        else
        {
          tempIndividual[i].jobsOrder[j] = IGindividual.jobsOrder[j - flag];
        }
      }
    }

    for(i = 0; i < (nGenes - nd + k + 1); i++)
      NEHfitnessEvaluation(i, (nGenes - nd + k + 1));

    bestAllocation = 0;
    for(i = 1; i < (nGenes - nd + k + 1); i++)
    {
      if(tempIndividual[i].fitMakespan < tempIndividual[bestAllocation].fitMakespan)
        bestAllocation = i;
    }
    for(i = 0; i < (nGenes - nd + k + 1); i++)
      IGindividual.jobsOrder[i] = tempIndividual[bestAllocation].jobsOrder[i];
    k++;
  }
  IGindividual.fitMakespan = tempIndividual[bestAllocation].fitMakespan;

  for(i = 0; i < nGenes; i++)
    free(tempIndividual[i].jobsOrder);
  free(tempIndividual);
} //constructionPhase

void LocalSearch(char *fileName, char *nRepeat, char pop[])
{
  int i, countEvaluation = 0;

  initializeIndividuals();

  for(i = 0; i < popLenght; i++)
    fitnessEvaluation(i);

  while(countEvaluation < orc)
  {
    for(i = 0; i < popLenght; i++)
    {
      iteratedGreedy(i, &countEvaluation);
      if(countEvaluation == orc)
        break;
    }
  }

  qsort(individuals, popLenght, sizeof(ind), sortFitMakespan);

  printMakespan(fileName, nRepeat);
}
