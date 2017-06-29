#ifndef ADAPTIVEPREPROCESSORS_H
#define ADAPTIVEPREPROCESSORS_H

void alocateMemoryPPC();
void initilizeAdaptiveMethods();
void initializePredictiveCounters();
void rouletteCrossoverProbabilityForPPCR();
void crossover(int countInd, int father1, int father2);
void rouletteMutationProbabilityForPPCR();
void mutation(int countInd, int rateSize, ProbabilitiesControl rateControl[rateSize], int operatorSize, ProbabilitiesControl operatorControl[operatorSize]);
void rouletteLocalSearchProbabilityForPPCR();
void APupdate(int countInd, int father1, int father2, double avgFitness);
void PredictiveMethodsGetSuccess(int countInd, int father1, int father2, double avgFitness);
void PredictiveMethodsUpdate(int countGen);
void FreeMemoryForPPC();

#endif //ADAPTIVEPREPROCESSORS_H
