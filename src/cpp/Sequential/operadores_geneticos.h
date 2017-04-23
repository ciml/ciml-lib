#ifndef OPERADORES_GENETICOS_H_INCLUDED
#define OPERADORES_GENETICOS_H_INCLUDED

#include "parametros_ag.h"
#include "representacao.h"

void Mutacao(individuo* individuo, float chance);

void CrossoverUmPonto(individuo *pai1, individuo*pai2,individuo *filho1, individuo *filho2);

void Recombinacao(individuo *pai1, individuo *pai2, individuo *filho1, individuo *filho2, float chance);

#endif // OPERADORES_GENETICOS_H_INCLUDED
