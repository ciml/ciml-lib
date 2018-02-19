//#pragma OPENCL EXTENSION all : disable

typedef struct{
    int numeroFilhos[MAX_NOS];
    int informacao[MAX_NOS];
    int numNos;
    float aptidao;
}Arvore;

typedef struct{
    int topo;
    int info[MAX_NOS];
}Pilha;

typedef struct{
    int topo;
    float info[MAX_NOS];
}PilhaEx;

void empilha(Pilha* pilha, int info){
    (pilha->topo)++;
    if(pilha->topo < MAX_NOS){
        pilha->info[pilha->topo] = info;
    }else{
        //printf("Erro empilhar\n");
    }
}

int desempilha(Pilha* pilha){
    if(pilha->topo >= 0){
        int val = pilha->info[pilha->topo];
        pilha->topo--;
        return val;
    }else{
        //printf("Erro desempilhar\n");
        return -1;
    }
}

void empilhaF(PilhaEx* pilha, float info){
    (pilha->topo)++;
    if(pilha->topo < MAX_NOS){
        pilha->info[pilha->topo] = info;
    }else{
        //printf("Erro empilhar 2\n");
    }
}

float desempilhaF(PilhaEx* pilha){
    if(pilha->topo >= 0){
        float val = pilha->info[pilha->topo];
        pilha->topo--;
        return val;
    }else{
        //printf("Erro desempilhar 2\n");
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
    return (*(unsigned int*)(&floatVal) >> TIPO);
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
    //if(i == 1) printf("tipo ini = %d\n", tipo);

    informacao = (tipo << (32-TIPO)) | valorInt;
    //if(i == 1) printf("tipo ini = %d\n", unpackTipo(informacao));
    return informacao;
}

/*
*Funcao para retornar o 'tipo' de uma informação codificada
*@param info  : informação codificada
*
*@return int: o tipo
*********************************************************/
unsigned int unpackTipo(unsigned int info){
    unsigned int tipo = (info>>(32-TIPO));
    return tipo;
}

/*
*Funcao para retornar um Int de uma informação codificada
*@param info  : informação codificada
*
*@return int: a informação
*********************************************************/
unsigned int unpackInt(unsigned int info){
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
    //printf("%f, %f\n",num, div);
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
                               __local float* error
                               ){

    int i, k = 0;
    float erro = 0;

    int local_id = get_local_id(0);
    int group_id = get_group_id(0);

    PilhaEx pilhaEx;
    pilhaEx.topo = -1;

    error[local_id] = 0.0f;
    float num, div;

    #ifndef NUM_POINTS_IS_NOT_DIVISIBLE_BY_LOCAL_SIZE
   /* When we know that NUM_POINTS is divisible by LOCAL_SIZE then we can avoid a
      comparison in each iteration due to the guarantee of not having work-items
      accessing beyond the available amount of points. */
    for(k = 0; k < (M/LOCAL_SIZE) ; k++){

    #else
        for(k = 0; k < ceil( M/ (float)LOCAL_SIZE ) ; k++){
            
            if( k * LOCAL_SIZE + local_id < M){
    #endif
            int j;
            int tipo;

            for(j = pop[group_id].numNos -1; j>=0; j= j-1){
                tipo = retornaTipo(&pop[group_id], j);

                switch(tipo){
                    case PLUS:
                        empilhaF(&pilhaEx,desempilhaF(&pilhaEx) + desempilhaF(&pilhaEx));
                        break;
                    case MIN:
                        empilhaF(&pilhaEx,desempilhaF(&pilhaEx) - desempilhaF(&pilhaEx));
                        break;
                    case MULT:
                        empilhaF(&pilhaEx,desempilhaF(&pilhaEx) * desempilhaF(&pilhaEx));
                        break;
                    case DIV:
                        num = desempilhaF(&pilhaEx);
                        div = desempilhaF(&pilhaEx);
                        empilhaF(&pilhaEx,proDiv(num,div));
                        break;
                    case SIN:
                        empilhaF(&pilhaEx,sin(desempilhaF(&pilhaEx)));
                        break;
                    case COS:
                        empilhaF(&pilhaEx,cos(desempilhaF(&pilhaEx)));
                        break;
                    case SQR:
                       empilhaF(&pilhaEx,proSqrt(desempilhaF(&pilhaEx)));
                        break;
                    case EXP:
                        empilhaF(&pilhaEx,exp(desempilhaF(&pilhaEx)));
                        break;
                    case CTE:;
                        float valorF = unpackFloat(pop[group_id].informacao[j]);
                        empilhaF(&pilhaEx, valorF);
                        break;
                    case VAR:;
                        int valor2 = unpackInt(pop[group_id].informacao[j]);
                        empilhaF(&pilhaEx, dados[k*LOCAL_SIZE+local_id+(valor2*M)]);
                        break;
                }
            }

            float erroF = desempilhaF(&pilhaEx)- dados[k*LOCAL_SIZE+local_id+M*(N-1)];
            erro = erro + (erroF * erroF);
            //if( isinf( erro ) || isnan( erro ) ) break;

    #ifdef NUM_POINTS_IS_NOT_DIVISIBLE_BY_LOCAL_SIZE
        }
    #endif
    }

    error[local_id] = erro;
    barrier(CLK_LOCAL_MEM_FENCE);
/*
    error[local_id] = erro;
    pop[group_id].aptidao = 0;
    barrier(CLK_LOCAL_MEM_FENCE);
    for(i = 0; i < LOCAL_SIZE; i++){
        pop[group_id].aptidao += error[i];
   }
   */

    ///redução erros por work group
    for(i =  LOCAL_SIZE_ROUNDED_UP_TO_POWER_OF_2 /2 ; i > 0; i>>=1){
        barrier(CLK_LOCAL_MEM_FENCE);


    #ifndef LOCAL_SIZE_IS_NOT_POWER_OF_2
        if( local_id < i )
    #else
        /* LOCAL_SIZE is not power of 2, so we need to perform an additional
        * check to ensure that no access beyond PE's range will occur. */ 
        if( (local_id < i) && (local_id + i < LOCAL_SIZE) )
    #endif 
           error[local_id] += error[local_id + i];
    }
        
    if(local_id == 0){
        pop[group_id].aptidao = ( isinf( error[0] ) || isnan( error[0] ) ) ? INFINITY : error[0]; //error[0];
    }

}


__kernel void avaliaIndividuosCPU(__global Arvore* pop,
                               __global float* dados,
                               __local float* error
                               ){
    int i, k = 0;
    float erro = 0;


    //int local_id = get_local_id(0);
    //int local_size = get_local_size(0);
    int group_id = get_group_id(0);
    //printf("%d\n", group_id);

    PilhaEx pilhaEx;
    pilhaEx.topo = -1;

    float num, div;
    
    for(k = 0; k < M ; k++){

        int j;
        int tipo;
        for(j = pop[group_id].numNos -1; j>=0; j--){
            tipo = retornaTipo(&pop[group_id], j);
            switch(tipo){
                case PLUS:
                    empilhaF(&pilhaEx,desempilhaF(&pilhaEx) + desempilhaF(&pilhaEx));
                    //printf("plus = %f\n", pilhaEx.info[pilhaEx.topo]);
                    break;
                case MIN:
                    empilhaF(&pilhaEx,desempilhaF(&pilhaEx) - desempilhaF(&pilhaEx));
                    //printf("min = %f\n", pilhaEx.info[pilhaEx.topo]);
                    break;
                case MULT:
                    empilhaF(&pilhaEx,desempilhaF(&pilhaEx) * desempilhaF(&pilhaEx));
                    //printf("mult = %f\n", pilhaEx.info[pilhaEx.topo]);
                    break;
                case DIV:
                    num = desempilhaF(&pilhaEx);
                    div = desempilhaF(&pilhaEx);
                    empilhaF(&pilhaEx,proDiv(num,div));
                    break;
                case SIN:
                    empilhaF(&pilhaEx,sin(desempilhaF(&pilhaEx)));
                    //printf("sin = %f\n", pilhaEx.info[pilhaEx.topo]);
                    break;
                case COS:
                    empilhaF(&pilhaEx,cos(desempilhaF(&pilhaEx)));
                    //printf("cos = %f\n", pilhaEx.info[pilhaEx.topo]);
                    break;
                case SQR:
                   empilhaF(&pilhaEx,proSqrt(desempilhaF(&pilhaEx)));
                   //printf("sqr = %f\n", pilhaEx.info[pilhaEx.topo]);
                    break;
                case EXP:
                    empilhaF(&pilhaEx,exp(desempilhaF(&pilhaEx)));
                    //printf("exp = %f\n", pilhaEx.info[pilhaEx.topo]);
                    break;
                case CTE:;//This is an empty statement.
                    //int c; scanf("%d", c);
                    float valorF = unpackFloat(pop[group_id].informacao[j]);
                    empilhaF(&pilhaEx, valorF);
                    //printf("cte = %.20f\n", pilhaEx.info[pilhaEx.topo]);
                    //empilhaF(&pilhaEx, as_float(pop[group_id].informacao[j]<<TIPO));
                    break;
                case VAR:;
                    int valor2 = unpackInt(pop[group_id].informacao[j]);
                    empilhaF(&pilhaEx, dados[k+valor2*M]);
                    //printf("var = %f\n", pilhaEx.info[pilhaEx.topo]);
                    break;
            }
        }

        float erroF = desempilhaF(&pilhaEx)-dados[k+M*(N-1)];
        //printf("%f - %f\n", erroF, dados[k+M*(N-1)]);
        erro = erro + (erroF*erroF);//pown(erroF, 2);//pown(erroF,2);

        //if(group_id == 0)
        //printf("%.20f\n", erro);
    }

    pop[group_id].aptidao = ( isinf( erro ) || isnan( erro ) ) ? INFINITY : erro; //erro;

}

/******************************************/



int rand2(int *seed){
    
    int s  = *seed;
    s = (unsigned int)(s * 16807) % 2147483647;//(int)(pown(2.0, 31)-1));
    *seed = s;
    return s;

}

float randomProb(int* seed){
    return (float)rand2(seed) / 2147483647;//pown(2.0, 31);
}


float randomConst(int* seed){
    float random = ((float)rand2(seed))/2147483647;
    float range = maxDados - minDados;
    float result = (range*random) + minDados;
    //printf("const = %.32f\n", result);
    return result;
}

int randomType(int* seed){
    int tipoAleatorio = rand2(seed)%(NUM_OPBIN+NUM_OPUN+(1)+N-1);
    return tipoAleatorio;
}


int randomLeafType(int* seed){
    int tipoAleatorio = (rand2(seed)%N) + NUM_OPBIN + NUM_OPUN;
    return tipoAleatorio;
}

int randomNodeType(int* seed){
    int tipoAleatorio = (rand2(seed)%(NUM_OPBIN+NUM_OPUN));
    return tipoAleatorio;
}


int calculaTamanhoSubArvore(__global Arvore* arv, int indice){
    int soma = arv->numeroFilhos[indice];
    int tam = 1;

    while(soma != tam-1){
        indice++;
        soma += arv->numeroFilhos[indice];
        tam++;
    }
    return tam;
}

void geradorArvore(Arvore* arv, int maxTam, __global int* conjuntoOpTerm, int* seed){
    Pilha pilha;
    pilha.topo = -1;

    int indice = 0;
    int ultimo = 0;
    int soma = 1;

    int aux[MAX_NOS];
    int sorteio, j, num, tipo;

    //usar 'num' o inves de j pra nao ter variavel sobrando...
    for(j = 0; j < maxTam; j++){
        aux[j] = 0;
    }

    arv->numNos = 0;
    empilha(&pilha, indice);

    while(pilha.topo != -1){
        indice = desempilha(&pilha);

        if(aux[indice] == 0){

            //int rdmType = randomType(seed);

           // printf("RDMtipo = %d\n", rdmType);
           // barrier(CLK_GLOBAL_MEM_FENCE);
            //printf("seed = %d\n", *seed);
            //barrier(CLK_GLOBAL_MEM_FENCE);

            sorteio = conjuntoOpTerm[randomType(seed)];

            //printf("sorteio = %d\n", sorteio);


            tipo = unpackTipo(sorteio);

           // printf("tipo = %d\n", tipo);
           // barrier(CLK_GLOBAL_MEM_FENCE);
            if(tipo == VAR || tipo == CTE){
                num = 0;
            } else if (tipo == FUN){
                num = 1;
            } else if (tipo == FBIN){
                num = 2;
            }

            arv->numeroFilhos[indice] = num;
            arv->numNos++;
            soma += num;

            if(soma > maxTam){
               // printf("chegou\n");
                soma-=arv->numeroFilhos[indice];
                arv->numeroFilhos[indice] = 0;
                sorteio = conjuntoOpTerm[randomLeafType(seed)];
                num = 0;
                tipo = unpackTipo(sorteio);
            }


            if(tipo == CTE){
                sorteio = packFloat(CTE, randomConst(seed));
            }
            arv->informacao[indice] = sorteio;
        }

        if(aux[indice] < arv->numeroFilhos[indice]){
            empilha(&pilha, indice);
            aux[indice]++;
            ultimo++;
            empilha(&pilha, ultimo);
        }
    }
}



void shift(__global Arvore* arv, int tam, int indice){//indice a partir de onde começa o shift, tam = tamanho do shift
    int i;
    if(tam > 0){
        for(i = arv->numNos-1; i >= indice; i--){
            arv->numeroFilhos[i+tam] = arv->numeroFilhos[i];
            arv->informacao[i+tam] = arv->informacao[i];
        }
    } else {
        for(i = indice; i < arv->numNos; i++){
            arv->numeroFilhos[i+tam] = arv->numeroFilhos[i];
            arv->informacao[i+tam] = arv->informacao[i];
        }
    }
    arv->numNos += tam;
}


void mutacao(__global Arvore* arvore, __global int* conjuntoOpTerm, int* seed/*, int NUM_OPBIN, int NUM_OPUN, int N*/){
    int i;
    //sorteia uma subarvore da arvore inicial
        //pega o tamanho dessa subarvore
    int indiceSub = rand2(seed) % (arvore->numNos);
    int tamanhoSub = calculaTamanhoSubArvore(arvore, indiceSub);

    //confere o tamanho max para criar uma nova arvore
        //tamanho disponivel+tamanho da subarvore sorteada
    int espacoLivre = MAX_NOS-(arvore->numNos)+tamanhoSub;

    //cria uma nova arvore aleatoria de até o tamanho maximo calculado
    //TODO: criar uma nova arvore ou nao?
    Arvore novaArvore;
    //inicializaArvore(&novaArvore);
    geradorArvore(&novaArvore, espacoLivre, conjuntoOpTerm, seed/*, NUM_OPBIN, NUM_OPUN, N*/);


    //determina o tamanho do deslocamento da arvore

//TODO: colocar if para raiz?
//if(indiceSub != 0 && indiceSub != arvore->numNos-1){
        int tamShift = novaArvore.numNos - tamanhoSub;
        shift(arvore, tamShift, indiceSub+tamanhoSub);



    //troca a informação
    for(i = 0; i < novaArvore.numNos; i++){
        arvore->numeroFilhos[i+indiceSub] = novaArvore.numeroFilhos[i];
        arvore->informacao[i+indiceSub] = novaArvore.informacao[i];
    }
}


void  trocaSubArv(__global Arvore* arvMaior, __global Arvore* arvMenor,int ind1,int ind2,int tamanhoSubMenor, int tamanhoSubMaior){
    int i=0, aux;
    while(i<tamanhoSubMenor){
        aux = arvMaior->informacao[ind1+i];
        arvMaior->informacao[ind1+i] = arvMenor->informacao[ind2+i];
        arvMenor->informacao[ind2+i] = aux;
        aux = arvMaior->numeroFilhos[ind1+i];
        arvMaior->numeroFilhos[ind1+i] = arvMenor->numeroFilhos[ind2+i];
        arvMenor->numeroFilhos[ind2+i] = aux;
        i++;
    }
    while(i<tamanhoSubMaior){
        arvMenor->informacao[ind2+i] = arvMaior->informacao[ind1+i];
        arvMenor->numeroFilhos[ind2+i] = arvMenor->numeroFilhos[ind2+i];
    }
}


void crossOver(__global Arvore* arvore1, __global Arvore* arvore2, int* seed){

    int espacoLivre1, espacoLivre2, indiceSub1, indiceSub2, tamanhoSub1, tamanhoSub2;
    int cont = 0;
    do{
        indiceSub1 = rand2(seed) % (arvore1->numNos);
        tamanhoSub1 = calculaTamanhoSubArvore(arvore1, indiceSub1);
        //if(id==0){
        //    printf("seed %d\n",arvore1->numNos);
        //}
        indiceSub2 = rand2(seed) % (arvore2->numNos);
        tamanhoSub2 = calculaTamanhoSubArvore(arvore2, indiceSub2);
        //if(id==0){
        //    printf("seed %d\n",arvore2->numNos);
        //}
        espacoLivre1 = MAX_NOS-(arvore1->numNos)+tamanhoSub1;
        espacoLivre2 = MAX_NOS-(arvore2->numNos)+tamanhoSub2;
        //if(id==0){
        //    printf("el1 = %d\n el2 = %d\n", espacoLivre1, espacoLivre2);
        //}
        if(cont++ == 5) return;
    }while(espacoLivre1-tamanhoSub2 < 0 || espacoLivre2-tamanhoSub1 < 0);
    //if(id==0)
    //printf("cont = %d\n", cont);


    int tamShift1 = tamanhoSub2 - tamanhoSub1;
    //shift(arvore1, tamShift1, indiceSub1+tamanhoSub1);

    int tamShift2 = tamanhoSub1 - tamanhoSub2;


    if(tamanhoSub1 >= tamanhoSub2){
        shift(arvore2, tamShift2, indiceSub2+tamanhoSub2);
        trocaSubArv(arvore1, arvore2, indiceSub1, indiceSub2, tamanhoSub1, tamanhoSub2);
        shift(arvore1, tamShift1, indiceSub1+tamanhoSub1);
    } else {
        shift(arvore1, tamShift1, indiceSub1+tamanhoSub1);
        trocaSubArv(arvore2, arvore1, indiceSub2, indiceSub1, tamanhoSub2, tamanhoSub1);
        shift(arvore2, tamShift2, indiceSub2+tamanhoSub2);
    }

}

void crossOverP(__global Arvore* arvore1, __global Arvore* arvore2, int* seed){

    int espacoLivre1, espacoLivre2, indiceSub1, indiceSub2, tamanhoSub1, tamanhoSub2;
    //printf("start\n");
    int cont = 0;
    //printf("%d  || %d \n", arvore1->numNos, arvore2->numNos);
    do{
        int random = rand2(seed);
        indiceSub1 = random % (arvore1->numNos);

        tamanhoSub1 = calculaTamanhoSubArvore(arvore1, indiceSub1);
        //printf("%d - ", random);
        random = rand2(seed);
        //printf("%d\n", random);
        indiceSub2 = random % (arvore2->numNos);
        tamanhoSub2 = calculaTamanhoSubArvore(arvore2, indiceSub2);


        espacoLivre1 = MAX_NOS-(arvore1->numNos+1)+tamanhoSub1;
        espacoLivre2 = MAX_NOS-(arvore2->numNos+1)+tamanhoSub2;

        //printf("%d - %d || %d - %d\n\n", espacoLivre1, tamanhoSub2, espacoLivre2 , tamanhoSub1);

    }while(espacoLivre1-tamanhoSub2 < 0 || espacoLivre2-tamanhoSub1 < 0);

    //printf("a\n");

    int tamShift1 = tamanhoSub2 - tamanhoSub1;
    //shift(arvore1, tamShift1, indiceSub1+tamanhoSub1);

    int tamShift2 = tamanhoSub1 - tamanhoSub2;
    //printf("b\n");


    if(tamanhoSub1 >= tamanhoSub2){
        shift(arvore2, tamShift2, indiceSub2+tamanhoSub2);
        trocaSubArv(arvore1, arvore2, indiceSub1, indiceSub2, tamanhoSub1, tamanhoSub2);
        shift(arvore1, tamShift1, indiceSub1+tamanhoSub1);
    } else {
        shift(arvore1, tamShift1, indiceSub1+tamanhoSub1);
        trocaSubArv(arvore2, arvore1, indiceSub2, indiceSub1, tamanhoSub2, tamanhoSub1);
        shift(arvore2, tamShift2, indiceSub2+tamanhoSub2);
    }
}

float fabs2(float val1, float val2){
    if(val1-val2 < 0){
        return (-1* (val1-val2));
    } else {
        return (val1-val2);
    }
}

int torneio(__global Arvore* pop, int* seed){
    int indiceMelhor = rand2(seed) % NUM_INDIV;
    int indice;
    int i;
    for(i = 0; i < NUM_TORNEIO-1; i++){
        indice = rand2(seed) % NUM_INDIV;
        if(pop[indice].aptidao < pop[indiceMelhor].aptidao){
        //if(fabs2(pop[indice].aptidao , pop[indiceMelhor].aptidao) < 0.0000001){
            //printf("entrouTorneio %d\n", indiceMelhor);
            indiceMelhor = indice;
        }
    }
    return indiceMelhor;
}


__kernel void evolucao(__global Arvore* popA,
                       __global Arvore* popF,
                       __const int elite,    
                       __global int* conjuntoOpTerm,
                       __global int* seeds ) {
    
    int group_id = get_group_id(0);
    int seed = seeds[group_id];

    int ind1 = torneio(popA, &seed);
    int ind2 = torneio(popA, &seed);


    popF[elite+2*group_id]  = popA[ind1];
    popF[elite+2*group_id+1]= popA[ind2];

    float cross = randomProb(&seed);
    float mut = randomProb(&seed);

    if(cross<=PROB_CROSS){
        crossOver(&popF[elite+2*group_id+1], &popF[elite+2*group_id], &seed);

    }

    if(mut <= PROB_MUT){                               
        mutacao(&popF[elite+2*group_id+1], conjuntoOpTerm, &seed);
        mutacao(&popF[elite+2*group_id], conjuntoOpTerm, &seed);
    }

    seeds[group_id] = seed;

}

__kernel void evolucaoSequencial(__global Arvore* popA,
                                 __global Arvore* popF,
                                 __const int elite,    
                                 __global int* conjuntoOpTerm,
                                 __global int* seeds ) {

    int j;
    for(j = elite; j < NUM_INDIV; j +=2){

        int id = (j/2)-(elite/2);
        int seed = seeds[id];

        int indice1 = torneio(popA, &seed);
        int indice2 = torneio(popA, &seed);


        popF[j] = popA[indice1];
        popF[j+1] = popA[indice2];

        ///testar imprimir o que está retornando na parte randomica
        float cross = randomProb(&seed);
        float mut = randomProb(&seed);

        if(cross <= PROB_CROSS){
            crossOver(&popF[j+1], &popF[j], &seed);
        }

        if(mut <= PROB_MUT){
            mutacao(&popF[j+1], conjuntoOpTerm,  &seed);
            mutacao(&popF[j], conjuntoOpTerm, &seed);
        }
        seeds[id] = seed;
    }
}





__kernel void  avaliaIndividuosSequencial(__global Arvore* pop,
                                          __global float* dados,
                                          __local float* error ){

    int k, l;

    //int local_id = get_local_id(0);
    //int local_size = get_local_size(0);
    //int group_id = get_group_id(0);
    //printf("%d\n", group_id);

    PilhaEx pilhaEx;
    pilhaEx.topo = -1;

    float num, div;
    //__attribute__((opencl_unroll_hint(1)))


    for(l = 0; l < NUM_INDIV; l++){

    //printf("%d\n", l);
    float erro = 0;
    
        for(k = 0; k < M; k++){

            int j;
            int tipo;
            for(j = pop[l].numNos -1; j>=0; j--){
                tipo = retornaTipo(&pop[l], j);
                switch(tipo){
                    case PLUS:
                        empilhaF(&pilhaEx,desempilhaF(&pilhaEx) + desempilhaF(&pilhaEx));
                        //printf("plus = %f\n", pilhaEx.info[pilhaEx.topo]);
                        break;
                    case MIN:
                        empilhaF(&pilhaEx,desempilhaF(&pilhaEx) - desempilhaF(&pilhaEx));
                        //printf("min = %f\n", pilhaEx.info[pilhaEx.topo]);
                        break;
                    case MULT:
                        empilhaF(&pilhaEx,desempilhaF(&pilhaEx) * desempilhaF(&pilhaEx));
                        //printf("mult = %f\n", pilhaEx.info[pilhaEx.topo]);
                        break;
                    case DIV:
                        num = desempilhaF(&pilhaEx);
                        div = desempilhaF(&pilhaEx);
                        empilhaF(&pilhaEx,proDiv(num,div));
                        break;
                    case SIN:
                        empilhaF(&pilhaEx,sin(desempilhaF(&pilhaEx)));
                        //printf("sin = %f\n", pilhaEx.info[pilhaEx.topo]);
                        break;
                    case COS:
                        empilhaF(&pilhaEx,cos(desempilhaF(&pilhaEx)));
                        //printf("cos = %f\n", pilhaEx.info[pilhaEx.topo]);
                        break;
                    case SQR:
                       empilhaF(&pilhaEx,proSqrt(desempilhaF(&pilhaEx)));
                       //printf("sqr = %f\n", pilhaEx.info[pilhaEx.topo]);
                        break;
                    case EXP:
                        empilhaF(&pilhaEx,exp(desempilhaF(&pilhaEx)));
                        //printf("exp = %f\n", pilhaEx.info[pilhaEx.topo]);
                        break;
                    case CTE:;//This is an empty statement.
                        //int c; scanf("%d", c);
                        float valorF = unpackFloat(pop[l].informacao[j]);
                        empilhaF(&pilhaEx, valorF);
                        //printf("cte = %.20f\n", pilhaEx.info[pilhaEx.topo]);
                        //empilhaF(&pilhaEx, as_float(pop[group_id].informacao[j]<<TIPO));
                        break;
                    case VAR:;
                        int valor2 = unpackInt(pop[l].informacao[j]);
                        empilhaF(&pilhaEx, dados[k+valor2*M]);
                        //printf("var = %f\n", pilhaEx.info[pilhaEx.topo]);
                        break;
                }
                barrier(CLK_GLOBAL_MEM_FENCE);
            }
             barrier(CLK_GLOBAL_MEM_FENCE);

            float erroF = desempilhaF(&pilhaEx)-dados[k+M*(N-1)];
            //printf("%f - %f\n", erroF, dados[k+M*(N-1)]);
            erro = erro + (erroF*erroF);//pown(erroF, 2);//pown(erroF,2);

        }

        pop[l].aptidao = ( isinf( erro ) || isnan( erro ) ) ? INFINITY : erro; //erro;
        barrier(CLK_GLOBAL_MEM_FENCE);
    }
}




//TESTE inicial comparando openMP e openCL
/*
__kernel void inicializaAleatorio(__global Arvore* pop, __global int* conjuntoOpTerm, __global int* seeds){
    int group_id = get_group_id(0);
    int seed = seeds[group_id];

    geradorArvore(&pop[group_id], MAX_NOS, conjuntoOpTerm, &seed);

    seeds[group_id] = seed;
}
*/