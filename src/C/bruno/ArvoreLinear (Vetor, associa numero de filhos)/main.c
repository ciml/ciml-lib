#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_NOS (int)15
#define MAX_FILHOS (int)2

/*-----PILHA-----*/
typedef struct{
    int topo;
    int info[MAX_NOS];
}Pilha;

void empilha(Pilha* pilha, int info){
    (pilha->topo)++;
    if(pilha->topo < MAX_NOS){
        pilha->info[pilha->topo] = info;
    }else{
        printf("Erro empilhar");
    }
}

int desempilha(Pilha* pilha){
    if(pilha->topo >= 0){
        int val = pilha->info[pilha->topo];
        pilha->topo--;
        return val;
    }else{
        printf("Erro desempilhar");
        return -1;
    }
}


/*-----ARVORE-----*/
typedef struct{
    int numeroFilhos[MAX_NOS];
    int informacao[MAX_NOS];
    int numNos;
}Arvore;

void inicializaArvore(Arvore* arv);//Inicializar todos os nós da arvore com um valor flag (verificar necessidade)
void geradorArvore(Arvore* arv, int maxTam);//preenche uma árvore com um numero máximo de nós 'maxTam'
void imprimeArvore(Arvore* arv);//Metodo para imprimir a árvore usando uma pilha
void criaCheia(Arvore* arv, int maxDepth);//dada uma profundidade maxima, preenche a arvore ate essa profundidade.
int calculaTamanhoSubArvore(Arvore* arv, int indice);//dado o indice de um nó, retorna o tamanho da subárvore a partir dele
void shift(Arvore* arv, int tam, int indice); //Desloca os elementos da árvore a partir de um 'indice' em determinado tamanho
void mutacao(Arvore* arvore); //Sorteia uma sub-arvore aleatória e troca por outra
void crossOver(Arvore* arvore1, Arvore* arvore2); //Troca informações entre duas arvores
void teste(Arvore *arvore1);//Cria uma árvore de forma conhecida para testes.
void geradorArvoreRec(Arvore* arv, int indice, int nivel);//mesmo que o anterior, porém recursivo
int imprimeArvoreRec(Arvore* arv, int indice, int nivel);//Metodo recursivo para imprimir a árvore. Retorna o valor do ultimo indice visitado para ser acessado pela recursão


int calculaTamanhoSubArvore(Arvore* arv, int indice){
    int soma = arv->numeroFilhos[indice];
    int tam = 1;

    while(soma != tam-1){
        indice++;
        soma += arv->numeroFilhos[indice];
        tam++;
    }
    return tam;
}

void criaCheia(Arvore* arv, int maxDepth){
    //conferir de alguma forma que cabe uma árvore cheia no vetor (no caso confere para uma arvore binaria)
    if(pow(2, maxDepth+1)-1 > MAX_NOS){
        printf("Espaco insuficiente para arvore cheia nesta profundidade.");
        return;
    }

    Pilha pilha;
    pilha.topo = -1;

    int indice = 0;
    int ultimo = 0;
    //se tirar 'soma' tem que garantir que a profundidade maxima garanta uma arvore nao-degenerada
    //int soma = 1;

    int aux[MAX_NOS];
    int j, num;

    //usar 'num' o inves de j pra nao ter variavel sobrando...
    for(j = 0; j < MAX_NOS; j++){
        aux[j] = 0;
    }

    empilha(&pilha, indice);

    while(pilha.topo != -1){
        indice = desempilha(&pilha);

        if(aux[indice] == 0){
            if(pilha.topo+1 != maxDepth){
                num = rand()%2 + 1;
                arv->numeroFilhos[indice] = num;
                //soma += num;
            } else {
                arv->numeroFilhos[indice] = 0;
            }

            arv->numNos++;

            if(num == 0)
                arv->informacao[indice] = rand()%10 +1;
            else
                arv->informacao[indice] = rand()%20 +11;
        }

        /*
        if(soma > MAX_NOS){
            soma-=arv->numeroFilhos[indice];
            arv->numeroFilhos[indice] = 0;
        }
        */

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

void mutacao(Arvore* arvore){
//    imprimeArvore(arvore);
//    printf("\n");

    int i;

    //sorteia uma subarvore da arvore inicial
        //pega o tamanho dessa subarvore
    int indiceSub = rand() % (arvore->numNos);
    int tamanhoSub = calculaTamanhoSubArvore(arvore, indiceSub);
    //printf("indiceSub = %d\n", indiceSub);
    //printf("tamanhoSub = %d\n", tamanhoSub);

    //confere o tamanho max para criar uma nova arvore
        //tamanho disponivel+tamanho da subarvore sorteada
    int espacoLivre = MAX_NOS-(arvore->numNos)+tamanhoSub;
    //printf("espacoLivre = %d\n", espacoLivre);
    //printf("numnos = %d\n\n", arvore->numNos);

    //cria uma nova arvore aleatoria de até o tamanho maximo calculado
    Arvore novaArvore;
    inicializaArvore(&novaArvore);
    geradorArvore(&novaArvore, espacoLivre);

    //imprimeArvore(&novaArvore);
    //printf("numnos = %d\n\n", novaArvore.numNos);

    //determina o tamanho do deslocamento da arvore

//TODO: colocar if para raiz?
//if(indiceSub != 0 && indiceSub != arvore->numNos-1){
        int tamShift = novaArvore.numNos - tamanhoSub;
    //printf("tamanhoShift = %d\n", tamShift);
    //printf("info = %d\n", arvore->informacao[0]);
        shift(arvore, tamShift, indiceSub+tamanhoSub);
    //printf("info = %d\n", arvore->informacao[0]);
   // printf("-----------------\n");
//}

    //troca a informação
    for(i = 0; i < novaArvore.numNos; i++){
        arvore->numeroFilhos[i+indiceSub] = novaArvore.numeroFilhos[i];
        arvore->informacao[i+indiceSub] = novaArvore.informacao[i];
    }

    //imprimeArvore(arvore);
}

void crossOver(Arvore* arvore1, Arvore* arvore2){
    int i;
//    imprimeArvore(arvore1);
//    for(i = 0; i < arvore1->numNos ; i++){
//        printf("%d ", arvore1->numeroFilhos[i]);
//    }
//    printf("\n");
//    imprimeArvore(arvore2);
//    for(i = 0; i < arvore2->numNos  ; i++){
//        printf("%d ", arvore2->numeroFilhos[i]);
//    }
//    printf("\n");

    int indiceSub1 = rand() % (arvore1->numNos);
    int tamanhoSub1 = calculaTamanhoSubArvore(arvore1, indiceSub1);
//    printf("ind1= %d\n", indiceSub1);
//    printf("tam1= %d\n", tamanhoSub1);
    int indiceSub2 = rand() % (arvore2->numNos);
    int tamanhoSub2 = calculaTamanhoSubArvore(arvore2, indiceSub2);
//    printf("ind2= %d\n", indiceSub2);
//    printf("tam1= %d\n", tamanhoSub2);
    int espacoLivre1 = MAX_NOS-(arvore1->numNos)+tamanhoSub1;
    int espacoLivre2 = MAX_NOS-(arvore2->numNos)+tamanhoSub2;

    if(espacoLivre1-tamanhoSub2 >= 0 && espacoLivre2-tamanhoSub1 >= 0){
        Arvore arvAux;
        for(i = 0; i < tamanhoSub1; i++){
            arvAux.informacao[i] = arvore1->informacao[indiceSub1+i];
            arvAux.numeroFilhos[i] = arvore1->numeroFilhos[indiceSub1+i];
        }

//        printf("AUX:  \n");
//        imprimeArvore(&arvAux);
//        printf("AUX:  \n\n");

        int tamShift1 = tamanhoSub2 - tamanhoSub1;
        shift(arvore1, tamShift1, indiceSub1+tamanhoSub1);

        for(i = 0; i < tamanhoSub2; i++){
            arvore1->numeroFilhos[i+indiceSub1] = arvore2->numeroFilhos[i+indiceSub2];
            arvore1->informacao[i+indiceSub1] = arvore2->informacao[i+indiceSub2];
        }

        int tamShift2 = tamanhoSub1 - tamanhoSub2;
        shift(arvore2, tamShift2, indiceSub2+tamanhoSub2);

        for(i = 0; i < tamanhoSub1; i++){
            arvore2->numeroFilhos[i+indiceSub2] = arvAux.numeroFilhos[i];
            arvore2->informacao[i+indiceSub2] = arvAux.informacao[i];
        }
    } else {
        printf("Espacos incompatíveis para cross-over.");
        return;
    }

//    imprimeArvore(arvore1);
//    for(i = 0; i < arvore1->numNos ; i++){
//        printf("%d ", arvore1->numeroFilhos[i]);
//    }
//    printf("\n");
//    imprimeArvore(arvore2);
//    for(i = 0; i < arvore2->numNos  ; i++){
//        printf("%d ", arvore2->numeroFilhos[i]);
//    }
//    printf("\n");
}

void geradorArvore(Arvore* arv, int maxTam){
    Pilha pilha;
    pilha.topo = -1;

    int indice = 0;
    int ultimo = 0;
    int soma = 1;

    int aux[maxTam];
    int j, num;

    //usar 'num' o inves de j pra nao ter variavel sobrando...
    for(j = 0; j < maxTam; j++){
        aux[j] = 0;
    }

    empilha(&pilha, indice);

    while(pilha.topo != -1){
        indice = desempilha(&pilha);

        if(aux[indice] == 0){
            num = rand()%3;
            arv->numeroFilhos[indice] = num;
            arv->numNos++;
            soma += num;

            //escolhendo um valor para a informação do nó
            if(num == 0)
                arv->informacao[indice] = rand()%10 +1;
            else
                arv->informacao[indice] = rand()%20 +11;
        }

        if(soma > maxTam){
            soma-=arv->numeroFilhos[indice];
            arv->numeroFilhos[indice] = 0;
        }

        if(aux[indice] < arv->numeroFilhos[indice]){
            empilha(&pilha, indice);
            aux[indice]++;
            ultimo++;
            empilha(&pilha, ultimo);
        }
    }
}

int soma = 1;
void geradorArvoreRec(Arvore* arv, int indice, int nivel){
    int i;
    //int soma = 0;
    int tam = 1;
    int num;

    //printf("%d -- ", indice);
    //scanf("%d", &num);
    num = 2;
    arv->numeroFilhos[indice] = num;
    arv->numNos++;

    if(num == 0){
        arv->informacao[indice] = rand()%10 +1;
    }else{
        arv->informacao[indice] = rand()%20 +11;
    }

    soma += num;
    printf("%d\n", soma);
    //printf("soma: %d -- \n", soma);


    if(soma > MAX_NOS/2){
        while(soma > MAX_NOS){
            soma--;
            arv->numeroFilhos[indice]--;
        }
        ////arv->numeroFilhos[indice]--;
    }

    if(arv->numeroFilhos[indice]==0){
        return;
    }

    for(i = 0; i != arv->numeroFilhos[indice]; i++){
        geradorArvoreRec(arv, indice+tam, nivel++);
        tam += calculaTamanhoSubArvore(arv, indice+tam);
    }
    return;
}

int imprimeArvoreRec(Arvore* arv, int indice, int nivel){
    int i;
    int ultimo = indice;
    //int tam = 1;

//    printf("(%d)", nivel);
//    for(i = 0; i <= nivel; i++)
//        printf("-");
//    printf("%d \n", arv->informacao[indice]);

    if(arv->numeroFilhos[indice]==0){
        printf("(%d)", arv->informacao[indice]);
        return indice;
    }

    for(i = 0; i < arv->numeroFilhos[indice]; i++){
        printf("(");
        ultimo = imprimeArvoreRec(arv, ultimo+1, nivel+1);
        if(i == 0)
            printf("(%d)", arv->informacao[indice]);
        //tam += calculaTamanhoSubArvore(arv, indice+tam);
    }

    printf(")");
    return ultimo;

}

//pos ordem -avaliar / conversor
void imprimeArvore(Arvore* arv){
    Pilha pilha;
    pilha.topo = -1;

    int indice = 0;
    int ultimo = 0;
    int aux[MAX_NOS];
    int j;

    for(j = 0; j < MAX_NOS; j++){
        aux[j] = 0;
    }

    if(arv->numeroFilhos[0]!= -1)
        empilha(&pilha, indice);

    while(pilha.topo != -1){

        indice = desempilha(&pilha);

        if(aux[indice] == 0){
            printf("(%d)", pilha.topo+1);
            for(j = 0; j <= pilha.topo+1; j++)
                printf("-");
            printf("%d \n", arv->informacao[indice]);
        }

        if(aux[indice] < arv->numeroFilhos[indice]){
            empilha(&pilha, indice);
            aux[indice]++;
            ultimo++;
            empilha(&pilha, ultimo);
        }
    }
}

void inicializaArvore(Arvore* arv){
    arv->numeroFilhos[0] = -1;
    arv->numNos = 0;
}

void teste(Arvore *arvore1){
    arvore1->numeroFilhos[0] = 2;
    arvore1->numeroFilhos[1] = 1;
    arvore1->numeroFilhos[2] = 0;
    arvore1->numeroFilhos[3] = 2;
    arvore1->numeroFilhos[4] = 1;
    arvore1->numeroFilhos[5] = 0;
    arvore1->numeroFilhos[6] = 0;

    arvore1->informacao[0] = 0;
    arvore1->informacao[1] = 1;
    arvore1->informacao[2] = 2;
    arvore1->informacao[3] = 3;
    arvore1->informacao[4] = 4;
    arvore1->informacao[5] = 5;
    arvore1->informacao[6] = 6;
}

int main(){
    srand(7);

    int i;
    Arvore arvore1, arvore2;
    inicializaArvore(&arvore1);
    inicializaArvore(&arvore2);

    //teste(&arvore1);

    geradorArvore(&arvore1, MAX_NOS);
    criaCheia(&arvore2, 3);

    imprimeArvore(&arvore1);
    printf("\n\n");
    imprimeArvore(&arvore2);
    printf("\n\n");

    for(i = 0; i < MAX_NOS ; i++){
        printf("%d ", arvore1.numeroFilhos[i]);
    }

    printf("\n");

    printf("MUTAÇÃO ARVORE 1: \n");
    mutacao(&arvore1);
    imprimeArvore(&arvore1);
    printf("\n\n");

    printf("CROSS OVER ARVORES 1 E 2: 1: \n");
    crossOver(&arvore1, &arvore2);

    imprimeArvore(&arvore1);
    printf("\n");
    imprimeArvore(&arvore2);
    printf("\n");

    return 0;
}
