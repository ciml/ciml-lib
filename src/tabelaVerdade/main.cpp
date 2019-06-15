#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

vector<bool> dec2bin(int num, int aux)
{
    vector<bool> bin(6, 0);
    for(int i = 0; i < aux; i++, num /= 2)
        if(num > 0)
            bin[i] = num%2;
    reverse(bin.begin(), bin.end());
    return bin;
}

int bin2dec(vector<bool> bin)
{
    int dec = 0;
    for(int i = bin.size()-1; i >= 0; i--)
        dec += pow(2, bin[i]);
    return dec;
}

int computaSaida(vector<bool> entrada, char c)
{
    int numA = 0, numB = 0;
    for(int i = 0; i < 3; i++)
    {
        numA += entrada[i]*pow(2, 2-i);
        numB += entrada[i+3]*pow(2, 2-i);
    }
    return ((c == '+') ? (numA+numB) : (numA*numB));
}

vector<bool> vecSaida(vector<bool> entrada, int nEntradas, char c, int numBits)
{
    int saida = computaSaida(entrada, c);
    return dec2bin(saida, numBits);
}

void criarTabelaVerdade(int nEntradas, char tipoSaida)
{
    ofstream outfile("../circuito/tabela6+.txt"); //pasta do projeto principal

    int nLinhasTabela = pow(2, nEntradas);
    vector<bool> auxSaida(6, 0), entrada, saida;
    for(int i = 6-nEntradas; i < 6; i++)
        auxSaida[i] = 1;
    int nSaidas = computaSaida(auxSaida, tipoSaida), maxSaida = (int)log2(nSaidas)+1;

    cout << "Linhas tabela | Saidas" << endl;
    cout << nLinhasTabela << "                    " << maxSaida << endl;
    outfile << nLinhasTabela << " " << maxSaida << endl;

    for(int i = 0; i < nLinhasTabela; i++)
    {
        entrada = dec2bin(i, nEntradas);
        saida = vecSaida(entrada, nEntradas, tipoSaida, nSaidas);

        for(int j = 6-nEntradas; j < 6; j++)
        {
            cout << entrada[j] << ((j == 5) ? " " : "");
            outfile << entrada[j] << " ";
        }
        for(int j = 6-maxSaida; j < 6; j++)
        {
            cout << saida[j];
            outfile << saida[j] << (j == 5 ? "" : " ");
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
    cout << "Tipo de saida (+ ou x):" << endl;
    cin >> tipoSaida;

    criarTabelaVerdade(nEntradas, tipoSaida);

    return 0;
}
