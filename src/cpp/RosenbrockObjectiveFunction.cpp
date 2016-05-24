/*
 * RosenbrockObjectiveFunction.cpp
 *
 *  Created on: 4 de mai de 2016
 *      Author: igor
 */

#include "RosenbrockObjectiveFunction.h"

RosenbrockObjectiveFunction::RosenbrockObjectiveFunction(int vectorLength)
: ObjectiveFunction(vectorLength) {
}

RosenbrockObjectiveFunction::~RosenbrockObjectiveFunction() {
	// TODO Auto-generated destructor stub
}

float RosenbrockObjectiveFunction::Evaluate(int *v)
{
	return 0;
}

float RosenbrockObjectiveFunction::EvaluateFloat(float *x)
{
	float sum = 0, a ,b;
	int j;
	for(int i=0; i < m_vectorLength/2; i++){
		a = x[(2*i)-1] - x[2*i];
		b = x[(2*i)-1]-1;
		sum += 100.0 *(a*a) + b*b;
	}

	return -sum;
}

string RosenbrockObjectiveFunction::getName()
{
	return "ROSENBROCK";
}

string RosenbrockObjectiveFunction::getFormula()
{
	//return "OBJECTIVE_FUNCTION(i,x)=(100*(x[(2*i)-1] - x[2*i])*(x[(2*i)-1] - x[2*i]) + (x[(2*i)-1]-1)*(x[(2*i)-1]-1))";
	return "OBJECTIVE_FUNCTION(i,x)=(100*(x[(2*i)-1]-x[2*i])*(x[(2*i)-1]-x[2*i])+(x[(2*i)-1]-1)*(x[(2*i)-1]-1))";
}

string RosenbrockObjectiveFunction::getSumLimit()
{
	return "SUM_LIMIT(dimensions)=dimensions/2";
}

