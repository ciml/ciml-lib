#include "Individuo.h"
#include "../No.h"

#include <cstdlib>
#include <stdlib.h>
#include <time.h>
#define tipos 7
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


    //pontuacao = 0;
    //qtdLigantes = 0;
}

Individuo::Individuo(Individuo *copia) : Individuo(copia->num_linhas, copia->num_colunas, copia->num_entradas, copia->num_saidas){
    int i, j;
    pontuacao = copia->pontuacao;
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
    bool saida;

    //isFact = false;
    qtdLigantes = 0;
    pontuacao = 0.0;
   /* int pont_saida[num_saidas];
    for(i=0; i< num_saidas; i++){
        pont_saida[i] = 0;
    }

*/
    for(j=0;j < pow(2,num_entradas); j++){
        for(i = 0; i< num_saidas; i++){
            saida = saidas[i]->getSaida(j, tabela_entrada, &qtdLigantes);
            //cout << saida << " : " << tabela_target[j][i] << endl;
            if(saida == tabela_target[j][i]){
                pontuacao++;
                //pont_saida[i]++;
            } else if(isFact){
                pontuacao = 0;
                isFact = false;
                return;
            }

        }
       // cout << endl;
    }
   /* for(i=0; i< num_saidas; i++){
        cout << "Saida " << i <<": " << pont_saida[i]<< endl;
    }
    cout << endl;
    */
    if(pontuacao == pow(2,num_entradas) * num_saidas){
        isFact = true;
        pontuacao = pontuacao + (1.0/qtdLigantes);
    }
}

void Individuo::imprime(){
    for(int i=0;i<num_linhas;i++){
        for(int j=0;j<num_colunas;j++){
            cout << matrizNo[i][j]->entradas[0]->linha << "; " << matrizNo[i][j]->entradas[0]->coluna <<".: "<< matrizNo[i][j]->tipo <<".   "<< matrizNo[i][j]->entradas[1]->linha << "; " << matrizNo[i][j]->entradas[1]->coluna<<"...  ";

        }
        cout << endl;
    }
    for(int i = 0; i<num_saidas; i++){
        cout<< saidas[i]->linha <<";"<<saidas[i]->coluna << endl;
    }
    cout << "Pontuacao: " << pontuacao << " Qtd Ligantes: " << qtdLigantes << endl;
}

void Individuo::mutation(){

    int lin, col, random;
    No *p = NULL;
    do{
        lin = rand() % num_linhas;
        col = rand() % (num_colunas);
        //cout << "Linha e coluna a serem mutados: " << lin << ";" << col <<endl;
        if(col < num_colunas){
            p = matrizNo[lin][col];
        } else {
            col = rand() % num_colunas;
            lin = rand() % num_linhas;
            //cout << "Saida mutada: " << lin << ";" << col <<endl;
            saidas[rand() % num_saidas] = matrizNo[lin][col];
            break;
        }
        random = rand() % (p->num_entradas + 1);
        if(random == p->num_entradas){ //muda tipo
            //cout << "Tipo mutado" << endl;
            p->tipo = (rand() % tipos);
        } else {
            col = (rand()%(col+1))-1;
            //cout << "Entrada mutada para col: " << col <<endl;
            if(col == -1){ //camada entrada
                p->entradas[random] = entradas[rand()%num_entradas];

            } else {
                p->entradas[random] = matrizNo[rand() % num_linhas][col];

            }
        }
    }while(!p->isLigante);
    for(int i =0; i<num_linhas; i++){
        for(int j = 0; j<num_colunas; j++){
            matrizNo[i][j]->isLigante = false;
        }
    }

}

void Individuo::recebe_copia(Individuo *copia){
    int i, j;
    pontuacao = copia->pontuacao;
    isFact = copia->isFact;
    qtdLigantes = copia->qtdLigantes;
    for(i = 0; i < num_colunas; i++){
        for(j = 0; j < num_linhas; j++){ //copia nos;
            if(copia->matrizNo[j][i]->entradas[0]->coluna == -1)
                matrizNo[j][i]->entradas[0] = entradas[copia->matrizNo[j][i]->entradas[0]->linha];
            else
                matrizNo[j][i]->entradas[0] = matrizNo[copia->matrizNo[j][i]->entradas[0]->linha][copia->matrizNo[j][i]->entradas[0]->coluna];

            if(copia->matrizNo[j][i]->entradas[1]->coluna == -1)
                matrizNo[j][i]->entradas[1] = entradas[copia->matrizNo[j][i]->entradas[1]->linha];
            else
                matrizNo[j][i]->entradas[1] = matrizNo[copia->matrizNo[j][i]->entradas[1]->linha][copia->matrizNo[j][i]->entradas[1]->coluna];

            matrizNo[j][i]->tipo = copia->matrizNo[j][i]->tipo;
            matrizNo[j][i]->isLigante = copia->matrizNo[j][i]->isLigante;
            matrizNo[j][i]->linha_cache = -1;
        }
    }
    for(i = 0; i < num_saidas; i++){
        saidas[i] = matrizNo[copia->saidas[i]->linha][copia->saidas[i]->coluna];
    }
}

Individuo::~Individuo()
{
    //dtor
    for(int i = 0; i < num_linhas; i++){
        for(int j = 0; j< num_colunas; j++){
            delete matrizNo[i][j];
        }
        delete[] matrizNo[i];
    }
    delete[] matrizNo;
    delete[] entradas;
    delete[] saidas;
}
