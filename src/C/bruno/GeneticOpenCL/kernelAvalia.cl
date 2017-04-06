/*
void avaliaIndividuos(Arvore pop[], float* dados[], int M, int N){

    int i, j = 0;
    float erro = 0;
    for(i = 0; i < NUM_INDIV; i++){
        for(j = 0; j < M; j++){
            erro = erro + executa(&pop[i], dados[j], N);
        }
        //int k; scanf("%d", &k);
        pop[i].aptidao = erro; // + 2 * pop[i].numNos;
        erro = 0;
    }
}


    __kernel void somatorioSequencialEsperto(__global float* values,
                                             __local float* localSum,
                                             __const int length,
                                             __global float* sum){
        int global_id = get_global_id(0);                               
        int local_id = get_local_id(0);
        int group_id = get_group_id(0);
        float accumulator = 0;

        while(global_id < length){
            accumulator += values[global_id];
            global_id += get_global_size(0);
        }

        localSum[local_id] = accumulator;
        barrier(CLK_LOCAL_MEM_FENCE);

        for(int i = get_local_size(0)/2 ; i > 0; i/=2){
            if(local_id < i){
               localSum[local_id] += localSum[local_id + i];
            }
            barrier(CLK_LOCAL_MEM_FENCE);
        }
        
        if(local_id == 0){
            sum[group_id] = localSum[0];
        }
    }
*/

#define TIPO    4

#define VAR     7
#define CONST   8
#define FBIN    2
#define FUN     1

#define PLUS    0
#define MIN     1
#define MULT    2
#define DIV     3

#define SIN     4
#define COS     5
#define SQR     6

#define EXP     12
#define LOG10   13


#define MAX_NOS     10
#define MAX_DEPTH   2
#define MAX_FILHOS  2

#define NUM_INDIV   4

#ifndef PROB_CROSS
#define PROB_CROSS  0.9
#endif // PROB_CROSS

#ifndef PROB_MUT
#define PROB_MUT    0.3
#endif // PROB_MUT

#define NUM_TORNEIO     2
#define ELITISMO        0.05
#define NUM_GERACOES    5

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
        //printf("Sucesso Empilhar 2\n");
        pilha->info[pilha->topo] = info;
    }else{
        printf("Erro empilhar 2\n");
    }
}

float desempilha2(PilhaEx* pilha){
    if(pilha->topo >= 0){
        //printf("Sucesso Desempilhar 2\n");
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


float executa(__global Arvore* arv, __global float* dados, int N, int M, int local_id){
    PilhaEx pilhaEx;
    pilhaEx.topo = -1;

    int j;
    int tipo;

    for(j = arv->numNos -1; j>=0; j= j-1){
        tipo = retornaTipo(arv, j);
        //unpackTipo(arv->informacao[j]);
        //printf("tipo = %d\n", tipo);
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
                empilha2(&pilhaEx,proDiv (desempilha2(&pilhaEx), desempilha2(&pilhaEx)));
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
            case CONST:;//This is an empty statement.
                //int c; scanf("%d", c);
                float valorF = unpackFloat(arv->informacao[j]);
                empilha2(&pilhaEx, valorF);
                break;
            case VAR:;
                int valor2 = unpackInt(arv->informacao[j]);
                empilha2(&pilhaEx, dados[local_id+(valor2*M)]);
                break;
        }
    }

    float erro = desempilha2(&pilhaEx)- dados[local_id+M*(N-1)];
    return erro*erro;
}



__kernel void avaliaIndividuos(__global Arvore* pop,
                               __global float* dados,
                               __local float* error, 
                               __const int M, 
                               __const int N){
    int i, j = 0;
    float erro = 0;


    int local_id = get_local_id(0);
    int local_size = get_local_size(0);
    int group_id = get_group_id(0);


    for(j = 0; j <= (M/local_size) - 1; j++){
        int n = j * local_size + local_id;
        if( n < M){
            erro = erro + executa(&pop[group_id], dados, N, M, local_id);
            //printf("%f, %f\n", dados[local_id+(0*M)], dados[local_id+(1*M)]);
            //printf("%f\n", erro);
        }
    }
    //mudar nome dessas variaveis
    error[local_id] = erro;
    if(group_id == 1){
    	printf("%f\n", error[local_id]);
    }
    barrier(CLK_LOCAL_MEM_FENCE);

    ///redução erros por work group
    for(int i = get_local_size(0)/2 ; i > 0; i/=2){
        if(local_id < i){
           error[local_id] += error[local_id + i];
        }
        barrier(CLK_LOCAL_MEM_FENCE);
    }
        
    if(local_id == 0){
        pop[group_id].aptidao = error[0];
    }

}