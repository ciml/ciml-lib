#include "Individuo.h"
#include "../No.h"

#include <cstdlib>
#include <stdlib.h>
#include <time.h>
#define tipos 3
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

Individuo::Individuo(Individuo *copia) : Individuo(copia->num_linhas, copia->num_colunas, copia->num_entradas, copia->num_saidas){
    int i, j;
    for(i = 0; i < num_colunas; i++){
        for(j = 0; j < num_linhas; j++){

            matrizNo[j][i] = new No(copia->matrizNo[j][i], this);
        }
    }
    for(i = 0; i < num_saidas; i++){
        saidas[i] = matrizNo[copia->saidas[i]->linha][copia->saidas[i]->coluna];
    }

}

void Individuo::avalia(bool **tabela_entrada, bool **tabela_target){
    int i, j;
    No *p;
    bool saida;
    for(i = 0; i< num_saidas; i++){
        //cout << "Saida " << i+1 << endl;
        for(j=0;j < pow(2,num_entradas); j++){
            saida = saidas[i]->getSaida(j, tabela_entrada);
            //cout << saida << " : " << tabela_target[j][i] << endl;
            if(saida == tabela_target[j][i])
                pontuacao++;
        }
       // cout << endl;
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

void Individuo::mutation(){
    //srand (time(NULL));

    int lin, col, random;
    No *p = NULL;
    do{
        lin = rand() % num_linhas;
        col = rand() % (num_colunas+1);
        if(col < num_colunas){
            p = matrizNo[lin][col];
        } else {
            lin = rand() % num_linhas;
            col = rand() % num_colunas;
            saidas[rand()%num_saidas] = matrizNo[lin][col];
            break;
        }
        random = rand() % (num_entradas + 1);
        if(random == num_entradas){ //muda tipo
            p->tipo = rand() % tipos;
        } else {
            col = (rand()%(col+1))-1;
            if(col == -1){ //camada entrada
                p->entradas[random] = entradas[rand()%num_entradas];
            } else {
                p->entradas[random] = matrizNo[rand() % num_linhas][col];
            }
        }
        //cout << p->isLigante << endl;
    }while(!p->isLigante);
    if(p != NULL)
        p->isLigante = false;

}

Individuo::~Individuo()
{
    //dtor
    free(matrizNo);
}
