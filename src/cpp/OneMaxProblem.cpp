#include "OneMaxProblem.h"

#include <iostream>
using namespace std;

OneMaxProblem::OneMaxProblem(int vectorLength) : ObjectiveFunction(vectorLength)
{
}

OneMaxProblem::~OneMaxProblem()
{
}

float OneMaxProblem::Evaluate(int *v)
{
	float soma = 0;
	for(int i=0;i< m_vectorLength; i++)
	{
		soma += v[i];
	}
	return -soma;
}

float OneMaxProblem::EvaluateFloat(float *v)
{
	float soma = 0;
	for(int i=0;i< m_vectorLength; i++)
	{
		soma += v[i];
	}
	return -soma;
}

string OneMaxProblem::getName()
{
	return "ONEMAX";
}

string OneMaxProblem::getFormula()
{
	return "OBJECTIVE_FUNCTION(idx,phenotype)=(phenotype[idx])";
}

string OneMaxProblem::getSumLimit()
{
	return "SUM_LIMIT(dimensions)=dimensions";
}
