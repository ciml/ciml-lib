#include <stdio.h>
#include <stdlib.h>
#include "aosgeneticalgorithm.h"
#include "adaptiveprobabilities.h"

void initializeRate()
{
	int i;
	for(i = 0; i < 3; i++)
	{
		crosR[i].probRate = (double)(1.0/3.0);
		crosR[i].qualityRate = 1.0;
		crosR[i].index[0] = 0;
		crosR[i].index[1] = 0;
		
		mut1R[i].probRate = (double)(1.0/3.0);
		mut1R[i].qualityRate = 1.0;
		mut1R[i].index[0] = 0;
		mut1R[i].index[1] = 0;
		
		mut2R[i].probRate = (double)(1.0/3.0);
		mut2R[i].qualityRate = 1.0;
		mut2R[i].index[0] = 0;
		mut2R[i].index[1] = 0;
	}
} //creditInitialize

double crossoverRate()
{
	double x;
	x = (double)((double)rand() / (double)RAND_MAX);
	if(x < crosR[0].probRate)
	{
		return 0.7;
	}
	else if(x < (crosR[1].probRate + crosR[0].probRate))
	{
		return 0.8;
	}
	else
	{
		return 0.9;
	}
} //crossoverSelection

double mutaRate(pro muta[2])
{
	double x;
	x = (double)((double)rand() / (double)RAND_MAX);
	if(x < muta[0].probRate)
	{
		return 0.3;
	}
	else if(x < (muta[1].probRate + muta[0].probRate))
	{
		return 0.4;
	}
	else
	{
		return 0.5;
	}
} //crossoverSelection

void crosRateProbabilityMatching(int countInd, int father1, int father2)
{
	int i, count = 0;
	int bestOperator;
	double prob_min = 0.15;
	double prob_max = 1 - (3 - 1)*prob_min;
	double alpha = 0.8;
	double beta = 0.8;

	for(i = 0; i < 3; i++)
	{
		if(crosR[i].index[1] > 0)
		{
			count++;
		}
	}
	if(count < 3)
	{
		for(i = 0; i < 3; i++)
		{
			if((crosR[i].index[1] == 1) && (crosR[i].index[1] != crosR[i].index[0]))
			{
				crosRateCreditAssignmentGetReward(countInd, father1, father2, i);
				crosR[i].index[0] = crosR[i].index[1];
				crosR[i].probRate = 0.0;
				crosR[i].qualityRate = (1.0 - alpha) *
									crosR[i].qualityRate +	alpha * crosR[i].rewardRate;
			}
			else if(crosR[i].index[1] == 0)
			{
				crosR[i].probRate = (1.0 / (3 - count));
			}
		}
	}
	else
	{
		for(i = 0; i < 3; i++)
		{
			if(crosR[i].index[0] < crosR[i].index[1])
			{
				crosRateCreditAssignmentGetReward(countInd, father1, father2, i);
				crosR[i].index[0] = crosR[i].index[1];
				crosR[i].qualityRate =	(1.0 - alpha) *
										crosR[i].qualityRate + alpha * crosR[i].rewardRate;

			}
		}
		bestOperator = 0;
		for(i = 1; i < 3; i++)
			if(crosR[bestOperator].qualityRate < crosR[i].qualityRate)
				bestOperator = i;
		for(i = 0; i < 3; i++)
		{
			if(i == bestOperator)
				crosR[i].probRate = crosR[i].probRate +
								beta*(prob_max - crosR[i].probRate);
			else
				crosR[i].probRate = crosR[i].probRate +
								beta*(prob_min - crosR[i].probRate);
		}
	}
} //crossProbabiltyMatching

int mutaRateProbabilityMatching(int countInd, int father1, int father2, pro muta[2])
{
	int i, count = 0;
	double alpha = 0.8;
	for(i = 0; i < 3; i++)
	{
		if(muta[i].index[1] != 0)
		{
			count++;
		}
	}
	if(count < 3)
	{
		for(i = 0; i < 3; i++)
		{
			if((muta[i].index[1] == 1) && (muta[i].index[1] != muta[i].index[0]))
			{
				mutaRateCreditAssignmentGetReward(countInd, father1, father2, i, muta);
				muta[i].index[0] = muta[i].index[1];
				muta[i].probRate = 0;
				muta[i].qualityRate = (1.0 - alpha) *
					muta[i].qualityRate +	alpha * muta[i].rewardRate;
			}
			else if(muta[i].index[1] == 0)
			{
				muta[i].probRate = (1.0 / (3 - count));
			}
		}
	}
	else
	{
		for(i = 0; i < 2; i++)
		{
			if(muta[i].index[0] != muta[i].index[1])
			{
				mutaRateCreditAssignmentGetReward(countInd, father1, father2, i, muta);
				muta[i].index[0] = muta[i].index[1];
				muta[i].qualityRate =	(1.0 - alpha) *
						muta[i].qualityRate + alpha * muta[i].rewardRate;
			}
		}
	}
	return count;
} //mutaProbabiltyMatching

void mutaRateFinalProbability(int countInd, int father1, int father2)
{
	int i, count[2] = {0};
	double qualitySum;
	double prob_min = 0.15;

	count[0] = mutaRateProbabilityMatching(countInd, father1, father2, mut1R);
	count[1] = mutaRateProbabilityMatching((countInd + 1), father1, father2, mut2R);

	if(count[0] > 1)
	{
		qualitySum = 0;
		for(i = 0; i < 2; i++)
			qualitySum += mut1R[i].qualityRate;
		for(i = 0; i < 2; i++)
		{
			mut1R[i].probRate = prob_min + (1.0 - 2.0 * prob_min) *
																	(mut1R[i].qualityRate / qualitySum);
		}
	}
	if(count[1] > 1)
	{
		qualitySum = 0;
		for(i = 0; i < 2; i++)
			qualitySum += mut2R[i].qualityRate;
		for(i = 0; i < 2; i++)
		{
			mut2R[i].probRate = prob_min + (1.0 - 2.0 * prob_min) *
																	(mut2R[i].qualityRate / qualitySum);
		}
	}
} //mutaFinalProbability

void crosRateCreditAssignmentGetReward(int countInd, int father1, int father2, int i)
{
	crosR[i].rewardRate = (individuals[father1].fitMakespan + individuals[father2].fitMakespan)/
		(individuals[countInd].fitMakespan + individuals[countInd + 1].fitMakespan);
}

void mutaRateCreditAssignmentGetReward(int countInd, int father1, int father2, int i, pro muta[2])
{
	muta[i].rewardRate = ((individuals[father1].fitMakespan + individuals[father2].fitMakespan)/2.0)
								/ (individuals[countInd].fitMakespan);
}
