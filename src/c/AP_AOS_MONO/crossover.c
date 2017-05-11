#include <stdio.h>
#include <stdlib.h>
#include "aosgeneticalgorithm.h"
#include "crossover.h"

void opCrossover(int countInd, int father1, int father2)
{
  int i, j, id, id2, gver;
  id = rand() % nJobs;
  id2 = id;
  //Copia metade dos genes do primeiro pai
  for(i = 0; i < id; i++)
  {
    individuals[countInd].jobsOrder[i] = individuals[father1].jobsOrder[i];
    individuals[countInd + 1].jobsOrder[i] = individuals[father2].jobsOrder[i];
  }
  
  //Primeiro filho
  /*Copia metade dos genes do segundo pai (na ordem que se apresenta 
                                    sem repetir os genes que o filho já tem)*/
  for(i = 0; i < nJobs; i++)
  {
    //Verifica se o gene já existe
    gver = 0;
    for (j = 0; j < id; j++)
    {
      if (individuals[father2].jobsOrder[i] == 
                                            individuals[countInd].jobsOrder[j])
      {
        gver++;
      }
    }
    if (gver == 0)
    {
      individuals[countInd].jobsOrder[id] = individuals[father2].jobsOrder[i];
      id++;
    }
  }
  //Segundo filho
  /*Copia metade dos genes do segundo pai (na ordem que se apresenta 
                                    sem repetir os genes que o filho já tem)*/
  for(i = 0; i < nJobs; i++)
  {
    //Verifica se o gene já existe
    gver = 0;
    for (j = 0; j < id2; j++)
    {
      if (individuals[father1].jobsOrder[i] == 
                                        individuals[countInd + 1].jobsOrder[j])
      {
        gver++;
      }
    }
    if (gver == 0)
    {
      individuals[countInd + 1].jobsOrder[id2] = 
                                              individuals[father1].jobsOrder[i];
      id2++;
    }
  }
} //opCrossover

/*Realiza a cópia um pedaço do Pai1 e 
                                completa com os genes do Pai2 mantendo a ordem*/
void LOXcrossover(int countInd, int father1, int father2)
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
      individuals[countInd].jobsOrder[firstCromossome + i] = 
                            individuals[father1].jobsOrder[firstCromossome + i];
      individuals[countInd + 1].jobsOrder[firstCromossome + i] = 
                            individuals[father2].jobsOrder[firstCromossome + i];
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
      if(individuals[father2].jobsOrder[i] == individuals[countInd].jobsOrder[j])
      {
        gver++;
      }
    }
    if(gver == 0)
    {
      while(individuals[countInd].jobsOrder[id] != -1)
      {
        id++;
      }
      individuals[countInd].jobsOrder[id] = individuals[father2].jobsOrder[i];
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
      if(individuals[father1].jobsOrder[i] == 
                                        individuals[countInd + 1].jobsOrder[j])
      {
        gver++;
      }
    }
    if(gver == 0)
    {
      while(individuals[countInd + 1].jobsOrder[id] != -1)
      {
        id++;
      }
      individuals[countInd + 1].jobsOrder[id] = 
                                              individuals[father1].jobsOrder[i];
    }
  }
} //LOXcrossover

// Crossover - Cruzamento Parcialmente Mapeado
void PMXcrossover(int countInd, int father1, int father2)
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
      individuals[countInd].jobsOrder[firstCromossome + i] = 
                            individuals[father1].jobsOrder[firstCromossome + i];
      individuals[countInd + 1].jobsOrder[firstCromossome + i] = 
                            individuals[father2].jobsOrder[firstCromossome + i];
    }
  }
  
  //Finaliza primeiro filho
  for(i = 0; i <= interval; i++)
  {
    vaux1 = 0;
    //Verifica se o gene já foi copiado para o filho
    for(j = 0; j <= interval; j++)
    {
      if(individuals[father2].jobsOrder[firstCromossome + i] == 
                            individuals[father1].jobsOrder[firstCromossome + j])
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
        if(individuals[father1].jobsOrder[firstCromossome + i] == 
                                              individuals[father2].jobsOrder[j])
        {
          if(individuals[countInd].jobsOrder[j] == (-1))
          {
            individuals[countInd].jobsOrder[j] = 
                            individuals[father2].jobsOrder[firstCromossome + i];
          } else
          {
            permutation(firstCromossome, i, countInd, father1, father2, j);
            break;
          }
        }
      }
    }
  }
  // Copia o restante dos genes do father2
  for(i = 0; i < nJobs; i++)
  {
    if(individuals[countInd].jobsOrder[i] == (-1))
    {
      individuals[countInd].jobsOrder[i] = individuals[father2].jobsOrder[i];
    }
  }
  
  //Finaliza segundo filho
  for(i = 0; i <= interval; i++)
  {
    vaux1 = 0;
    //Verifica se o gene já foi copiado para o filho
    for(j = 0; j <= interval; j++)
    {
      if(individuals[father1].jobsOrder[firstCromossome + i] == 
                            individuals[father2].jobsOrder[firstCromossome + j])
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
        if(individuals[father2].jobsOrder[firstCromossome + i] == 
                                            individuals[father1].jobsOrder[j])
        {
          if(individuals[countInd + 1].jobsOrder[j] == (-1))
          {
            individuals[countInd + 1].jobsOrder[j] = 
                          individuals[father1].jobsOrder[firstCromossome + i];
          } else
          {
            permutation(firstCromossome, i, countInd + 1, father2, father1, j);
            break;
          }
        }
      }
    }
  }
  // Copia o restante dos genes do father2
  for(i = 0; i < nJobs; i++)
  {
    if(individuals[countInd + 1].jobsOrder[i] == (-1))
    {
      individuals[countInd + 1].jobsOrder[i] = 
                                              individuals[father1].jobsOrder[i];
    }
  }
} // PMXcrossover

void permutation(int firstCromossome, int i, int countInd, 
                                                int father1, int father2, int j)
{
  int k;
  for(k = 0; k < nJobs; k++)
  {
    if(individuals[father1].jobsOrder[j] == individuals[father2].jobsOrder[k])
    {
      if(individuals[countInd].jobsOrder[k] == (-1))
      {
        individuals[countInd].jobsOrder[k] = 
                            individuals[father2].jobsOrder[firstCromossome + i];
      }
      else
      {
        permutation(firstCromossome, i, countInd, father1, father2, k);
        break;
      }
    }
  }
} //permutation