/*
 * ProblemFactory.h
 *
 *  Created on: 4 de jun de 2016
 *      Author: igor
 */

#ifndef PROBLEMFACTORY_H_
#define PROBLEMFACTORY_H_

#include "ObjectiveFunction.h"
#include "OneMaxProblem.h"
#include "ElipsoidalObjectiveFunction.h"
#include "RosenbrockObjectiveFunction.h"

class ProblemFactory {
public:
	ProblemFactory();
	virtual ~ProblemFactory();

	static ObjectiveFunction* CreateProblem(int ID, int dimensions);
};

#endif /* PROBLEMFACTORY_H_ */
