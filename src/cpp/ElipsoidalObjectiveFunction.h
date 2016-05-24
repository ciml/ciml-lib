/*
 * ElipsoidalObjectiveFunction.h
 *
 *  Created on: 4 de mai de 2016
 *      Author: igor
 */

#ifndef ELIPSOIDALOBJECTIVEFUNCTION_H_
#define ELIPSOIDALOBJECTIVEFUNCTION_H_

#include "ObjectiveFunction.h"

class ElipsoidalObjectiveFunction: public ObjectiveFunction {
public:
	ElipsoidalObjectiveFunction(int vectorLength);
	virtual ~ElipsoidalObjectiveFunction();
	float Evaluate(int *v);
	float EvaluateFloat(float *v);
	string getName();
	string getFormula();
	string getSumLimit();
};

#endif /* ELIPSOIDALOBJECTIVEFUNCTION_H_ */


