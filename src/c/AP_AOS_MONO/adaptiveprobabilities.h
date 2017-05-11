#ifndef ADAPTIVEPROBABILITIES_H
#define ADAPTIVEPROBABILITIES_H

typedef struct probabilities{
	int index[2];
	double probRate;
	double qualityRate;
	double rewardRate;
}pro;

pro crosR[3];
pro mut1R[3];
pro mut2R[3];

void initializeRate();
double crossoverRate();
double mutaRate(pro muta[2]);
void crosRateProbabilityMatching(int countInd, int father1, int father2);
void crosRateCreditAssignmentGetReward(int countInd, int father1, int father2, int i);
int mutaRateProbabilityMatching(int countInd, int father1, int father2, pro muta[3]);
void mutaRateCreditAssignmentGetReward(int countInd, int father1, int father2, int i, pro muta[3]);
void mutaRateFinalProbability(int countInd, int father1, int father2);


#endif // ADAPTIVEPROBABILITIES_H
