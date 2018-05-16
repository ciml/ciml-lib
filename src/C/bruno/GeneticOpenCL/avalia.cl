__kernel void avaliaIndividuos(__global Arvore* pop,
                               __global float* dados,
                               __local float* error
                               )
{

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
                               )
{
    int i, k = 0;
    float erro = 0;

    int group_id = get_group_id(0);

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
                    empilhaF(&pilhaEx, dados[k+valor2*M]);
                    break;
            }
        }

        float erroF = desempilhaF(&pilhaEx)-dados[k+M*(N-1)];

        erro = erro + (erroF*erroF);//pown(erroF, 2);

    }

    pop[group_id].aptidao = ( isinf( erro ) || isnan( erro ) ) ? INFINITY : erro;

}


__kernel void  avaliaIndividuosSequencial(__global Arvore* pop,
                                          __global float* dados,
                                          __local float* error )
{

    int k, l;

    PilhaEx pilhaEx;
    pilhaEx.topo = -1;

    float num, div;

    for(l = 0; l < NUM_INDIV; l++){

    float erro = 0;
    
        for(k = 0; k < M; k++){

            int j;
            int tipo;
            for(j = pop[l].numNos -1; j>=0; j--){
                tipo = retornaTipo(&pop[l], j);
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
                        float valorF = unpackFloat(pop[l].informacao[j]);
                        empilhaF(&pilhaEx, valorF);
                        break;
                    case VAR:;
                        int valor2 = unpackInt(pop[l].informacao[j]);
                        empilhaF(&pilhaEx, dados[k+valor2*M]);
                        break;
                }
                barrier(CLK_GLOBAL_MEM_FENCE);
            }
             barrier(CLK_GLOBAL_MEM_FENCE);

            float erroF = desempilhaF(&pilhaEx)-dados[k+M*(N-1)];
            erro = erro + (erroF*erroF);//pown(erroF, 2);

        }

        pop[l].aptidao = ( isinf( erro ) || isnan( erro ) ) ? INFINITY : erro;
        barrier(CLK_GLOBAL_MEM_FENCE);
    }
}