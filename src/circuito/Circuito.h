#ifndef CIRCUITO_H
#define CIRCUITO_H
#include <algorithm>
#include <cmath>
#include <fstream>
#include <math.h>
#include <numeric>
#include <vector>
#include <time.h>

using namespace std;

class Circuito
{
    public:
        Circuito(int nLinhas, int nColunas, int levelsBack, int filhos); //lê o .txt e cria a tabela
        virtual ~Circuito(){ };

        bool geneAtivo(vector <int>mapa, size_t val);

        int calcAcertos(vector< vector<bool> > entrada);
        int levelBackFunc(int col);
        int funcNosAtivos(vector< vector<bool> > entrada, vector< vector<int> > mapa);
        int nNosAtivos(vector<int> mapa);
        int novoValor(size_t pos);

        vector<int> funcAlteracao(vector<int> vec, vector<int> mapa);
        vector<int> funcVetorAleatorio();
        vector< vector<bool> > funcIn(vector<int> vec);
        vector< vector<int> > mapear(vector<int> vec);

        void auxMapear(vector< vector <int> > *mapa, int pos);
        void funcV7();

    private:
        int nEntradas;
        int nLinhas;
        int nColunas;
        int nSaidas;
        int nLinhasTabela;
        int levelsBack;
        int nFilhos;
        size_t vecSize, mapSize;

        vector< vector<bool> > In;
        vector< vector<bool> > Tabela;

    protected:

};

#endif // CIRCUITO_H
