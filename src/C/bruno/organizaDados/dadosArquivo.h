#ifndef DADOSARQUIVO_H_INCLUDED
#define DADOSARQUIVO_H_INCLUDED

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>

using namespace std;
#ifdef __cplusplus
extern "C"{
#endif

#include "constantes.h"
#include "bitwise.h"
#include "arvore.h"
#include "pilha.h"

void cabecalho();
void montaTabelas(Arvore melhores[]);
void ordena(Arvore pop[], int k);
void salvaArvorePre(Arvore* arv, fstream& arq);
void salvaSinxate(int info, fstream& arq);

#ifdef __cplusplus
}
#endif

#endif // DADOSARQUIVO_H_INCLUDED
