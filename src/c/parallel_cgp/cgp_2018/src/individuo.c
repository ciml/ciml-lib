#include <stdlib.h>
#include "../include/individuo.h"

Infos infos;

/*
 * Function:  empilha 
 * --------------------
 * This function push an value at the stack.
 *
 * pilha: the stack
 * info : the value to be pushed
 *
 * returns: this is a void function
 */
void empilha(Pilha *pilha, int info)
{
    pilha->topo++;
    if (pilha->topo < MAX_NOS)
    {
        pilha->info[pilha->topo] = info;
    }
    else
    {
        printf("Erro empilhar\n");
    }
}

/*
 * Function:  desempilha 
 * --------------------
 * This function pop the last value of the stack.
 *
 * pilha: the stack
 *
 * returns: this is a void function
 */
void desempilha(Pilha *pilha)
{
    if (pilha->topo >= 0)
    {
        pilha->info[pilha->topo] = -1;
        pilha->topo--;
    }
    else
    {
        printf("Erro desempilhar\n");
    }
}

/*
 * Function:  topo 
 * --------------------
 * This function queries the top value of the stack and return it.
 *
 * pilha: the stack
 *
 * returns: the top value of the stack
 */
int topo(Pilha *pilha)
{
    if (pilha->topo >= 0)
        return pilha->info[pilha->topo];
    else
        printf("retorno impossivel!\n");

    return 0;
}

/*
 * Function:  retornaEDesempilha 
 * --------------------
 * This function pops the top value from the stack and return it.
 *
 * pilha: the stack
 *
 * returns: the value removed from the stack
 */
int retornaEDesempilha(Pilha *pilha)
{
    int auxiliar = topo(pilha);
    desempilha(pilha);

    return auxiliar;
}

/*
 * Function:  retornaPosicaoPopulacao 
 * --------------------
 * This function calculates the position of the gene 
 * at the population array.
 *
 * individuo: the position of the individual at the population
 * linha: the line of the gene at the 2d array
 * coluna: the column of the gene at the 2d array
 *
 * returns: the position of the gene at the population array
 */
int retornaPosicaoDoGene(int individuo, int linha, int coluna)
{
    return individuo * NLIN * NCOL + linha * NCOL + coluna;
}

/*
 * Function:  inicializaPopulacao 
 * --------------------
 * This function initialize the individuals.
 *
 * Populacao: the array of individuals
 *
 * returns: this is a void function
 */
void inicializaPopulacao(Gene populacao[])
{
    int posicao = 0;
    for (int individuo = 0; individuo < NPOPULACAO; individuo++)
    {
        for (int linha = 0; linha < NLIN; linha++)
        {
            for (int coluna = 0; coluna < NCOL; coluna++)
            {
                posicao = retornaPosicaoDoGene(individuo, linha, coluna);
                populacao[posicao].lig1 = -1;
                populacao[posicao].lig2 = -1;
                populacao[posicao].porta = -1;
                populacao[posicao].saida = malloc(sizeof(int) * infos.numeroDeLinhasDaTabela);
                populacao[posicao].computado = FALSE;
            }
        }
    }
}

int geraNumeroAleatorio(int menorValor, int maiorValor)
{
    return (int)(menorValor + rand() % (maiorValor - menorValor + 1));
}

/*
 * Function:  aleatorizaLBMenorColuna 
 * --------------------
 * Initialize the population
 *
 * Populacao: the array of individuals
 *
 * returns: this is a void function
 */
void aleatorizaLBMenorColuna(int individuo, int linha, int coluna, Gene populacao[], int ligante)
{
    int aleatorio = geraNumeroAleatorio(0, (infos.numeroDeEntradas) + LB * NLIN - 1);
    int posicao = retornaPosicaoDoGene(individuo, linha, coluna);
    if (ligante == LIGANTE1)
    {
        if (aleatorio >= 0 && aleatorio < (infos.numeroDeEntradas - 1))
        {
            populacao[posicao].lig1 = aleatorio;
        }
        else
        {
            populacao[posicao].lig1 = geraNumeroAleatorio((infos.numeroDeEntradas) + (coluna - LB) * NLIN, (infos.numeroDeEntradas) + (NLIN * coluna) - 1);
        }
    }
    else if (ligante == LIGANTE2)
    {
        if (aleatorio >= 0 && aleatorio < (infos.numeroDeEntradas - 1))
        {
            populacao[posicao].lig2 = aleatorio;
        }
        else
        {
            populacao[posicao].lig2 = geraNumeroAleatorio((infos.numeroDeEntradas) + (coluna - LB) * NLIN, (infos.numeroDeEntradas) + (NLIN * coluna) - 1);
        }
    }
}

void aleatorizaLigantes(int individuo, int linha, int coluna, Gene populacao[], int ligante)
{
    int posicao = 0;
    posicao = retornaPosicaoDoGene(individuo, linha, coluna);
    if (ligante == LIGANTE1)
    {
        if (coluna == 0 || LB == 0) ///aleatoriza uma entrada
        {
            populacao[posicao].lig1 = geraNumeroAleatorio(0, infos.numeroDeEntradas - 1);
        }

        else if (LB >= coluna || LB == -1) ///aleatoriza qualquer coluna antes da entrada
        {
            populacao[posicao].lig1 = geraNumeroAleatorio(0, (infos.numeroDeEntradas) + (NLIN * coluna) - 1);
        }
        else if (LB < coluna) ///aleatoriza coluna até o levels-back
        {
            aleatorizaLBMenorColuna(individuo, linha, coluna, populacao, LIGANTE1);
        }
    }
    else if (ligante == LIGANTE2)
    {
        if (coluna == 0 || LB == 0) ///aleatoriza uma entrada
        {
            populacao[posicao].lig2 = geraNumeroAleatorio(0, infos.numeroDeEntradas - 1);
        }

        else if (LB >= coluna || LB == -1) ///aleatoriza qualquer coluna antes da entrada
        {
            populacao[posicao].lig2 = geraNumeroAleatorio(0, (infos.numeroDeEntradas) + (NLIN * coluna) - 1);
        }
        else if (LB < coluna) ///aleatoriza coluna até o levels-back
        {
            aleatorizaLBMenorColuna(individuo, linha, coluna, populacao, LIGANTE2);
        }
    }
}

/*
 * Function:  criaPrimeiroIndividuo 
 * --------------------
 * This function create the parent 
 * of the individuals, it defines the logic gates 
 * and the connections between genes, based on 
 * an acyclic graph and the level's back
 * defined by the user.
 *
 * Populacao: the array of individuals
 * Portas: the array of logic gates to be used
 * defined by the user
 *
 * returns: this is a void function
 */
void criaPrimeiroIndividuo(Gene populacao[], int portas[])
{
    int auxiliar = 0, posicao = 0;
    int individuo = 0;
    for (int linha = 0; linha < NLIN; linha++) /// (For) utilizado para preencher os valores de cada nó do indivíduo
    {
        for (int coluna = 0; coluna < NCOL; coluna++)
        {
            aleatorizaLigantes(individuo, linha, coluna, populacao, LIGANTE1);
            aleatorizaLigantes(individuo, linha, coluna, populacao, LIGANTE2);
            auxiliar = geraNumeroAleatorio(0, PORTAS - 1);
            posicao = retornaPosicaoDoGene(individuo, linha, coluna);
            populacao[posicao].porta = portas[auxiliar];
        }
    }
}

/*
 * Function:  geneEhSaida 
 * --------------------
 * This function verifies if the gene
 * is an output gene.
 *
 * Gene: the number of the gene
 *
 * returns: 1 if the gene is an output gene
 * and 0 if not
 */
int geneEhSaida(int gene)
{
    if (gene >= (infos.numeroDeEntradas) + (NLIN * NCOL))
        return 1;
    else
        return 0;
}

/*
 * Function:  mutaSaida 
 * --------------------
 * This function mutates the aleatorized output of the individual.
 *
 * Gene: the number of the gene
 * Saidas: the array with the outputs of the individual.
 *
 * returns: this is a void function
 */
void mutaSaida(int gene, int individuo, int saidas[])
{
    int saida;
    int novaPosicaoDeSaida = 0;
    int colunaSaidas = gene - (infos.numeroDeEntradas + (NCOL * NLIN));
    int flag = 1;
    while (flag)
    {
        flag = 0;
        novaPosicaoDeSaida = geraNumeroAleatorio(0, (infos.numeroDeEntradas) + (NLIN * NCOL) - 1);
        for (saida = 0; saida < infos.numeroDeSaidas; saida++)
        {
            if (novaPosicaoDeSaida == saidas[individuo * infos.numeroDeSaidas + saida])
            {
                flag = 1;
                break;
            }
        }
    }
    saidas[individuo * infos.numeroDeSaidas + colunaSaidas] = novaPosicaoDeSaida;
}

/*
 * Function:  mutacaoSinglePoint 
 * --------------------
 * This function mutates the aleatorized gene of the individual.
 *
 * Populacao: the array with all the individuals 
 * Individuo: the individual.
 * Portas: the array with the logic gates defined by the user
 * Gene: the number of the gene
 * Saidas: the 2d array with the outputs of the individual
 *
 * returns: this is a void function
 */
void mutacaoSinglePoint(Gene populacao[], int individuo, int portas[], int gene, int saidas[])
{
    int mutacao, auxiliar;

    for (mutacao = 0; mutacao < NMUTACOES; mutacao++)
    {
        if (geneEhSaida(gene))
        {
            mutaSaida(gene, individuo, saidas);
        }
        else
        {
            int linha = 0, coluna = 0, posicao = 0;
            calculaPosicao(&linha, &coluna, gene);
            posicao = retornaPosicaoDoGene(individuo, linha, coluna);
            int aleat = geraNumeroAleatorio(0, 2);

            if (aleat == 0)
            {
                aleatorizaLigantes(individuo, linha, coluna, populacao, LIGANTE1);
            }
            else if (aleat == 1)
            {
                aleatorizaLigantes(individuo, linha, coluna, populacao, LIGANTE2);
            }
            else
            {
                auxiliar = geraNumeroAleatorio(0, PORTAS - 1);
                populacao[posicao].porta = portas[auxiliar];
            }
        }
    }
}

int ativoEmCadaSaida(Gene populacao[], int individuo, int geneAtual, int posicaoSorteada)
{
    if (geneAtual >= 0 && geneAtual < infos.numeroDeEntradas)
        return FALSE;
    else
    {
        int linha = 0, coluna = 0, posicao = 0;
        calculaPosicao(&linha, &coluna, geneAtual);
        posicao = retornaPosicaoDoGene(individuo, linha, coluna);
        if (posicao == posicaoSorteada)
            return TRUE;
        ativoEmCadaSaida(populacao, individuo, populacao[posicao].lig1, posicaoSorteada);
        ativoEmCadaSaida(populacao, individuo, populacao[posicao].lig2, posicaoSorteada);
    }

    return FALSE;
}

/*
 * Function:  ehAtivo 
 * --------------------
 * This function verifies if the specified gene 
 * is an active gene.
 *
 * Saidas: the 2d array with the output genes from 
 * every individuals
 * Populacao: the array with all the individuals
 * Individuo: the individual
 * Gene: the number of the gene
 *
 * returns: this function returns 1 if its an active gene
 * and 0 if not.
 */
int ehAtivo(int saidas[], Gene populacao[], int individuo, int gene)
{
    int aux = 0;
    int linha = 0, coluna = 0, posicao = 0;
    calculaPosicao(&linha, &coluna, gene);
    posicao = retornaPosicaoDoGene(individuo, linha, coluna);
    for (int saida = 0; saida < infos.numeroDeSaidas; saida++)
    {
        aux = ativoEmCadaSaida(populacao, individuo, saidas[individuo * infos.numeroDeSaidas + saida], posicao);
        if (aux == TRUE)
            return TRUE;
    }
    return FALSE;
}

/*
 * Function:  mutacao 
 * --------------------
 * This function mutates an individual based on the mutation
 * type defined at individuo.h.
 * MUTACAO = 1 is single point mutation, where we mutate
 * N diferents genes defined at NMUTACOES.
 * MUTACAO = 2 is single active mutation, where we mutate
 * the genes until the aleatorized gene is an active gene.
 *
 * Populacao: the array with all the individuals
 * Individuo: the individual
 * Portas: the array of logic gates defined by the user
 * Saidas: the 2d array with the output genes from 
 * every individuals
 *
 * returns: this is a void function
 */
void mutacao(Gene populacao[], int individuo, int portas[], int saidas[])
{
    int gene;
    int criterioDeParada = TRUE;
    while (criterioDeParada)
    {
        gene = geraNumeroAleatorio(infos.numeroDeEntradas, infos.numeroDeSaidas + infos.numeroDeEntradas + (NCOL * NLIN) - 1);
        mutacaoSinglePoint(populacao, individuo, portas, gene, saidas);
        if (MUTACAO == 1)
            criterioDeParada = FALSE;
        else if (MUTACAO == 2)
        {
            if (gene >= (infos.numeroDeEntradas) + (NLIN * NCOL) || ehAtivo(saidas, populacao, individuo, gene))
                criterioDeParada = FALSE;
        }
    }
}

/*
 * Function:  igualaIndividuosAoPai 
 * --------------------
 * This function copies the informations of
 * the parent to the others individuals.
 *
 * Populacao: the array with all the individuals
 * Saidas: the 2d array with the output genes from 
 * every individuals
 *
 * returns: this is a void function
 */
void igualaIndividuosAoPai(Gene populacao[], int saidas[])
{
    int posicaoDoFilho = 0, posicaoDoPai = 0;
    for (int individuo = 1; individuo < NPOPULACAO; individuo++)
    {
        for (int linha = 0; linha < NLIN; linha++)
        {
            for (int coluna = 0; coluna < NCOL; coluna++)
            {
                posicaoDoPai = retornaPosicaoDoGene(0, linha, coluna);
                posicaoDoFilho = retornaPosicaoDoGene(individuo, linha, coluna);
                populacao[posicaoDoFilho].lig1 = populacao[posicaoDoPai].lig1;
                populacao[posicaoDoFilho].lig2 = populacao[posicaoDoPai].lig2;
                populacao[posicaoDoFilho].porta = populacao[posicaoDoPai].porta;
                populacao[posicaoDoFilho].computado = FALSE;
            }
        }
    }

    for (int individuo = 1; individuo < NPOPULACAO; individuo++)
    {
        for (int saida = 0; saida < infos.numeroDeSaidas; saida++)
        {
            saidas[individuo * infos.numeroDeSaidas + saida] = saidas[saida];
        }
    }
}

/*
 * Function:  portaAND 
 * --------------------
 * This function calculate the AND gate
 * between entrada1 and entrada2.
 *
 * Entrada1: the first input of the gene
 * Entrada2: the second input of th gene
 *
 * returns: the output of the AND gate
 */
int portaAND(int entrada1, int entrada2)
{
    if (entrada1 == 1 && entrada2 == 1)
        return 1;
    else
        return 0;
}

/*
 * Function:  portaOR 
 * --------------------
 * This function calculate the OR gate
 * between entrada1 and entrada2.
 *
 * Entrada1: the first input of the gene
 * Entrada2: the second input of th gene
 *
 * returns: the output of the OR gate
 */
int portaOR(int entrada1, int entrada2)
{
    if (entrada1 == 1 || entrada2 == 1)
        return 1;
    else
        return 0;
}

/*
 * Function:  portaNAND 
 * --------------------
 * This function calculate the NAND gate
 * between entrada1 and entrada2.
 *
 * Entrada1: the first input of the gene
 * Entrada2: the second input of th gene
 *
 * returns: the output of the NAND gate
 */
int portaNAND(int entrada1, int entrada2)
{
    if (entrada1 == 1 && entrada2 == 1)
        return 0;
    else
        return 1;
}

/*
 * Function:  portaNOR 
 * --------------------
 * This function calculate the NOR gate
 * between entrada1 and entrada2.
 *
 * Entrada1: the first input of the gene
 * Entrada2: the second input of the gene
 *
 * returns: the output of the NOR gate
 */
int portaNOR(int entrada1, int entrada2)
{
    if (entrada1 == 1 || entrada2 == 1)
        return 0;
    else
        return 1;
}

/*
 * Function:  portaXOR 
 * --------------------
 * This function calculate the XOR gate
 * between entrada1 and entrada2.
 *
 * Entrada1: the first input of the gene
 * Entrada2: the second input of th gene
 *
 * returns: the output of the XOR gate
 */
int portaXOR(int entrada1, int entrada2)
{
    if (entrada1 != entrada2)
        return 1;
    else
        return 0;
}

/*
 * Function:  portaXNOR 
 * --------------------
 * This function calculate the XNOR gate
 * between entrada1 and entrada2.
 *
 * Entrada1: the first input of the gene
 * Entrada2: the second input of th gene
 *
 * returns: the output of the XNOR gate
 */
int portaXNOR(int entrada1, int entrada2)
{
    if (entrada1 == entrada2)
        return 1;
    else
        return 0;
}

/*
 * Function:  portaAAndNotB 
 * --------------------
 * This function calculate the AND gate
 * between entrada1 and entrada2 complemented.
 *
 * Entrada1: the first input of the gene
 * Entrada2: the second input of th gene
 *
 * returns: the output of the AND gate
 */
int portaAAndNotB(int entrada1, int entrada2)
{
    if (entrada1 == 1 && entrada2 == 0)
        return 1;
    else
        return 0;
}

/*
 * Function:  portaBAndNotBA
 * --------------------
 * This function calculate the AND gate
 * between entrada2 and entrada1 complemented.
 *
 * Entrada1: the first input of the gene
 * Entrada2: the second input of th gene
 *
 * returns: the output of the AND gate
 */
int portaBAndNotA(int entrada1, int entrada2)
{
    if (entrada1 == 0 && entrada2 == 1)
        return 1;
    else
        return 0;
}

/*
 * Function:  portaNotA 
 * --------------------
 * This function calculate the NOT gate
 * of entrada1. 
 *
 * Entrada1: the first input of the gene
 *
 * returns: the output of the NOT gate
 */
int portaNotA(int entrada1)
{
    if (entrada1 == 1)
        return 0;
    else
        return 1;
}

/*
 * Function:  portaNotB 
 * --------------------
 * This function calculate the NOT gate
 * of entrada2. 
 *
 * Entrada2: the second input of the gene
 *
 * returns: the output of the NOT gate
 */
int portaNotB(int entrada2)
{
    if (entrada2 == 1)
        return 0;
    else
        return 1;
}

/*
 * Function:  portaAOrNotB 
 * --------------------
 * This function calculate the OR gate
 * between entrada1 and entrada2 complemented.
 *
 * Entrada1: the first input of the gene
 * Entrada2: the second input of th gene
 *
 * returns: the output of the OR gate
 */
int portaAOrNotB(int entrada1, int entrada2)
{
    if (entrada1 == 0 && entrada2 == 1)
        return 0;
    else
        return 1;
}

/*
 * Function:  portaBOrNotA 
 * --------------------
 * This function calculate the OR gate
 * between entrada2 and entrada1 complemented.
 *
 * Entrada1: the first input of the gene
 * Entrada2: the second input of th gene
 *
 * returns: the output of the OR gate
 */
int portaBOrNotA(int entrada1, int entrada2)
{
    if (entrada1 == 1 && entrada2 == 0)
        return 0;
    else
        return 1;
}

/*
 * Function:  calculaSaidaDoGene 
 * --------------------
 * This function calculates the output
 * of the gene based on the inputs and
 * the logic gate.
 *
 * Entrada1: the first input of the gene
 * Entrada2: the second input of th gene
 * portaLogica: the logic gate of the gene
 *
 * returns: the output of the gene
 */
int calculaSaidaDoGene(int entrada1, int entrada2, int portaLogica)
{

    switch (portaLogica)
    {
    case 0: /// AND
        return portaAND(entrada1, entrada2);

    case 1: /// OR
        return portaOR(entrada1, entrada2);

    case 2: /// NAND
        return portaNAND(entrada1, entrada2);

    case 3: /// NOR
        return portaNOR(entrada1, entrada2);

    case 4: /// XOR
        return portaXOR(entrada1, entrada2);

    case 5: /// XNOR
        return portaXNOR(entrada1, entrada2);

    case 6: /// A AND NOT B
        return portaAAndNotB(entrada1, entrada2);

    case 7: /// A
        return entrada1;

    case 8: /// B AND NOT A
        return portaBAndNotA(entrada1, entrada2);

    case 9: /// B
        return entrada2;

    case 10: /// NOT B
        return portaNotB(entrada2);

    case 11: /// A OR NOT B
        return portaAOrNotB(entrada1, entrada2);

    case 12: /// NOT A
        return portaNotA(entrada1);

    case 13: /// B OR NOT A
        return portaBOrNotA(entrada1, entrada2);

    default:
        perror("Porta nao existe!");
        exit(1);
    }
}

/*
 * Function:  calculaPosicao 
 * --------------------
 * This function calculates the line and
 * the column of the gene.
 *
 * Linha: the memory of the variable to save the line of the gene
 * Coluna: the memory of the variable to save the column of the gene
 * Gene: the number of the gene
 *
 * returns: this is a void function
 */
void calculaPosicao(int *linha, int *coluna, int valor)
{
    if (valor >= infos.numeroDeEntradas)
    {
        *linha = (valor - infos.numeroDeEntradas) % (NLIN);
        *coluna = (valor - infos.numeroDeEntradas) / (NLIN);
    }
    else
        printf("Valor nao pode ser calculado!\n");
}

/*
 * Function:  calculaNumeroDeGenesAtivos 
 * --------------------
 * This function calculates the number of
 * active gene from an individual.
 *
 * Populacao: the array of individuals
 * Individuo: the individual
 * Saida: the output gene
 *
 * returns: the number of active genes
 */
int calculaNumeroDePortasAtivas(Gene populacao[], int individuo, int saida)
{
    if (saida >= 0 && saida < infos.numeroDeEntradas)
        return 1;
    else
    {
        int linha = 0, coluna = 0, posicao = 0;

        calculaPosicao(&linha, &coluna, saida);
        posicao = retornaPosicaoDoGene(individuo, linha, coluna);

        return 1 + calculaNumeroDePortasAtivas(populacao, individuo, populacao[posicao].lig1) +
               calculaNumeroDePortasAtivas(populacao, individuo, populacao[posicao].lig2);
    }
}

/*
 * Function:  tornaPaiDaProximaGeracao 
 * --------------------
 * This function copies the informations 
 * of the better individual to the parent.
 *
 * Populacao: the array of individuals
 * IndividuoDeMaiorPontuacao: the position of the better individual
 * Saidas: the outputs gene from every individuals
 *
 * returns: this functions is a void function
 */
void tornaPaiDaProximaGeracao(Gene populacao[], int individuoDeMaiorPontuacao, int saidas[])
{
    int linha = 0;
    int coluna = 0;
    int posicaoDoPai = 0;
    int posicaoDoFilho = 0;
    for (linha = 0; linha < NLIN; linha++)
    {
        for (coluna = 0; coluna < NCOL; coluna++)
        {
            posicaoDoPai = retornaPosicaoDoGene(0, linha, coluna);
            posicaoDoFilho = retornaPosicaoDoGene(individuoDeMaiorPontuacao, linha, coluna);
            populacao[posicaoDoPai].lig1 = populacao[posicaoDoFilho].lig1;
            populacao[posicaoDoPai].lig2 = populacao[posicaoDoFilho].lig2;
            populacao[posicaoDoPai].porta = populacao[posicaoDoFilho].porta;
        }
    }
    for (int saida = 0; saida < infos.numeroDeSaidas; saida++)
    {
        saidas[saida] = saidas[individuoDeMaiorPontuacao * infos.numeroDeSaidas + saida];
    }
}

/*
 * Function:  criaVetorPosOrdemStack 
 * --------------------
 * This function is a iterative postorder
 * traversal through the individual
 * creating an postorder stack.
 *
 * Populacao: the array of individuals
 * Individuo: the individual
 * Saida: the output gene
 * PosOrdem: the stack to save the postorder traversal
 *
 * returns: this is a void function
 */
void criaVetorPosOrdemStack(Gene populacao[], int individuo, int saida, Pilha *posOrdem)
{
    Pilha *pilha1 = malloc(sizeof(Pilha));
    int temp = 0;
    int posicao = 0;
    int linha = 0, coluna = 0;

    pilha1->topo = -1;

    if (saida >= infos.numeroDeEntradas)
        empilha(pilha1, saida);
    else
        return;

    while (pilha1->topo >= 0)
    {
        temp = retornaEDesempilha(pilha1);
        empilha(posOrdem, temp);

        if (temp >= infos.numeroDeEntradas)
        {
            calculaPosicao(&linha, &coluna, temp);
            posicao = retornaPosicaoDoGene(individuo, linha, coluna);
            empilha(pilha1, populacao[posicao].lig1);
            empilha(pilha1, populacao[posicao].lig2);
        }
    }

    free(pilha1);
}

/*
 * Function:  avaliaPontuacaoPilha 
 * --------------------
 * This function calculates the number of hits from an individual
 * using the post order stack.
 *
 * Populacao: the individuals array
 * Individuo: the individual
 * Saida: the output gene
 * TabelaVerdade: the truth table array
 * ColunaDaSaida: the column of the output
 * 
 *
 * returns: the number of hits of the individual
 */
int avaliaPontuacao(Gene populacao[], int individuo, int saida, int tabelaVerdade[], int colunaDaSaida)
{
    int linha = 0;
    int coluna = 0;
    int acertos = 0;
    int posicao;

    if (saida < infos.numeroDeEntradas)
    {
        for (int j = 0; j < infos.numeroDeLinhasDaTabela; j++)
        {
            if (tabelaVerdade[j * infos.numeroDeColunasDaTabela + saida] == tabelaVerdade[j * infos.numeroDeColunasDaTabela + infos.numeroDeEntradas + colunaDaSaida])
            {
                acertos++;
            }
        }
    }
    else
    {
        Pilha *vetorPosOrdem = malloc(sizeof(Pilha));
        vetorPosOrdem->topo = -1;

        criaVetorPosOrdemStack(populacao, individuo, saida, vetorPosOrdem);

        for (int i = 0; i < infos.numeroDeLinhasDaTabela; i++)
        {
            Pilha *pilha = malloc(sizeof(Pilha));
            pilha->topo = -1;
            for (int j = vetorPosOrdem->topo; j > -1; j--)
            {
                if (vetorPosOrdem->info[j] < infos.numeroDeEntradas)
                {
                    //printf("Empilhando Linha:%d Coluna:%d Valor:%d \n", i, vetor->info[j], entrada[i][vetor->info[j]]);
                    empilha(pilha, tabelaVerdade[i * infos.numeroDeColunasDaTabela + vetorPosOrdem->info[j]]);
                }
                else
                {
                    calculaPosicao(&linha, &coluna, vetorPosOrdem->info[j]);
                    //printf("PORTA:%d Entrada1:%d Entrada2:%d Saida:%d\n",individuo[linha][coluna].porta, auxiliar, auxiliar2, calculaSaidaDoGene(auxiliar, auxiliar2, individuo[linha][coluna].porta));
                    posicao = retornaPosicaoDoGene(individuo, linha, coluna);
                    empilha(pilha, calculaSaidaDoGene(retornaEDesempilha(pilha), retornaEDesempilha(pilha), populacao[posicao].porta));
                }
            }
            if (topo(pilha) == tabelaVerdade[i * infos.numeroDeColunasDaTabela + infos.numeroDeEntradas + colunaDaSaida])
                acertos++;
            free(pilha);
        }
        free(vetorPosOrdem);
    }

    return acertos;
}