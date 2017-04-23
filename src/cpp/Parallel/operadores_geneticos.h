#ifndef OPERADORES_GENETICOS_H_INCLUDED
#define OPERADORES_GENETICOS_H_INCLUDED

#include "parametros_ag.h"
#include "representacao.h"

void mutacao(individuo* individuo, float chance);
void crossover_um_ponto(individuo *pai1, individuo*pai2,individuo *filho1, individuo *filho2);

void recombinacao(individuo *pai1, individuo *pai2, individuo *filho1, individuo *filho2, float chance);

#endif // OPERADORES_GENETICOS_H_INCLUDED
