#ifndef ADAPTIVEPURSUIT_H
#define ADAPTIVEPURSUIT_H

double prob_min;
double alpha;
double beta;

void APInitializeProbabilities(int size, ProbabilitiesControl[size]);
void crossoverAdaptivePursuit(int countInd, int father1, int father2, int size, ProbabilitiesControl probControl[size], double avgFitness);
void crossoverGetReward(int countInd, int father1, int father2, int i, int size, ProbabilitiesControl probControl[size], double avgFitness);
void mutationAdaptivePursuit(int countInd, int father1, int father2, int size, ProbabilitiesControl probControl[size], double avgFitness);
void mutationGetReward(int countInd, int father1, int father2, int i, int size, ProbabilitiesControl probControl[size], double avgFitness);
void localSearchAdaptivePursuit(int countInd, int father1, int father2, int size, ProbabilitiesControl probControl[size], double avgFitness);
void localSearchGetReward(int countInd, int father1, int father2, int i, int size, ProbabilitiesControl probControl[size], double avgFitness);


#endif // ADAPTIVEPURSUIT_H
