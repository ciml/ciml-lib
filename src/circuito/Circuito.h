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

        bool geneAtivo(int val);

        int calcAcertos(int row);
        int levelBackFunc(int col);
        int nosAtivos(vector< vector<int> > mapa);
        int novoValor(int pos);

        vector<int> funcAlteracao(vector<int> vec, vector< vector<int> > mapa);
        vector<int> funcVetorAleatorio();
        void funcIn();
        int funcNosAtivos(vector< vector<int> > mapa);
        void mapear(vector<int> vec, vector< vector<int> > mapa);

        void auxMapear(int pos);
        void funcV7();

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
        vector< vector<int> > Mapa;
        vector<int> Vec;

    protected:

};

#endif // CIRCUITO_H
