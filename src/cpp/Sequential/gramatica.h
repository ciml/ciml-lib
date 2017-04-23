#ifndef GRAMATICA_H_INCLUDED
#define GRAMATICA_H_INCLUDED

#include "parser.h"
#include <string.h>
#include <stdlib.h>

#define DELIMITADOR_REGRAS "$"
#define DELIMITADOR_ESCOLHAS "|"
#define DELIMITADOR_TERMINAIS " "

void LeGramatica(char nomeArquivo[], t_regra  * Gramatica);
void ProcessaEscolha(char * escolha, t_escolha * nova_escolha);
int Decodifica(t_regra * gramatica, short * fenotipo, t_item_programa * programa);

#endif // GRAMATICA_H_INCLUDED
