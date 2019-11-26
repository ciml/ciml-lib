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
    this->nLinhasTabela = aux;
    this->nEntradas = log2(aux);
    read>>aux;
    this->nSaidas = aux;
    this->nFilhos = filhos;

    cout << endl << "Numero de entradas: " << nEntradas << endl;
    cout << "Numero de saidas: " << nSaidas << endl;

    vector< vector<bool> > auxEntrada(nLinhasTabela, vector<bool>(nEntradas, 0)), auxTabela(nLinhasTabela, vector<bool>(nSaidas, 0));

    cout << "Tabela:" << endl;
    for(size_t i = 0; i < size_t(nLinhasTabela); i++)
    {
        for(size_t j = 0; j < size_t(nEntradas); j++)
        {
            read>>aux;
            auxEntrada[i][j] = aux;
            cout << auxEntrada[i][j] << ((j == size_t(nEntradas-1)) ? " " : "");
        }
        for(size_t j = 0; j < size_t(nSaidas); j++)
        {
            read>>aux;
            auxTabela[i][j] = aux;
            cout << auxTabela[i][j] << ((j == size_t(nSaidas-1)) ? ((i == size_t(nLinhasTabela-1)) ? "\n\n" : "\n") : "");
        }
    }
    this->nLinhas = nLinhas;
    this->nColunas = nColunas;
    this->levelsBack = levelsBack;
    this->In = auxEntrada;
    this->Tabela = auxTabela;
    this->vecSize = (nLinhas*nColunas*3)+nSaidas;
    this->mapSize = (nLinhas*nColunas);
}

int Circuito::levelBackFunc(int col)
{
    int lim = nEntradas+nLinhas*(col-1), num = rand()%lim;
    if(col > levelsBack)
        while((num >= nEntradas) && (num <= ((lim-1)-levelsBack*nLinhas)))
            num = rand()%lim;
    return num;
}

vector< vector<bool> > Circuito::funcIn(vector<int> vec)
{
    vector< vector<bool> > entrada(nLinhasTabela, vector<bool>(nEntradas+(nLinhas*nColunas)+nSaidas, 0));
    int cont = nLinhas*nColunas*3;
    for(size_t i = 0; i < size_t(nLinhasTabela); i++)
    {
        for(size_t aux = 0; aux < size_t(nEntradas); aux++)
            entrada[i][aux] = In[i][aux];
        for(size_t j = 2, aux = nEntradas; (j < vecSize-nSaidas) && (aux < vecSize-nSaidas); j += 3, aux++)
        {
            int aux1 = vec[j-2], aux2 = vec[j-1];
            if(vec[j] == 1) entrada[i][aux] = (entrada[i][aux1] && entrada[i][aux2]);       //AND
            else if(vec[j] == 2) entrada[i][aux] = (entrada[i][aux1] || entrada[i][aux2]);  //OR
            else if(vec[j] == 3) entrada[i][aux] = !(entrada[i][aux1] && entrada[i][aux2]); //NOR
            else if(vec[j] == 4) entrada[i][aux] = (entrada[i][aux1] != entrada[i][aux2]);  //XOR
        }
        for(size_t j = nEntradas+(nLinhas*nColunas), auxTotal = cont; auxTotal < vecSize; j++, auxTotal++)
        {
            int aux = vec[auxTotal];
            entrada[i][j] = entrada[i][aux];
        }
    }
    return entrada;
}

int Circuito::calcAcertos(vector< vector<bool> > entrada)
{
    int acertos = 0;
    for(size_t i = 0; i < size_t(nLinhasTabela); i++)
        for(size_t j = 0, auxIn = nEntradas+(nLinhas*nColunas); j < size_t(nSaidas); j++)
            acertos += (entrada[i][auxIn+j] == this->Tabela[i][j]);
    return acertos;
}

void Circuito::auxMapear(vector< vector <int> > *mapa, int pos)
{
    if(!(*mapa)[0][pos])
    {
        (*mapa)[0][pos] = true;
        if((*mapa)[1][pos] >= nEntradas) auxMapear(mapa, (*mapa)[1][pos]-nEntradas);
        if((*mapa)[2][pos] >= nEntradas) auxMapear(mapa, (*mapa)[2][pos]-nEntradas);
    }
}

vector< vector<int> > Circuito::mapear(vector<int> vec)
{
    vector< vector<int> > mapa(3, vector<int>(mapSize, 0));
    for(size_t i = 0, j = 2; i < mapSize; i++, j += 3)
    {
        mapa[1][i] = vec[j-2];
        mapa[2][i] = vec[j-1];
    }
    for(size_t i = vecSize-nSaidas; i < vecSize; i++)
        if(vec[i] >= nEntradas)
            auxMapear(&mapa, vec[i]-nEntradas);
    return mapa;
}

vector<int> Circuito::funcVetorAleatorio()
{
    vector<int> vec(vecSize, 0);

    for(size_t i = 2, col = 1; i < vecSize-nSaidas; i += 3)
    {
        vec[i] = rand()%4+1;
        vec[i-2] = levelBackFunc(col);
        vec[i-1] = levelBackFunc(col);

        col += (i%nLinhas != 0);
    }

    for(size_t i = vecSize-nSaidas; i < vecSize; i++) //saidas
        vec[i] = levelBackFunc(nColunas+1);

    return vec;
}

int Circuito::novoValor(size_t pos)
{
    if(pos >= vecSize-nSaidas) return levelBackFunc(nColunas+1);
    else return (!((pos+1)%3) ? rand()%4+1 : levelBackFunc((int(pos/(nLinhas*nColunas))+1)));
}

bool Circuito::geneAtivo(vector<int> mapa, size_t val)
{
    return ((val >= mapSize) ? true : mapa[val]);
}

vector<int> Circuito::funcAlteracao(vector<int> vec, vector<int> mapa)
{
    bool flag = false;
    while(!flag)
    {
        int novoIndice = rand()%vecSize;
        flag = geneAtivo(mapa, int(novoIndice/3));

        int aux = novoValor(novoIndice);
        while(vec[novoIndice] == aux)
            aux = novoValor(novoIndice);

        vec[novoIndice] = aux;
    }
    return vec;
}

int Circuito::nNosAtivos(vector<int> mapa)
{
    return accumulate(mapa.begin(), mapa.end(), 0);
}

int Circuito::funcNosAtivos(vector< vector<bool> > entrada, vector< vector<int> > mapa)
{
    bool flag = false;
    for(size_t i = 0; i < unsigned(nLinhasTabela); i++)
    {
        for(size_t j = 0; j < unsigned(nSaidas); j++)
        {
            int auxIn = nEntradas+(nLinhas*nColunas)+j;
            if(entrada[i][auxIn] != Tabela[i][j])
            {
                flag = true;
                break;
            }
        }
        if(flag) return mapSize;
    }
    return nNosAtivos(mapa[0]);
}

void Circuito::funcV7()
{
    vector< vector<int> > mapa(3, vector<int>(mapSize, 0)), mapa2 = mapa, mapaAux = mapa;
    vector<int> vec = funcVetorAleatorio(), vec2 = vec, vecAux = vec;

    int auxAcerto = 0;
    bool maximo = false;

    for(int auxRep = 1; auxRep <= 10000000; auxRep++)
    {
//        bool novoFilho = false;
        vec = vecAux;
        mapa = mapear(vecAux);

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

        for(int i = 0, acertos = 0; i < 6; i++, acertos = 0)
        {
            vec2 = funcAlteracao(vec, mapa[0]);
            vector< vector<bool> > entrada = funcIn(vec2);
            acertos = calcAcertos(entrada);
            if(acertos >= auxAcerto)
            {
//                if(!novoFilho)
//                {
//                    auxRazao++;
//                    novoFilho = true;
//                }
//                if(acertos == auxAcerto)
//                {
//                    mapa2 = mapear(vec2);
//                    mapaAux = mapear(vecAux);
//                }
//                if((acertos > auxAcerto) || ((acertos == auxAcerto) && (nNosAtivos(mapa2[0]) <= nNosAtivos(mapaAux[0]))))
//                {
                    if((acertos > auxAcerto))
                    {
                        cout << "i = " << auxRep << ": " << acertos << "/" << nLinhasTabela*nSaidas << endl;
//                        for(unsigned int i = 0; i < vecAux.size(); i++)
//                            cout << vecAux[i] << (i == vecAux.size()-1 ? "\n" : " ");
                    }
//                    else cout << nosAtivos(mapa2) << "nos ativos / "  << filhos << "filhos" << endl;
                    auxAcerto = acertos;
                    vecAux = vec2;
                    mapaAux = mapear(vec2);
//                }

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
    mapa = mapear(vec);
    cout << endl;
    for(size_t i = 0 ; i < vecSize; i++)
        cout << vec[i] << ((i+1)%3 == 0 ? ((i < vecSize-nSaidas-1) ? " | " : " || ") : " ") << ((i == vecSize-1) ? " <-------\n\n" : "");

    for(size_t i = 0; i < 3; i++)
        for(size_t j = 0; j < mapSize; j++)
            cout << mapa[i][j] << (j == mapSize-1 ? "\n" : " ");


//    cout << nosAtivos(mapa) << " nos ativos\nDiminuindo nos ativos agora!" << endl << endl;

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
    cout << "Reducao final: " << nNosAtivos(mapaAux[0]) << " nos ativos" << endl << endl;
}
