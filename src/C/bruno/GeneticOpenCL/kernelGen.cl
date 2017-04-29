//tamanho do problema: (NUM_INDIV-torneio)/2

typedef struct{
    int topo;
    int info[MAX_NOS];
}Pilha;


typedef struct{
    int numeroFilhos[MAX_NOS];
    int informacao[MAX_NOS];
    int numNos;
    float aptidao;
}Arvore;


void empilha(Pilha* pilha, int info){
    (pilha->topo)++;
    if(pilha->topo < MAX_NOS){
        pilha->info[pilha->topo] = info;
    }else{
        printf("Erro empilhar\n");
    }
}

int desempilha(Pilha* pilha){
    if(pilha->topo >= 0){
        int val = pilha->info[pilha->topo];
        pilha->topo--;
        return val;
    }else{
        printf("Erro desempilhar\n");
        return -1;
    }
}


float randomConst(){
    return (rand()/(float)(RAND_MAX));
}

int randomType(int NUM_OPBIN, int NUM_OPUN, int N){
    int tipoAleatorio = rand()%(NUM_OPBIN+NUM_OPUN+(1)+N-1);
    return tipoAleatorio;
}


int randomLeafType(int NUM_OPBIN, int NUM_OPUN, int N){
    int tipoAleatorio = (rand()%N) + NUM_OPBIN + NUM_OPUN;
    return tipoAleatorio;
}

int randomNodeType(int NUM_OPBIN, int NUM_OPUN, int N){
    int tipoAleatorio = (rand()%(NUM_OPBIN+NUM_OPUN));
    return tipoAleatorio;
}

void geradorArvore(Arvore* arv, int maxTam, int* conjuntoOpTerm, int NUM_OPBIN, int NUM_OPUN, int N){
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


            sorteio = conjuntoOpTerm[randomType(NUM_OPBIN, NUM_OPUN, N)];
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
            sorteio = conjuntoOpTerm[randomLeafType(NUM_OPBIN, NUM_OPUN, N)];
            num = 0;

        }

//        tipo = unpackTipo(sorteio);

        if(tipo == CTE){
            sorteio = packFloat(CTE, randomConst());
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



void shift(Arvore* arv, int tam, int indice){//indice a partir de onde começa o shift, tam = tamanho do shift
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

void mutacao(Arvore* arvore, int* conjuntoOpTerm, int NUM_OPBIN, int NUM_OPUN, int N){
    int i;
    //sorteia uma subarvore da arvore inicial
        //pega o tamanho dessa subarvore
    int indiceSub = rand() % (arvore->numNos);
    int tamanhoSub = calculaTamanhoSubArvore(arvore, indiceSub);

    //confere o tamanho max para criar uma nova arvore
        //tamanho disponivel+tamanho da subarvore sorteada
    int espacoLivre = MAX_NOS-(arvore->numNos)+tamanhoSub;

    //cria uma nova arvore aleatoria de até o tamanho maximo calculado
    //TODO: criar uma nova arvore ou nao?
    Arvore novaArvore;
    //inicializaArvore(&novaArvore);
    geradorArvore(&novaArvore, espacoLivre, conjuntoOpTerm, NUM_OPBIN, NUM_OPUN, N);


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



void crossOver(Arvore* arvore1, Arvore* arvore2){

    int espacoLivre1, espacoLivre2, indiceSub1, indiceSub2, tamanhoSub1, tamanhoSub2;

    do{
        indiceSub1 = rand() % (arvore1->numNos);
        tamanhoSub1 = calculaTamanhoSubArvore(arvore1, indiceSub1);

        indiceSub2 = rand() % (arvore2->numNos);
        tamanhoSub2 = calculaTamanhoSubArvore(arvore2, indiceSub2);

        espacoLivre1 = MAX_NOS-(arvore1->numNos)+tamanhoSub1;
        espacoLivre2 = MAX_NOS-(arvore2->numNos)+tamanhoSub2;

    }while(espacoLivre1-tamanhoSub2 < 0 || espacoLivre2-tamanhoSub1 < 0);



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

int torneio(Arvore pop[]){
    int indiceMelhor = rand() % NUM_INDIV;
    int indice;
    int i;
    for(i = 0; i < NUM_TORNEIO-1; i++){
        indice = rand()%NUM_INDIV;
        if(pop[indice].aptidao < pop[indiceMelhor].aptidao)
            indiceMelhor = indice;
    }
    return indiceMelhor;
}

void  trocaSubArv(Arvore* arvMaior,Arvore* arvMenor,int ind1,int ind2,int tamanhoSubMenor, int tamanhoSubMaior){
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

float randomProb(){
    return rand()/(float)(RAND_MAX+1);
}


__kernel void evolucao(__global Arvore* popA,
					   __global Arvore* popF,
					   __const int elite,    
					   __global int* conjuntoOpTerm) {

	int group_id = get_group_id(0);

	int ind1 = torneio(popA);
	int ind2 = torneio(popA);

	popF[elite+2*group_id]	= popA[ind1];
	popF[elite+2*group_id+1]= popA[ind2];

	float cross = randomProb();
    float mut = randomProb();


	if(cross <= PROB_CROSS){
        crossOver(&popF[elite+2*group_id+1], &popF[elite+2*group_id]);
    }
    if(mut <= PROB_MUT){									//ISSO CONSIGO PASSAR COMO CONSTANTE POR FORA
        mutacao(&popF[elite+2*group_id+1], conjuntoOpTerm, NUM_OPBIN, NUM_OPUN, N);
        mutacao(&popF[elite+2*group_id], conjuntoOpTerm, NUM_OPBIN, NUM_OPUN, N);
    }
}

