#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_NOS 15

typedef struct{
    int topo;
    int info[MAX_NOS];
}Pilha;

void empilha(Pilha* pilha, int info){
    (pilha->topo)++;
    if(pilha->topo < MAX_NOS){
        pilha->info[pilha->topo] = info;
    }else{
        printf("Erro");
    }
}

int desempilha(Pilha* pilha){
    if(pilha->topo >= 0){
        int val = pilha->info[pilha->topo];
        pilha->topo--;
        return val;
    }else{
        printf("Erro");
        return -1;
    }
}

typedef struct{
    int numeroFilhos[MAX_NOS];
    int informacao[MAX_NOS];
}Arvore;


//        printf("(%d)", pilha.topo);
//        for(j = 0; j <= pilha.topo; j++)
//            printf("-");
//        printf("%d \n", arv->informacao[indice]);


//TODO: retirar os 'while(1)'
//TODO: repensar a função de criar uma árvore cheia
//TODO: mudar a forma de verificar se o nó ja foi visitado (fazer de maneira que possamos utilizar um método para economizar memória no futuro)
//TODO: Mutação e Cross-Over
//TODO:

int calculaTamanhoSubArvore(Arvore* arv, int indice);//dado o indice de um nó, retorna o tamanho da subárvore a partir dele
void criaCheia(Arvore* arv, int maxDepth);//dada uma profundidade maxima, inicialmente preenche a arvore ate essa profundidade e depis termina de preencher aleatoriamente
void geradorArvore(Arvore* arv); //percorre a estrutura sorteando valores para o 'numero de filhos' de cada nó (com pilha)
void geradorArvoreRec(Arvore* arv, int indice, int nivel);//mesmo que o anterior, porém recursivo
int imprimeArvoreRec(Arvore* arv, int indice, int nivel);//Metodo recursivo para imprimir a árvore. Retorna o valor do ultimo indice visitado para ser acessado pela recursão
void imprimeArvore(Arvore* arv);//Metodo para imprimir a árvore usando uma pilha
void imprimeArvore2(Arvore* arv);//EM CONSTRUÇÃO: melhorar o metodo de imprimir anterior
void inicializaArvore(Arvore* arv);//Inicializar todos os nós da arvore com um valor flag (verificar necessidade)
void teste(Arvore *arvore1);//Cria uma árvore de forma conhecida para testes.

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
    Pilha pilha;
    pilha.topo = -1;

    int indice = 0;
    int ultimo = 0;
    //int soma = 1;

    int teste1 = 2;
    int teste2 = 1;

    int aux[MAX_NOS];

    int num;

    while(1){

        if(pilha.topo != maxDepth-1){
            num = rand()%teste1 + teste2;
            arv->numeroFilhos[indice] = num;
            aux[indice] = num;
            //soma += num;
        }else{
            teste1 = 3;
            teste2 = 0;
            arv->numeroFilhos[indice] = 0;
            aux[indice] = 0;
        }

//        if(soma > MAX_NOS){
//            while(soma > MAX_NOS){
//                soma--;
//                arv->numeroFilhos[indice]--;
//                aux[indice]--;
//            }
//          }

        if(aux[indice] == 0){
            arv->informacao[indice] = rand()%10 +1;
        }else{
            arv->informacao[indice] = rand()%20 +11;
        }

        if(aux[indice] == 0){
            while(aux[indice] == 0){
                if(indice == 0){
                   return;
                }
                indice = desempilha(&pilha);
                (aux[indice])--;
            }
            empilha(&pilha, indice);
            ultimo++;
            indice = ultimo;
        }else{
            empilha(&pilha, indice);
            indice++;
            ultimo = indice;
        }
    }
}

void geradorArvore(Arvore* arv){
    Pilha pilha;
    pilha.topo = -1;

    int indice = 0;
    int ultimo = 0;
    int soma = 1;

    int aux[MAX_NOS];
    int num;

    while(1){
        num = rand()%3;
        arv->numeroFilhos[indice] = num;
        aux[indice] = num;
        soma += num;

        if(num == 0){
            arv->informacao[indice] = rand()%10 +1;
        }else{
            arv->informacao[indice] = rand()%20 +11;
        }

        //Poderia colocar o numero de filhos como 0 tambem ao inves de ir diminuindo
        while(soma > MAX_NOS){
            soma--;
            arv->numeroFilhos[indice]--;
            aux[indice]--;
        }

        if(aux[indice] == 0){
            while(aux[indice] == 0){
                if(indice == 0){
                   return;
                }
                indice = desempilha(&pilha);
                (aux[indice])--;
            }

            empilha(&pilha, indice);
            ultimo++;
            indice = ultimo;
        }else{
            empilha(&pilha, indice);
            indice++;
            ultimo = indice;
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

    /*
    percorreArvore(arv, indice++, soma);
    int tam = calculaTamanhoSubArvore(arv, indice);
    percorreArvore(arv, indice+tam, soma);
    */
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

void imprimeArvore2(Arvore* arv){
    Pilha pilha;
    pilha.topo = -1;

    int indice = 0;
    int ultimo = 0;
    int aux[MAX_NOS];
    int j;

    //Tentar inicializar aux durante o while, sem necessidade desse for
    for(j = 0; j < MAX_NOS; j++){
        aux[j] = 0;
    }

    empilha(&pilha, indice);

    while(pilha.topo != -1){

        if(arv->numeroFilhos[indice] == 0){
            indice = desempilha(&pilha);
            aux[indice] = 1;

            while(aux[indice] == 1){
                indice = desempilha(&pilha);
                aux[indice] = 1;
            }
        }

        indice++;
        empilha(&pilha, indice);
        ultimo = indice;

    }

}

void imprimeArvore(Arvore* arv){
    Pilha pilha;
    pilha.topo = -1;
    int indice = 0;
    int ultimo = 0;
    int aux[MAX_NOS];
    int j;

    //Tentar inicializar aux durante o while, sem necessidade desse for
    for(j = 0; j < MAX_NOS; j++){
        aux[j] = arv->numeroFilhos[j];
    }

    //empilha(&pilha, arv->numeroFilhos[indice]);

    while(1){
        printf("(%d)", pilha.topo+1);
        for(j = 0; j <= pilha.topo+1; j++)
            printf("-");
        printf("%d \n", arv->informacao[indice]);
        //printf("(");

        if(aux[indice] == 0){

           // printf("(%d)", arv->numeroFilhos[indice]);

            while(aux[indice] == 0){

                if(indice == 0){
                   return;
                }
                //printf("indice1 %d\n", indice);
                indice = desempilha(&pilha);
                //if(arv->numeroFilhos[indice] - aux[indice] == 0)
                    //printf("%d", arv->numeroFilhos[indice]);

                (aux[indice])--;

                //printf("indice %d\n", indice);
            }
            //printf(")");
            empilha(&pilha, indice);
            ultimo++;
            indice = ultimo;

            //printf("indice1: %d ", indice);
            //printf("indice2: %d ", ultimo);
            //empilha(&pilha, ultimo+1);
        }else{
           // printf("indice %d\n", indice);
            empilha(&pilha, indice);
            indice++;
            ultimo = indice;
        }
    }
}

void inicializaArvore(Arvore* arv){
    int i;
    for(i = 0; i < MAX_NOS; i++){
        arv->numeroFilhos[i] = -99;
    }
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


int main()
{
    srand(7);

    int i;
    Arvore arvore1, arvore2;

    teste(&arvore1);
    criaCheia(&arvore2, 5);

    //geradorArvore(&arvore1);
    //geradorArvoreRec(&arvore1, 0 , 0);

    for(i = 0; i <MAX_NOS ; i++){
        printf("%d ", arvore1.numeroFilhos[i]);
    }
    printf("\n\n");
    imprimeArvoreRec(&arvore1, 0 ,0);
    printf("\n");

//    imprimeArvore2(&arvore1);
//    printf("\n");
//
//    for(i = 0; i <MAX_NOS ; i++){
//        printf("%d ", arvore2.numeroFilhos[i]);
//    }
//    printf("\n");
//    k = 0;
//    ind = imprimeArvoreRec(&arvore2,0, 0);
//    printf("\n");
//    imprimeArvore2(&arvore2);

    return 0;
}
