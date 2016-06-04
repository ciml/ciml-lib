/*
 * ObjectiveFunction.h
 *
 *  Created on: 02/07/2015
 *      Author: igorsr
 */

#ifndef OBJECTIVEFUNCTION_H_
#define OBJECTIVEFUNCTION_H_

#include <string>
#include "representation.h"

using namespace std;

class ObjectiveFunction {
public:
	ObjectiveFunction(int vectorLength);
	virtual ~ObjectiveFunction();
	virtual float Evaluate(int *v)=0;
	virtual float EvaluateFloat(float *v)=0;
	virtual string getName() = 0;
	virtual string getFormula() = 0;
	virtual string getSumLimit() = 0;
protected:
	int m_vectorLength;
};

#endif /* OBJECTIVEFUNCTION_H_ */
