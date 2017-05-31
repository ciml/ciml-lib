#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "geneticalgorithm.h"
#include "predictiveparametercontrol.h"
#include "gausselimination.h"
#include "crossover.h"
#include "mutation.h"

void PPCInitializeProbabilities(int size, ProbabilitiesControl probControl[size])
{
	int i;
	for(i = 0; i < size; i++)
	{
		probControl[i].prob = 1.0/size;
	}
} //initializeProbabilities

void initializeCounters(int size, ProbabilitiesControl probControl[size])
{
	int i;
	for(i = 0; i < size; i++)
	{
		probControl[i].index[0] = 0;
		probControl[i].index[1] = 0;
		probControl[i].successApplication = 0;
	}
} //initializeCounters

void UpdateSuccessIndicator(int countGen, int size, ProbabilitiesControl probControl[size])
{
	int i;
	for(i = 0; i < size; i++)
	{
		if(probControl[i].index[1] != 0)
			probControl[i].success[countGen] =
				(double)probControl[i].successApplication / (double)probControl[i].index[1];
		else
			probControl[i].success[countGen] = 0.0;
	}

	if(countGen > 1)
	{
		ForecastNextProbability(countGen, size, probControl);
	}
}//UpdateSuccessIndicator

void ForecastNextProbability(int countGen, int size, ProbabilitiesControl probControl[size])
{
	int i, j;
	double x1, y1, xy, x2, x3, x4, x2y;
	double successSum = 0, prob_min = 0.05;

	double *b = (double*)calloc(3, sizeof(double));
	double *x = (double*)calloc(3, sizeof(double));
	double **matrix = (double**)calloc(3, sizeof(double*));
	for(i = 0; i < 3; i++)
		matrix[i] = (double*)calloc(3, sizeof(double));

	for(i = 0; i < size; i++) /*Loop criado para calculcar a probabilidade de cada indicador */
	{
		x1 = 0; x2 = 0; x3 = 0; x4 = 0; y1 = 0; xy = 0; x2y = 0;
		if(countGen < 4)
		{
			// numberOfPoints = countGen + 1;
			matrix[0][0] = countGen + 1;
			for(j = 0; j < matrix[0][0]; j++)
			{
				x1 = x1 + j + 1;
				x2 = x2 + ((j + 1) * (j + 1));
				x3 = x3 + ((j + 1) * (j + 1) * (j + 1));
				x4 = x4 + ((j + 1) * (j + 1) * (j + 1) * (j + 1));
				y1 = y1 + probControl[i].success[j];
				xy = xy + ((j + 1) * probControl[i].success[j]);
				x2y = x2y + ((j + 1) * (j + 1) * probControl[i].success[j]);
			}
		}
		else
		{
			matrix[0][0] = 5;
			for(j = 0; j < 5; j++)
			{
				x1 = x1 + j + 1;
				x2 = x2 + ((j + 1) * (j + 1));
				x3 = x3 + ((j + 1) * (j + 1) * (j + 1));
				x4 = x4 + ((j + 1) * (j + 1) * (j + 1) * (j + 1));
				y1 = y1 + probControl[i].success[j + countGen - 4];
				xy = xy + ((j + 1) * probControl[i].success[j + countGen - 4]);
				x2y = x2y + ((j + 1) * (j + 1) * probControl[i].success[j + countGen - 4]);
			}
		}

		matrix[0][1] = x1;
		matrix[0][2] = x2;
		matrix[1][0] = x1;
		matrix[1][1] = x2;
		matrix[1][2] = x3;
		matrix[2][0] = x2;
		matrix[2][1] = x3;
		matrix[2][2] = x4;

		b[0] = y1;
		b[1] = xy;
		b[2] = x2y;

		gauss(matrix, b, x);

		probControl[i].quality = x[2] * (double)(countGen + 2)  * (double)(countGen + 2) +
			x[1] * (double)(countGen + 2) + x[0];
		if(probControl[i].quality < 0.0)
			probControl[i].quality = 0.0;
	} //Fim do loop

	for(i = 0; i < size; i++)
	successSum += probControl[i].quality;

	if(successSum == 0)
	{
		for(i = 0; i < size; i++)
			probControl[i].prob = (double)1.0/size;
	}
	else
	{
		for(i = 0; i < size; i++)
			probControl[i].prob =  prob_min +
				(1.0 - ((double)size) * prob_min) * probControl[i].quality / successSum;
	}

	for(i = 0; i < 3; i++)
		free(matrix[i]);
	free(matrix);
	free(b);
	free(x);
}//ForecastNextProbability

void crossoverSuccessEvaluation(int countInd, int father1, int father2, int size, ProbabilitiesControl probControl[size])
{
	int i;
	for(i = 0; i < size; i++)
	{
		if((probControl[i].index[1] > 0) && (probControl[i].index[1] != probControl[i].index[0]))
		{
			probControl[i].successApplication +=
											crossoverGetSuccess(countInd, father1, father2, i, size, probControl);
			probControl[i].index[0] = probControl[i].index[1];
		}
	}
}//crossoverSuccessEvaluation

int crossoverGetSuccess(int countInd, int father1, int father2, int i, int size, ProbabilitiesControl probControl[size])
{
	probControl[i].reward =
		(double)((double)individuals[father1].fitMakespan + (double)individuals[father2].fitMakespan)/
			((double)individuals[countInd].fitMakespan + (double)individuals[countInd + 1].fitMakespan);
	if(probControl[i].reward > 1)
		return 1;
	else
		return 0;
}// crossoverGetSuccess

void mutationSuccessEvaluation(int countInd, int father1, int father2, int size, ProbabilitiesControl probControl[size])
{
	int i;
	for(i = 0; i < size; i++)
	{
		if((probControl[i].index[1] > 0) && (probControl[i].index[1] != probControl[i].index[0]))
		{
			probControl[i].successApplication +=
											mutationGetSuccess(countInd, father1, father2, i, size, probControl);
			probControl[i].index[0] = probControl[i].index[1];
		}
	}
}//mutationSuccessEvaluation

int mutationGetSuccess(int countInd, int father1, int father2, int i, int size, ProbabilitiesControl probControl[size])
{
	probControl[i].reward =
		(double)(((double)individuals[father1].fitMakespan + (double)individuals[father2].fitMakespan)/2.0)
																	/ ((double)individuals[countInd].fitMakespan);
	if(probControl[i].reward > 1)
		return 1;
	else
		return 0;
}//mutationGetSuccess
