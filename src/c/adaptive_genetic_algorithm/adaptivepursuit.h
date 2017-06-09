#ifndef ADAPTIVEPURSUIT_H
#define ADAPTIVEPURSUIT_H

void APInitializeProbabilities(int size, ProbabilitiesControl[size]);
void crossoverAdaptivePursuit(int countInd, int father1, int father2, int size, ProbabilitiesControl probControl[size]);
void crossoverGetReward(int countInd, int father1, int father2, int i, int size, ProbabilitiesControl probControl[size]);
int mutationAdaptivePursuit(int countInd, int father1, int father2, int size, ProbabilitiesControl probControl[size], double avgFitness);
void mutationGetReward(int countInd, int father1, int father2, int i, int size, ProbabilitiesControl probControl[size],double avgFitness);
void mutationFinalAdaptivePursuit(int countInd, int father1, int father2, int size, ProbabilitiesControl probControl[size], double avgFitness);

#endif // ADAPTIVEPURSUIT_H
