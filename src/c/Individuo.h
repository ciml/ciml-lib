#ifndef INDIVIDUO_H
#define INDIVIDUO_H
class No;
#include <cstdlib>
#include <iostream>
#include <math.h>
using namespace std;
class Individuo
{
    public:
        Individuo(int lin, int col, int num_entradas, int num_saidas);
        Individuo(Individuo *copia);
        virtual ~Individuo();
        No ***matrizNo;
        No **entradas, **saidas;
        double pontuacao;
        int qtdLigantes;
        int num_linhas, num_colunas, num_saidas, num_entradas;
        bool isFact;


        void imprime();
        void mutation();
        void avalia(bool **tabela_entrada, bool **tabela_saida);
        void recebe_copia(Individuo *copia);
    protected:
    private:
};

#endif // INDIVIDUO_H
