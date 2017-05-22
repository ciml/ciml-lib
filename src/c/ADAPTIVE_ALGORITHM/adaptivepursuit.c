#include <stdio.h>
#include <stdlib.h>
#include "aosgeneticalgorithm.h"
#include "adaptivepursuit.h"
#include "crossover.h"
#include "mutation.h"

void APInitializeProbabilities(int size, ProbabilitiesControl probControl[size])
{
	int i;
	for(i = 0; i < size; i++)
	{
		probControl[i].prob = (double)(1.0/size);
		probControl[i].quality = 1.0;
		probControl[i].index[0] = 0;
		probControl[i].index[1] = 0;
	}
} //initializeProbabilities

void crossoverAdaptivePursuit(int countInd, int father1, int father2, int size, ProbabilitiesControl probControl[size])
{
	int i, count = 0;
	int bestOperator;
	double prob_min = 0.05;
	double prob_max = 1 - (size - 1)*prob_min;
	double alpha = 0.8;
	double beta = 0.8;

	for(i = 0; i < size; i++)
	{
		if(probControl[i].index[1] > 0)
		{
			count++;
		}
	}
	if(count < size)
	{
		for(i = 0; i < size; i++)
		{
			if((probControl[i].index[1] == 1) && (probControl[i].index[1] != probControl[i].index[0]))
			{
				crossoverGetReward(countInd, father1, father2, i, size , probControl);
				probControl[i].index[0] = probControl[i].index[1];
				probControl[i].prob = 0.0;
				probControl[i].quality = (1.0 - alpha) *
									probControl[i].quality +	alpha * probControl[i].reward;
			}
			else if(probControl[i].index[1] == 0)
			{
				probControl[i].prob = (1.0 / (size - count));
			}
		}
	}
	else
	{
		for(i = 0; i < size; i++)
		{
			if(probControl[i].index[0] < probControl[i].index[1])
			{
				crossoverGetReward(countInd, father1, father2, i, size, probControl);
				probControl[i].index[0] = probControl[i].index[1];
				probControl[i].quality =	(1.0 - alpha) *
										probControl[i].quality + alpha * probControl[i].reward;

			}
		}
		bestOperator = 0;
		for(i = 1; i < size; i++)
			if(probControl[bestOperator].quality < probControl[i].quality)
				bestOperator = i;
		for(i = 0; i < size; i++)
		{
			if(i == bestOperator)
				probControl[i].prob = probControl[i].prob +
								beta*(prob_max - probControl[i].prob);
			else
				probControl[i].prob = probControl[i].prob +
								beta*(prob_min - probControl[i].prob);
		}
	}
} //crossoverAdaptivePursuit

void crossoverGetReward(int countInd, int father1, int father2, int i, int size, ProbabilitiesControl probControl[size])
{
	probControl[i].reward = (individuals[father1].fitMakespan + individuals[father2].fitMakespan)/
		(individuals[countInd].fitMakespan + individuals[countInd + 1].fitMakespan);
}//crossoverGetReward

int mutationAdaptivePursuit(int countInd, int father1, int father2, int size, ProbabilitiesControl probControl[size])
{
	int i, count = 0;
	double alpha = 0.8;
	for(i = 0; i < size; i++)
	{
		if(probControl[i].index[1] != 0)
		{
			count++;
		}
	}
	if(count < size)
	{
		for(i = 0; i < size; i++)
		{
			if((probControl[i].index[1] == 1) && (probControl[i].index[1] != probControl[i].index[0]))
			{
				mutationGetReward(countInd, father1, father2, i, size, probControl);
				probControl[i].index[0] = probControl[i].index[1];
				probControl[i].prob = 0;
				probControl[i].quality = (1.0 - alpha) *
					probControl[i].quality +	alpha * probControl[i].reward;
			}
			else if(probControl[i].index[1] == 0)
			{
				probControl[i].prob = (1.0 / (size - count));
			}
		}
	}
	else
	{
		for(i = 0; i < size; i++)
		{
			if(probControl[i].index[0] != probControl[i].index[1])
			{
				mutationGetReward(countInd, father1, father2, i, size, probControl);
				probControl[i].index[0] = probControl[i].index[1];
				probControl[i].quality =	(1.0 - alpha) *
						probControl[i].quality + alpha * probControl[i].reward;
			}
		}
	}
	return count;
} //mutationAdaptivePursuit

void mutationGetReward(int countInd, int father1, int father2, int i, int size, ProbabilitiesControl probControl[size])
{
	probControl[i].reward = ((individuals[father1].fitMakespan + individuals[father2].fitMakespan)/2.0)
								/ (individuals[countInd].fitMakespan);
}//mutationGetReward

void mutationFinalAdaptivePursuit(int countInd, int father1, int father2, int size, ProbabilitiesControl probControl[size])
{
	int i, count = 0;
	double qualitySum;
	double prob_min = 0.05;

	count = mutationAdaptivePursuit(countInd, father1, father2, size, probControl);

	if(count > size - 1)
	{
		qualitySum = 0;
		for(i = 0; i < size; i++)
			qualitySum += probControl[i].quality;
		for(i = 0; i < size; i++)
		{
			probControl[i].prob = prob_min +
        (1.0 - ((double)size) * prob_min) *	(probControl[i].quality / qualitySum);
		}
	}
} //mutationFinalAdaptivePursuit
