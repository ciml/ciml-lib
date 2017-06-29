#include <stdio.h>
#include <stdlib.h>
#include "crossover.h"
#include "mutation.h"
#include "geneticalgorithm.h"
#include "crossover.h"
#include "mutation.h"
#include "adaptivepreprocessors.h"

#ifdef AP
  #include "adaptivepursuit.h"
#endif

#ifdef PPC
  #include "predictiveparametercontrol.h"
#endif

#ifdef PPCR
  #include "predictiveparametercontrol.h"
#endif

#ifdef IG
  #include "localsearch.h"
#endif

void alocateMemoryPPC()
{
  #ifdef PPC
    int i;
    for(i = 0; i < 3; i++)
    {
      crossRateControl[i].success = malloc(numGeneration * sizeof(double));
      mut1RateControl[i].success = malloc(numGeneration * sizeof(double));
      mut2RateControl[i].success = malloc(numGeneration * sizeof(double));
      #ifdef IG
        localSearch1RateControl[i].success = malloc(numGeneration * sizeof(double));
        localSearch2RateControl[i].success = malloc(numGeneration * sizeof(double));
      #endif
      crossOperatorControl[i].success = malloc(numGeneration * sizeof(double));
    }
    for(i = 0; i < 2; i++)
    {
      mut1OperatorControl[i].success = malloc(numGeneration * sizeof(double));
      mut2OperatorControl[i].success = malloc(numGeneration * sizeof(double));
    }
  #endif

  #ifdef PPCR
    int i;
    for(i = 0; i < 3; i++)
    {
      crossRateControl[i].success = malloc(numGeneration * sizeof(double));
      mut1RateControl[i].success = malloc(numGeneration * sizeof(double));
      mut2RateControl[i].success = malloc(numGeneration * sizeof(double));
      #ifdef IG
        localSearch1RateControl[i].success = malloc(numGeneration * sizeof(double));
        localSearch3RateControl[i].success = malloc(numGeneration * sizeof(double));
      #endif
      crossOperatorControl[i].success = malloc(numGeneration * sizeof(double));
    }
    for(i = 0; i < 2; i++)
    {
      mut1OperatorControl[i].success = malloc(numGeneration * sizeof(double));
      mut2OperatorControl[i].success = malloc(numGeneration * sizeof(double));
    }
  #endif
}

void initilizeAdaptiveMethods()
{
  #ifdef AP
    APInitializeProbabilities(3, crossOperatorControl);
    APInitializeProbabilities(2, mut1OperatorControl);
    APInitializeProbabilities(2, mut2OperatorControl);
    APInitializeProbabilities(3, crossRateControl);
    APInitializeProbabilities(3, mut1RateControl);
    APInitializeProbabilities(3, mut2RateControl);
    #ifdef IG
      APInitializeProbabilities(3, localSearch1RateControl);
      APInitializeProbabilities(3, localSearch2RateControl);
    #endif
  #endif

  #ifdef PPC
    PPCInitializeProbabilities(3, crossOperatorControl);
    PPCInitializeProbabilities(2, mut1OperatorControl);
    PPCInitializeProbabilities(2, mut2OperatorControl);
    PPCInitializeProbabilities(3, crossRateControl);
    PPCInitializeProbabilities(3, mut1RateControl);
    PPCInitializeProbabilities(3, mut2RateControl);
    #ifdef IG
      PPCInitializeProbabilities(3, localSearch1RateControl);
      PPCInitializeProbabilities(3, localSearch2RateControl);
    #endif
  #endif

  #ifdef PPCR
    PPCInitializeProbabilities(3, crossOperatorControl);
    PPCInitializeProbabilities(2, mut1OperatorControl);
    PPCInitializeProbabilities(2, mut2OperatorControl);
    PPCInitializeProbabilities(3, crossRateControl);
    PPCInitializeProbabilities(3, mut1RateControl);
    PPCInitializeProbabilities(3, mut2RateControl);
    #ifdef IG
      PPCInitializeProbabilities(3, localSearch1RateControl);
      PPCInitializeProbabilities(3, localSearch2RateControl);
    #endif
  #endif
}

void initializePredictiveCounters()
{
  #ifdef PPC
    //Inicializa os contadores de ocorrência e successo
    initializeCounters(3, crossOperatorControl);
    initializeCounters(2, mut1OperatorControl);
    initializeCounters(2, mut2OperatorControl);
    initializeCounters(3, crossRateControl);
    initializeCounters(3, mut1RateControl);
    initializeCounters(3, mut2RateControl);
    #ifdef IG
      initializeCounters(3, localSearch1RateControl);
      initializeCounters(3, localSearch2RateControl);
    #endif
  #endif

  #ifdef PPCR
    //Inicializa os contadores de ocorrência e successo
    initializeCounters(3, crossOperatorControl);
    initializeCounters(2, mut1OperatorControl);
    initializeCounters(2, mut2OperatorControl);
    initializeCounters(3, crossRateControl);
    initializeCounters(3, mut1RateControl);
    initializeCounters(3, mut2RateControl);
    #ifdef IG
      initializeCounters(3, localSearch1RateControl);
      initializeCounters(3, localSearch2RateControl);
    #endif
  #endif
}

void rouletteCrossoverProbabilityForPPCR()
{
  #ifdef PPCR
    if(probCrossover == 0.7)
      probCrossover = ((double)rand() / (double)(RAND_MAX)) * 0.0667 + 0.7;
    if(probCrossover == 0.8)
      probCrossover = ((double)rand() / (double)(RAND_MAX)) * 0.0667 + 0.7667;
    if(probCrossover == 0.9)
      probCrossover = ((double)rand() / (double)(RAND_MAX)) * 0.0666 + 0.8334;
  #endif
}

void crossover(int countInd, int father1, int father2)
{
  #ifdef GA
    PMXcrossover(countInd, father1, father2);
  #else
    #ifdef PPCR
      if(probCrossover <= 0.7667)
      {
        crossRateControl[0].index[1]++;
      }
      else if(probCrossover <= 0.8334)
      {
        crossRateControl[1].index[1]++;
      }
      else if(probCrossover <= 0.9)
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
}

void rouletteMutationProbabilityForPPCR()
{
  #ifdef PPCR
    if(probMutation == 0.3)
      probMutation = ((double)rand() / (double)(RAND_MAX)) * 0.0667 + 0.3;
    if(probMutation == 0.4)
      probMutation = ((double)rand() / (double)(RAND_MAX)) * 0.0667 + 0.3667;
    if(probMutation == 0.5)
      probMutation = ((double)rand() / (double)(RAND_MAX)) * 0.0666 + 0.4334;
  #endif
}

void mutation(int countInd, int rateSize, ProbabilitiesControl rateControl[rateSize], int operatorSize, ProbabilitiesControl operatorControl[operatorSize])
{
  #ifdef GA
    shiftMutation(countInd);
  #else
    #ifdef PPCR
      if(probMutation <= 0.3667)
        rateControl[0].index[1]++;
      else if(probMutation <= 0.4334)
        rateControl[1].index[1]++;
      else if(probMutation <= 0.5)
        rateControl[2].index[1]++;
    #else
      if(probMutation == 0.3)
        rateControl[0].index[1]++;
      if(probMutation == 0.4)
        rateControl[1].index[1]++;
      if(probMutation == 0.5)
        rateControl[2].index[1]++;
    #endif
    mutaOperatorSelection(countInd, operatorSize, operatorControl);
  #endif
}

void rouletteLocalSearchProbabilityForPPCR()
{
  #ifdef PPCR
    if(probLocalSearch == 0.030)
      probLocalSearch = ((double)rand() / (double)(RAND_MAX)) * 0.02333 + 0.03;
    if(probLocalSearch == 0.065)
      probLocalSearch = ((double)rand() / (double)(RAND_MAX)) * 0.02333 + 0.05333;
    if(probLocalSearch == 0.100)
      probLocalSearch = ((double)rand() / (double)(RAND_MAX)) * 0.02334 + 0.07666;
  #endif
}

void APupdate(int countInd, int father1, int father2, double avgFitness)
{
  #ifdef AP
    crossoverAdaptivePursuit(countInd, father1, father2, 3, crossOperatorControl, avgFitness);
    mutationAdaptivePursuit(countInd, father1, father2, 2, mut1OperatorControl, avgFitness);
    mutationAdaptivePursuit(countInd + 1, father1, father2, 2, mut2OperatorControl, avgFitness);
    crossoverAdaptivePursuit(countInd, father1, father2, 3,crossRateControl, avgFitness);
    mutationAdaptivePursuit(countInd, father1, father2, 3, mut1RateControl, avgFitness);
    mutationAdaptivePursuit(countInd + 1, father1, father2, 3, mut2RateControl, avgFitness);
    #ifdef IG
      localSearchAdaptivePursuit(countInd, father1, father2, 3, localSearch1RateControl, avgFitness);
      localSearchAdaptivePursuit(countInd + 1, father1, father2, 3, localSearch2RateControl, avgFitness);
    #endif
  #endif
}

void PredictiveMethodsGetSuccess(int countInd, int father1, int father2, double avgFitness)
{
  #ifdef PPC
    crossoverSuccessEvaluation(countInd, father1, father2, 3, crossOperatorControl, avgFitness);
    mutationSuccessEvaluation(countInd, father1, father2, 2, mut1OperatorControl, avgFitness);
    mutationSuccessEvaluation(countInd + 1, father1, father2, 2, mut2OperatorControl, avgFitness);
    crossoverSuccessEvaluation(countInd, father1, father2, 3, crossRateControl, avgFitness);
    mutationSuccessEvaluation(countInd, father1, father2, 3, mut1RateControl, avgFitness);
    mutationSuccessEvaluation(countInd + 1, father1, father2, 3, mut2RateControl, avgFitness);
    #ifdef IG
      localSearchSuccessEvaluation(countInd, father1, father2, 3, localSearch1RateControl, avgFitness);
      localSearchSuccessEvaluation(countInd + 1, father1, father2, 3, localSearch2RateControl, avgFitness);
    #endif
  #endif

  #ifdef PPCR
    crossoverSuccessEvaluation(countInd, father1, father2, 3, crossOperatorControl, avgFitness);
    mutationSuccessEvaluation(countInd, father1, father2, 2, mut1OperatorControl, avgFitness);
    mutationSuccessEvaluation(countInd + 1, father1, father2, 2, mut2OperatorControl, avgFitness);
    crossoverSuccessEvaluation(countInd, father1, father2, 3, crossRateControl, avgFitness);
    mutationSuccessEvaluation(countInd, father1, father2, 3, mut1RateControl, avgFitness);
    mutationSuccessEvaluation(countInd + 1, father1, father2, 3, mut2RateControl, avgFitness);
    #ifdef IG
      localSearchSuccessEvaluation(countInd, father1, father2, 3, localSearch1RateControl, avgFitness);
      localSearchSuccessEvaluation(countInd + 1, father1, father2, 3, localSearch2RateControl, avgFitness);
    #endif
  #endif
}

void PredictiveMethodsUpdate(int countGen)
{
  #ifdef PPC
    UpdateSuccessIndicator(countGen, 3, crossOperatorControl);
    UpdateSuccessIndicator(countGen, 2, mut1OperatorControl);
    UpdateSuccessIndicator(countGen, 2, mut2OperatorControl);
    UpdateSuccessIndicator(countGen, 3, crossRateControl);
    UpdateSuccessIndicator(countGen, 3, mut1RateControl);
    UpdateSuccessIndicator(countGen, 3, mut2RateControl);
    #ifdef IG
      UpdateSuccessIndicator(countGen, 3, localSearch1RateControl);
      UpdateSuccessIndicator(countGen, 3, localSearch2RateControl);
    #endif
  #endif

  #ifdef PPCR
    UpdateSuccessIndicator(countGen, 3, crossOperatorControl);
    UpdateSuccessIndicator(countGen, 2, mut1OperatorControl);
    UpdateSuccessIndicator(countGen, 2, mut2OperatorControl);
    UpdateSuccessIndicator(countGen, 3, crossRateControl);
    UpdateSuccessIndicator(countGen, 3, mut1RateControl);
    UpdateSuccessIndicator(countGen, 3, mut2RateControl);
    #ifdef IG
      UpdateSuccessIndicator(countGen, 3, localSearch1RateControl);
      UpdateSuccessIndicator(countGen, 3, localSearch2RateControl);
    #endif
  #endif
}

void FreeMemoryForPPC()
{
  #ifdef PPC
    int i;
    for(i = 0; i < 3; i++)
    {
      free(crossRateControl[i].success);
      free(mut1RateControl[i].success);
      free(mut2RateControl[i].success);
      #ifdef IG
        free(localSearch1RateControl[i].success);
        free(localSearch2RateControl[i].success);
      #endif
      free(crossOperatorControl[i].success);
    }
    for(i = 0; i < 2; i++)
    {
      free(mut1OperatorControl[i].success);
      free(mut2OperatorControl[i].success);
    }
  #endif

  #ifdef PPCR
    int i;
    for(i = 0; i < 3; i++)
    {
      free(crossRateControl[i].success);
      free(mut1RateControl[i].success);
      free(mut2RateControl[i].success);
      #ifdef IG
        free(localSearch1RateControl[i].success);
        free(localSearch2RateControl[i].success);
      #endif
      free(crossOperatorControl[i].success);
    }
    for(i = 0; i < 2; i++)
    {
      free(mut1OperatorControl[i].success);
      free(mut2OperatorControl[i].success);
    }
  #endif
}
