#include <stdio.h>
#include <stdlib.h>
#include "localsearch.h"
#include "geneticalgorithm.h"
#include "neh.h"

void iteratedGreedy(int countInd, int *countEvaluation)
{
  int i, flag, countLS = 0;
  int nd = nJobs * 0.1; //número de itens na fase de destruição
  int *d = (int*)malloc(nd * sizeof(int)); // aloca os genes que serão retirados
  // ind IGindividual;

  IGindividual.jobsOrder = (int*)malloc(nGenes * sizeof(int));

  do {
    destructionPhase(countInd, nd,/* &IGindividual,*/ d);
    constructionPhase(nd, /*&IGindividual,*/ d);
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
    (*countEvaluation)++;
    if((*countEvaluation) == orc)
      break;
  } while(flag == 1);


  free(d);
  free(IGindividual.jobsOrder);
} //integratedGreedy

void destructionPhase(int countInd, int nd,/* ind *IGindividual,*/ int *d)
{
  int i, j, flag;
  int wd; //Número para sorteio de qual gene será retirado

  for(i = 0; i < nd; i++)
  {
    flag = 1;
    while(flag == 1)
    {
      wd = rand() % nJobs;
      if(nd > 1)
      {
        for(j = 0; j < i; j++)
        {
          if(wd == d[j])
            break;
          else
            flag = 0;
        }
      }
      else{
        flag = 0;
      }
    }
    d[i] = wd;
  }

  for(i = 0; i < nGenes; i++)
  {
    flag = 0;
    for(j = 0; j < nd; j++)
    {
      if(individuals[countInd].jobsOrder[i] == d[j])
        flag = 1;
    }
    if(flag == 1)
      IGindividual.jobsOrder[i] = -1;
    else
      IGindividual.jobsOrder[i] = individuals[countInd].jobsOrder[i];
  }
  for(i = 0; i < (nGenes - 1); i++)
  {
    if(IGindividual.jobsOrder[i] == -1)
    {
      IGindividual.jobsOrder[i] = IGindividual.jobsOrder[i + 1];
      IGindividual.jobsOrder[i + 1] = -1;
    }
  }

} //destructionPhase

void constructionPhase(int nd,/* ind *IGindividual,*/ int *d)
{
  int i, j, k = 0, bestAllocation, flag;

  tempIndividual = (ind*)malloc((nGenes - nd + 1) * sizeof(ind));
  for(i = 0; i < (nGenes - nd + 1); i++)
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

  for(i = 0; i < (nGenes - nd + 1); i++)
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
