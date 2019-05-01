#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

vector<int> dec2bin(int num, int aux)
{
    vector<int> bin(aux);
    for(int i = 0; i < aux; i++, num /= 2)
        if(num > 0)
            bin[i] = num%2;
    reverse(bin.begin(), bin.end());
    return bin;
}

int bin2dec(int num)
{
    int dec = 0, bin = 1;
    while(num > 0)
    {
        dec += (num%10)*bin;
        bin *= 2;
        num /= 10;
    }
    return dec;
}

int computaSaida(vector<int> entrada, int nEntradas, char c)
{
    int numA = 0, numB = 0;
    for(int i = 0; i < (nEntradas/2); i++)
    {
        numA += entrada[i]*pow(2, 2-i);
        numB += entrada[i+3]*pow(2, 2-i); //nEntradas/2
    }
    return ((c == '+') ? (numA+numB) : (numA*numB));
}

vector<int> vecSaida(vector<int> entrada, int nEntradas, char c, int numBits)
{
    int saida = computaSaida(entrada, nEntradas, c);
    return dec2bin(saida, numBits);
}

void criarTabelaVerdade(int nEntradas, char tipoSaida)
{
    ofstream outfile("../circuito/tabela6x.txt"); //pasta do projeto principal

    vector<int> entrada(nEntradas, 1);
    int maiorSaida = computaSaida(entrada, nEntradas, tipoSaida);
    int nLinhasTabela = pow(2, nEntradas), nSaidas = ceil(log2(maiorSaida+1));
    vector<int> saidas(nSaidas);

    cout << "Entradas | Saidas" << endl;
    cout << nLinhasTabela << "        " << nSaidas << endl;
    outfile << nLinhasTabela << " " << nSaidas << endl;

    for(int i = 0; i < nLinhasTabela; i++)
    {
        entrada = dec2bin(i, nEntradas);
        saidas = vecSaida(entrada, nEntradas, tipoSaida, nSaidas);

        cout << "Entrada " << ((i+1 < 10) ? "0" : "") << i+1 << ": ";
        for(int j = 0; j < nEntradas; j++)
        {
            cout << entrada[j] << " ";
            outfile << entrada[j] << " ";
        }
        for(int j = 0; j < nSaidas; j++)
        {
            cout << saidas[j] <<  ((j == nSaidas-1) ? "\n" : " ");
            outfile << saidas[j] <<  ((j == nSaidas-1) ? "\n" : " ");
        }
    }
    outfile.close();
}

int main()
{
    int nEntradas;
    char tipoSaida;

    cout << "Numero de entradas:" << endl;
    cin >> nEntradas;
    cout << "Tipo de saida (+ ou *):" << endl;
    cin >> tipoSaida;

    criarTabelaVerdade(nEntradas, tipoSaida);

    return 0;
}
