#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "crossover.h"
#include "mutation.h"
#include "geneticalgorithm.h"
#ifdef AP
  #include "adaptivepursuit.h"
#endif
#ifdef PPC
  #include "predictiveparametercontrol.h"
#endif
#ifdef PPCR
  #include "predictiveparametercontrol.h"
#endif
#ifdef NEH
  #include "neh.h"
#endif

void GeneticAlgorithm(char *fileName, char *nRepeat, char pop[])
{
  int i, countGen = 0, father1 = 0, father2 = 0, countInd;
  double avgFitness, x; //Número para o sorteio

  #ifdef PPC
    for(i = 0; i < 3; i++)
    {
      crossRateControl[i].success = malloc(numGeneration * sizeof(double));
      mut1RateControl[i].success = malloc(numGeneration * sizeof(double));
      mut2RateControl[i].success = malloc(numGeneration * sizeof(double));
      crossOperatorControl[i].success = malloc(numGeneration * sizeof(double));
    }
    for(i = 0; i < 2; i++)
    {
      mut1OperatorControl[i].success = malloc(numGeneration * sizeof(double));
      mut2OperatorControl[i].success = malloc(numGeneration * sizeof(double));
    }
  #endif

  #ifdef PPCR
    for(i = 0; i < 3; i++)
    {
      crossRateControl[i].success = malloc(numGeneration * sizeof(double));
      mut1RateControl[i].success = malloc(numGeneration * sizeof(double));
      mut2RateControl[i].success = malloc(numGeneration * sizeof(double));
      crossOperatorControl[i].success = malloc(numGeneration * sizeof(double));
    }
    for(i = 0; i < 2; i++)
    {
      mut1OperatorControl[i].success = malloc(numGeneration * sizeof(double));
      mut2OperatorControl[i].success = malloc(numGeneration * sizeof(double));
    }
  #endif

  initializeIndividuals();

  #ifdef AP
    APInitializeProbabilities(3, crossOperatorControl);
    APInitializeProbabilities(2, mut1OperatorControl);
    APInitializeProbabilities(2, mut2OperatorControl);
    APInitializeProbabilities(3, crossRateControl);
    APInitializeProbabilities(3, mut1RateControl);
    APInitializeProbabilities(3, mut2RateControl);
  #endif

  //Inicializa o Predictive Parameter Control
  #ifdef PPC
    PPCInitializeProbabilities(3, crossOperatorControl);
    PPCInitializeProbabilities(2, mut1OperatorControl);
    PPCInitializeProbabilities(2, mut2OperatorControl);
    PPCInitializeProbabilities(3, crossRateControl);
    PPCInitializeProbabilities(3, mut1RateControl);
    PPCInitializeProbabilities(3, mut2RateControl);
  #endif

  #ifdef PPCR
    PPCInitializeProbabilities(3, crossOperatorControl);
    PPCInitializeProbabilities(2, mut1OperatorControl);
    PPCInitializeProbabilities(2, mut2OperatorControl);
    PPCInitializeProbabilities(3, crossRateControl);
    PPCInitializeProbabilities(3, mut1RateControl);
    PPCInitializeProbabilities(3, mut2RateControl);
  #endif

  // Laço que calcula o fitness do pai para as duas funções objetivo
  #ifdef OBR
    for(i = 0; i < popLenght; i++)
      fitnessEvaluationOBR(i);
  #else
    for(i = 0; i < popLenght; i++)
      fitnessEvaluationJBR(i);
  #endif


  //Laço principal do AG
  while(countGen < numGeneration)
  {
    initializeGeneration();
    countInd = popLenght;

    for(i = 0; i < popLenght; i++)
      avgFitness += (double)individuals[i].fitMakespan;
    avgFitness = avgFitness / popLenght;
    printf("%lf\n", avgFitness);

    #ifdef PPC
      //Inicializa os contadores de ocorrência e successo
      initializeCounters(3, crossOperatorControl);
      initializeCounters(2, mut1OperatorControl);
      initializeCounters(2, mut2OperatorControl);
      initializeCounters(3, crossRateControl);
      initializeCounters(3, crossRateControl);
      initializeCounters(3, mut2RateControl);
    #endif

    #ifdef PPCR
      //Inicializa os contadores de ocorrência e successo
      initializeCounters(3, crossOperatorControl);
      initializeCounters(2, mut1OperatorControl);
      initializeCounters(2, mut2OperatorControl);
      initializeCounters(3, crossRateControl);
      initializeCounters(3, crossRateControl);
      initializeCounters(3, mut2RateControl);
    #endif

    while(countInd < (2 * popLenght))
    {
      // Seleção dos pais
      tournament(&father1, &father2);

      // Cálculo da chance de realizar crossover
      #ifndef GA
        probCrossover = crossoverRateSelection();
      #endif

      #ifdef PPCR
        if(probCrossover == 0.7)
          probCrossover = ((double)rand() / (double)(RAND_MAX)) * 0.0667 + 0.7;
        if(probCrossover == 0.8)
          probCrossover = ((double)rand() / (double)(RAND_MAX)) * 0.1334 + 0.7;
        if(probCrossover == 0.9)
          probCrossover = ((double)rand() / (double)(RAND_MAX)) * 0.2000 + 0.7;
      #endif

      x = (double)((double)rand() / (double)RAND_MAX);
      if(x < probCrossover)
      {
        #ifdef GA
          PMXcrossover(countInd, father1, father2);
        #else
          #ifdef PPCR
            if(probCrossover < 0.7667)
            {
              crossRateControl[0].index[1]++;
            }
            else if(probCrossover < 0.8334)
            {
              crossRateControl[1].index[1]++;
            }
            else if(probCrossover < 0.9)
            {
              crossRateControl[2].index[1]++;
            }
          #else
            if(probCrossover == 0.7)
            {
              crossRateControl[0].index[1]++;
            }
            else if(probCrossover == 0.8)
            {
              crossRateControl[1].index[1]++;
            }
            else if(probCrossover == 0.9)
            {
              crossRateControl[2].index[1]++;
            }
          #endif
          crossoverOperatorSelection(countInd, father1, father2);
        #endif
      }else
      {
        for(i = 0; i < nGenes; i++)
        {
          individuals[countInd].jobsOrder[i] =
                                              individuals[father1].jobsOrder[i];
          individuals[countInd + 1].jobsOrder[i] =
                                              individuals[father2].jobsOrder[i];
        }
      }

      // Chance de realizar mutação
      #ifndef GA
        probMutation = mutaRateSelection(3, mut1RateControl);
      #endif

      #ifdef PPCR
        if(probMutation == 0.3)
          probMutation = ((double)rand() / (double)(RAND_MAX)) * 0.0667 + 0.3;
        if(probMutation == 0.4)
          probMutation = ((double)rand() / (double)(RAND_MAX)) * 0.1334 + 0.3;
        if(probMutation == 0.5)
          probMutation = ((double)rand() / (double)(RAND_MAX)) * 0.2000 + 0.3;
      #endif

      x = (double)((double)rand() / (double)RAND_MAX);
      if(x < probMutation)
      {
        #ifdef GA
          shiftMutation(countInd);
        #else
          #ifdef PPCR
            if(probMutation < 0.3667)
              mut1RateControl[0].index[1]++;
            if(probMutation < 0.4334)
              mut1RateControl[1].index[1]++;
            if(probMutation < 0.5)
              mut1RateControl[2].index[1]++;
          #else
            if(probMutation == 0.3)
              mut1RateControl[0].index[1]++;
            if(probMutation == 0.4)
              mut1RateControl[1].index[1]++;
            if(probMutation == 0.5)
              mut1RateControl[2].index[1]++;
          #endif
          mutaOperatorSelection(countInd, 2, mut1OperatorControl);
        #endif
      }

      // Chance de realizar mutação
      #ifndef GA
        probMutation = mutaRateSelection(3, mut2RateControl);
      #endif

      #ifdef PPCR
        if(probMutation == 0.3)
          probMutation = ((double)rand() / (double)(RAND_MAX)) * 0.0667 + 0.3;
        if(probMutation == 0.4)
          probMutation = ((double)rand() / (double)(RAND_MAX)) * 0.1334 + 0.3;
        if(probMutation == 0.5)
          probMutation = ((double)rand() / (double)(RAND_MAX)) * 0.2000 + 0.3;
      #endif

      x = (double)((double)rand() / (double)RAND_MAX);
      if(x < probMutation)
      {
        #ifdef GA
          shiftMutation(countInd + 1);
        #else
          #ifdef PPCR
            if(probMutation < 0.3667)
              mut2RateControl[0].index[1]++;
            if(probMutation < 0.4334)
              mut2RateControl[1].index[1]++;
            if(probMutation < 0.5)
              mut2RateControl[2].index[1]++;
          #else
            if(probMutation == 0.3)
              mut2RateControl[0].index[1]++;
            if(probMutation == 0.4)
              mut2RateControl[1].index[1]++;
            if(probMutation == 0.5)
              mut2RateControl[2].index[1]++;
          #endif
          mutaOperatorSelection(countInd + 1, 2, mut2OperatorControl);
        #endif
      }

      //Avalia os filhos geradas
      #ifdef OBR
        fitnessEvaluationOBR(countInd);
        fitnessEvaluationOBR(countInd + 1);
      #else
        fitnessEvaluationJBR(countInd);
        fitnessEvaluationJBR(countInd + 1);
      #endif

      //Adaptação da probabildiade das taxas de crossover
      #ifdef AP
        crossoverAdaptivePursuit(countInd, father1, father2, 3,crossOperatorControl);
        mutationFinalAdaptivePursuit(countInd, father1, father2, 2, mut1OperatorControl, avgFitness);
        mutationFinalAdaptivePursuit(countInd + 1, father1, father2, 2, mut2OperatorControl, avgFitness);
        crossoverAdaptivePursuit(countInd, father1, father2, 3,crossRateControl);
        mutationFinalAdaptivePursuit(countInd, father1, father2, 3, mut1RateControl, avgFitness);
        mutationFinalAdaptivePursuit(countInd + 1, father1, father2, 3, mut2RateControl, avgFitness);
      #endif

      #ifdef PPC
        crossoverSuccessEvaluation(countInd, father1, father2, 3, crossOperatorControl);
        mutationSuccessEvaluation(countInd, father1, father2, 2, mut1OperatorControl);
        mutationSuccessEvaluation(countInd + 1, father1, father2, 2, mut2OperatorControl);
        crossoverSuccessEvaluation(countInd, father1, father2, 3, crossRateControl);
        mutationSuccessEvaluation(countInd, father1, father2, 3, mut1RateControl);
        mutationSuccessEvaluation(countInd + 1, father1, father2, 3, mut2RateControl);
      #endif

      #ifdef PPCR
        crossoverSuccessEvaluation(countInd, father1, father2, 3, crossOperatorControl);
        mutationSuccessEvaluation(countInd, father1, father2, 2, mut1OperatorControl);
        mutationSuccessEvaluation(countInd + 1, father1, father2, 2, mut2OperatorControl);
        crossoverSuccessEvaluation(countInd, father1, father2, 3, crossRateControl);
        mutationSuccessEvaluation(countInd, father1, father2, 3, mut1RateControl);
        mutationSuccessEvaluation(countInd + 1, father1, father2, 3, mut2RateControl);
      #endif

      countInd = countInd + 2;
    } // while(countInd < (2 * popLenght))

    #ifdef PPC
      UpdateSuccessIndicator(countGen, 3, crossOperatorControl);
      UpdateSuccessIndicator(countGen, 2, mut1OperatorControl);
      UpdateSuccessIndicator(countGen, 2, mut2OperatorControl);
      UpdateSuccessIndicator(countGen, 3, crossRateControl);
      UpdateSuccessIndicator(countGen, 3, mut1RateControl);
      UpdateSuccessIndicator(countGen, 3, mut2RateControl);
    #endif

    #ifdef PPCR
      UpdateSuccessIndicator(countGen, 3, crossOperatorControl);
      UpdateSuccessIndicator(countGen, 2, mut1OperatorControl);
      UpdateSuccessIndicator(countGen, 2, mut2OperatorControl);
      UpdateSuccessIndicator(countGen, 3, crossRateControl);
      UpdateSuccessIndicator(countGen, 3, mut1RateControl);
      UpdateSuccessIndicator(countGen, 3, mut2RateControl);
    #endif

    #ifndef GA
      printProbabilities(countGen, fileName, nRepeat, pop);
    #endif

    qsort(individuals, popLenght, sizeof(ind), sortFitMakespan);

    selection();

    printBestIndividual(fileName, nRepeat, countGen);

    countGen++;
  } // while(countGen < numGeneration)

  #ifdef PPC
    for(i = 0; i < 3; i++)
    {
      free(crossRateControl[i].success);
      free(mut1RateControl[i].success);
      free(mut2RateControl[i].success);
      free(crossOperatorControl[i].success);
    }
    for(i = 0; i < 2; i++)
    {
      free(mut1OperatorControl[i].success);
      free(mut2OperatorControl[i].success);
    }
  #endif

  #ifdef PPCR
    for(i = 0; i < 3; i++)
    {
      free(crossRateControl[i].success);
      free(mut1RateControl[i].success);
      free(mut2RateControl[i].success);
      free(crossOperatorControl[i].success);
    }
    for(i = 0; i < 2; i++)
    {
      free(mut1OperatorControl[i].success);
      free(mut2OperatorControl[i].success);
    }
  #endif
} // AGinterno

void initializeIndividuals()
{
  int i, j, r, temp;
  double proportionNEH = 0.0;
  int pNEH = (int)(proportionNEH * popLenght);

  #ifdef NEH
    proportionNEH = 0.2; //Altera a proporção de indivíduos que serão iniciados com o procedimento
    pNEH = (int)(proportionNEH * popLenght);

    ind NEHindividual;
    NEHindividual.jobsOrder = (int*)malloc(nGenes * sizeof(int));
    executeNEH(NEHindividual);

    for(i = 0; i < pNEH; i++)
    {
      for(j = 0; j < nGenes; j++)
        individuals[i].jobsOrder[j] = NEHindividual.jobsOrder[j];
      individuals[i].fitMakespan = NEHindividual.fitMakespan;
    }

    //libera memória
    free(NEHindividual.jobsOrder);
  #endif

  for (i = pNEH; i < popLenght; i++)
  {
    // Gera o indivíduo
    for (j = 0; j < nGenes; j++)
      individuals[i].jobsOrder[j] = j;
    // Realiza o embaralhamento
    for (j = 0; j < nGenes; j++)
    {
      r = rand() % nGenes;
      temp = individuals[i].jobsOrder[j];
      individuals[i].jobsOrder[j] = individuals[i].jobsOrder[r];
      individuals[i].jobsOrder[r] = temp;
    }
  }
} //initializeIndividuals

void fitnessEvaluationOBR(int w)
{
  int i, j, k, aux[3], flag[3], timeGap, bestAllocation;
  int *jobFinishTime;
  int *machineAux = (int*)calloc(nJobs, sizeof(int));

  startList();

  jobFinishTime = (int*)calloc(nJobs, sizeof(int));

  for(i = 0; i < nGenes; i++)
  {
    j = (int)floor((double)individuals[w].jobsOrder[i]/(double)nJobs);

    for(k = 0; k < qMachines[jobMachine[j][machineAux[j]][0]]; k++)
    {
      if(Gantt[jobMachine[j][machineAux[j]][0]][k]->prox == NULL)
      {
        aux[k] = jobFinishTime[j];
        flag[k] = 1;
      }else
      {
        tmp[k][0] = Gantt[jobMachine[j][machineAux[j]][0]][k];
        tmp[k][1] = Gantt[jobMachine[j][machineAux[j]][0]][k]->prox;
        if((*tmp[k][1]).elem.startTime >
                    (jobFinishTime[j] + jobMachine[j][machineAux[j]][1]))
        {
          aux[k] = jobFinishTime[j];
          flag[k] = 2;
        }else
        {
          while(1)
          {
            tmp[k][0] = tmp[k][1];
            tmp[k][1] = tmp[k][1]->prox;
            if(tmp[k][1] == NULL)
            {
              if(jobFinishTime[j] > (*tmp[k][0]).elem.endTime)
              {
                aux[k] = jobFinishTime[j];
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
              if(jobFinishTime[j] > (*tmp[k][0]).elem.endTime)
              {
                timeGap = (*tmp[k][1]).elem.startTime - jobFinishTime[j];
                if(timeGap > jobMachine[j][machineAux[j]][1])
                {
                  aux[k] = jobFinishTime[j];
                  flag[k] = 5;
                  break;
                }
              }
              else
              {
                timeGap = (*tmp[k][1]).elem.startTime - (*tmp[k][0]).elem.endTime;
                if(timeGap > jobMachine[j][machineAux[j]][1])
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
    for(k = 0; k < qMachines[jobMachine[j][machineAux[j]][0]]; k++)
    {
      if(aux[k] < aux[bestAllocation])
        bestAllocation = k;
    }

    //aloca a tarefa
    switch(flag[bestAllocation])
    {
      case 1:
        (*operation[i]).elem.startTime = jobFinishTime[j];
        (*operation[i]).elem.endTime = (*operation[i]).elem.startTime
                              + jobMachine[j][machineAux[j]][1];
        (*operation[i]).elem.jobId = j + 1;
        operation[i]->prox = NULL;
        Gantt[jobMachine[j][machineAux[j]][0]][bestAllocation]->prox =
                                                    operation[i];
        jobFinishTime[j] = (*operation[i]).elem.endTime;
      break;
      case 2:
        (*operation[i]).elem.startTime = jobFinishTime[j];
        (*operation[i]).elem.endTime = (*operation[i]).elem.startTime
                            + jobMachine[j][machineAux[j]][1];
        (*operation[i]).elem.jobId = j + 1;
        operation[i]->prox = tmp[bestAllocation][1];
        Gantt[jobMachine[j][machineAux[j]][0]][bestAllocation]->prox =
                                                  operation[i];
        jobFinishTime[j] = (*operation[i]).elem.endTime;
      break;
      case 3:
        (*operation[i]).elem.startTime = jobFinishTime[j];
        (*operation[i]).elem.endTime = (*operation[i]).elem.startTime
                               + jobMachine[j][machineAux[j]][1];
        (*operation[i]).elem.jobId = j + 1;
        operation[i]->prox = NULL;
        tmp[bestAllocation][0]->prox = operation[i];
        jobFinishTime[j] = (*operation[i]).elem.endTime;
      break;
      case 4:
        (*operation[i]).elem.startTime = (*tmp[bestAllocation][0]).elem.endTime;
        (*operation[i]).elem.endTime = (*operation[i]).elem.startTime
                            + jobMachine[j][machineAux[j]][1];
        (*operation[i]).elem.jobId = j + 1;
        operation[i]->prox = NULL;
        tmp[bestAllocation][0]->prox = operation[i];
        jobFinishTime[j] = (*operation[i]).elem.endTime;
      break;
      case 5:
        (*operation[i]).elem.startTime = jobFinishTime[j];
        (*operation[i]).elem.endTime = (*operation[i]).elem.startTime
                          + jobMachine[j][machineAux[j]][1];
        (*operation[i]).elem.jobId = j + 1;
        operation[i]->prox = tmp[bestAllocation][1];
        tmp[bestAllocation][0]->prox = operation[i];
        jobFinishTime[j] = (*operation[i]).elem.endTime;
      break;
      case 6:
        (*operation[i]).elem.startTime = (*tmp[bestAllocation][0]).elem.endTime;
        (*operation[i]).elem.endTime = (*operation[i]).elem.startTime
                          + jobMachine[j][machineAux[j]][1];
        (*operation[i]).elem.jobId = j + 1;
        operation[i]->prox = tmp[bestAllocation][1];
        tmp[bestAllocation][0]->prox = operation[i];
        jobFinishTime[j] = (*operation[i]).elem.endTime;
      break;
    }
    // Procura o término das operações
    jobFinishTime[j] = (*operation[i]).elem.endTime;

    machineAux[j]++;
  }

  individuals[w].fitMakespan = fitnessMakespan(nJobs, jobFinishTime);

  free(jobFinishTime);
  free(machineAux);
}//createGantt

void fitnessEvaluationJBR(int w)
{
  int i, j, k, aux[3], flag[3], timeGap, timeAux, bestAllocation;
  int *jobFinishTime;
  // node ***tmp;

  startList();

  jobFinishTime = (int*)calloc(nJobs, sizeof(int));

  for(i = 0; i < nGenes; i++)
  {
    timeAux = 0;
    for(j = 0; j < nMachines; j++)
    {
      for(k = 0; k < qMachines[jobMachine[individuals[w].jobsOrder[i]][j][0]]; k++)
      {
        if(Gantt[jobMachine[individuals[w].jobsOrder[i]][j][0]][k]->prox == NULL)
        {
          aux[k] = timeAux;
          flag[k] = 1;
        }else
        {
          tmp[k][0] = Gantt[jobMachine[individuals[w].jobsOrder[i]][j][0]][k];
          tmp[k][1] = Gantt[jobMachine[individuals[w].jobsOrder[i]][j][0]][k]->prox;
          if((*tmp[k][1]).elem.startTime >
                      (timeAux + jobMachine[individuals[w].jobsOrder[i]][j][1]))
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
                  if(timeGap > jobMachine[individuals[w].jobsOrder[i]][j][1])
                  {
                    aux[k] = timeAux;
                    flag[k] = 5;
                    break;
                  }
                }
                else
                {
                  timeGap = (*tmp[k][1]).elem.startTime - (*tmp[k][0]).elem.endTime;
                  if(timeGap > jobMachine[individuals[w].jobsOrder[i]][j][1])
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
      for(k = 0; k < qMachines[jobMachine[individuals[w].jobsOrder[i]][j][0]]; k++)
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
                                + jobMachine[individuals[w].jobsOrder[i]][j][1];
          (*operation[i*nMachines + j]).elem.jobId = individuals[w].jobsOrder[i] + 1;
          operation[i*nMachines + j]->prox = NULL;
          Gantt[jobMachine[individuals[w].jobsOrder[i]][j][0]][bestAllocation]->prox =
                                                      operation[i*nMachines + j];
          timeAux = (*operation[i*nMachines + j]).elem.endTime;
          break;
        case 2:
          (*operation[i*nMachines + j]).elem.startTime = timeAux;
          (*operation[i*nMachines + j]).elem.endTime = (*operation[i*nMachines + j]).elem.startTime
                              + jobMachine[individuals[w].jobsOrder[i]][j][1];
          (*operation[i*nMachines + j]).elem.jobId = individuals[w].jobsOrder[i] + 1;
          operation[i*nMachines + j]->prox = tmp[bestAllocation][1];
          Gantt[jobMachine[individuals[w].jobsOrder[i]][j][0]][bestAllocation]->prox =
                                                    operation[i*nMachines + j];
          timeAux = (*operation[i*nMachines + j]).elem.endTime;
          break;
        case 3:
          (*operation[i*nMachines + j]).elem.startTime = timeAux;
          (*operation[i*nMachines + j]).elem.endTime = (*operation[i*nMachines + j]).elem.startTime
                                 + jobMachine[individuals[w].jobsOrder[i]][j][1];
          (*operation[i*nMachines + j]).elem.jobId = individuals[w].jobsOrder[i] + 1;
          operation[i*nMachines + j]->prox = NULL;
          tmp[bestAllocation][0]->prox = operation[i*nMachines + j];
          timeAux = (*operation[i*nMachines + j]).elem.endTime;
          break;
        case 4:
          (*operation[i*nMachines + j]).elem.startTime = (*tmp[bestAllocation][0]).elem.endTime;
          (*operation[i*nMachines + j]).elem.endTime = (*operation[i*nMachines + j]).elem.startTime
                              + jobMachine[individuals[w].jobsOrder[i]][j][1];
          (*operation[i*nMachines + j]).elem.jobId = individuals[w].jobsOrder[i] + 1;
          operation[i*nMachines + j]->prox = NULL;
          tmp[bestAllocation][0]->prox = operation[i*nMachines + j];
          timeAux = (*operation[i*nMachines + j]).elem.endTime;
          break;
        case 5:
          (*operation[i*nMachines + j]).elem.startTime = timeAux;
          (*operation[i*nMachines + j]).elem.endTime = (*operation[i*nMachines + j]).elem.startTime
                            + jobMachine[individuals[w].jobsOrder[i]][j][1];
          (*operation[i*nMachines + j]).elem.jobId = individuals[w].jobsOrder[i] + 1;
          operation[i*nMachines + j]->prox = tmp[bestAllocation][1];
          tmp[bestAllocation][0]->prox = operation[i*nMachines + j];
          timeAux = (*operation[i*nMachines + j]).elem.endTime;
          break;
        case 6:
          (*operation[i*nMachines + j]).elem.startTime = (*tmp[bestAllocation][0]).elem.endTime;
          (*operation[i*nMachines + j]).elem.endTime = (*operation[i*nMachines + j]).elem.startTime
                            + jobMachine[individuals[w].jobsOrder[i]][j][1];
          (*operation[i*nMachines + j]).elem.jobId = individuals[w].jobsOrder[i] + 1;
          operation[i*nMachines + j]->prox = tmp[bestAllocation][1];
          tmp[bestAllocation][0]->prox = operation[i*nMachines + j];
          timeAux = (*operation[i*nMachines + j]).elem.endTime;
          break;
      }
    }
    // Procura o término das operações
    jobFinishTime[individuals[w].jobsOrder[i]] =
                                    (*operation[i*nMachines + nMachines - 1]).elem.endTime;
  }

  individuals[w].fitMakespan = fitnessMakespan(nJobs, jobFinishTime);

  free(jobFinishTime);
}//fitnessEvaluationJBR

// Calcula o makespan dos indivíduos
int fitnessMakespan(int numJobs, int jobFinishTime[numJobs])
{
  int i, makespan, makespanId;
  makespanId = 0;
  for(i = 1; i < numJobs; i++)
    if(jobFinishTime[makespanId] < jobFinishTime[i])
      makespanId = i;
  makespan = jobFinishTime[makespanId];
  return makespan;
} // fitnessMakespan

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
  return (a2->fitMakespan - b2->fitMakespan);
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
  for(i = popLenght; i < (2*popLenght); i++)
    for(j = 0; j < nGenes; j++)
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
    if(individuals[candidateFather[i]].fitMakespan < individuals[(*father1)].fitMakespan)
      (*father1) = candidateFather[i];
  }

  //Seleciona o segundo pai
  for(i = 0; i < nCandidate; i++)
  {
    candidateFather[i] = rand() % popLenght;
  }
  (*father2) = candidateFather[0];
  for(i = 1; i < nCandidate; i++)
  {
    if(individuals[candidateFather[i]].fitMakespan < individuals[(*father2)].fitMakespan)
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

// Função para calcular o hypervolume
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

void printProbabilities(int countInd, char *fileName, char *nRepeat, char pop[])
{
  char *outputName =
        (char*)malloc((strlen(fileName) + strlen(nRepeat) +
                                                strlen(pop) + 19)*sizeof(char));
  strcpy(outputName, fileName);
  strcat(outputName, "_prob_");
  strcat(outputName, nRepeat);
  strcat(outputName, "_");
  strcat(outputName, pop);
  strcat(outputName, "_output.txt");
  FILE *output;
  output = fopen(outputName, "a");
  if (output == NULL)
  {
    printf("Arquivo não encontrado!\n");
    exit(1);
  }

  fprintf(output, "%d \t", countInd);
  fprintf(output, "%lf \t %lf \t %lf \t %lf \t %lf \t %lf \t",
    crossOperatorControl[0].prob, crossOperatorControl[1].prob, crossOperatorControl[2].prob,
      crossRateControl[0].prob, crossRateControl[1].prob, crossRateControl[2].prob);
  fprintf(output, "%lf \t %lf \t %lf \t %lf \t %lf \t",
    mut1OperatorControl[0].prob, mut1OperatorControl[1].prob,
      mut1RateControl[0].prob, mut1RateControl[1].prob, mut1RateControl[2].prob);
  fprintf(output, "%lf \t %lf \t %lf \t %lf \t %lf\n",
    mut2OperatorControl[0].prob, mut2OperatorControl[1].prob,
      mut2RateControl[0].prob, mut2RateControl[1].prob, mut2RateControl[2].prob);

  fclose(output);
  free(outputName);
} //printProbabilities

void printBestIndividual(char *fileName, char *nRepeat, int countGen)
{
  char *outputName =
        (char*)malloc((strlen(fileName) + 10)*sizeof(char));
  strcpy(outputName, fileName);
  strcat(outputName, "_best.txt");
  FILE *output;
  output = fopen(outputName , "a");
  if (output == NULL)
  {
    printf("Arquivo não encontrado!\n");
    exit(1);
  }

  fprintf(output, "%d \t %d\n", countGen, individuals[0].fitMakespan);

  fclose(output);
  free(outputName);

} //printBestIndividual

void selection()
{
  int i, j;
  for(i = popLenght; i < 2*popLenght; i++)
  {
    for(j = 0; j < nGenes; j++)
      selectedIndividuals[i - popLenght].jobsOrder[j] = individuals[i].jobsOrder[j];
    selectedIndividuals[i - popLenght].fitMakespan = individuals[i].fitMakespan;
  }
  qsort(selectedIndividuals, popLenght, sizeof(ind), sortFitMakespan);
  for(i = 1; i < popLenght; i++)
  {
    for(j = 0; j < nGenes; j++)
      individuals[i].jobsOrder[j] = selectedIndividuals[i].jobsOrder[j];
    individuals[i].fitMakespan = selectedIndividuals[i].fitMakespan;
  }
}

void selectionWithTournament()
{
  int i, j, x;

  for(i = 0; i < popLenght; i++)
  {
    x = selectionTournament();
    for(j = 0; j < nJobs; j++)
      selectedIndividuals[i].jobsOrder[j] = individuals[x].jobsOrder[j];
    selectedIndividuals[i].fitMakespan = individuals[x].fitMakespan;
  }

  for(i = 0; i < popLenght; i++)
  {
    for(j = 0; j < nJobs; j++)
      individuals[i].jobsOrder[j] = selectedIndividuals[i].jobsOrder[j];
    individuals[i].fitMakespan = selectedIndividuals[i].fitMakespan;
  }

} //selection

int selectionTournament()
{
  int x, y;
  x = rand() % (2*popLenght);
  y = rand() % (2*popLenght);
  if(individuals[x].fitMakespan <= individuals[y].fitMakespan)
    return x;
  else
    return y;
} //selectionTournament

void startList()
{
  int i, j;
  for(i = 0; i < nMachines; i++)
    for(j = 0; j < qMachines[i]; j++)
      Gantt[i][j]->prox = NULL;
  for(i = 0; i < nJobs; i++)
    for(j = 0; j < nMachines; j++)
      operation[j*nMachines + j]->prox = NULL;
}




void crossoverOperatorSelection(int countInd, int father1, int father2)
{
	double x;
	x = (double)((double)rand() / (double)RAND_MAX);
	if(x < crossOperatorControl[0].prob)
	{
		opCrossover(countInd, father1, father2);
		crossOperatorControl[0].index[1]++;
	}
	else if(x < (crossOperatorControl[1].prob + crossOperatorControl[0].prob))
	{
		LOXcrossover(countInd, father1, father2);
		crossOperatorControl[1].index[1]++;
	}
	else
	{
		PMXcrossover(countInd, father1, father2);
		crossOperatorControl[2].index[1]++;
	}
} //crossoverOperatorSelection

void mutaOperatorSelection(int countInd, int size, ProbabilitiesControl probControl[size])
{
	double x;
	x = (double)((double)rand() / (double)RAND_MAX);
	if(x < probControl[0].prob)
	{
		shiftMutation(countInd);
		probControl[0].index[1]++;
	}
	else
	{
		interchangeMutation(countInd);
		probControl[1].index[1]++;
	}
} //mutationOperatorSelection

double crossoverRateSelection()
{
	double x;
	x = (double)((double)rand() / (double)RAND_MAX);
	if(x < crossRateControl[0].prob)
	{
		return 0.7;
	}
	else if(x < (crossRateControl[1].prob + crossRateControl[0].prob))
	{
		return 0.8;
	}
	else
	{
		return 0.9;
	}
} //crossoverRateSelection

double mutaRateSelection(int size, ProbabilitiesControl probControl[size])
{
	double x;
	x = (double)((double)rand() / (double)RAND_MAX);
	if(x < probControl[0].prob)
	{
		return 0.3;
	}
	else if(x < (probControl[1].prob + probControl[0].prob))
	{
		return 0.4;
	}
	else
	{
		return 0.5;
	}
} //mutaRateSelection
