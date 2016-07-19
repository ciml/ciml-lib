#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_DEPTH 3
#define NUM_NOS (int)pow(2,MAX_DEPTH+1) - 1
#define FLAG -999
#define ESQ 0
#define DIR 1
#define TAM_PILHA 10



typedef struct {
    int indice;
    int direcao;
} Info;


typedef struct {
    int topo;
    Info info[TAM_PILHA];
}Pilha;


void empilha(Pilha* pilha, Info info){
    pilha->topo = (pilha->topo) + 1;
    int topo = pilha->topo;
    pilha->info[topo] = info;
}


Info desempilha(Pilha* pilha){
    int topo = pilha->topo;
    pilha->topo = pilha->topo - 1;
    return pilha->info[topo];
}

void arvoreCompletaAleatoria(int arvore[], int inicio, int fim); //percorre o vetor sorteando valores
void criaArvore(int arvore[], int indice); //coloca um valor na raiz
void imprimeEstrutura(int arvore[]); //imprime iterativamente os nós da arvore na forma de uma arvore
void copiaArvore(int arvore[], int novaArvore[]); //faz a cópia de uma arvore
void apagaBlocoPercorrendo(int arvore[], int indice, int nivelInicial); //Percorre a estrutura da árvore apagando os valores a partir de um nó específico
void apagaBlocoLinear(int arvore[], int indice, int nivelInicial);//Apaga uma subárvore sem percorrer a árvore (calcula o tamanho do bloco)
void alteraSubArvore(int arvore[], int indice, int nivelInicial);//Percorre a árvore a partir de um nó específico, definindo novos valores para a subárvore
void percorreArvore(int arvore[]);//Vai percorrendo uma árvore até o usuario encontrar o ponto a partir do qual deseja alterá-la
void preencheArvoreRec(int arvore[], int indice, int nivel); //Função recursiva para percorrer a árvore e inserir valores para os nós
void imprimeArvoreRec(int arvore[], int indice, int nivel); //Função recursiva para imprimir a árvore
void preencheArvore(int arvore[]);//Percorre a árvore iterativamente (pilha), lendo valores
void preencheArvore2(int arvore[]);//faz o mesmo que a primeira, mas em ordem diferente(lembrar o motivo)
void imprimeArvore(int arvore[]);//percorre a arvore imprimindo seus valores, porém está um pouco confuso
void imprimeArvore2(int arvore[]);//percorre a arvore imprimindo seus valores (de uma forma um pouco mais compreensível)



void arvoreCompletaAleatoria(int arvore[], int inicio, int fim){
    int i;
    for(i = 0; i < NUM_NOS; i++){
        arvore[i] = rand()%201 -100;
    }
}

void criaArvore(int arvore[], int indice){
    printf("A arvore esta vazia! Entre com o valor da raiz: ");
    scanf("%d", &arvore[indice]);
    return;
}

void imprimeEstrutura(int arvore[]){
    int i;
    int j;
    int indice = NUM_NOS;
    indice = indice/2;

    if(arvore[indice] == FLAG){
        printf("\nArvore vazia!\n");
        return;
    } else {
        printf("%d\n", arvore[indice]);
    }

    for(i = 1; i <= MAX_DEPTH; i++){
        for(j = 0; j < (int)pow(2, i); j++){
            if(arvore[(indice/2) + j*(indice+1)] == FLAG){
                printf("--  ");
            } else {
                printf("%d  ", arvore[(indice/2) + j*(indice+1)]);
            }
        }
        printf("\n");
        indice = indice/2;
    }
}

void copiaArvore(int arvore[], int novaArvore[]){
    int i;
    for(i=0; i<NUM_NOS; i++){
        novaArvore[i] = arvore[i];
    }
}

void apagaBlocoPercorrendo(int arvore[], int indice, int nivelInicial){
    Info aux;
    Pilha pilha;
    pilha.topo = -1;

    aux.indice = indice;
    aux.direcao = ESQ; //default: comeca percorrendo o lado esquerdo
    arvore[aux.indice] = FLAG;
    empilha(&pilha, aux);

    while(1){
        //imprimeEstrutura(arvore);
        //imprimeArvore(arvore);
        if(pilha.topo + nivelInicial == MAX_DEPTH){
            aux = desempilha(&pilha);
            aux.direcao = DIR;
            while(aux.direcao == DIR){
                aux = desempilha(&pilha);
                if(pilha.topo == -1 && aux.direcao == DIR)
                    return;
            }
            aux.direcao = DIR;
            empilha(&pilha, aux);
        }


        //printf("\nind:%d\n", aux.indice);
        //printf("\ntopo: %d\n", pilha.topo);
        if(aux.direcao == ESQ){
            aux.indice = aux.indice - (NUM_NOS)/pow(2, pilha.topo+nivelInicial+2);
        }else{
            aux.indice = aux.indice + (NUM_NOS)/pow(2, pilha.topo+nivelInicial+2)+1;
            aux.direcao = ESQ;
        }
        //printf("\nnovo ind: %d\n", aux.indice);
        arvore[aux.indice] = FLAG;

        empilha(&pilha, aux);
    }
}

void apagaBlocoLinear(int arvore[], int indice, int nivelInicial){
    int tamanhoBloco = (NUM_NOS)/pow(2, nivelInicial+1);

    int k = indice - tamanhoBloco;
    int j = indice + tamanhoBloco;

//    printf("\nk: %d\n", k);
//    printf("j: %d\n", j);
//    printf("conta: %f\n", (NUM_NOS)/pow(2, nivelInicial));
//    printf("tamanhoBloco: %d\n", tamanhoBloco);

    while(k <= j){
        arvore[k] = FLAG;
        k++;

    }
}

void alteraSubArvore(int arvore[], int indice, int nivelInicial){
    Info aux;
    Pilha pilha;
    pilha.topo = -1;

    aux.indice = indice;
    aux.direcao = ESQ; //default: comeca percorrendo o lado esquerdo
    printf("Insira o valor: ");
    scanf("%d", &arvore[aux.indice]);
    empilha(&pilha, aux);

    while(1){
        imprimeEstrutura(arvore);
        //imprimeArvore(arvore);
        if(arvore[aux.indice] == FLAG||pilha.topo + nivelInicial == MAX_DEPTH){
            aux = desempilha(&pilha);
            aux.direcao = DIR;
            while(aux.direcao == DIR){
                aux = desempilha(&pilha);
                if(pilha.topo == -1 && aux.direcao == DIR)
                    return;
            }
            aux.direcao = DIR;
            empilha(&pilha, aux);
        }


        //printf("\nind:%d\n", aux.indice);
        //printf("\ntopo: %d\n", pilha.topo);
        if(aux.direcao == ESQ){
            aux.indice = aux.indice - (NUM_NOS)/pow(2, pilha.topo+nivelInicial+2);
        }else{
            aux.indice = aux.indice + (NUM_NOS)/pow(2, pilha.topo+nivelInicial+2)+1;
            aux.direcao = ESQ;
        }
        //printf("\nnovo ind: %d\n", aux.indice);
        printf("Insira o valor: ");
        scanf("%d", &arvore[aux.indice]);

        empilha(&pilha, aux);
    }
}

void percorreArvore(int arvore[]){
    char escolha;
    Info aux;
    Pilha pilha;
    pilha.topo = -1;
    imprimeEstrutura(arvore);
    aux.indice = (NUM_NOS)/2;
    aux.direcao = ESQ; //default: comeca percorrendo o lado esquerdo

    empilha(&pilha, aux);

    while(1){
        //imprimeArvore(arvore);
        //imprimeEstrutura(arvore);
        if(arvore[aux.indice] == FLAG || pilha.topo == MAX_DEPTH){
            aux = desempilha(&pilha);
            aux.direcao = DIR;
            while(aux.direcao == DIR){
                aux = desempilha(&pilha);
                if(pilha.topo == -1 && aux.direcao == DIR)
                    return;
            }
            aux.direcao = DIR;
            empilha(&pilha, aux);
        }

        //printf("\nind:%d\n", aux.indice);
        //printf("\ntopo: %d\n", pilha.topo);
        if(aux.direcao == ESQ){
            aux.indice = aux.indice - (NUM_NOS)/pow(2, pilha.topo+2);
        }else{
            aux.indice = aux.indice + (NUM_NOS)/pow(2, pilha.topo+2)+1;
            aux.direcao = ESQ;
        }
        //printf("\nnovo ind: %d\n", aux.indice);
        empilha(&pilha, aux);

        printf("\nDeseja alterar a partir deste no? Nivel(%d)---(%d): ", pilha.topo, arvore[aux.indice]);
        fflush(stdin);
        scanf("%c", &escolha);

        switch(escolha){
        case 's':
            apagaBlocoLinear(arvore, aux.indice, pilha.topo);
            imprimeEstrutura(arvore);
            alteraSubArvore(arvore, aux.indice, pilha.topo);
            return;
        case 'n':
            printf("Continuando percurso na arvore...\n");
            break;
        default:
            printf("Opcao invalida. Operacao cancelada.");
            return;
        }
    }
}

void preencheArvoreRec(int arvore[], int indice, int nivel){
    if(nivel > MAX_DEPTH+1)
        return;

    int k = NUM_NOS;

    printf("Insira o valor: ");
    scanf("%d", &arvore[indice]);
    if(arvore[indice]== FLAG){
        return;
    }

    imprimeArvoreRec(arvore, 7, 1);

    preencheArvoreRec(arvore, indice - k/pow(2, nivel+1), nivel+1);
    preencheArvoreRec(arvore, indice + k/pow(2, nivel+1)+1, nivel+1);

}

void imprimeArvoreRec(int arvore[], int indice, int nivel){
    int k = NUM_NOS;
    int i;

//    printf("-------------------------\n");
//    printf("VALORES EMPILHADOS: \n");
//    printf("    INDICE: %d\n", indice);
//    printf("    NIVEL: %d\n", nivel);
//    printf("-------------------------\n");

    if(nivel > MAX_DEPTH+1)
        return;

    if(arvore[indice] == FLAG)
        return;

    printf("(%d)", nivel-1);
    for(i = 0; i<nivel; i++)
        printf("-");
    printf("%d\n", arvore[indice]);

    imprimeArvoreRec(arvore, indice - k/pow(2, nivel+1), nivel+1);
    imprimeArvoreRec(arvore, indice + k/pow(2, nivel+1)+1, nivel+1);
}

void preencheArvore(int arvore[]){
    Info aux;
    Pilha pilha;
    pilha.topo = -1;

    aux.indice = (NUM_NOS)/2;
    aux.direcao = ESQ; //default: comeca percorrendo o lado esquerdo
    printf("Insira o valor: ");
    scanf("%d", &arvore[aux.indice]);
    empilha(&pilha, aux);

    while(1){
        imprimeEstrutura(arvore);
        //imprimeArvore2(arvore);
        if(arvore[aux.indice] == FLAG || pilha.topo == MAX_DEPTH){
            aux = desempilha(&pilha);
            aux.direcao = DIR;
            while(aux.direcao == DIR){
                aux = desempilha(&pilha);
                if(pilha.topo == -1 && aux.direcao == DIR)
                    return;
            }
            aux.direcao = DIR;
            empilha(&pilha, aux);
        }
        //printf("\nind:%d\n", aux.indice);
        //printf("\ntopo: %d\n", pilha.topo);
        if(aux.direcao == ESQ){
            aux.indice = aux.indice - (NUM_NOS)/pow(2, pilha.topo+2);
        }else{
            aux.indice = aux.indice + (NUM_NOS)/pow(2, pilha.topo+2)+1;
            aux.direcao = ESQ;
        }
        //printf("\nnovo ind: %d\n", aux.indice);
        printf("Insira o valor: ");
        scanf("%d", &arvore[aux.indice]);

        empilha(&pilha, aux);
    }
}

void preencheArvore2(int arvore[]){
    Info aux;
    Pilha pilha;
    pilha.topo = -1;

    aux.indice = (NUM_NOS)/2;
    aux.direcao = ESQ; //default: comeca percorrendo o lado esquerdo
    empilha(&pilha, aux);
    printf("Insira o valor: ");
    scanf("%d", &arvore[aux.indice]);

    while(1){
        imprimeEstrutura(arvore);
        //imprimeArvore2(arvore);

        if(aux.direcao == ESQ){
            aux.indice = aux.indice - (NUM_NOS)/pow(2, pilha.topo+2);
        }else{
            aux.indice = aux.indice + (NUM_NOS)/pow(2, pilha.topo+2)+1;
            aux.direcao = ESQ;
        }
        printf("Insira o valor: ");
        scanf("%d", &arvore[aux.indice]);
        empilha(&pilha, aux);

        if(arvore[aux.indice] == FLAG || pilha.topo == MAX_DEPTH){
            aux = desempilha(&pilha);
            aux.direcao = DIR;
            while(aux.direcao == DIR){
                aux = desempilha(&pilha);
                if(pilha.topo == -1 && aux.direcao == DIR)
                    return;
            }
            aux.direcao = DIR;
            empilha(&pilha, aux);
        }
    }
}

void imprimeArvore(int arvore[]){
    Info aux;
    Pilha pilha;
    pilha.topo = -1;
    int i;

    aux.indice = (NUM_NOS)/2;
    aux.direcao = ESQ; //default: comeca percorrendo o lado esquerdo

    empilha(&pilha, aux);

    printf("(%d)", pilha.topo);
    for(i = 0; i < pilha.topo+1; i++)
        printf("-");
    printf("%d\n", arvore[aux.indice]);

    while(1){
/*
    printf("-------------------------\n");
    printf("VALORES EMPILHADOS: \n");
    printf("    INDICE: %d\n", aux.indice);
    printf("    NIVEL: %d\n", pilha.topo+1);
    printf("-------------------------\n");
*/
        if(pilha.topo == MAX_DEPTH){
            aux = desempilha(&pilha);
            aux.direcao = DIR;
            while(aux.direcao == DIR){
                aux = desempilha(&pilha);
                if(pilha.topo == -1 && aux.direcao == DIR)
                    return;
            }
            aux.direcao = DIR;
            empilha(&pilha, aux);
        }

        if(aux.direcao == ESQ){
            aux.indice = aux.indice - (NUM_NOS)/pow(2, pilha.topo+2);
        }else{
            aux.indice = aux.indice + (NUM_NOS)/pow(2, pilha.topo+2)+1;
            aux.direcao = ESQ;
        }

        if(arvore[aux.indice] == FLAG){
            //aux = desempilha(&pilha);
            aux.direcao = DIR;
            while(aux.direcao == DIR){
                aux = desempilha(&pilha);
                if(pilha.topo == -1 && aux.direcao == DIR)
                    return;
            }
            //aux.indice = aux.indice + (NUM_NOS)/pow(2, pilha.topo+2)+1;
            aux.direcao = DIR;
            empilha(&pilha, aux);
            continue;
        }
        empilha(&pilha, aux);

        printf("(%d)", pilha.topo);
        for(i = 0; i < pilha.topo + 1; i++)
            printf("-");
        printf("%d\n", arvore[aux.indice]);
    }
}

void imprimeArvore2(int arvore[]){
    Info aux;
    Pilha pilha;
    pilha.topo = -1;
    int i;

    aux.indice = (NUM_NOS)/2;
    aux.direcao = ESQ; //default: comeca percorrendo o lado esquerdo

    empilha(&pilha, aux);

    printf("(%d)", pilha.topo);
    for(i = 0; i < pilha.topo+1; i++)
        printf("-");
    printf("%d\n", arvore[aux.indice]);

    while(1){
        if(aux.direcao == ESQ){
            aux.indice = aux.indice - (NUM_NOS)/pow(2, pilha.topo+2);
        } else {
            aux.indice = aux.indice + (NUM_NOS)/pow(2, pilha.topo+2)+1;
            aux.direcao = ESQ;
        }

        empilha(&pilha, aux);

//    printf("-------------------------\n");
//    printf("VALORES EMPILHADOS: \n");
//    printf("    INDICE: %d\n", aux.indice);
//    printf("    NIVEL: %d\n", pilha.topo);
//    printf("-------------------------\n");

        if(arvore[aux.indice] == FLAG || pilha.topo == MAX_DEPTH){
            if(arvore[aux.indice] != FLAG){
                printf("(%d)", pilha.topo);
                for(i = 0; i < pilha.topo + 1; i++)
                    printf("-");
                printf("%d\n", arvore[aux.indice]);
            }
            aux = desempilha(&pilha);
            aux.direcao = DIR;
            while(aux.direcao == DIR){
                aux = desempilha(&pilha);
                if(pilha.topo == -1 && aux.direcao == DIR)
                    return;
            }
            aux.direcao = DIR;
            empilha(&pilha, aux);
        } else {
            printf("(%d)", pilha.topo);
            for(i = 0; i < pilha.topo + 1; i++)
                printf("-");
            printf("%d\n", arvore[aux.indice]);
        }
    }
}

int main()
{
    srand(4);
    int i;
    int arvore1[NUM_NOS];
    int arvore2[NUM_NOS];
    for(i = 0; i < NUM_NOS; i++){
        arvore1[i] = FLAG;
        arvore2[i] = FLAG;
    }

    //criaArvore(arvore, (NUM_NOS)/2);
    //arvoreCheiaAleatoria(arvore, 0, NUM_NOS-1);
    //preencheArvoreRec(arvore, 7, 1);

    preencheArvore2(arvore1);
    preencheArvore(arvore2);

    //arvoreCheiaAleatoria(arvore1, 0, 14);
    //copiaArvore(arvore1, arvore2);

    //percorreArvore(arvore2);

    //percorreArvoreUsuario(arvore, 0, NUM_NOS-1);

    //imprimePorNivel(arvore, 0, NUM_NOS-1, 0);

    printf("\n\n\n");
    printf("ARVORE 1: \n");
    imprimeEstrutura(arvore1);
    printf("ARVORE 2: \n");
    imprimeEstrutura(arvore2);


    return 0;
}
