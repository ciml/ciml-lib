#include <math.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NCOL 100 /// NÚMERO DE COLUNAS
#define NLIN 1 /// NÚMERO DE LINHAS
#define LB 50 /// LEVEL-BACK
#define PORTAS 5 /// NÚMERO DE PORTAS
#define NPOPULACAO 4 /// NÚMERO DE INDIVÍDUOS NA POPULAÇÃO
#define M1 2 /// CONJUNTO DE VARIÁVEIS 1
#define M2 2  /// CONJUNTO DE VARIÁVEIS 2
///#define SAIDAS (M1 + M2) /// NÚMERO DE SAÍDAS DE CADA INDIVÍDUO
#define NMUTACOES 1 /// QUANTAS MUTAÇÕES SERÃO FEITAS NO INDIVÍDUO


/// Lucas Augusto Müller de Souza
/// Engenharia Computacional - UFJF

/*
PORTAS:

0  - and
1  - or
2  - nand
3  - nor
4  - xor
5  - xnor
6  - A and Not B
7  - A
8  - B and Not A
9  - B
10 - Not B
11 - A or Not B
12 - Not A
13 - B or Not A

*/

typedef struct
{
    int lig1;
    int lig2;
    int porta;

} Gene;

/// FUNÇÕES *****
void criaInd(Gene ind[NLIN][NCOL],int portas[PORTAS]); /// cria um indivíduo, o primeiro indivíduo
int numAleatorio(int a, int b); /// gera número aleatório de 'a' a 'b'
void criaTabela(int vet[][M1+M2+1], int nL, int nC); /// cria tabela verdade
int bin_to_dec(int bin); /// transforma número binário em decimal
int dec_to_bin(int dec); /// transforma núemro decimal em binário
void mutacao(Gene pop[NPOPULACAO][NLIN][NCOL],int portas[PORTAS]); /// iguala os filhos ao pai e faz as mutações neles
void sorteiaSaidas(int saidas[],int numSaidas); /// sorteia os genes de saida
int retornaValor(Gene pop[NLIN][NCOL], int linha[], int saida); /// retorna o valor da saida em determinada linha da tabela
void zeraPontuacao(int pontuacaoIndividuos[]); /// zera a pontuação dos indivíduos
void leTxt(int nC,int tabela[][nC],int nL); /// le tabela verdade do txt e salva na matriz
void imprimeTabela(int nC,int vet[][nC], int nL); /// imprime tabela verdade
int calculaPortasAtivas(Gene pop[NLIN][NCOL],int saida);
///******

int main()
{

    int numSaidas;
    int linhaTabela;
    int colunaTabela;
    scanf("%d %d %d",&linhaTabela,&colunaTabela,&numSaidas);
    Gene pop[NPOPULACAO][NLIN][NCOL];
    int tabela[linhaTabela][colunaTabela+numSaidas];
    int saidas[numSaidas];
    int pontuacaoMaxima = linhaTabela*numSaidas;
    int resultados[NPOPULACAO][numSaidas];
    int pontuacaoIndividuos[NPOPULACAO];
    int portas[PORTAS] = {0,1,2,3,4};


    leTxt(colunaTabela+numSaidas,tabela,linhaTabela);
    imprimeTabela(colunaTabela+numSaidas,tabela,linhaTabela);


    srand(time(NULL));
    criaInd(pop[0],portas); /// iniciando primeiro indivíduo
    sorteiaSaidas(saidas,numSaidas);
    zeraPontuacao(pontuacaoIndividuos);
    int ponttt;
    while(pontuacaoIndividuos[0] != pontuacaoMaxima)
    {

        zeraPontuacao(pontuacaoIndividuos);
        mutacao(pop,portas);
        int i, j, z;
        for(z = 0; z < linhaTabela; z++)
        {
            for(i = 0; i < NPOPULACAO; i++)
            {
                for(j = 0; j < numSaidas; j++)
                {
                    resultados[i][j] = retornaValor(pop[i], tabela[z], saidas[j]);
                }
                int pontuacao = 0;
                for(j = 0; j < numSaidas; j++)
                {
                    if(tabela[z][numSaidas+j] == resultados[i][j])  ///compara o numero de bits iguais na tabela verdade e na saida calculada
                        pontuacao+=1;
                }
                pontuacaoIndividuos[i] += pontuacao;
                //printf("Individuo : %d   Pontuacao: %d\n", i, pontuacaoIndividuos[i]);
            }
        }
        int maiorPont = 0; /// guarda o indice do individuo com maior pontuacao
        for(i = 0; i < NPOPULACAO; i++)
        {
            if(pontuacaoIndividuos[i] >= pontuacaoIndividuos[maiorPont])
            {
                maiorPont = i;
            }
        }

        pontuacaoIndividuos[0] = pontuacaoIndividuos[maiorPont];
        printf("Individuo de maior pontuacao: %d    PONTUACAO: %d    PONTUACAO DESEJADA: %d\n", maiorPont, pontuacaoIndividuos[0], pontuacaoMaxima);
        for(i = 0 ; i < NLIN; i++)
        {
            for(j = 0; j< NCOL; j++)
            {
                pop[0][i][j].lig1 = pop[maiorPont][i][j].lig1;
                pop[0][i][j].lig2 = pop[maiorPont][i][j].lig2;
                pop[0][i][j].porta = pop[maiorPont][i][j].porta;
            }
        }
        ponttt = maiorPont;
    }



    printf("ACHOU!!!  INDIVIDUO %d\n", ponttt);
    printf("PONTUAÇÃO: %d\n", pontuacaoMaxima);
    printf("\a");

    int i, j;
    for(i = 0; i < NCOL; i++) ///criei esse for apenas pra analisar se os dados estavam entrando na matriz de forma correta
    {
        for(j = 0; j < NLIN; j++)
        {
            printf("Número do gene: %d.\n", (M1+M2) + i*NLIN+ j);
            printf("Lig 1: %d \t Lig2: %d \t Porta: %d \n\n", pop[0][j][i].lig1, pop[0][j][i].lig2, pop[0][j][i].porta);
        }
    }
    for(i = 0; i < numSaidas; i++)
    {
        printf("Saida: %d\n", saidas[i]);
        printf("Numero de portas ativas: %d\n",calculaPortasAtivas(pop[0],saidas[i]));
    }
}

///LE A TABELA VERDADE DO TXT
void leTxt(int nC,int tabela[][nC],int nL)
{
    int i = 0;
    int j = 0;
    while(i < nL)
    {
        j = 0;
        while(j < nC)
        {
            scanf("%d ",&tabela[i][j]);
            j++;
        }
        i++;
    }
}

/// CRIA 1 INDIVÍDUO, O PRIMEIRO
void criaInd(Gene ind[NLIN][NCOL],int portas[PORTAS])
{
    int i,j,auxiliar;

    for(i = 0; i < NLIN; i++) /// (For) utilizado para preencher os valores de cada nó do indivíduo
    {
        for(j = 0; j < NCOL; j++)
        {
            if(j == 0 || LB == 0)
            {
                ind[i][j].lig1 = numAleatorio(0, M1 + M2 - 1) ;
                ind[i][j].lig2 = numAleatorio(0, M1 + M2 - 1) ;
            }
            else /// nesse else eu avalio a coluna de acordo com o level-back
            {
                if(LB >= j || LB == -1)
                {
                    ind[i][j].lig1 = numAleatorio(0, (M1+M2) + (NLIN*j) - 1);
                    ind[i][j].lig2 = numAleatorio(0, (M1+M2) + (NLIN*j) - 1);
                }
                else
                {
                    if(LB < j)
                    {
                        int aleatorio = numAleatorio(0,(M1+M2)+ LB*NLIN - 1);
                        if(aleatorio >= 0  && aleatorio < (M1+M2-1))
                        {
                            ind[i][j].lig1 = aleatorio;
                        }
                        else
                        {
                            ind[i][j].lig1 = numAleatorio((M1+M2) + (j - LB)*NLIN, (M1+M2) + (NLIN*j) - 1);
                        }

                        aleatorio = numAleatorio(0,(M1+M2)+ LB*NLIN - 1);

                        if(aleatorio >= 0  && aleatorio < (M1+M2-1))
                        {
                            ind[i][j].lig2 = aleatorio;
                        }
                        else
                        {
                            ind[i][j].lig2 = numAleatorio((M1+M2) + (j - LB)*NLIN, (M1+M2) + (NLIN*j) - 1);
                        }
                    }
                }
            }
            auxiliar = numAleatorio(0, PORTAS);
            ind[i][j].porta = portas[auxiliar];
        }
    }
}

/// GERA NÚMERO ALEATÓRIO NUM INTERVALO [a,b]
int numAleatorio(int a, int b)
{
    return (a + rand()%(b - a + 1)) ;
}

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

/// IMPRIME TABELA VERDADE
void imprimeTabela(int nC,int vet[][nC], int nL)
{
    int i = 0,j = 0;
    for(i = 0; i < nL; i++)
    {
        for(j = 0; j < nC; j++)
        {
            printf("%d  ",vet[i][j]);
        }
        printf("\n");
    }
}

/// ESSA FUNÇÃO IGUALA OS INDIVÍDUOS AO PAI E REALIZA A MUTAÇÃO NELES
void mutacao(Gene pop[NPOPULACAO][NLIN][NCOL],int portas[PORTAS])
{

    int i, j, z,auxiliar;


    for(i = 1; i < NPOPULACAO; i++)
    {
        for(j = 0; j < NLIN; j++)
        {
            for(z = 0; z < NCOL; z++)
            {
                pop[i][j][z].lig1 = pop[0][j][z].lig1;
                pop[i][j][z].lig2 = pop[0][j][z].lig2;
                pop[i][j][z].porta = pop[0][j][z].porta;
            }
        }
    }

    for(z = 0; z < NMUTACOES; z++)
    {
        for(i = 1; i < NPOPULACAO; i++)
        {
            int gen = numAleatorio((M1+M2), (M1+M2) + NCOL*NLIN - 1);
            int linha = (gen - (M1+M2))%(NLIN);
            int coluna  = (gen - (M1+M2))/(NLIN);
            int aleat = numAleatorio(0, 2);
            if(aleat == 0)
            {
                if(coluna == 0 || LB == 0)
                {
                    pop[i][linha][coluna].lig1 = numAleatorio(0, M1 + M2 - 1) ;
                }
                else /// nesse else eu avalio a coluna de acordo com o level-back
                {
                    if(LB >= coluna || LB == -1)
                    {
                        pop[i][linha][coluna].lig1 = numAleatorio(0, (M1+M2) + (NLIN*coluna) - 1);
                    }
                    else
                    {
                        if(LB < coluna && LB > 0)
                        {
                            int aleatorio = numAleatorio(0,(M1+M2)+ LB*NLIN - 1);
                            if(aleatorio >= 0  && aleatorio < (M1+M2-1))
                            {
                                pop[i][linha][coluna].lig1 = aleatorio;
                            }
                            else
                            {
                                pop[i][linha][coluna].lig1 = numAleatorio((M1+M2) + (coluna - LB)*NLIN, (M1+M2) + (NLIN*coluna) - 1);
                            }
                        }
                    }
                }
            }
            else
            {
                if(aleat == 1)
                {
                    if(coluna == 0 || LB == 0)
                    {
                        pop[i][linha][coluna].lig2 = numAleatorio(0, M1 + M2 - 1) ;
                    }
                    else /// nesse else eu avalio a coluna de acordo com o level-back
                    {
                        if(LB >= coluna || LB == -1)
                        {
                            pop[i][linha][coluna].lig2 = numAleatorio(0, (M1+M2) + (NLIN*coluna) - 1);
                        }
                        else
                        {
                            if(LB < coluna && LB > 0)
                            {
                                int aleatorio = numAleatorio(0,(M1+M2)+ LB*NLIN - 1);
                                if(aleatorio >= 0  && aleatorio < (M1+M2-1))
                                {
                                    pop[i][linha][coluna].lig2 = aleatorio;
                                }
                                else
                                {
                                    pop[i][linha][coluna].lig2 = numAleatorio((M1+M2) + (coluna - LB)*NLIN, (M1+M2) + (NLIN*coluna) - 1);
                                }
                            }
                        }
                    }
                }
                else
                {
                    auxiliar = numAleatorio(0, PORTAS);
                    pop[i][linha][coluna].porta = portas[auxiliar];
                }
            }
        }
    }
}

/// ESSA FUNÇÃO ESCOLHE OS GENES DE SAIDA
void sorteiaSaidas(int saidas[], int numSaidas)
{
    int i;

    for(i = 0; i < numSaidas; i++)
    {
        saidas[i] = numAleatorio(0, (NLIN*NCOL)+(M1 + M2) - 1);
    }
}

/// ESSA FUNÇÃO RETORNA O VALOR DA SAIDA
int retornaValor(Gene pop[NLIN][NCOL], int linha[], int saida)
{
    if(saida >= 0 && saida < (M1+M2))
    {
        return linha[saida];
    }
    else
    {
        if(saida >= (M1+M2))
        {
            int numeroLinha = (saida - (M1+M2))%(NLIN);
            int numeroColuna = (saida - (M1+M2))/(NLIN);

            int r1 = retornaValor(pop, linha, pop[numeroLinha][numeroColuna].lig1);
            int r2 = retornaValor(pop, linha, pop[numeroLinha][numeroColuna].lig2);

            /// Váriaveis que serão usadas apenas se a porta for XOR
            int r1n;
            int r2n;
            int op1;
            int op2;
            ///
            switch(pop[numeroLinha][numeroColuna].porta)
            {
            case 0: /// AND
                if(r1 == 1 && r2 == 1)
                    return 1;
                else
                    return 0;
                break;

            case 1: /// OR
                if(r1 == 1 || r2 == 1)
                    return 1;
                else
                    return 0;
                break;

            case 2: /// NAND
                if(r1 == 1 && r2 == 1)
                    return 0;
                else
                    return 1;
                break;

            case 3: /// NOR
                if(r1 == 1 || r2 == 1)
                    return 0;
                else
                    return 1;
                break;

            case 4: /// XOR
                if(r1 == 1)
                    r1n = 0;
                else
                    r1n = 1;

                if(r2 == 1)
                    r2n = 0;
                else
                    r2n = 1;

                if(r1n == 1 && r2 == 1)
                    op1 = 1;
                else
                    op1 = 0;

                if(r1 == 1 && r2n == 1)
                    op2 = 1;
                else
                    op2 = 0;

                if(op1 == 1 || op2 == 1)
                    return 1;
                else
                    return 0;
                break;
            case 5: /// XNOR
                if(r1 == r2)
                    return 1;
                else
                    return 0;
                break;
            case 6: /// A AND NOT B
                if(r1 == 1 && r2 == 0)
                    return 1;
                else
                    return 0;
                break;
            case 7: /// A
                return r1;
                break;
            case 8: /// B AND NOT A
                if(r1 == 0 && r2 == 1)
                    return 1;
                else
                    return 0;
                break;
            case 9: /// B
                return r2;
                break;
            case 10: /// NOT B
                if(r2 == 1)
                    return 0;
                else
                    return 1;
                break;
            case 11: /// A OR NOT B
                if(r1 == 0 && r2 == 1)
                    return 0;
                else
                    return 1;
                break;
            case 12: /// NOT A
                if(r1 == 1)
                    return 0;
                else
                    return 1;
                break;
            case 13: /// B OR NOT A
                if(r1 == 1 && r2 == 0)
                    return 0;
                else
                    return 1;
                break;
            }
        }
    }
    return 0;
}

int calculaPortasAtivas(Gene pop[NLIN][NCOL],int saida)
{
    if(saida >=0 && saida < M1+M2)
        return 0;
    else
    {
        int numeroLinha = (saida - (M1+M2))%(NLIN);
        int numeroColuna = (saida - (M1+M2))/(NLIN);

        return 1 + calculaPortasAtivas(pop,pop[numeroLinha][numeroColuna].lig1) + calculaPortasAtivas(pop,pop[numeroLinha][numeroColuna].lig2);
    }
}

void zeraPontuacao(int pontuacaoIndividuos[])
{
    int i;
    for(i = 0; i < NPOPULACAO; i++)
    {
        pontuacaoIndividuos[i] = 0;
    }
}
