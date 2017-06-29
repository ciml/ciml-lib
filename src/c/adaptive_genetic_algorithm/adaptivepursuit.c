#include <stdio.h>
#include <stdlib.h>
#include "geneticalgorithm.h"
#include "adaptivepursuit.h"
#include "crossover.h"
#include "mutation.h"

#ifdef AP
	#define CR_V3
#endif

void APInitializeProbabilities(int size, ProbabilitiesControl probControl[size])
{
	int i;
	alpha = 0.2;
	beta = 0.2;
	prob_min = 0.05;
	for(i = 0; i < size; i++)
	{
		probControl[i].prob = (double)(1.0/size);
		probControl[i].quality = 1.0;
		probControl[i].index[0] = 0;
		probControl[i].index[1] = 0;
	}
} //initializeProbabilities

void crossoverAdaptivePursuit(int countInd, int father1, int father2, int size, ProbabilitiesControl probControl[size], double avgFitness)
{
	int i, count = 0;
	int bestOperator;
	double prob_max = 1 - (size - 1)*prob_min;

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
				crossoverGetReward(countInd, father1, father2, i, size , probControl, avgFitness);
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
				crossoverGetReward(countInd, father1, father2, i, size, probControl, avgFitness);
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

void crossoverGetReward(int countInd, int father1, int father2, int i, int size, ProbabilitiesControl probControl[size], double avgFitness)
{
	#ifdef CR_V0
		probControl[i].reward = (((double)individuals[father1].fitMakespan +
			individuals[father2].fitMakespan) / ((double)individuals[countInd].fitMakespan +
				individuals[countInd + 1].fitMakespan));
	#endif
	#ifdef CR_V1
		int bestFather, bestSon;

		if (individuals[father1].fitMakespan < individuals[father2].fitMakespan)
			bestFather = father1;
		else
			bestFather = father2;
		if (individuals[countInd].fitMakespan < individuals[countInd + 1].fitMakespan)
			bestSon = countInd;
		else
			bestSon = countInd + 1;

		probControl[i].reward = (double)((double)individuals[bestFather].fitMakespan /
			(double)individuals[bestSon].fitMakespan);
	#endif
	#ifdef CR_V2
		int bestSon;

		if (individuals[countInd].fitMakespan < individuals[countInd + 1].fitMakespan)
			bestSon = countInd;
		else
			bestSon = countInd + 1;
		probControl[i].reward = (double)(avgFitness / (double)individuals[bestSon].fitMakespan);
	#endif
	#ifdef CR_V3
		int bestFather, bestSon;

		if (individuals[father1].fitMakespan < individuals[father2].fitMakespan)
			bestFather = father1;
		else
			bestFather = father2;
		if (individuals[countInd].fitMakespan < individuals[countInd + 1].fitMakespan)
			bestSon = countInd;
		else
			bestSon = countInd + 1;

		probControl[i].reward = (double)((double)individuals[bestFather].fitMakespan /
			(double)individuals[bestSon].fitMakespan);

	#endif

}//crossoverGetReward

void mutationAdaptivePursuit(int countInd, int father1, int father2, int size, ProbabilitiesControl probControl[size], double avgFitness)
{
	int i, count = 0;
	int bestOperator;
	double prob_max = 1 - (size - 1)*prob_min;

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
				mutationGetReward(countInd, father1, father2, i, size , probControl, avgFitness);
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
				mutationGetReward(countInd, father1, father2, i, size, probControl, avgFitness);
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
} //mutationAdaptivePursuit

void mutationGetReward(int countInd, int father1, int father2, int i, int size, ProbabilitiesControl probControl[size], double avgFitness)
{
	#ifdef CR_V0
		probControl[i].reward = (((double)individuals[father1].fitMakespan +
			(double)individuals[father2].fitMakespan)	/
				((double)individuals[countInd].fitMakespan * 2.0));
	#endif

	#ifdef CR_V1
	probControl[i].reward = (avgFitness	/ (double)individuals[countInd].fitMakespan);
	#endif

	#ifdef CR_V2
	probControl[i].reward = (avgFitness	/ (double)individuals[countInd].fitMakespan);
	#endif

	#ifdef CR_V3
		int bestFather;

		if(individuals[father1].fitMakespan < individuals[father2].fitMakespan)
			bestFather = father1;
		else
			bestFather = father2;

		probControl[i].reward = (double)individuals[bestFather].fitMakespan /
			individuals[countInd].fitMakespan;
	#endif
}//mutationGetReward

void localSearchAdaptivePursuit(int countInd, int father1, int father2, int size, ProbabilitiesControl probControl[size], double avgFitness)
{
	int i, count = 0;
	int bestOperator;
	double prob_max = 1 - (size - 1)*prob_min;

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
				localSearchGetReward(countInd, father1, father2, i, size , probControl, avgFitness);
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
				localSearchGetReward(countInd, father1, father2, i, size, probControl, avgFitness);
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
} //localSearchAdaptivePursuit

void localSearchGetReward(int countInd, int father1, int father2, int i, int size, ProbabilitiesControl probControl[size], double avgFitness)
{
	#ifdef CR_V0
		probControl[i].reward = (((double)individuals[father1].fitMakespan +
			(double)individuals[father2].fitMakespan)	/
				((double)individuals[countInd].fitMakespan * 2.0));
	#endif

	#ifdef CR_V1
	probControl[i].reward = (avgFitness	/ (double)individuals[countInd].fitMakespan);
	#endif

	#ifdef CR_V2
	probControl[i].reward = (avgFitness	/ (double)individuals[countInd].fitMakespan);
	#endif

	#ifdef CR_V3
		int bestFather;

		if(individuals[father1].fitMakespan < individuals[father2].fitMakespan)
			bestFather = father1;
		else
			bestFather = father2;

		probControl[i].reward = (double)individuals[bestFather].fitMakespan /
			individuals[countInd].fitMakespan;
	#endif
}//localSearchGetReward
