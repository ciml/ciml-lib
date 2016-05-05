#include "Individuo.h"

Individuo::Individuo(int lin, int col, int num_entradas, int num_saidas)
{
    //ctor
    int i;
    this->num_linhas = lin;
    this->num_colunas = col;
    matrizNo = new No **[lin];
    for(i=0;i<lin;i++){
        matrizNo[i] = new No *[col];
    }
    entradas = new No *[num_entradas];
    saidas = new No *[num_saidas];
    this->num_saidas = num_saidas;
    this->num_entradas = num_entradas;
    for(i=0;i<num_entradas;i++){
        entradas[i] = new No(-1,i,-1);
    }


    pontuacao = 0;
    qtdLigantes = 0;
}


void Individuo::avalia(bool **tabela_entrada, bool **tabela_target){
    int i, j;
    No *p;
    bool saida;
    for(i = 0; i< num_saidas; i++){
        for(j=0;j < pow(2,num_entradas); j++){
            saida = saidas[i]->entradas[0]->getSaida(j, tabela_entrada);
            if(saida == tabela_target[j][i])
                pontuacao++;
        }
    }
}

void Individuo::imprime(){
    for(int i=0;i<num_linhas;i++){
        for(int j=0;j<num_colunas;j++){
            cout << matrizNo[i][j]->entradas[0]->linha << "; " << matrizNo[i][j]->entradas[0]->coluna <<".  "<< matrizNo[i][j]->entradas[1]->linha << "; " << matrizNo[i][j]->entradas[1]->coluna<<"...  ";

        }
        cout << endl;
    }
    for(int i = 0; i<num_saidas; i++){
        cout<< saidas[i]->linha <<";"<<saidas[i]->coluna << endl;
    }
}


Individuo::~Individuo()
{
    //dtor
}
