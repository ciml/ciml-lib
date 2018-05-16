typedef struct{
    int numeroFilhos[MAX_NOS];
    int informacao[MAX_NOS];
    int numNos;
    float aptidao;
}Arvore;

typedef struct{
    int topo;
    float info[MAX_NOS];
}PilhaEx;

void empilha2(PilhaEx* pilha, float info){
    (pilha->topo)++;
    if(pilha->topo < MAX_NOS){
        pilha->info[pilha->topo] = info;
    }else{
        printf("Erro empilhar 2\n");
    }
}

float desempilha2(PilhaEx* pilha){
    if(pilha->topo >= 0){
        float val = pilha->info[pilha->topo];
        pilha->topo--;
        return val;
    }else{
        printf("Erro desempilhar 2\n");
        return -1;
    }
}



/*
*Funcao para retornar um valor Int que representa um Float
*@param floatVal  : valor a ser transformado
*
*@return unsigned int: o inteiro desejado deslocado "TIPO" bits para a direita
*********************************************************/
unsigned int floatToInt(float floatVal){
    return (*(int*)(&floatVal) >> TIPO);
}


/*
*Funcao para retornar um valor Float que estava salvo em um Int
*@param intVal  : valor a ser transformado
*
*@return float: o float desejado
*********************************************************/
float intToFloat(unsigned int intVal){
    intVal = intVal << TIPO;
    return *(float*)(&intVal);
}


/*
*Funcao para armazenar duas informações em um unico Int
*@param tipo  : primeira informação que deve ter no máxima um tamanho "TIPO" em bits
*       valor : segunda informação (int ou float já transformado em int e deslocado "TIPO" vezes
*
*@return unsigned int: a informação codificada
*********************************************************/
unsigned int packInfo(int tipo, int valor){
    unsigned int informacao = 0;
    informacao = (tipo << (32-TIPO)) | valor;
    return informacao;
}


/*
*Funcao para armazenar duas informações em um unico Int
*@param tipo  : primeira informação que deve ter no máxima um tamanho "TIPO" em bits
*       valor : segunda informação (int)
*
*@return unsigned int: a informação codificada
*********************************************************/
unsigned int packInt(int tipo, int valor){
    unsigned int informacao = 0;
    informacao = (tipo << (32-TIPO)) | valor;
    return informacao;
}

/*
*Funcao para armazenar duas informações em um unico Int
*@param tipo  : primeira informação que deve ter no máxima um tamanho "TIPO" em bits
*       valor : segunda informação (float)
*
*@return unsigned int: a informação codificada
*********************************************************/
unsigned int packFloat(int tipo, float valor){
    unsigned int informacao = 0;
    unsigned int valorInt = floatToInt(valor);

    informacao = (tipo << (32-TIPO)) | valorInt;
    return informacao;
}

/*
*Funcao para retornar o 'tipo' de uma informação codificada
*@param info  : informação codificada
*
*@return int: o tipo
*********************************************************/
int unpackTipo(unsigned int info){
    unsigned int tipo = (info>>(32-TIPO));
    return tipo;
}

/*
*Funcao para retornar um Int de uma informação codificada
*@param info  : informação codificada
*
*@return int: a informação
*********************************************************/
int unpackInt(unsigned int info){
    int valor = (info << TIPO) >> TIPO;
    return valor;
}

/*
*Funcao para retornar um Float de uma informação codificada
*@param info  : informação codificada
*
*@return float: a informação
*********************************************************/
float unpackFloat(unsigned int info){
    //info = info;
    float valorF = intToFloat(info);
    return valorF;
}


int retornaTipo(__global Arvore* arv, int j){
    if(arv->numeroFilhos[j] == 0){
        return unpackTipo(arv->informacao[j]);
    } else {
        return unpackInt(arv->informacao[j]);
    }
}

float proDiv(float num, float div){
    if(div == 0){
        return 1;
    } else {
        return (num/div);
    }
}



float proSqrt(float num){
    if(num < 0){
        return 1;
    } else {
        return sqrt(num);
    }
}


__kernel void avaliaIndividuos(__global Arvore* pop,
                               __global float* dados,
                               __local float* error, 
                               __const int M, 
                               __const int N){
    int i, k = 0;
    float erro = 0;


    //int local_id = get_local_id(0);
    //int local_size = get_local_size(0);
    int group_id = get_group_id(0);

    PilhaEx pilhaEx;
    pilhaEx.topo = -1;


        for(k = 0; k < M ; k++){
            int j;
            int tipo;
            for(j = pop[group_id].numNos -1; j>=0; j= j-1){
                tipo = retornaTipo(&pop[group_id], j);
                switch(tipo){
                    case PLUS:
                        empilha2(&pilhaEx,desempilha2(&pilhaEx) + desempilha2(&pilhaEx));
                        break;
                    case MIN:
                        empilha2(&pilhaEx,desempilha2(&pilhaEx) - desempilha2(&pilhaEx));
                        break;
                    case MULT:
                        empilha2(&pilhaEx,desempilha2(&pilhaEx) * desempilha2(&pilhaEx));
                        break;
                    case DIV:
                        empilha2(&pilhaEx,proDiv(desempilha2(&pilhaEx), desempilha2(&pilhaEx)));
                        break;
                    case SIN:
                        empilha2(&pilhaEx,sin(desempilha2(&pilhaEx)));
                        break;
                    case COS:
                        empilha2(&pilhaEx,cos(desempilha2(&pilhaEx)));
                        break;
                    case SQR:
                       empilha2(&pilhaEx,proSqrt(desempilha2(&pilhaEx)));
                        break;
                    case CTE:;//This is an empty statement.
                        //int c; scanf("%d", c);
                        float valorF = unpackFloat(pop[group_id].informacao[j]);
                        empilha2(&pilhaEx, valorF);
                        break;
                    case VAR:;
                        int valor2 = unpackInt(pop[group_id].informacao[j]);
                        empilha2(&pilhaEx, dados[k+valor2*M]);
                        break;
                }
            }

            float erroF = desempilha2(&pilhaEx)- dados[k+M*(N-1)];
            erro = erro + (erroF * erroF);
    }

    pop[group_id].aptidao = erro;

}