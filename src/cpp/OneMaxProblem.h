/*
 * OneMaxProblem.h
 *
 *  Created on: 02/07/2015
 *      Author: igorsr
 */

#ifndef ONEMAXPROBLEM_H_
#define ONEMAXPROBLEM_H_

#include "ObjectiveFunction.h"

class OneMaxProblem: public ObjectiveFunction {
public:
	OneMaxProblem(int vectorLength);
	virtual ~OneMaxProblem();
	float Evaluate(int *v);
	float EvaluateFloat(float *v);
	string getName();
	string getFormula();
	string getSumLimit();
};

#endif /* ONEMAXPROBLEM_H_ */
