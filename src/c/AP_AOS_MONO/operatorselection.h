#ifndef OPERATORSELECTION_H
#define OPERATORSELECTION_H

typedef struct oper{
	int index[2];
	double probOperator;
	double qualityOperator;
	double rewardOperator;
}oper;

oper cross[3];
oper mut1[2];
oper mut2[2];

void crossoverOperator(int countInd, int father1, int father2);
void mut1Operator(int countInd);
void mut2Operator(int countInd);
void secondMutation(int countInd);
void creditInitialize();
void crossProbabilityMatching(int countInd, int father1, int father2);
void crossCreditAssignmentGetReward(int countInd, int father1, int father2, int i);
int mutaProbabilityMatching(int countInd, int father1, int father2, oper muta[2]);
void mutaCreditAssignmentGetReward(int countInd, int father1, int father2, int i, oper muta[2]);
void mutaFinalProbability(int countInd, int father1, int father2);

#endif // OPERATORSELECTION_H
