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

        bool geneAtivo(vector< vector <int> > mapa, int val);

        int calcAcertos(int row);
        int levelBackFunc(int col);
        int funcNosAtivos(vector< vector<int> > mapa);
        int nosAtivos(vector< vector<int> > mapa);
        int novoValor(int pos);

        vector<int> funcAlteracao(vector<int> vec, vector< vector<int> > mapa);
        vector<int> funcVetorAleatorio(vector< vector <int> > mapa);

        void auxMapear(vector< vector <int> > mapa, int pos);
        void funcIn(vector<int> vec);
        void funcV7();
        void mapear(vector<int> vec, vector< vector<int> > mapa);

    private:
        int nEntradas;
        int nLinhas;
        int nColunas;
        int nSaidas;
        int nLinhasTabela;
        int levelsBack;
        int nFilhos;

        vector< vector<bool> > Entrada;
        vector< vector<bool> > Tabela;

    protected:

};

#endif // CIRCUITO_H
