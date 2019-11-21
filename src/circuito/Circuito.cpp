#include "Circuito.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <math.h>
#include <numeric>
#include <string>
#include <vector>
#include <time.h>

using namespace std;

Circuito::Circuito(int nLinhas, int nColunas, int levelsBack, int filhos)
{
    int aux;

    ifstream read("tabela4+.txt");

    read>>aux;
    nLinhasTabela = aux;
    nEntradas = log2(aux);
    read>>aux;
    nSaidas = aux;
    nFilhos = filhos;

    //criaTabela(inLinhasTabela, nEntradas, nSaidas);

    cout << endl << "Numero de entradas: " << nEntradas << endl;
    cout << "Numero de saidas: " << nSaidas << endl;

    vector< vector<bool> > auxEntrada(nLinhasTabela, vector<bool>(nEntradas, 0)), auxTabela(nLinhasTabela, vector<bool>(nSaidas, 0));

    cout << "Tabela:" << endl;
    for(int i = 0; i < nLinhasTabela; i++)
    {
        for(int j = 0; j < nEntradas; j++)
        {
            read>>aux;
            auxEntrada[i][j] = aux;
            cout << auxEntrada[i][j] << ((j == nEntradas-1) ? " " : "");
        }
        for(int j = 0; j < nSaidas; j++)
        {
            read>>aux;
            auxTabela[i][j] = aux;
            cout << auxTabela[i][j] << ((j == nSaidas-1) ? ((i == nLinhasTabela-1) ? "\n\n" : "\n") : "");
        }
    }

    this->nLinhas = nLinhas;
    this->nColunas = nColunas;
    this->levelsBack = levelsBack;
    this->Tabela = auxTabela;
    this->Entrada = auxEntrada;
}

int Circuito::levelBackFunc(int col)
{
    int lim = nEntradas+nLinhas*(col-1), num = rand()%lim;
    if(col > levelsBack)
        while((num >= nEntradas) && (num <= ((lim-1)-levelsBack*nLinhas)))
            num = rand()%lim;
    return num;
}

void Circuito::funcIn(vector<int> vec)
{
    int aux = nEntradas, cont = nLinhas*nColunas*3, contIn = nEntradas+(nLinhas*nColunas)+nSaidas;
    for(int i = 0; i < nLinhasTabela; i++, aux = nEntradas)
        for(int j = 2; j < cont; j += 3, aux++)
        {
            int aux1 = vec[j-2], aux2 = vec[j-1];

            if(vec[j] == 1) Entrada[i][aux] = (Entrada[i][aux1] && Entrada[i][aux2]); //AND
            else if(vec[j] == 2) Entrada[i][aux] = (Entrada[i][aux1] || Entrada[i][aux2]); //OR
            else if(vec[j] == 3) Entrada[i][aux] = !(Entrada[i][aux1]); //NOT
            else Entrada[i][aux] = (Entrada[i][aux1] != Entrada[i][aux2]); //XOR
        }
    for(int i = 0; i < nLinhasTabela; i++)
        for(int j = nEntradas+(nLinhas*nColunas), auxTotal = cont; j < contIn; j++, auxTotal++)
        {
            int aux2 = vec[auxTotal];
            Entrada[i][j] = Entrada[i][aux2];
        }
}

int Circuito::calcAcertos(int row)
{
    int acertos = 0;
    for(int i = 0, auxIn = nEntradas+(nLinhas*nColunas); i < nSaidas; i++, auxIn++)
        acertos += (Entrada[row][auxIn] == Tabela[row][i]);
    return acertos;
}

void Circuito::auxMapear(vector< vector <int> > mapa, int pos)
{
    if(!mapa[0][pos])
    {
        mapa[0][pos] = true;
        if(mapa[1][pos] >= nEntradas) auxMapear(mapa, mapa[1][pos]-nEntradas);
        if(mapa[2][pos] >= nEntradas) auxMapear(mapa, mapa[2][pos]-nEntradas);
    }
}

void Circuito::mapear(vector<int> vec, vector< vector<int> > mapa)
{
    mapa[0] = vector<int>(mapa[0].size(), 0);
    for(int i = 0, j = (mapa[0].size()*3); i < nSaidas; i++, j++)
        if(vec[j] >= nEntradas)
            auxMapear(mapa, vec[j]-nEntradas);
}

vector<int> Circuito::funcVetorAleatorio(vector< vector <int> > mapa)
{
    vector<int> vec((nLinhas*nColunas*3)+nSaidas, -1);
    int col = 1, cont = nLinhas*nColunas*3;

    for(int i = 2, j = 0; i < cont; i += 3, j++)
    {
        vec[i] = rand()%4+1;
        vec[i-2] = levelBackFunc(col);
        vec[i-1] = levelBackFunc(col);

        mapa[1][j] = vec[i-2];
        mapa[2][j] = vec[i-1];

        col += (i%nLinhas != 0);
    }

    for(int i = cont; i < (cont+nSaidas); i++)
        vec[i] = levelBackFunc(nColunas+1);

    return vec;
}

int Circuito::novoValor(int pos)
{
    if(pos >= nLinhas*nColunas*3) return levelBackFunc(nColunas+1);
    else return (!((pos+1)%3) ? (rand()%4+1) : levelBackFunc(((int)(pos/(nLinhas*nColunas))+1)));
}

bool Circuito::geneAtivo(vector< vector <int> > mapa, int val)
{
    return ((unsigned(val) >= mapa[0].size()) ? true : mapa[0][val]);
}

vector<int> Circuito::funcAlteracao(vector<int> vec, vector< vector<int> > mapa)
{
    vector<int> vec2 = vec;
    bool flag = false;
    while(!flag)
    {
        int novoIndice = rand()%(vec2.size());
        flag = geneAtivo(mapa, (int)(novoIndice/3));

        int aux = novoValor(novoIndice);
        while(vec2[novoIndice] == aux)
            aux = novoValor(novoIndice);

        vec2[novoIndice] = aux;
        if((unsigned(novoIndice) < mapa[0].size()*3) && ((novoIndice+1)%3))
        {
            int auxLinha = (novoIndice+1)%3, auxCol = (novoIndice/3);
            mapa[auxLinha][auxCol] = aux;
        }
    }
    return vec2;
}

int Circuito::nosAtivos(vector< vector<int> > mapa)
{
    return accumulate(mapa[0].begin(), mapa[0].end(), 0);
}

int Circuito::funcNosAtivos(vector< vector<int> > mapa)
{
    bool flag = false;
    for(int i = 0; i < nLinhasTabela; i++)
    {
        for(int j = 0; j < nSaidas; j++)
        {
            int auxIn = nEntradas+(nLinhas*nColunas)+j;
            if(Entrada[i][auxIn] != Tabela[i][j])
            {
                flag = true;
                break;
            }
        }
        if(flag) return mapa[0].size();
    }
    return nosAtivos(mapa);
}

void Circuito::funcV7()
{
    vector< vector<int> > mapa(3, vector<int>(nLinhas*nColunas, 0));
    vector<int> vec = funcVetorAleatorio(mapa);

    int auxAcerto = 0, auxRazao = 1;
    bool maximo = false;

    vector< vector<int> > mapa2 = mapa, mapaAux = mapa;
    vector<int> vec2 = vec, vecAux = vec;

    for(int auxRep = 1; auxRep <= 1000000; auxRep++)
    {
        bool novoFilho = false;
        vec = vecAux;
        mapear(vecAux, mapaAux);

//        if(auxRazao/auxRep >= .95)
//        {
//            filhos = rep;
//            auxRazao = auxRep*.95;
//        }
//        if(auxRazao/auxRep <= .05)
//        {
//            if(filhos < pow(rep,2)) filhos++;
//            auxRazao = auxRep*.95;
//        }

        for(int i = 0, acertos = 0; i < nFilhos; i++, acertos = 0)
        {
            vec2 = funcAlteracao(vec, mapa2);
            for(int j = 0; j < nLinhasTabela; j++)
            {
                funcIn(vec2);
                acertos += calcAcertos(j);
            }

            if(acertos >= auxAcerto)
            {
                if(!novoFilho)
                {
                    auxRazao++;
                    novoFilho = true;
                }
                if(acertos == auxAcerto)
                {
                    mapear(vec2, mapa2);
                    mapear(vecAux, mapaAux);
                }
                if((acertos > auxAcerto) || ((acertos == auxAcerto) && (nosAtivos(mapa2) <= nosAtivos(mapaAux))))
                {
                    if(acertos > auxAcerto) cout << "i = " << auxRep << ": " << acertos << "/" << nLinhasTabela*nSaidas << endl;
//                    else cout << nosAtivos(mapa2) << "nos ativos / "  << filhos << "filhos" << endl;
                    auxAcerto = acertos;
                    vecAux = vec2;
                    mapaAux = mapa2;
                }

                if(auxAcerto >= nLinhasTabela*nSaidas)
                {
                    cout << "Maximo (" << auxAcerto << "/" << nLinhasTabela*nSaidas << ") atingido ";
                    cout << "em " << auxRep << " iteracoes!\n" << endl;
                    maximo = true;
                }
            }
        }

        if(!(auxRep%100000))
            cout << "--checkpoint: i = " << auxRep << "--" << endl;

        if(maximo) break;
    }

    vec = vecAux;
    mapear(vecAux, mapa);

    for(unsigned int i = 0 ; i < vec.size(); i++)
        cout << vec[i] << (unsigned(i+1)%3 == 0 ? ((unsigned(i) <= vec.size()-nSaidas) ? " | " : " ") : " ") << ((unsigned(i) == vec.size()-1) ? "   <-------\n\n" : "");

    cout << nosAtivos(mapa) << " nos ativos\nDiminuindo nos ativos agora!" << endl << endl;

//    for(int auxRep = 1, cont = 0; auxRep <= 1; auxRep++, cont++)
//    {
//        int auxNosAtivos = nosAtivos(mapa);
//        if(auxNosAtivos == 0) break;
//        vec = vecAux;
//        mapa = mapaAux;
//
//        vec2 = funcAlteracao();
//        for(int j = 0; j < nLinhasTabela; j++)
//            funcIn();
//
//        for(int i = 0; i < nFilhos; i++)
//        {
//            if(funcNosAtivos(mapa2) < auxNosAtivos)
//            {
//                cout << cont << ": i = " << auxRep << " -> " << nosAtivos(mapa2) << " nos ativos" << endl;
//                vecAux = vec2;
//                mapaAux = mapa2;
//                auxNosAtivos = nosAtivos(mapa2);
//
//                cont = 0;
//            }
//        }
//
//        if(!(auxRep%100000))
//            cout << "--checkpoint: i = " << auxRep << " -- " << nosAtivos(mapaAux) << "nos ativos" << endl;
//    }
    cout << "Reducao final: " << nosAtivos(mapaAux) << " nos ativos" << endl << endl;
}
