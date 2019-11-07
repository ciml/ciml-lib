#include "estruturas.cl"
#include "utils.cl"
#include "avalia.cl"

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

            sorteio = conjuntoOpTerm[randomType(seed)];

            tipo = unpackTipo(sorteio);

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


void mutacao(__global Arvore* arvore, __global int* conjuntoOpTerm, int* seed){
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
    geradorArvore(&novaArvore, espacoLivre, conjuntoOpTerm, seed);


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

        indiceSub2 = rand2(seed) % (arvore2->numNos);
        tamanhoSub2 = calculaTamanhoSubArvore(arvore2, indiceSub2);
 
        espacoLivre1 = MAX_NOS-(arvore1->numNos)+tamanhoSub1;
        espacoLivre2 = MAX_NOS-(arvore2->numNos)+tamanhoSub2;

        if(cont++ == 5) return;
    }while(espacoLivre1-tamanhoSub2 < 0 || espacoLivre2-tamanhoSub1 < 0);


    int tamShift1 = tamanhoSub2 - tamanhoSub1;

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

    int cont = 0;
    do{
        int random = rand2(seed);
        indiceSub1 = random % (arvore1->numNos);

        tamanhoSub1 = calculaTamanhoSubArvore(arvore1, indiceSub1);

        random = rand2(seed);

        indiceSub2 = random % (arvore2->numNos);
        tamanhoSub2 = calculaTamanhoSubArvore(arvore2, indiceSub2);


        espacoLivre1 = MAX_NOS-(arvore1->numNos+1)+tamanhoSub1;
        espacoLivre2 = MAX_NOS-(arvore2->numNos+1)+tamanhoSub2;


    }while(espacoLivre1-tamanhoSub2 < 0 || espacoLivre2-tamanhoSub1 < 0);

    int tamShift1 = tamanhoSub2 - tamanhoSub1;
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
            mutacao(&popF[j+1], conjuntoOpTerm, &seed);
            mutacao(&popF[j], conjuntoOpTerm, &seed);
        }
        seeds[id] = seed;
    }
}


__kernel void  replacePopulation(__global Arvore* popA, __global Arvore* popF){
    popA[get_global_id(0)] = popF[get_global_id(0)];
}

__kernel void testRandomGenerator(__global int* seeds){
    int group_id = get_group_id(0);
    int seed = seeds[group_id];
    //printf("%d - %d\n", group_id, seeds[group_id]);
    seeds[group_id] = rand2(&seed);
    //printf("%d - %d\n", group_id, seeds[group_id]);
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