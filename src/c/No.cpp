#include "No.h"
#include <cstddef>
No::No(int tipo, int j, int k)
{
    this->tipo = tipo;
    linha = j;
    coluna = k;
    switch(tipo){
        case -1: //entrada
            num_entradas = 0;
            break;

        case 0: //and
            num_entradas = 2;
            entradas = new No *[num_entradas];
            break;
        default:
            throw "tipo não permitido";
            break;
    }
    this->custo = 0;
    isLigante = false;
    //entradas = ind_entradas;
    //ctor
}

No::~No()
{
    //dtor
}

bool No::getSaida(int j, bool **tabela_entrada){
    switch(tipo){
        case 0: //and
            return entradas[0]&&entradas[1];
        case -1:
            return tabela_entrada[j][this->linha];
        default:
            return 0;
            break;
    }
}



