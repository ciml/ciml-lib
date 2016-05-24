/*
 * RosenbrockObjectiveFunction.h
 *
 *  Created on: 4 de mai de 2016
 *      Author: igor
 */

#ifndef ROSENBROCKOBJECTIVEFUNCTION_H_
#define ROSENBROCKOBJECTIVEFUNCTION_H_

#include "ObjectiveFunction.h"

class RosenbrockObjectiveFunction: public ObjectiveFunction {
public:
	RosenbrockObjectiveFunction(int vectorLength);
	virtual ~RosenbrockObjectiveFunction();
	float Evaluate(int *v);
	float EvaluateFloat(float *v);
	string getName();
	string getFormula();
	string getSumLimit();
};

#endif /* ROSENBROCKOBJECTIVEFUNCTION_H_ */
