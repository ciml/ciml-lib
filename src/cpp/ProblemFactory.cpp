/*
 * ProblemFactory.cpp
 *
 *  Created on: 4 de jun de 2016
 *      Author: igor
 */

#include "ProblemFactory.h"

ProblemFactory::ProblemFactory() {
	// TODO Auto-generated constructor stub

}

ProblemFactory::~ProblemFactory() {
	// TODO Auto-generated destructor stub
}

ObjectiveFunction* ProblemFactory::CreateProblem(int ID, int dimensions)
{
	switch (ID) {
			case 1:
				return new OneMaxProblem(dimensions);
				break;
			case 2:
				return new ElipsoidalObjectiveFunction(dimensions);
				break;
			case 3:
				return new RosenbrockObjectiveFunction(dimensions);
				break;
			default:
				return new OneMaxProblem(dimensions);
				break;
		}
}

