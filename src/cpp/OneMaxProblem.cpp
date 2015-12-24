#include "OneMaxProblem.h"

OneMaxProblem::OneMaxProblem(int vectorLength) : ObjectiveFunction(vectorLength) {
}

OneMaxProblem::~OneMaxProblem() {
}

float OneMaxProblem::Evaluate(int *v)
{
	float soma = 0;
	for(int i=0;i< m_vectorLength; i++){
		soma += v[i];
	}
	return -soma;
}
