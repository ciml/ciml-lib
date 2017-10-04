#ifndef AG_H_INCLUDED
#define AG_H_INCLUDED

#include "representacao.h"

void CriaPopulacaoInicial(individuo * pop);

void AvaliaPopulacao(individuo * pop);

void Torneio(int indice_participante, individuo *populacao, individuo *retorno);

int ComparaIndividuo(const void *a, const void *b);

void AdicionaIndividuo(individuo *p, individuo * pop, int indice);

void Sort(individuo * pop);

void CriaNovaPopulacao(individuo * pop, individuo * newPop);

void SubstituiPopulacao(individuo *pop, individuo * newPop);

#endif // AG_H_INCLUDED
