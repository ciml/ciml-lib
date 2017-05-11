#include <stdio.h>
#include <stdlib.h>
#include "geneticalgorithm.h"
#include "mutation.h"


// Mutação por inserção
void shiftMutation(int countInd)
{
  int gene, place, vaux1, i;
  gene = rand() % nJobs;
  place = rand() % nJobs;
  while(gene == place)
    place = rand() % nJobs;
  if(gene < place)
  {
    vaux1 = individuals[countInd].jobsOrder[gene];
    for (i = gene; i < place; i++)
    {
      individuals[countInd].jobsOrder[i] = individuals[countInd].jobsOrder[i+1];
    }
    individuals[countInd].jobsOrder[place] = vaux1;
  }
  else
  {
    vaux1 = individuals[countInd].jobsOrder[gene];
    for(i = gene ; i > place; i--)
    {
      individuals[countInd].jobsOrder[i] = individuals[countInd].jobsOrder[i-1];
    }
    individuals[countInd].jobsOrder[place] = vaux1;
  }
} // shiftMutation

//mutação por troca
void interchangeMutation(int countInd)
{
  int gene, place, temp;
  gene = rand() % nJobs;
  place = rand() % nJobs;
  temp = individuals[countInd].jobsOrder[gene];
  individuals[countInd].jobsOrder[gene] = 
                                        individuals[countInd].jobsOrder[place];
  individuals[countInd].jobsOrder[place] = temp;
} // interchangeMutation