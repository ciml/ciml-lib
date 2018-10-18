#include "../include/operacaoTxt.h"

/*
 * Function:  extraiTabelaVerdadeDoTxt 
 * --------------------
 * This function reads from the file the truth table.
 *
 * numeroDeLinhas: the number of lines from truth table
 * numeroDeColunas : the number of columns from truth table
 * TabelaVerdade: the array to store the truth table
 * NomeDoArquivo: the name of the file
 *
 * returns: this is a void function
 */
void extraiTabelaVerdadeDoTxt(int numeroDeLinhas, int numeroDeColunas,
                              int tabelaVerdade[], char *nomeDoArquivo)
{
    FILE *arquivo = fopen(nomeDoArquivo, "rt");
    int contador = 0;
    if (arquivo == NULL)
        printf("Arquivo não encontrado!\n");
    else
    {
        char aux[100];
        if (fgets(aux, 100, arquivo))
        {
        };
        while (contador < numeroDeLinhas * numeroDeColunas)
        {
            if (fscanf(arquivo, "%d ", &tabelaVerdade[contador]))
            {
            };
            contador++;
        }
    }
    fclose(arquivo);
}

/*
 * Function:  leituraDadosDaTabelaVerdade 
 * --------------------
 * This function reads from the file the main informations 
 * of the truth table.
 *
 * numeroDeLinhas: the intenger to store the number of lines
 * numeroDeColunas : the intenger to store the number of columns
 * NumeroDeEntradas: the intenger to store the number of inputs
 * NumeroDeSaidas: the intenger to store the number of outputs
 * NomeDoArquivo: the name of the file
 *
 * returns: this is a void function
 */
void leituraDadosDaTabelaVerdade(int *numeroDeLinhas, int *numeroDeColunas, int *numeroDeEntradas, int *numeroDeSaidas,
                                 char *nomeDoArquivo)
{
    FILE *arquivo = fopen(nomeDoArquivo, "rt");
    if (arquivo == NULL)
        printf("Arquivo não encontrado!\n");
    else
    {
        if (fscanf(arquivo, "%d %d %d\n", numeroDeLinhas, numeroDeEntradas, numeroDeSaidas))
        {
        };

        *numeroDeColunas = *numeroDeEntradas + *numeroDeSaidas;
    }
    fclose(arquivo);
}

/*
 * Function:  imprimePontuacaoEmEstatisticas 
 * --------------------
 * This function writes at the file estatisticas.txt 
 * the fitness of the individual.
 *
 * pontuacaoFinal: the fitness of the individual
 *
 * returns: this is a void function
 */
void imprimePontuacaoEmEstatisticas(int pontuacaoFinal)
{
    FILE *arquivo = fopen("testes.txt", "a");
    if (arquivo == NULL)
        printf("Arquivo estatisticas.txt nao encontrado!\n");
    else
    {
        fprintf(arquivo, "Pontuacao Final: %d\t", pontuacaoFinal);
    }
    fclose(arquivo);
}

/*
 * Function:  imprimeEstatisticasNoTxt 
 * --------------------
 * This function writes at the file estatisticas.txt 
 * some informations about the code.
 *
 * Tempo: the time to find an eletric circuit
 * to solve the truth table
 * GeracaoInicial: the initial generation
 * GeracaoFinal: the final generation
 * Seed: the seed used at the librarie random.h 
 *
 * returns: this is a void function
 */
void imprimeEstatisticasNoTxt(double tempo, long int *geracaoInicial, long int *geracaoFinal, int seed)
{
    FILE *arq;
    arq = fopen("testes.txt", "a");
    fprintf(arq, "Tempo gasto: %f\tNumero de geracoes: %ld\tSemente: %d\n", tempo, *geracaoInicial - *geracaoFinal, seed);
    fclose(arq);
}
