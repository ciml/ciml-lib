
#include "No.h"
#include <../include/Individuo.h>
#include <iostream>
#include <cstddef>
No::No(int tipo, int j, int k)
{
    this->tipo = tipo;
    linha = j;
    coluna = k;
    switch(tipo){
        case -1: //entrada
            num_entradas = 0;
            this->custo = 0;
            break;

        case 0: //and
            num_entradas = 2;
            this->custo = 0;
            entradas = new No *[num_entradas];
            break;
        case 1: //or
            num_entradas = 2;
            this->custo = 0;
            entradas = new No *[num_entradas];
            break;
        case 2: //nand
            num_entradas = 2;
            this->custo = 0;
            entradas = new No *[num_entradas];
            break;
        default:
            throw "tipo não permitido";
            break;
    }

    isLigante = false;
    //entradas = ind_entradas;
    //ctor
}

No::No(No *copia, Individuo *ind) : No(copia->tipo, copia->linha, copia->coluna){



    isLigante = copia->isLigante;
    for(int i=0;i<num_entradas;i++){
        if(copia->entradas[i]->coluna == -1){
            entradas[i] = ind->entradas[copia->entradas[i]->linha];
        } else {
            entradas[i] = ind->matrizNo[copia->entradas[i]->linha][copia->entradas[i]->coluna];
        }

    }
}

No::~No()
{
    //dtor

}

bool No::getSaida(int j, bool **tabela_entrada){
    isLigante = true;
    switch(tipo){
        case 0: //and
            return entradas[0]->getSaida(j, tabela_entrada)&&entradas[1]->getSaida(j, tabela_entrada);
        case 1: //or
            return entradas[0]->getSaida(j, tabela_entrada)||entradas[1]->getSaida(j, tabela_entrada);
        case 3: //n-and
            return !(entradas[0]->getSaida(j, tabela_entrada)&&entradas[1]->getSaida(j, tabela_entrada));
        case -1:
            return tabela_entrada[j][this->linha];
        default:
            return 0;
            break;
    }
}



