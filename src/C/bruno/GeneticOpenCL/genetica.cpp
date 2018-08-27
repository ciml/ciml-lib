#include "genetica.h"

void inicializaCheia(Arvore pop[], int num, int indice, int* conjuntoOpTerm,int NUM_OPBIN, int NUM_OPUN, int N, int* seed,
                     float maxDados, float minDados){
    int i;
    while((num-indice)%MAX_DEPTH != 0){
        criaCheia(&pop[indice], MAX_DEPTH, conjuntoOpTerm, NUM_OPBIN, NUM_OPUN, N, seed, maxDados, minDados);
        indice++;
    }

    int k = (num - indice) / MAX_DEPTH;
    int prof = 1;
    for(i = indice; i < num; ){
        int j;
        for(j = 0; j < k; j++){
            criaCheia(&pop[i], prof, conjuntoOpTerm, NUM_OPBIN, NUM_OPUN, N, seed, maxDados, minDados);
            i++;
            if(i >= num) break;
        }
        prof++;
    }


}

void inicializaAleatorio(Arvore pop[], int num, int indice, int* conjuntoOpTerm,int NUM_OPBIN, int NUM_OPUN, int N, int* seed,
                         float maxDados, float minDados){
    int i;
    for(i = indice; i < num; i++){
        geradorArvore(&pop[i], MAX_NOS, conjuntoOpTerm, NUM_OPBIN, NUM_OPUN, N, seed, maxDados, minDados);
    }

}

void inicializaPopulacao(Arvore pop[], int* conjuntoOpTerm, int NUM_OPBIN, int NUM_OPUN, int N, int* seed, float maxDados, float minDados){
    inicializaAleatorio(pop, NUM_INDIV/2, 0, conjuntoOpTerm, NUM_OPBIN, NUM_OPUN, N, seed, maxDados, minDados);
    inicializaCheia(pop, NUM_INDIV, NUM_INDIV/2, conjuntoOpTerm, NUM_OPBIN, NUM_OPUN, N, seed, maxDados, minDados);
//    int i;
//    for(i = 0; i < NUM_INDIV/2; i++){
//        geradorArvore(&pop[i], MAX_NOS); //sortear a quantidade de nós em determinado intervalo
//    }
//    for( ; i<NUM_INDIV; i++){
//        criaCheia(&pop[i], MAX_DEPTH); // sortear a profundidade máxima em determinado intervalo
//    }
}


void avaliaIndividuos(Arvore pop[], float** dados, int M, int N){

    int i, j = 0;
    float erro = 0;
    for(i = 0; i < NUM_INDIV; i++){
        //printf("i = %d \n", i);
        for(j = 0; j < M; j++){
            erro = erro + executa(&pop[i], dados[j], N);
            //if(i == 0)
            //   printf("%f\n", erro);
        }
        //int k; scanf("%d", &k);
        pop[i].aptidao = erro; // + 2 * pop[i].numNos;
        erro = 0;
    }

return;

//    int k, l;
//
//
//    //int local_id = get_local_id(0);
//    //int local_size = get_local_size(0);
//    //int group_id = get_group_id(0);
//    //printf("%d\n", group_id);
//
//    PilhaEx pilhaEx;
//    pilhaEx.topo = -1;
//
//    float num, div;
//    for(l = 0; l < NUM_INDIV; l++){
//    float erro = 0;
//
//        for(k = 0; k < M ; k++){
//
//            int j;
//            int tipo;
//            for(j = pop[l].numNos -1; j>=0; j--){
//                tipo = retornaTipo(&pop[l], j);
//                switch(tipo){
//                    case PLUS:
//                        empilhaF(&pilhaEx,desempilhaF(&pilhaEx) + desempilhaF(&pilhaEx));
//                        //printf("plus = %f\n", pilhaEx.info[pilhaEx.topo]);
//                        break;
//                    case MIN:
//                        empilhaF(&pilhaEx,desempilhaF(&pilhaEx) - desempilhaF(&pilhaEx));
//                        //printf("min = %f\n", pilhaEx.info[pilhaEx.topo]);
//                        break;
//                    case MULT:
//                        empilhaF(&pilhaEx,desempilhaF(&pilhaEx) * desempilhaF(&pilhaEx));
//                        //printf("mult = %f\n", pilhaEx.info[pilhaEx.topo]);
//                        break;
//                    case DIV:
//                        num = desempilhaF(&pilhaEx);
//                        div = desempilhaF(&pilhaEx);
//                        empilhaF(&pilhaEx,proDiv(num,div));
//                        break;
//                    case SIN:
//                        empilhaF(&pilhaEx,sin(desempilhaF(&pilhaEx)));
//                        //printf("sin = %f\n", pilhaEx.info[pilhaEx.topo]);
//                        break;
//                    case COS:
//                        empilhaF(&pilhaEx,cos(desempilhaF(&pilhaEx)));
//                        //printf("cos = %f\n", pilhaEx.info[pilhaEx.topo]);
//                        break;
//                    case SQR:
//                       empilhaF(&pilhaEx,proSqrt(desempilhaF(&pilhaEx)));
//                       //printf("sqr = %f\n", pilhaEx.info[pilhaEx.topo]);
//                        break;
//                    case EXP:
//                        empilhaF(&pilhaEx,exp(desempilhaF(&pilhaEx)));
//                        //printf("exp = %f\n", pilhaEx.info[pilhaEx.topo]);
//                        break;
//                    case CTE:;//This is an empty statement.
//                        //int c; scanf("%d", c);
//                        float valorF = unpackFloat(pop[l].informacao[j]);
//                        empilhaF(&pilhaEx, valorF);
//                        //printf("cte = %.20f\n", pilhaEx.info[pilhaEx.topo]);
//                        //empilhaF(&pilhaEx, as_float(pop[group_id].informacao[j]<<TIPO));
//                        break;
//                    case VAR:;
//                        int valor2 = unpackInt(pop[l].informacao[j]);
//                        empilhaF(&pilhaEx, dados[k+valor2*M]);
//                        //printf("var = %f\n", pilhaEx.info[pilhaEx.topo]);
//                        break;
//                }
//            }
//
//            float erroF = desempilhaF(&pilhaEx)-dados[k+M*(N-1)];
//            //printf("%f - %f\n", erroF, dados[k+M*(N-1)]);
//            erro = erro + (erroF*erroF);//pown(erroF, 2);//pown(erroF,2);
//
//            //if(group_id == 0)
//            //printf("%.20f\n", erro);
//        }
//
//        pop[l].aptidao = ( isinf( erro ) || isnan( erro ) ) ? INFINITY : erro; //erro;
//    }
}

//TODO: imprimir profundidade tambem
void imprimePopulacao(Arvore pop[], char** LABELS){
    int i;
    for(i = 0; i < NUM_INDIV; i++){
        imprimeArvorePre(&pop[i], LABELS);
        //imprimeArvoreNivel(&pop[i]);
        printf(" | Numero de nos: %d | Erro: %.25f |\n",pop[i].numNos, pop[i].aptidao);
    }
    printf("\n");
}

void imprimeMelhor(Arvore pop[], char** LABELS){
    int i;
    int indiceMelhor = 0;
    float melhor = pop[0].aptidao;
    for(i = 1; i < NUM_INDIV; i++){
        if(pop[i].aptidao < melhor){
            indiceMelhor = i;
            melhor = pop[i].aptidao;
        }
    }
    printf("MELHOR: ");
    imprimeArvorePre(&pop[indiceMelhor], LABELS);
    printf(" | Numero de nos: %d | Erro: %.25f |\n",pop[indiceMelhor].numNos, pop[indiceMelhor].aptidao);
    //return pop[indiceMelhor];
}

void ordenaElite(Arvore pop[], int k){
    Arvore aux;
    int i, j;
    int indiceMelhor;

    for(i = 0; i < k; i++){
        indiceMelhor = i;
        for(j = i+1; j < NUM_INDIV; j++){
            if(pop[j].aptidao < pop[indiceMelhor].aptidao){
                indiceMelhor = j;
            }
        }
        aux = pop[i];
        pop[i] = pop[indiceMelhor];
        pop[indiceMelhor] = aux;
    }
}

int selecionaElite(Arvore popAtual[], Arvore popFutura[]){
    int i;
    int cont = (int)(ELITISMO*NUM_INDIV);

    if(cont%2 != 0)
        cont++;
    ordenaElite(popAtual, cont);
    for(i = 0; i < cont; i++){
        popFutura[i] = popAtual[i];
    }

    return cont;
}


int torneio(Arvore pop[], int* seed){
    int indiceMelhor = rand2(seed) % NUM_INDIV;
    int indice;
    int i;
    for(i = 0; i < NUM_TORNEIO-1; i++){
        indice = rand2(seed)%NUM_INDIV;
        if(pop[indice].aptidao < pop[indiceMelhor].aptidao){
            indiceMelhor = indice;
        }
    }
    return indiceMelhor;
}

int criterioDeParada(int iteracoes){
    int it = CARGA/NUM_INDIV;
    //printf("%d\n", it);
    return iteracoes < it;
    //return iteracoes < NUM_GERACOES;

}
