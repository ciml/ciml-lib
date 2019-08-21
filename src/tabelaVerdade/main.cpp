#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

vector<bool> dec2bin(int num, int tam)
{
    vector<bool> bin(tam, 0);
    for(int i = 0; i < tam; i++, num /= 2)
        if(num > 0)
            bin[i] = num%2;
    reverse(bin.begin(), bin.end());
    return bin;
}

int computaSaida(vector<bool> entrada, int nEntradas, char c)
{
    int numA = 0, numB = 0, aux = nEntradas/2;
    for(int i = 0; i < aux; i++)
    {
        numA += entrada[i]*pow(2, (aux-1)-i);
        numB += entrada[i+aux]*pow(2, (aux-1)-i);
    }
    return ((c == '+') ? (numA+numB) : (numA*numB));
}

vector<bool> vecSaida(vector<bool> entrada, int nEntradas, int nSaidas, char c)
{
    int saida = computaSaida(entrada, nEntradas, c);
    return dec2bin(saida, nSaidas);
}

void tabelaVerdade(int nEntradas, char tipoSaida)
{
    ofstream outfile("../circuito/teste.txt");

    int nLinhasTabela = pow(2, nEntradas), nSaidas = computaSaida(vector<bool>(nEntradas, 1), nEntradas, tipoSaida);
    nSaidas = (int)log2(nSaidas)+1;

    cout << "Linhas tabela | Saidas" << endl;
    cout << nLinhasTabela << "                    " << nSaidas << endl;
    outfile << nLinhasTabela << " " << nSaidas << endl;

    for(int i = 0; i < nLinhasTabela; i++)
    {
        vector<bool> entrada = dec2bin(i, nEntradas);
        vector<bool> saida = vecSaida(entrada, nEntradas, nSaidas, tipoSaida);
        for(int j = 0; j < entrada.size(); j++)
        {
            cout << entrada[j] << ((j == entrada.size()-1) ? " " : "");
            outfile << entrada[j] << " ";
        }
        for(int j = 0; j < saida.size(); j++)
        {
            cout << saida[j];
            outfile << saida[j] << (j == saida.size()-1 ? "" : " ");
        }
        cout << ((i == nLinhasTabela-1) ? "" : "\n");
        outfile << ((i == nLinhasTabela-1) ? "" : "\n");
    }
    outfile.close();
}

int main()
{
    int nEntradas;
    char tipoSaida;

    cout << "Numero de entradas:" << endl;
    cin >> nEntradas;
    if((nEntradas%2) != 0)
    {
        cout << "ERRO" << endl;
        return -1;
    }
    cout << "Tipo de saida (+ ou x):" << endl;
    cin >> tipoSaida;

    tabelaVerdade(nEntradas, tipoSaida);

    return 0;
}
