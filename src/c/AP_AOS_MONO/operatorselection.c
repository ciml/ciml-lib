#include <stdio.h>
#include <stdlib.h>
#include "aosgeneticalgorithm.h"
#include "operatorselection.h"
#include "crossover.h"
#include "mutation.h"

void crossoverOperator(int countInd, int father1, int father2)
{
	double x;
	x = (double)((double)rand() / (double)RAND_MAX);
	if(x < cross[0].probOperator)
	{
		opCrossover(countInd, father1, father2);
		cross[0].index[1]++;
	}
	else if(x < (cross[1].probOperator + cross[0].probOperator))
	{
		LOXcrossover(countInd, father1, father2);
		cross[1].index[1]++;
	}
	else
	{
		PMXcrossover(countInd, father1, father2);
		cross[2].index[1]++;
	}
} //crossoverSelection

void mut1Operator(int countInd)
{
	double x;
	x = (double)((double)rand() / (double)RAND_MAX);
	if(x < mut1[0].probOperator)
	{
		shiftMutation(countInd);
		mut1[0].index[1]++;
	}
	else
	{
		interchangeMutation(countInd);
		mut1[1].index[1]++;
	}
} //mutationSelection

void mut2Operator(int countInd)
{
	double x;
	x = (double)((double)rand() / (double)RAND_MAX);
	if(x < mut2[0].probOperator)
	{
		shiftMutation(countInd);
		mut2[0].index[1]++;
	}
	else
	{
		interchangeMutation(countInd);
		mut2[1].index[1]++;
	}
} //mutationSelection

void creditInitialize()
{
	int i;
	for(i = 0; i < 3; i++)
	{
		cross[i].probOperator = (double)(1.0/3.0);
		cross[i].qualityOperator = 1.0;
		cross[i].index[0] = 0;
		cross[i].index[1] = 0;
	}

	for(i = 0; i < 2; i++)
	{
		mut1[i].probOperator = (double)(1.0/2.0);
		mut1[i].qualityOperator = 1.0;
		mut1[i].index[0] = 0;
		mut1[i].index[1] = 0;
	}

	for(i = 0; i < 2; i++)
	{
		mut2[i].probOperator = (double)(1.0/2.0);
		mut2[i].qualityOperator = 1.0;
		mut2[i].index[0] = 0;
		mut2[i].index[1] = 0;
	}
} //creditInitialize

void crossProbabilityMatching(int countInd, int father1, int father2)
{
	int i, count = 0;
	int bestOperator;
	double prob_min = 0.15;
	double prob_max = 1 - (3 - 1)*prob_min;
	double alpha = 0.8;
	double beta = 0.8;

	for(i = 0; i < 3; i++)
	{
		if(cross[i].index[1] > 0)
		{
			count++;
		}
	}
	if(count < 3)
	{
		for(i = 0; i < 3; i++)
		{
			if((cross[i].index[1] == 1) && (cross[i].index[1] != cross[i].index[0]))
			{
				crossCreditAssignmentGetReward(countInd, father1, father2, i);
				cross[i].index[0] = cross[i].index[1];
				cross[i].probOperator = 0.0;
				cross[i].qualityOperator = (1.0 - alpha) *
									cross[i].qualityOperator +	alpha * cross[i].rewardOperator;
			}
			else if(cross[i].index[1] == 0)
			{
				cross[i].probOperator = (1.0 / (3 - count));
			}
		}
	}
	else
	{
		for(i = 0; i < 3; i++)
		{
			if(cross[i].index[0] < cross[i].index[1])
			{
				crossCreditAssignmentGetReward(countInd, father1, father2, i);
				cross[i].index[0] = cross[i].index[1];
				cross[i].qualityOperator =	(1.0 - alpha) *
										cross[i].qualityOperator + alpha * cross[i].rewardOperator;

			}
		}
		bestOperator = 0;
		for(i = 1; i < 3; i++)
			if(cross[bestOperator].qualityOperator < cross[i].qualityOperator)
				bestOperator = i;
		for(i = 0; i < 3; i++)
		{
			if(i == bestOperator)
				cross[i].probOperator = cross[i].probOperator +
								beta*(prob_max - cross[i].probOperator);
			else
				cross[i].probOperator = cross[i].probOperator +
								beta*(prob_min - cross[i].probOperator);
		}
	}
} //crossProbabiltyMatching

int mutaProbabilityMatching(int countInd, int father1, int father2, oper muta[2])
{
	int i, count = 0;
	double alpha = 0.8;
	for(i = 0; i < 2; i++)
	{
		if(muta[i].index[1] != 0)
		{
			count++;
		}
	}
	if(count < 2)
	{
		for(i = 0; i < 2; i++)
		{
			if((muta[i].index[1] == 1) && (muta[i].index[1] != muta[i].index[0]))
			{
				mutaCreditAssignmentGetReward(countInd, father1, father2, i, muta);
				muta[i].index[0] = muta[i].index[1];
				muta[i].probOperator = 0;
				muta[i].qualityOperator = (1.0 - alpha) *
					muta[i].qualityOperator +	alpha * muta[i].rewardOperator;
			}
			else if(muta[i].index[1] == 0)
			{
				muta[i].probOperator = (1.0 / (2 - count));
			}
		}
	}
	else
	{
		for(i = 0; i < 2; i++)
		{
			if(muta[i].index[0] != muta[i].index[1])
			{
				mutaCreditAssignmentGetReward(countInd, father1, father2, i, muta);
				muta[i].index[0] = muta[i].index[1];
				muta[i].qualityOperator =	(1.0 - alpha) *
						muta[i].qualityOperator + alpha * muta[i].rewardOperator;
			}
		}
	}
	return count;
} //mutaProbabiltyMatching

void mutaFinalProbability(int countInd, int father1, int father2)
{
	int i, count[2] = {0};
	double qualitySum;
	double prob_min = 0.15;

	count[0] = mutaProbabilityMatching(countInd, father1, father2, mut1);
	count[1] = mutaProbabilityMatching((countInd + 1), father1, father2, mut2);

	if(count[0] > 1)
	{
		qualitySum = 0;
		for(i = 0; i < 2; i++)
			qualitySum += mut1[i].qualityOperator;
		for(i = 0; i < 2; i++)
		{
			mut1[i].probOperator = prob_min + (1.0 - 2.0 * prob_min) *
																	(mut1[i].qualityOperator / qualitySum);
		}
	}
	if(count[1] > 1)
	{
		qualitySum = 0;
		for(i = 0; i < 2; i++)
			qualitySum += mut2[i].qualityOperator;
		for(i = 0; i < 2; i++)
		{
			mut2[i].probOperator = prob_min + (1.0 - 2.0 * prob_min) *
																	(mut2[i].qualityOperator / qualitySum);
		}
	}
} //mutaFinalProbability

void crossCreditAssignmentGetReward(int countInd, int father1, int father2, int i)
{
	cross[i].rewardOperator = (individuals[father1].fitMakespan + individuals[father2].fitMakespan)/
		(individuals[countInd].fitMakespan + individuals[countInd + 1].fitMakespan);
}

void mutaCreditAssignmentGetReward(int countInd, int father1, int father2, int i, oper muta[2])
{
	muta[i].rewardOperator = ((individuals[father1].fitMakespan + individuals[father2].fitMakespan)/2.0)
								/ (individuals[countInd].fitMakespan);
}
