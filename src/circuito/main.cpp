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
    ifstream read("tabela6+.txt");
    int aux;

    read>>aux;
    *nLinhasTabela = aux;
    *nEntradas = log2(aux);
    read>>aux;
    *nSaidas = aux;

    cout << "numero de entradas: " << *nEntradas << endl;
    cout << "numero de saidas: " << *nSaidas << endl;
}

vector< vector<bool> > criaTabela(int nEntradas, int nSaidas, int nLinhasTabela, vector< vector<bool> > *entrada)
{
    ifstream read("tabela6+.txt");
    vector< vector<bool> > tabela(nLinhasTabela, vector<bool>(nSaidas, 1));

    int aux;
    read>>aux; read>>aux;

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
    int lim = nEntradas+nLinhas*(col-1);
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

            if(vec[j] == 1) (*in)[i][aux] = ((*in)[i][aux1] && (*in)[i][aux2]); //AND
            else if(vec[j] == 2) (*in)[i][aux] = ((*in)[i][aux1] || (*in)[i][aux2]); //OR
            else if(vec[j] == 3) (*in)[i][aux] = !((*in)[i][aux1]); //NOT
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

//    if(imprime)
//    {
//        int auxCont = nEntradas + (nLinhas*nColunas);
//        for(int i = 0; i < nLinhasTabela; i++)
//        {
//            cout << "Entrada " << ((i+1 < 10) ? "0" : "") << i+1 << ": ";
//            for(int j = 0; j < nEntradas; j++)
//                cout << (*in)[i][j] << ((j == nEntradas-1) ? "  ->  " : " ");
//            for(int j = auxCont; j < auxCont+nSaidas; j++)
//                cout << ((j == auxCont) ? "[" : "") << (*in)[i][j] << ((j == auxCont+nSaidas-1) ? "]\n" : " ");
//        }
//    }
}

void mapear(vector< vector<int> > *mapa, vector< vector<bool> > *ativo, int pos, int nEntradas, int saida)
{
    if(!(*mapa)[0][pos])
    {
        (*mapa)[0][pos] = true;
        (*ativo)[saida][pos] = true;

        if((*mapa)[1][pos] >= nEntradas) //esq
            mapear(mapa, ativo,(*mapa)[1][pos]-nEntradas, nEntradas, saida);
        if((*mapa)[2][pos] >= nEntradas) //dir
            mapear(mapa, ativo, (*mapa)[2][pos]-nEntradas, nEntradas, saida);
    }
}

vector<int> funcVetorAleatorio(vector< vector<int> > *mapa, vector< vector<bool> > *ativos, int nEntradas, int nLinhas, int nColunas, int nSaidas, int lb)
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

    for(int i = cont, j = i-cont; i < cont+nSaidas; i++)
    {
        vec[i] = levelBackFunc(nColunas+1, nLinhas, lb, nEntradas);
        if(vec[i] >= nEntradas)
            mapear(mapa, ativos, vec[i]-nEntradas, nEntradas, j);
    }

    return vec;
}

int novoValor(int pos, int nLinhas, int nColunas, int lb, int nEntradas)
{
    if(pos >= (nLinhas*nColunas*3)) return levelBackFunc(nColunas+1, nLinhas, lb, nEntradas);
    else return (!((pos+1)%3) ? rand()%4+1 : levelBackFunc((int)(pos/(nLinhas*nColunas))+1, nLinhas, lb, nEntradas));
}

bool geneAtivo(vector< vector<int> > mapa, int val)
{
    return ((val >= mapa[0].size()) ? true : mapa[0][val]); //nó de saida sempre é maior ou igual
}

vector<int> funcAlteracao(vector<int> vec, vector< vector<int> > *mapa, vector< vector<bool> > *ativos, int nEntradas, int nLinhas, int nColunas, int nSaidas, int lb, bool imprime = false)
{
//    int cont = vec.size() - nSaidas;
//    int cont = vec.size();

//    //<antigo> //só troca nó ativo
//    int novoIndice = rand()%cont;
//    while(!geneAtivo(mapa, nLinhas, nColunas, nSaidas, (int)novoIndice/3))
//            novoIndice = rand()%cont;
//
//    int aux = novoValor(novoIndice, nLinhas, nColunas, lb, nEntradas);
//    while(vec[novoIndice] == aux)
//        aux = novoValor(novoIndice, nLinhas, nColunas, lb, nEntradas);
//
//    vec[novoIndice] = aux;
//    //</antigo>

    //<novo> //troca até encontrar nó ativo
    bool flag = false;
    while(!flag)
    {
        int novoIndice = rand()%(vec.size());
        flag = geneAtivo(*mapa, (int)(novoIndice/3));

        int aux = novoValor(novoIndice, nLinhas, nColunas, lb, nEntradas);
        while(vec[novoIndice] == aux)
            aux = novoValor(novoIndice, nLinhas, nColunas, lb, nEntradas);

        int pos = (novoIndice+1)%3;
        if(pos)
            if(vec[novoIndice] >= nLinhas*nColunas)
            {
                int auxPos = vec[novoIndice] - nEntradas;
                bool auxFlag = false;
                while((!auxFlag) && (vec[auxPos] >= nEntradas))
                {
                    int auxAtivo = 0;
                    for(int i = 0; i < nSaidas; i++)
                    {
                        auxAtivo += (*ativos)[i][auxPos];
                        (*ativos)[i][auxPos] = false;
                    }
                    if(auxAtivo == 1)
                        (*mapa)[0][auxPos] = false;
                    else
                        auxFlag = true;

                    auxPos = vec[auxPos] - nEntradas;
                }
            }

        vec[novoIndice] = aux;
    }
    //</novo>

    return vec;
}

int numNosAtivos(vector< vector<int> > mapa)
{
    int nosAtivos = 0;
    for(int i = 0; i < mapa[0].size(); i++)
        nosAtivos += mapa[0][i];
    return nosAtivos;
}

//vector<int> funcNosAtivos(vector<int> vec, vector< vector<bool> > subgrafos, int nEntradas, int nLinhas, int nColunas, int nSaidas, int lb)
//{
//    bool flag = false;
//    while(!flag)
//    {
//        int novoIndice = rand()%(vec.size());
//        while(!geneAtivo(subgrafos, nLinhas, nColunas, nSaidas, (int)novoIndice/3))
//            novoIndice = rand()%(vec.size());
//
//        int aux = novoValor(novoIndice, nLinhas, nColunas, lb, nEntradas);
//        while(vec[novoIndice] == aux)
//            aux = novoValor(novoIndice, nLinhas, nColunas, lb, nEntradas);
//
//        vec[novoIndice] = aux;
//
//        for(int i = 0; i < nLinhasTabela; i++)
//        {
//            flag = true;
//            funcIn(&in, vec, nEntradas, nLinhas, nColunas, nSaidas, lb, nLinhasTabela, false);
//            for(int j = 0; j < nSaidas; j++)
//            {
//                int auxInd = nEntradas + (nLinhas*nColunas) + j;
//                if(in[i][auxInd] != tabela[i][j])
//                {
//                    flag = false;
//                    break;
//                }
//            }
//            if(!flag)
//                break;
//        }
//    }
//    return vec;
//}

int funcV5(int nEntradas, int nLinhas, int nColunas, int nSaidas, int lb, int nLinhasTabela, int rep, bool imprime = false)
{
    int auxAcerto = 0, n = nEntradas + (nLinhas*nColunas) + nSaidas, cont = 1;
    vector< vector<bool> > in(nLinhasTabela, vector<bool>(n, 0));
    vector< vector<bool> > tabela = criaTabela(nEntradas, nSaidas, nLinhasTabela, &in);

    vector< vector<int> > mapa(3, vector<int>(nLinhas*nColunas, 0)), mapa2 = mapa, mapaAux = mapa;
    vector< vector<bool> > ativos(nSaidas, vector<bool>(nLinhas*nColunas, 0)), ativos2 = ativos, ativosAux = ativos;
    vector<int> vec = funcVetorAleatorio(&mapa, &ativos, nEntradas, nLinhas, nColunas, nSaidas, lb), vec2 = vec, vecAux = vec;

    for(int auxRep = 1; auxRep <= 5000000; auxRep++)
    {
        vec = vecAux;
        mapa = mapaAux;
        ativos = ativosAux;

        if(imprime)
        {
            cout << "Novo vetor: ";
            for(int i = 0 ; i < vec.size(); i++)
                cout << vec[i] << ((i+1)%3 == 0 ? ((i <= vec.size()-nSaidas) ? " | " : " ") : " ") << ((i == vec.size()-1) ? "   <-------\n\n" : "");
        }

        for(int i = 0; i < rep; i++)
        {
            int acertos = 0;

            vec2 = funcAlteracao(vec, &mapa2, &ativos2, nEntradas, nLinhas, nColunas, nSaidas, lb, imprime);
            for(int j = 0; j < nLinhasTabela; j++)
            {
                funcIn(&in, vec2, nEntradas, nLinhas, nColunas, nSaidas, lb, nLinhasTabela, imprime);
                for(int k = 0; k < nSaidas; k++)
                {
                    int auxInd = nEntradas + (nLinhas*nColunas) + k;
                    if(in[j][auxInd] == tabela[j][k])
                        acertos++;
                }
            }
//            cout << acertos << "/" << nLinhasTabela*nSaidas;
            if(acertos >= auxAcerto)
            {
//                cout << "    -------maior---------";
                cont++;
                if(acertos > auxAcerto)
                    cout << cont << ": i = " << auxRep << " -> " << acertos  << "/" << nLinhasTabela*nSaidas << endl;
                auxAcerto = acertos;
                vecAux = vec2;
                mapaAux = mapa2;
                ativosAux = ativos2;
            }
//            cout << (imprime ? "\n-------------\n\n\n" : "\n");
        }

        if(auxRep%50000 == 0)
            cout << "--checkpoint: i = " << auxRep << "--" << endl;

//        cout << endl << auxRep << (imprime ? "\n\n\n-------------\n" : "\n\n");
        if(auxAcerto >= nLinhasTabela*nSaidas)
        {
            auxAcerto = auxRep;
            break;
        }
    }

    cout << endl;
    for(int i = 0; i < vec.size(); i++)
        cout << vecAux[i] << ((i+1)%3 == 0 ? ((i <= vecAux.size()-nSaidas) ? " | " : " ") : " ") << ((i == vecAux.size()-1) ? "\n" : "");

//    for(int i = 1; i <= 5000000; i++)
//    {
//        flag = funcNosAtivos(vec, subgrafo, nEntradas, nLinhas, nColunas, nSaidas, lb);
//    }

    return auxAcerto;
}

void teste(int nEntradas, int nLinhas, int nColunas, int nSaidas, int lb, int nLinhasTabela)
{
    int n = nEntradas + (nLinhas*nColunas) + nSaidas;

    vector< vector<bool> > in(nLinhasTabela, vector<bool>(n, 0));
    vector< vector<bool> > ativos(nLinhasTabela, vector<bool>(nLinhas*nColunas, 0)), ativos2 = ativos;
    vector< vector<bool> > tabela = criaTabela(nEntradas, nSaidas, nLinhasTabela, &in);
    vector< vector<int> > mapa(3, vector<int>(nLinhas*nColunas, 0)), mapa2 = mapa;

    vector<int> vec = funcVetorAleatorio(&mapa, &ativos, nEntradas, nLinhas, nColunas, nSaidas, lb), vec2 = vec;

    for(int i = 0; i < 100; i++)
    {
        mapa2 = mapa;
        ativos2 = ativos;
        vec2 = funcAlteracao(vec, &mapa2, &ativos2, nEntradas, nLinhas, nColunas, nSaidas, lb);
//        for(int i = 0 ; i < vec2.size(); i++)
//            cout << vec2[i] << ((i+1)%3 == 0 ? ((i <= vec2.size()-nSaidas) ? " | " : " ") : " ") << ((i == vec2.size()-1) ? "   <-------\n\n" : "");

        for(int i = 0; i < mapa2.size(); i++)
            for(int j = 0; j < mapa2[0].size(); j++)
                cout << mapa2[i][j] << (j == mapa2[0].size()-1 ? "\n" : " ");

        cout << endl << endl;
    }
}

int main()
{
    int seed, nLinhas, nColunas, levelback, nEntradas, nSaidas, nLinhasTabela;

    cout << "Seed:" << endl;
    cin >> seed;
    cout << "Linhas:" << endl;
    cin >> nLinhas;
    cout << "Colunas:" << endl;
    cin >> nColunas;
    cout << "Levelback:" << endl;
    cin >> levelback;
    if(levelback <= 0) levelback = nLinhas*nColunas;

    leTxt(&nEntradas, &nSaidas, &nLinhasTabela);

    srand(seed);

    int cont = funcV5(nEntradas, nLinhas, nColunas, nSaidas, levelback, nLinhasTabela, 4, false);
    cout << endl << cont << endl;

//    for(int i = 0; i < 120; i++)
//    {
//        srand(i);
//        teste(nEntradas, nLinhas, nColunas, nSaidas, levelback, nLinhasTabela);
//        cout << endl;
//    }
//
//    cout << cont1 << endl;

//    teste(nEntradas, nLinhas, nColunas, nSaidas, levelback, nLinhasTabela);

    cout << "-------------------\nEnter pra continuar" << endl;
    cin.ignore();

    return 0;
}
