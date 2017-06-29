#ifndef PREDICTIVEPARAMETERCONTROL_H
#define PREDICTIVEPARAMETERCONTROL_H

//Funções Gerais
void PPCInitializeProbabilities(int size, ProbabilitiesControl probControl[size]);
void initializeCounters(int size, ProbabilitiesControl probControl[size]);
void UpdateSuccessIndicator(int countGen, int size, ProbabilitiesControl probControl[size]);
void ForecastNextProbability(int countGen, int size, ProbabilitiesControl probControl[size]);

// Funções para o CROSSOVER
void crossoverSuccessEvaluation(int countInd, int father1, int father2, int size, ProbabilitiesControl probControl[size], double avgFitness);
int crossoverGetSuccess(int countInd, int father1, int father2, int i, int size, ProbabilitiesControl probControl[size], double avgFitness);

// Funções para a MUTAÇÃO
void mutationSuccessEvaluation(int countInd, int father1, int father2, int size, ProbabilitiesControl probControl[size], double avgFitness);
int mutationGetSuccess(int countInd, int father1, int father2, int i, int size, ProbabilitiesControl probControl[size], double avgFitness);

// Funções para a LOCALSERACH
void localSearchSuccessEvaluation(int countInd, int father1, int father2, int size, ProbabilitiesControl probControl[size], double avgFitness);
int localSearchGetSuccess(int countInd, int father1, int father2, int i, int size, ProbabilitiesControl probControl[size], double avgFitness);

#endif // PREDICTIVEPARAMETERCONTROL_H
