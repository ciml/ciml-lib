#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define M1 3 /// CONJUNTO DE VARIÁVEIS 1
#define M2 3 /// CONJUNTO DE VARIÁVEIS 2
#define PROBLEMA 1 /// 1- MULTIPLICADOR/SOMADOR 2- EVEN PARITY 3- MULTIPLEXER
#define OPERACAO '*'

/// TRANSFORMA NÚMERO BINÁRIO EM DECIMAL
int bin_to_dec(int bin)
{
    int total  = 0;
    int potenc = 1;

    while(bin > 0)
    {
        total += bin % 10 * potenc;
        bin = bin / 10;
        potenc = potenc * 2;
    }
    return total;
}

/// TRANSFORMA NÚMERO DECIMAL EM BINÁRIO
int dec_to_bin(int dec)
{
    int pot = 1;
    int bin = 0;
    while (dec > 0)
    {
        bin += (dec % 2)* pot;
        pot *= 10;
        dec = dec/2;
    }
    return bin;
}

///IMPRIME NUMEROS BINARIOS
void imprime_bin(int n)
{
    int i = 0;
    int vet[M1+M2];
    while (i < M1+M2)
    {
        if (n % 2 == 1)
            vet[i] = 1;
        else
            vet[i] = 0;
        n >>= 1;
        i++;
    }

    for(i = M1+M2-1; i >= 0; i--)
    {
        printf("%d ",vet[i]);
    }
    printf("\n");
}

void geraSaida(int nC,int vet[][nC], int nL)
{
    int i,j,cont = 0;
    int conta = 1;
    int resultado = 0;
    int coluna;
    switch(PROBLEMA)
    {
    case 1:
        for(i = 0; i < nL; i++)
        {
            int um, dois, j, conta;

            conta = 10;
            j = M1 - 1;

            while(j != -1)
            {
                if(j == M1 - 1)
                {
                    um = vet[i][j];
                }
                else
                {
                    if(vet[i][j] == 0)
                    {
                        conta = conta*10;
                    }
                    else
                    {
                        if(vet[i][j] == 1)
                        {
                            um = conta*(vet[i][j]) + um;
                            conta = conta*10;
                        }
                    }
                }
                j = j - 1;
            }

            conta = 10;
            j = M1 + M2 - 1;

            while(j != (M1-1))
            {
                if(j == (M1 + M2 -1))
                {
                    dois = vet[i][j];
                }
                else
                {
                    if(vet[i][j] == 0)
                    {
                        conta = conta*10;
                    }
                    else
                    {
                        if(vet[i][j] == 1)
                        {
                            dois = conta*(vet[i][j]) + dois;
                            conta = conta*10;
                        }
                    }
                }
                j = j - 1;
            }
            int auxiliar = 0;
            switch(OPERACAO)
            {
            case '+':
                auxiliar = bin_to_dec(um) + bin_to_dec(dois);
                vet[i][M1+M2] = auxiliar;
                break;
            case '*':
                auxiliar = bin_to_dec(um) * bin_to_dec(dois);
                vet[i][M1+M2] = auxiliar;
                break;
            default:
                printf("ERROR\n");
            }
        }
        break;
    case 2:
        for(i = 0; i < nL; i++)
        {
            for(j = 0; j < M1+M2; j++)
            {
                if( vet[i][j] == 0)
                    cont++;
            }
            if(cont % 2 ==0)
            {
                vet[i][j] = 1;
            }
            else
            {
                vet[i][j] = 0;
            }
            cont = 0;
        }
        break;
    case 3:
        for(i = 0; i < nL; i++)
        {
            for(j = 0; j < M1; j++)
            {
                resultado+= vet[i][j]*conta;
                conta*=10;
            }
            coluna = bin_to_dec(resultado);
            vet[i][nC-1] = vet[i][coluna+M1];
            resultado = 0;
            conta = 1;
        }
        break;
    }
}

void criaTabela(int nC,int vet[][nC],int nL)
{
    int i, j;
    int variacao = nL/2;
    int cont = 0;

    for(j = 0; j < M1+M2; j++)
    {
        for(i = 0; i < nL; i++)
        {
            if(cont < variacao)
            {
                vet[i][j] = 0;
                cont = cont + 1;
            }
            else
            {
                if(cont == variacao || (cont > variacao && cont < 2*variacao))
                {
                    vet[i][j] = 1;
                    cont = cont + 1;
                }
                else
                {
                    if(cont == 2*variacao)
                    {
                        vet[i][j] = 0;
                        cont = 1;
                    }
                }
            }
        }
        variacao = variacao/2;
        cont = 0;
    }
    geraSaida(nC,vet,nL);
}

/// IMPRIME TABELA VERDADE MULTIPLICADOR/SOMADOR
void imprimeTabela(int nC,int vet[][nC], int nL)
{
    int i = 0,j = 0;
    if(PROBLEMA == 1)
        printf("%d %d %d\n",nL,M1+M2,M1+M2);
    else
        printf("%d %d %d\n",nL,M1+M2,1);
    for(i = 0; i < nL; i++)
    {
        for(j = 0; j < nC; j++)
        {
            if(j != nC-1)
                printf("%d ",vet[i][j]);
            else
            {
                if(PROBLEMA == 1)
                    imprime_bin(vet[i][j]);
                else
                    printf("%d\n",vet[i][j]);
            }
        }
    }
}

int main()
{
    int linhaTabela = pow(2,(M1+M2));
    int colunaTabela = M1+M2+1;


    int tabela[linhaTabela][colunaTabela];
    criaTabela(colunaTabela,tabela,linhaTabela);
    imprimeTabela(colunaTabela,tabela,linhaTabela);


    return 0;
}
