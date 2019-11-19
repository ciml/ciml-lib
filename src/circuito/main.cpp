#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<vector>
#include<algorithm>
#include<cmath>
#include<fstream>
#include<math.h>
#include<numeric>
#include "Circuito.h"

using namespace std;

void leTxt(int *nEntradas, int *nSaidas, int *nLinhasTabela)
{
    int aux;
    ifstream read("tabela4+.txt");
    read>>aux;
    *nLinhasTabela = aux;
    *nEntradas = log2(aux);
    read>>aux;
    *nSaidas = aux;

    cout << endl << "Numero de entradas: " << *nEntradas << endl;
    cout << "Numero de saidas: " << *nSaidas << endl;
}

vector< vector<bool> > criaTabela(int nEntradas, int nSaidas, int nLinhasTabela, vector< vector<bool> > *entrada)
{
    int aux;
    ifstream read("tabela4+.txt");
    vector< vector<bool> > tabela(nLinhasTabela, vector<bool>(nSaidas, 0));
    read>>aux;
    read>>aux;

    cout << "Tabela:" << endl;
    for(int i = 0; i < nLinhasTabela; i++)
    {
        for(int j = 0; j < nEntradas; j++)
        {
            read>>aux;
            (*entrada)[i][j] = aux;
            cout << (*entrada)[i][j] << ((j == nEntradas-1) ? " " : "");
        }
        for(int j = 0; j < nSaidas; j++)
        {
            read>>aux;
            tabela[i][j] = aux;
            cout << tabela[i][j] << ((j == nSaidas-1) ? ((i == nLinhasTabela-1) ? "\n\n" : "\n") : "");
        }
    }

    return tabela;
}

int levelBackFunc(int col, int nLinhas, int lb, int nEntradas)
{
    int lim = nEntradas+nLinhas*(col-1), num = rand()%lim;
    if(col > lb)
        while((num >= nEntradas) && (num <= ((lim-1)-lb*nLinhas)))
            num = rand()%lim;
    return num;
}

vector< vector<bool> > funcIn(vector< vector<bool> > in, vector<int> vec, int nEntradas, int nLinhas, int nColunas, int nSaidas, int lb, int nLinhasTabela, bool imprime = false)
{
    int aux = nEntradas, cont = nLinhas*nColunas*3, contIn = nEntradas+(nLinhas*nColunas)+nSaidas;
    for(int i = 0; i < nLinhasTabela; i++, aux = nEntradas)
        for(int j = 2; j < cont; j += 3, aux++)
        {
            int aux1 = vec[j-2], aux2 = vec[j-1];

            if(vec[j] == 1) in[i][aux] = (in[i][aux1] && in[i][aux2]); //AND
            else if(vec[j] == 2) in[i][aux] = (in[i][aux1] || in[i][aux2]); //OR
            else if(vec[j] == 3) in[i][aux] = !(in[i][aux1]); //NOT
            else in[i][aux] = (in[i][aux1] != in[i][aux2]); //XOR
        }
    for(int i = 0; i < nLinhasTabela; i++) //saidas
        for(int j = nEntradas+(nLinhas*nColunas), auxTotal = cont; j < contIn; j++, auxTotal++)
        {
            int aux2 = vec[auxTotal];
            in[i][j] = in[i][aux2];
        }
    return in;
}

int calcAcertos(vector<bool> in, vector<bool> tabela, int nEntradas, int nLinhas, int nColunas, int nSaidas, int nLinhasTabela)
{
    int acertos = 0;
    for(int i = 0, auxIn = nEntradas+(nLinhas*nColunas); i < nSaidas; i++, auxIn++)
        acertos += (in[auxIn] == tabela[i]);
    return acertos;
}

void auxMapear(vector< vector<int> > *mapa, int pos, int nEntradas)
{
    if(!(*mapa)[0][pos])
    {
        (*mapa)[0][pos] = true;
        if((*mapa)[1][pos] >= nEntradas) auxMapear(mapa, (*mapa)[1][pos]-nEntradas, nEntradas);
        if((*mapa)[2][pos] >= nEntradas) auxMapear(mapa, (*mapa)[2][pos]-nEntradas, nEntradas);
    }
}

vector< vector<int> > mapear(vector< vector<int> > mapa, vector<int> vec, int nEntradas, int nSaidas)
{
    mapa[0] = vector<int>(mapa[0].size(), 0);
    for(int i = 0, j = (mapa[0].size()*3); i < nSaidas; i++, j++)
        if(vec[j] >= nEntradas)
            auxMapear(&mapa, vec[j]-nEntradas, nEntradas);
    return mapa;
}

vector<int> funcVetorAleatorio(vector< vector<int> > *mapa, int nEntradas, int nLinhas, int nColunas, int nSaidas, int lb)
{
    vector<int> vec((nLinhas*nColunas*3)+nSaidas);
    int col = 1, cont = nLinhas*nColunas*3;

    for(int i = 2, j = 0; i < cont; i += 3, j++)
    {
        vec[i] = rand()%4+1;
        vec[i-2] = levelBackFunc(col, nLinhas, lb, nEntradas);
        vec[i-1] = levelBackFunc(col, nLinhas, lb, nEntradas);

        (*mapa)[1][j] = vec[i-2];
        (*mapa)[2][j] = vec[i-1];

        col += (i%nLinhas != 0);
    }

    for(int i = cont; i < (cont+nSaidas); i++)
        vec[i] = levelBackFunc(nColunas+1, nLinhas, lb, nEntradas);

    return vec;
}

int novoValor(int pos, int nLinhas, int nColunas, int lb, int nEntradas)
{
    if(pos >= nLinhas*nColunas*3) return levelBackFunc(nColunas+1, nLinhas, lb, nEntradas);
    else return (!((pos+1)%3) ? (rand()%4+1) : levelBackFunc(((int)(pos/(nLinhas*nColunas))+1), nLinhas, lb, nEntradas));
}

bool geneAtivo(vector< vector<int> > mapa, int val)
{
    return ((unsigned(val) >= mapa[0].size()) ? true : mapa[0][val]);
}

vector<int> funcAlteracao(vector<int> vec, vector< vector<int> > *mapa, int nEntradas, int nLinhas, int nColunas, int nSaidas, int lb, bool imprime = false)
{
    bool flag = false;
    while(!flag)
    {
        int novoIndice = rand()%(vec.size());
        flag = geneAtivo(*mapa, (int)(novoIndice/3));

        int aux = novoValor(novoIndice, nLinhas, nColunas, lb, nEntradas);
        while(vec[novoIndice] == aux)
            aux = novoValor(novoIndice, nLinhas, nColunas, lb, nEntradas);

        vec[novoIndice] = aux;
        if((unsigned(novoIndice) < (*mapa)[0].size()*3) && ((novoIndice+1)%3))
        {
            int auxLinha = (novoIndice+1)%3, auxCol = (novoIndice/3);
            (*mapa)[auxLinha][auxCol] = aux;
        }
    }

    return vec;
}

int nosAtivos(vector< vector<int> > mapa)
{
    return accumulate(mapa[0].begin(), mapa[0].end(), 0);
}

vector< vector<int> > funcNosAtivos(vector< vector<bool> > in, vector< vector<int> > mapa, vector< vector<bool> > tabela, int nEntradas, int nLinhas, int nColunas, int nSaidas, int nLinhasTabela)
{
    bool flag = false;
    for(int i = 0; i < nLinhasTabela; i++)
    {
        for(int j = 0; j < nSaidas; j++)
        {
            int auxIn = nEntradas+(nLinhas*nColunas)+j;
            if(in[i][auxIn] != tabela[i][j])
            {
                flag = true;
                break;
            }
        }
        if(flag)
        {
            mapa[0] = vector<int>(mapa[0].size(), 1);
            break;
        }
    }
    return mapa;
}

void funcV6(int nEntradas, int nLinhas, int nColunas, int nSaidas, int lb, int nLinhasTabela, int rep, int seed, bool imprime = false)
{
    int auxAcerto = 0, auxRazao = 1, n = nEntradas+(nLinhas*nColunas)+nSaidas, filhos = rep;
    bool maximo = false;
    vector< vector<bool> > in(nLinhasTabela, vector<bool>(n, 0)), tabela = criaTabela(nEntradas, nSaidas, nLinhasTabela, &in);
    vector< vector<int> > mapa(3, vector<int>(nLinhas*nColunas, 0)), mapa2 = mapa, mapaAux = mapa;
    vector<int> vec = funcVetorAleatorio(&mapa, nEntradas, nLinhas, nColunas, nSaidas, lb), vec2 = vec, vecAux = vec;

    for(int auxRep = 1; auxRep <= 10000000; auxRep++)
    {
        bool novoFilho = false;
        vec = vecAux;
        mapa = mapear(mapaAux, vec, nEntradas, nSaidas);
//        if(imprime)
//        {
//            cout << "Novo vetor: ";
//            for(int i = 0 ; i < vec.size(); i++)
//                cout << vec[i] << ((i+1)%3 == 0 ? ((i <= vec.size()-nSaidas) ? " | " : " ") : " ") << ((i == vec.size()-1) ? "   <-------\n\n" : "");
//        }

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

        for(int i = 0, acertos = 0; i < filhos; i++, acertos = 0)
        {
            vec2 = funcAlteracao(vec, &mapa2, nEntradas, nLinhas, nColunas, nSaidas, lb, imprime);
            for(int j = 0; j < nLinhasTabela; j++)
            {
                in = funcIn(in, vec2, nEntradas, nLinhas, nColunas, nSaidas, lb, nLinhasTabela, imprime);
                acertos += calcAcertos(in[j], tabela[j], nEntradas, nLinhas, nColunas, nSaidas, nLinhasTabela);
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
                    mapa2 = mapear(mapa2, vec, nEntradas, nSaidas);
                    mapaAux = mapear(mapaAux, vec, nEntradas, nSaidas);
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
    mapa = mapear(mapaAux, vec, nEntradas, nSaidas);

    for(unsigned int i = 0 ; i < vec.size(); i++)
        cout << vec[i] << (unsigned(i+1)%3 == 0 ? ((unsigned(i) <= vec.size()-nSaidas) ? " | " : " ") : " ") << ((unsigned(i) == vec.size()-1) ? "   <-------\n\n" : "");

    cout << nosAtivos(mapa) << " nos ativos\nDiminuindo nos ativos agora!" << endl << endl;

    for(int auxRep = 1, cont = 0; (auxRep <= 5000000); auxRep++, cont++)
    {
        int auxNosAtivos = nosAtivos(mapa);
        if(auxNosAtivos == 0) break;
        vec = vecAux;
        mapa = mapaAux;

        vec2 = funcAlteracao(vec, &mapa2, nEntradas, nLinhas, nColunas, nSaidas, lb, imprime);
        for(int j = 0; j < nLinhasTabela; j++)
            in = funcIn(in, vec2, nEntradas, nLinhas, nColunas, nSaidas, lb, nLinhasTabela, imprime);

        for(int i = 0; i < rep; i++)
        {
            mapa2 = funcNosAtivos(in, mapa2, tabela, nEntradas, nLinhas, nColunas, nSaidas, nLinhasTabela);
            if(nosAtivos(mapa2) < auxNosAtivos)
            {
                cout << cont << ": i = " << auxRep << " -> " << nosAtivos(mapa2) << " nos ativos" << endl;
                vecAux = vec2;
                mapaAux = mapa2;
                auxNosAtivos = nosAtivos(mapa2);

                cont = 0;
            }
        }

        if(!(auxRep%100000))
            cout << "--checkpoint: i = " << auxRep << " -- " << nosAtivos(mapaAux) << "nos ativos" << endl;
    }
    cout << "Reducao final: " << nosAtivos(mapaAux) << " nos ativos" << endl << endl;
}

void teste()
{
    int seed, nLinhas, nColunas, lb;

    cout << "Seed: ";
    cin >> seed;
    cout << "Linhas: ";
    cin >> nLinhas;
    cout << "Colunas: ";
    cin >> nColunas;
    cout << "Levelback: ";
    cin >> lb;


    srand(seed);
    Circuito circuito(nLinhas, nColunas, lb, 4);
    circuito.funcV7();

//    int n = nEntradas + (nLinhas*nColunas) + nSaidas;
//
//    vector< vector<bool> > in(nLinhasTabela, vector<bool>(n, 0));
//    vector< vector<bool> > tabela = criaTabela(nEntradas, nSaidas, nLinhasTabela, &in);
//    vector< vector<int> > mapa(3, vector<int>(nLinhas*nColunas, 0)), mapa2 = mapa;
//
//    vector<int> vec = funcVetorAleatorio(&mapa, nEntradas, nLinhas, nColunas, nSaidas, lb), vec2 = vec;
//
//    for(unsigned int i = 0 ; i < vec.size(); i++)
//        cout << vec[i] << (unsigned(i+1)%3 == 0 ? ((unsigned(i) <= vec.size()-nSaidas) ? " | " : " ") : " ") << ((unsigned(i) == vec.size()-1) ? "\n\n" : "");
//    for(unsigned int i = 0; i < mapa.size(); i++)
//        for(unsigned int j = 0; j < mapa[0].size(); j++)
//            cout << mapa[i][j] << (unsigned(j) == (mapa[0].size() - 1) ? "\n" : " ");
//    cout << endl << endl;
//
//    for(int i = 0; i < 1; i++)
//    {
//        mapa2 = mapa;
//        vec2 = funcAlteracao(vec, &mapa2, nEntradas, nLinhas, nColunas, nSaidas, lb);
//        mapa2 = mapear(mapa2, vec, nEntradas, nSaidas);
//
//        for(unsigned int i = 0 ; i < vec2.size(); i++)
//            cout << vec2[i] << (unsigned(i+1)%3 == 0 ? ((unsigned(i) <= vec2.size()-nSaidas) ? " | " : " ") : " ") << ((unsigned(i) == vec2.size()-1) ? "\n\n" : "");
//        for(unsigned int i = 0; i < mapa2.size(); i++)
//            for(unsigned int j = 0; j < mapa2[0].size(); j++)
//                cout << mapa2[i][j] << (unsigned(j) == (mapa2[0].size()-1) ? "\n" : " ");
//
//        cout << endl << endl;
//    }
}

int main()
{

    //leTxt(&nEntradas, &nSaidas, &nLinhasTabela);



    //funcV6(nEntradas, nLinhas, nColunas, nSaidas, levelback, nLinhasTabela, 4, seed);
    teste();

    cout << "-------------------\nEnter pra continuar" << endl;
    cin.ignore();

    return 0;
}
