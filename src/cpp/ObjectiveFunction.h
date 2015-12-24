/*
 * ObjectiveFunction.h
 *
 *  Created on: 02/07/2015
 *      Author: igorsr
 */

#ifndef OBJECTIVEFUNCTION_H_
#define OBJECTIVEFUNCTION_H_

#include "representacao.h"

class ObjectiveFunction {
public:
	ObjectiveFunction(int vectorLength);
	virtual ~ObjectiveFunction();
	virtual float Evaluate(int *v) = 0;
protected:
	int m_vectorLength;
};

#endif /* OBJECTIVEFUNCTION_H_ */
