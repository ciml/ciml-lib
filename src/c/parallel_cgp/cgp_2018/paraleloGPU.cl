typedef struct
{
    int lig1;
    int lig2;
    int porta;
    int *saida;
    int computado;

} Gene;

typedef struct
{
    int numeroDeSaidas;
    int numeroDeLinhasDaTabela;
    int numeroDeColunasDaTabela;
    int numeroDeEntradas;
    int nlin;
    int ncol;
    int npopulacao;
} Infos;

typedef struct
{
    int topo;
    int info[10000];
} Pilha;


/* Functions related with Stack operation */
void empilha(Pilha *pilha, int info)
{
    pilha->topo++;
    if (pilha->topo < 10000)
    {
        pilha->info[pilha->topo] = info;
    }
    else
    {
        printf("Erro empilhar\n");
    }
}

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

int topo(Pilha *pilha)
{
    if (pilha->topo >= 0)
        return pilha->info[pilha->topo];
    else
        printf("retorno impossivel!\n");

    return 0;
}

int retornaEDesempilha(Pilha *pilha)
{
    int auxiliar = topo(pilha);
    desempilha(pilha);

    return auxiliar;
}

/* Functions related with position at population array*/
void calculaPosicao(int *linha, int *coluna, int valor,__global Infos* infos)
{
    if (valor >= infos->numeroDeEntradas)
    {
        *linha = (valor - infos->numeroDeEntradas) % (infos->nlin);
        *coluna = (valor - infos->numeroDeEntradas) / (infos->nlin);
    }
    else
        printf("Valor nao pode ser calculado!\n");
}

int retornaPosicaoDoGene(int individuo, int linha, int coluna,__global Infos *infos)
{
    return individuo * infos->nlin * infos->ncol + linha * infos->ncol + coluna;
}

/* Functions related with individuals fitness*/
int portaAND(int entrada1, int entrada2)
{
    return entrada1 && entrada2;
}

int portaOR(int entrada1, int entrada2)
{
    return entrada1 || entrada2;
}

int portaNAND(int entrada1, int entrada2)
{
    return !(entrada1 && entrada2);
}

int portaNOR(int entrada1, int entrada2)
{
    return !(entrada1 || entrada2);
}

int portaXOR(int entrada1, int entrada2)
{
    return entrada1 != entrada2;
}

int portaXNOR(int entrada1, int entrada2)
{
    if (entrada1 == entrada2)
        return 1;
    else
        return 0;
}

int portaAAndNotB(int entrada1, int entrada2)
{
    if (entrada1 == 1 && entrada2 == 0)
        return 1;
    else
        return 0;
}

int portaBAndNotA(int entrada1, int entrada2)
{
    if (entrada1 == 0 && entrada2 == 1)
        return 1;
    else
        return 0;
}

int portaNotA(int entrada1)
{
    if (entrada1 == 1)
        return 0;
    else
        return 1;
}

int portaNotB(int entrada2)
{
    if (entrada2 == 1)
        return 0;
    else
        return 1;
}

int portaAOrNotB(int entrada1, int entrada2)
{
    if (entrada1 == 0 && entrada2 == 1)
        return 0;
    else
        return 1;
}

int portaBOrNotA(int entrada1, int entrada2)
{
    if (entrada1 == 1 && entrada2 == 0)
        return 0;
    else
        return 1;
}

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
        printf("Porta nao existe!");
    }

    return -1;
}

void criaVetorPosOrdemStack(__global Gene populacao[], int individuo, int saida,
    Pilha *posOrdem,__global Infos *infos)
{
    Pilha pilha1;
    int temp = 0;
    int posicao = 0;
    int linha = 0, coluna = 0;

    pilha1.topo = -1;

    if (saida >= infos->numeroDeEntradas)
        empilha(&pilha1, saida);
    else
        return;

    while (pilha1.topo >= 0)
    {
        temp = retornaEDesempilha(&pilha1);
        empilha(posOrdem, temp);

        if (temp >= infos->numeroDeEntradas)
        {
            calculaPosicao(&linha, &coluna, temp, infos);
            posicao = retornaPosicaoDoGene(individuo, linha, coluna, infos);
            empilha(&pilha1, populacao[posicao].lig1);
            empilha(&pilha1, populacao[posicao].lig2);
        }
    }
}

int avaliaPontuacao(__global Gene populacao[], int individuo, int saida,__global int tabelaVerdade[],
    int linhaTabela,__global Infos *infos)
{
    int linha = 0;
    int coluna = 0;
    int posicao;

    if (saida < infos->numeroDeEntradas)
    {
        return tabelaVerdade[linhaTabela * infos->numeroDeColunasDaTabela + saida];
    }
    else
    {
        Pilha vetorPosOrdem;
        vetorPosOrdem.topo = -1;

        criaVetorPosOrdemStack(populacao, individuo, saida, &vetorPosOrdem, infos);

        Pilha pilha;
        pilha.topo = -1;
        for (int j = vetorPosOrdem.topo; j > -1; j--)
        {
            if (vetorPosOrdem.info[j] < infos->numeroDeEntradas)
            {
                empilha(&pilha, tabelaVerdade[linhaTabela * infos->numeroDeColunasDaTabela + vetorPosOrdem.info[j]]);
            }
            else
            {
                calculaPosicao(&linha, &coluna, vetorPosOrdem.info[j], infos);
                posicao = retornaPosicaoDoGene(individuo, linha, coluna, infos);
                empilha(&pilha, calculaSaidaDoGene(retornaEDesempilha(&pilha), retornaEDesempilha(&pilha), populacao[posicao].porta));
            }
        }
        return topo(&pilha);
    }
    return 5000;
}

__kernel void avaliacaoGPU(__global Gene *populacao, __global int *tabelaVerdade,
    __global int *saidas, __global int *pontos, __global Infos *infos)
{
    int local_id =  get_local_id(0);
    int group_id =  get_group_id(0);
    int group_size = get_local_size(0);
    pontos[group_id * group_size + local_id] = 0;
    int resultado;

    /*Evaluate outputs and compares with truth table*/
    for(int i = 0; i < infos->numeroDeSaidas; i++)
    {
        resultado = avaliaPontuacao(populacao, group_id + 1, saidas[(group_id + 1) * infos->numeroDeSaidas + i], tabelaVerdade, local_id, infos);
        if(resultado == tabelaVerdade[local_id * infos->numeroDeColunasDaTabela + infos->numeroDeEntradas + i])
            pontos[group_id * group_size + local_id] += 1;
    }

    barrier(CLK_LOCAL_MEM_FENCE);

    /*Execute reduction*/
    for(int i = group_size/2; i > 0; i /= 2)
    {
      if(local_id < i)
      {
        pontos[group_id * group_size + local_id] += pontos[group_id * group_size + local_id + i];
      }
      barrier(CLK_LOCAL_MEM_FENCE);
   }
}