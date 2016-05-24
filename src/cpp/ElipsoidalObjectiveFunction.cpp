/*
 * ElipsoidalObjectiveFunction.cpp
 *
 *  Created on: 4 de mai de 2016
 *      Author: igor
 */

#include "ElipsoidalObjectiveFunction.h"

#include <iostream>

using namespace std;

ElipsoidalObjectiveFunction::ElipsoidalObjectiveFunction(int vectorLength)
: ObjectiveFunction(vectorLength) {
}


ElipsoidalObjectiveFunction::~ElipsoidalObjectiveFunction() {
	// TODO Auto-generated destructor stub
}

float ElipsoidalObjectiveFunction::Evaluate(int *v)
{
	return 0;
}

float ElipsoidalObjectiveFunction::EvaluateFloat(float *v)
{
	float sum = 0;
	for(int i=0;i< m_vectorLength; i++){
		//cout << v[i] << " ";
		sum += i*v[i]*v[i];
	}
	//cout << " -> " << sum << endl;
	return -sum;
}

string ElipsoidalObjectiveFunction::getName() {
	return "ELIPSOIDAL";
}

string ElipsoidalObjectiveFunction::getFormula()
{
	return "OBJECTIVE_FUNCTION(idx,phenotype)=(idx*phenotype[idx]*phenotype[idx])";
}

string ElipsoidalObjectiveFunction::getSumLimit()
{
	return "SUM_LIMIT(dimensions)=dimensions";
}

