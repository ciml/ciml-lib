#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <math.h>

using namespace std;

void leTxt(int *nEntradas, int *nSaidas, int *nLinhasTabela)
{
    ifstream read("tabela3+.txt");
    int aux;

    read>>aux;
    *nLinhasTabela = aux;
    *nEntradas = log2(aux);
    read>>aux;
    *nSaidas = aux;

    cout << "Numero de entradas: " << *nEntradas << endl;
    cout << "Numero de saidas: " << *nSaidas << endl;
}

vector< vector<bool> > criaTabela(int nEntradas, int nSaidas, int nLinhasTabela, vector< vector<bool> > *entrada)
{
    ifstream read("tabela3+.txt");
    vector< vector<bool> > tabela(nLinhasTabela, vector<bool>(nSaidas, 0));

    int aux;
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
            cout << tabela[i][j] << ((j == nSaidas-1) ? "\n" : "");
        }
    }
    cout << endl;
    return tabela;
}

int levelBackFunc(int col, int nLinhas, int lb, int nEntradas)
{
    int lim = nEntradas + nLinhas*(col-1);
    int num = rand()%lim;

    if(col > lb)
        while((num <= ((lim-1)-lb*nLinhas)) && (num >= nEntradas))
            num = rand()%lim;

    return num;
}

void funcIn(vector< vector<bool> > *in, vector<int> vec, int nEntradas, int nLinhas, int nColunas, int nSaidas, int lb, int nLinhasTabela, bool imprime = false)
{
    int aux = nEntradas, cont = (nLinhas*nColunas*3);
    for(int i = 0; i < nLinhasTabela; i++)
    {
        aux = nEntradas;
        for(int j = 2; j < cont; j += 3)
        {
            int aux1 = vec[j-2], aux2 = vec[j-1];

            if(vec[j] == 1)
                (*in)[i][aux] = ((*in)[i][aux1] && (*in)[i][aux2]); //AND
            else if(vec[j] == 2)
                (*in)[i][aux] = ((*in)[i][aux1] || (*in)[i][aux2]); //OR
            else if(vec[j] == 3)
                (*in)[i][aux] = !((*in)[i][aux1]); //NOT
            else (*in)[i][aux] = ((*in)[i][aux1] != (*in)[i][aux2]); //XOR
            aux++;
        }
    }

    int contIn = nEntradas + (nLinhas*nColunas) + nSaidas;
    for(int i = 0; i < nLinhasTabela; i++)
    {
        int auxTotal = cont;
        for(int j = aux; j < contIn; j++)
        {
            int aux2 = vec[auxTotal];
            (*in)[i][j] = (*in)[i][aux2];
            auxTotal++;
        }
    }
}

void mapear(vector< vector<int> > *mapa, int pos, int nEntradas)
{
    if(!(*mapa)[0][pos])
    {
        (*mapa)[0][pos] = true;

        if((*mapa)[1][pos] >= nEntradas) //esq
            mapear(mapa, (*mapa)[1][pos]-nEntradas, nEntradas);
        if((*mapa)[2][pos] >= nEntradas) //dir
            mapear(mapa, (*mapa)[2][pos]-nEntradas, nEntradas);
    }
}

vector<int> funcVetorAleatorio(vector< vector<int> > *mapa, int nEntradas, int nLinhas, int nColunas, int nSaidas, int lb)
{
    vector<int> vec((nLinhas*nColunas*3)+nSaidas);
    int col = 1, cont = vec.size() - nSaidas;

    for(int i = 2, j = 0; i < cont; i += 3, j++)
    {
        vec[i] = rand()%4+1;

        vec[i-2] = levelBackFunc(col, nLinhas, lb, nEntradas);
        vec[i-1] = levelBackFunc(col, nLinhas, lb, nEntradas);

        (*mapa)[1][j] = vec[i-2];
        (*mapa)[2][j] = vec[i-1];

        if(i%nLinhas != 0)
            col++;
    }

    for(int i = cont; i < (cont+nSaidas); i++)
    {
        vec[i] = levelBackFunc(nColunas+1, nLinhas, lb, nEntradas);
        if(vec[i] >= nEntradas)
            mapear(mapa, vec[i]-nEntradas, nEntradas);
    }

    return vec;
}

int novoValor(int pos, int nLinhas, int nColunas, int lb, int nEntradas)
{
    if(pos >= (nLinhas*nColunas*3))
        return levelBackFunc((nColunas+1), nLinhas, lb, nEntradas);
    else
        return (!((pos+1)%3) ? (rand()%4+1) : levelBackFunc(((int)(pos/(nLinhas*nColunas))+1), nLinhas, lb, nEntradas));
}

bool geneAtivo(vector< vector<int> > mapa, int val)
{
    return ((val >= mapa[0].size()) ? true : mapa[0][val]); //nó de saida sempre é maior ou igual, e sempre ativo
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
        if((novoIndice < (*mapa)[0].size()*3) && ((novoIndice+1)%3 > 0))
        {
            int auxLinha = (novoIndice+1)%3, auxCol = (novoIndice/3);
            (*mapa)[auxLinha][auxCol] = aux;
        }
    }

//    (*mapa)[0] = vector<int>((*mapa)[0].size(), 0);
//    for(int i = 0, j = 0; i < (*mapa)[0].size(); i++, j += 3)
//    {
//        (*mapa)[1][i] = vec[j];
//        (*mapa)[2][i] = vec[j+1];
//    }

    return vec;
}

int nosAtivos(vector< vector<int> > mapa)
{
    int soma = 0;
    for(int i = 0; i < mapa[0].size(); i++)
        soma += mapa[0][i];
    return soma;
}

vector<int> funcNosAtivos(vector<int> vec, vector< vector<int> > *mapa, vector< vector<bool> > tabela, int nEntradas, int nLinhas, int nColunas, int nSaidas, int lb, int nLinhasTabela)
{
    int n = nEntradas + (nLinhas*nColunas) + nSaidas;
    bool flag = false;

    vector< vector<bool> > in(nLinhasTabela, vector<bool>(n, 0));
    vector<int> vec2 = vec;
    vector< vector<int> > mapaAux = *mapa;

    int auxCont = 0;

    while(!flag)
    {
        vec2 = vec;
        *mapa = mapaAux;

        int novoIndice = rand()%(vec.size()); //muta só se for ativo
        while(!geneAtivo(*mapa, (int)(novoIndice/3)))
            novoIndice = rand()%(vec.size());

        int aux = novoValor(novoIndice, nLinhas, nColunas, lb, nEntradas);
        while(vec[novoIndice] == aux)
            aux = novoValor(novoIndice, nLinhas, nColunas, lb, nEntradas);

        vec[novoIndice] = aux;
        flag = true;
        for(int i = 0; i < nLinhasTabela; i++)
        {
            funcIn(&in, vec2, nEntradas, nLinhas, nColunas, nSaidas, lb, nLinhasTabela, false);
            for(int j = 0; j < nSaidas; j++)
            {
                int auxIn = nEntradas + (nLinhas*nColunas) + j;
                if(in[i][auxIn] != tabela[i][j])
                {
                    flag = false;
                    break;
                }
            }
        }

        auxCont++;
        if(!(auxCont%500000))
            cout << auxCont/500000 << endl;
    }

    if(flag)
        cout << flag << endl;

    (*mapa)[0] = vector<int>((*mapa)[0].size(), 0);
    for(int i = 0; i < nSaidas; i++)
    {
        int j = ((*mapa)[0].size()*3) + i;
        if(vec[j] >= nEntradas)
            mapear(mapa, (vec[j]-nEntradas), nEntradas);
    }

    return vec2;
}

void funcV6(int nEntradas, int nLinhas, int nColunas, int nSaidas, int lb, int nLinhasTabela, int rep, bool imprime = false)
{
    int auxAcerto = 0, n = nEntradas + (nLinhas*nColunas) + nSaidas, cont = 0;
    vector< vector<bool> > in(nLinhasTabela, vector<bool>(n, 0));
    vector< vector<bool> > tabela = criaTabela(nEntradas, nSaidas, nLinhasTabela, &in);

    vector< vector<int> > mapa(3, vector<int>(nLinhas*nColunas, 0)), mapa2 = mapa, mapaAux = mapa;
    vector<int> vec = funcVetorAleatorio(&mapa, nEntradas, nLinhas, nColunas, nSaidas, lb), vec2 = vec, vecAux = vec;

    for(int auxRep = 1; auxRep <= 2500000; auxRep++)
    {
//        if(imprime)
//        {
//            cout << "Novo vetor: ";
//            for(int i = 0 ; i < vec.size(); i++)
//                cout << vec[i] << ((i+1)%3 == 0 ? ((i <= vec.size()-nSaidas) ? " | " : " ") : " ") << ((i == vec.size()-1) ? "   <-------\n\n" : "");
//        }

        for(int i = 0; i < rep; i++)
        {
            mapa2 = mapa;
            int acertos = 0;

            vec2 = funcAlteracao(vec, &mapa2, nEntradas, nLinhas, nColunas, nSaidas, lb, imprime);
            for(int j = 0; j < nLinhasTabela; j++)
            {
                funcIn(&in, vec2, nEntradas, nLinhas, nColunas, nSaidas, lb, nLinhasTabela, imprime);
                for(int k = 0; k < nSaidas; k++)
                {
                    int auxIn = nEntradas + (nLinhas*nColunas) + k;
                    if(in[j][auxIn] == tabela[j][k])
                        acertos++;
                }
            }
            if(acertos >= auxAcerto)
            {
//                cont++;
//                if(acertos > auxAcerto)
//                {
//                    cout << cont << ": i = " << auxRep << " -> " << acertos << "/" << nLinhasTabela*nSaidas << endl;
//                    cont = 0;
//                }
                auxAcerto = acertos;
                vecAux = vec2;
                mapaAux = mapa2;
            }
//            cout << (imprime ? "\n-------------\n\n\n" : "\n");
        }

        if(!(auxRep%100000))
            cout << "--checkpoint: i = " << auxRep << "--" << endl;

        vec = vecAux;
        mapa = mapaAux;
        mapa[0] = vector<int>(mapa[0].size(), 0);
        for(int i = 0; i < nSaidas; i++)
        {
            int j = (mapa[0].size()*3) + i;
            if(vec[j] >= nEntradas)
                mapear(&mapa, vec[j]-nEntradas, nEntradas);
        }
        if(auxAcerto >= nLinhasTabela*nSaidas)
        {
            auxAcerto = auxRep;
            cout << auxRep << endl << endl;
            cont = 0;

            cout << "Vetor encontrado:" << endl;
            for(int i = 0; i < vec.size(); i++)
                cout << vec[i] << ((i+1)%3 == 0 ? ((i <= vec.size()-nSaidas) ? " | " : " ") : " ") << ((i == vec.size()-1) ? "\n" : "");
            cout << endl << "Nós ativos: " << nosAtivos(mapa) << endl;
            for(int i = 0; i < mapa.size(); i++)
                for(int j = 0; j < mapa[0].size(); j++)
                    cout << mapa[i][j] << (j == mapa[0].size()-1 ? "\n" : "");

            break;
        }
    }

    cout << endl;

    for(int auxRep = 1; auxRep = 2500000; auxRep++)
    {
        int auxNosAtivos = mapa[0].size();
        vec = vecAux;
        mapa = mapaAux;

        vec2 = funcNosAtivos(vec, &mapa, tabela, nEntradas, nLinhas, nColunas, nSaidas, lb, nLinhasTabela);

        if(nosAtivos(mapa) <= auxNosAtivos)
        {
            cont++;
            if(nosAtivos(mapa) < auxNosAtivos)
            {
                cout << cont << ": i = " << auxRep << " -> " << nosAtivos(mapa) << " nós ativos" << endl;
                cont = 0;
            }
            vecAux = vec2;
            mapaAux = mapa;
            auxNosAtivos = nosAtivos(mapa);
        }

        if(!(auxRep%100000))
            cout << "--checkpoint: i = " << auxRep << " -- " << nosAtivos(mapaAux) << endl;

        if(auxNosAtivos <= abs(nEntradas-nSaidas))
        {
            cout << endl << "Vetor encontrado:" << endl;
            for(int i = 0; i < vecAux.size(); i++)
                cout << vecAux[i] << ((i+1)%3 == 0 ? ((i <= vecAux.size()-nSaidas) ? " | " : " ") : " ") << ((i == vecAux.size()-1) ? "\n" : "");
            cout << endl << "Nós ativos: " << nosAtivos(mapa) << endl;
            for(int i = 0; i < mapaAux.size(); i++)
                for(int j = 0; j < mapaAux[0].size(); j++)
                    cout << mapaAux[i][j] << (j == mapaAux[0].size()-1 ? "\n" : "");

            break;
        }
    }
}

void teste(int nEntradas, int nLinhas, int nColunas, int nSaidas, int lb, int nLinhasTabela)
{
    int n = nEntradas + (nLinhas*nColunas) + nSaidas;

    vector< vector<bool> > in(nLinhasTabela, vector<bool>(n, 0));
    vector< vector<bool> > tabela = criaTabela(nEntradas, nSaidas, nLinhasTabela, &in);
    vector< vector<int> > mapa(3, vector<int>(nLinhas*nColunas, 0)), mapa2 = mapa;

    vector<int> vec = funcVetorAleatorio(&mapa, nEntradas, nLinhas, nColunas, nSaidas, lb), vec2 = vec;

    for(int i = 0 ; i < vec.size(); i++)
        cout << vec[i] << ((i+1)%3 == 0 ? ((i <= vec.size()-nSaidas) ? " | " : " ") : " ") << ((i == vec.size()-1) ? "\n\n" : "");
    for(int i = 0; i < mapa.size(); i++)
        for(int j = 0; j < mapa[0].size(); j++)
            cout << mapa[i][j] << (j == (mapa[0].size() - 1) ? "\n" : " ");
    cout << endl << endl;

    for(int i = 0; i < 1; i++)
    {
        mapa2 = mapa;
        vec2 = funcAlteracao(vec, &mapa2, nEntradas, nLinhas, nColunas, nSaidas, lb);
        mapa2[0] = vector<int>(mapa2[0].size(), 0);
        for(int i = 0; i < nSaidas; i++)
        {
            int j = (mapa2[0].size()*3) + i;
            if(vec2[j] >= nEntradas)
                mapear(&mapa2, vec2[j]-nEntradas, nEntradas);
        }

        for(int i = 0 ; i < vec2.size(); i++)
            cout << vec2[i] << ((i+1)%3 == 0 ? ((i <= vec2.size()-nSaidas) ? " | " : " ") : " ") << ((i == vec2.size()-1) ? "\n\n" : "");
        for(int i = 0; i < mapa2.size(); i++)
            for(int j = 0; j < mapa2[0].size(); j++)
                cout << mapa2[i][j] << (j == (mapa2[0].size() - 1) ? "\n" : " ");

        cout << endl << endl;
    }
}

int main()
{
    int seed, nLinhas, nColunas, levelback, nEntradas, nSaidas, nLinhasTabela;

    cout << "Seed: ";
    cin >> seed;
    cout << "Linhas: ";
    cin >> nLinhas;
    cout << "Colunas: ";
    cin >> nColunas;
    cout << "Levelback: ";
    cin >> levelback;
    if(levelback <= 0)
        levelback = nLinhas*nColunas;

    leTxt(&nEntradas, &nSaidas, &nLinhasTabela);

    srand(seed);

    funcV6(nEntradas, nLinhas, nColunas, nSaidas, levelback, nLinhasTabela, 4, false);
//    teste(nEntradas, nLinhas, nColunas, nSaidas, levelback, nLinhasTabela);

    cout << "-------------------\nEnter pra continuar" << endl;
    cin.ignore();

    return 0;
}
