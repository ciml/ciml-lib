
#include "No.h"
#include <../include/Individuo.h>
#include <iostream>
#include <cstddef>
#include <vector>
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
            //entradas = new No *[num_entradas];
            break;
        case 1: //or
            num_entradas = 2;
            this->custo = 0;
            //entradas = new No *[num_entradas];
            break;
        case 2: //nand
            num_entradas = 2;
            this->custo = 0;
            //entradas = new No *[num_entradas];
            break;
        case 3: //nor
            num_entradas = 2;
            this->custo = 0;
            break;
        case 4: //not
            num_entradas = 2;
            this->custo = 0;
            break;
        case 5:
            num_entradas = 2;
            this->custo = 0;
            break;
        case 6:
            num_entradas = 2;
            this->custo = 0;
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
        if(copia->entradas[i].coluna == -1){
            entradas.push_back(*ind->entradas[copia->entradas[i].linha]);
        } else {
            entradas.push_back(*ind->matrizNo[copia->entradas[i].linha][copia->entradas[i].coluna]);
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
            return ((entradas[0].getSaida(j, tabela_entrada))&&(entradas[1].getSaida(j, tabela_entrada)));
        case 1: //or
            return entradas[0].getSaida(j, tabela_entrada)|entradas[1].getSaida(j, tabela_entrada);
        case 2: //n-and
            return !(entradas[0].getSaida(j, tabela_entrada)*entradas[1].getSaida(j, tabela_entrada));
        case 3: //nor
            return !(entradas[0].getSaida(j, tabela_entrada)|entradas[1].getSaida(j, tabela_entrada));
        case 4: //not
            return !(entradas[0].getSaida(j, tabela_entrada));
        case 5: //ID
            return (entradas[0].getSaida(j, tabela_entrada));
        case 6: //xor
            return (entradas[0].getSaida(j, tabela_entrada)*!entradas[1].getSaida(j, tabela_entrada))|(!entradas[0].getSaida(j, tabela_entrada)*entradas[1].getSaida(j, tabela_entrada));
        case -1:
            return tabela_entrada[j][this->linha];
        default:
            return 0;
            break;
    }
}

void No::mudaTipo(int tipo){
    this->tipo = tipo;
    switch(tipo){
        case 4:
            num_entradas = 2;
            break;
            default:
            num_entradas = 2;
            break;
    }
}



