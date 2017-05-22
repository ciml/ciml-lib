#ifndef PREDICTIVEPARAMETERCONTROL_H
#define PREDICTIVEPARAMETERCONTROL_H

//Funções Gerais
void PPCInitializeProbabilities(int size, ProbabilitiesControl probControl[size]);
void initializeCounters(int size, ProbabilitiesControl probControl[size]);
void UpdateSuccessIndicator(int countGen, int size, ProbabilitiesControl probControl[size]);
void ForecastNextProbability(int countGen, int size, ProbabilitiesControl probControl[size]);

// Funções para o CROSSOVER
void crossoverSuccessEvaluation(int countInd, int father1, int father2, int size, ProbabilitiesControl probControl[size]);
int crossoverGetSuccess(int countInd, int father1, int father2, int i, int size, ProbabilitiesControl probControl[size]);

// Funções para a MUTAÇÃO
void mutationSuccessEvaluation(int countInd, int father1, int father2, int size, ProbabilitiesControl probControl[size]);
int mutationGetSuccess(int countInd, int father1, int father2, int i, int size, ProbabilitiesControl probControl[size]);


#endif // PREDICTIVEPARAMETERCONTROL_H
