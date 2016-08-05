#include"pilha.h"
#include<stdio.h>
#include<stdlib.h>

void iniPilha(Pilha *p,int k){
    /*if(p->max!=-1){
        printf("ja inicializada");
    }
    else{*/
        p->carga=malloc(sizeof(int)*k);
        p->max=k;
        p->numero=-1;
}
void empilha(Pilha *p, int valorEmp){
    p->numero++;
    if(p->numero < p->max){
        p->carga[p->numero]=valorEmp;
        //p->soma+=valorEmp;//att soma      
    }
    else{
        printf("pilha cheia");
        exit(1);
        p->numero--;
    }
}

void desempilha(Pilha * p){
    int j;
    if(p->numero!=-1){
        //p->soma-=p->carga[p->numero+1];//att soma
        j=p->carga[p->numero];
                p->numero--;
        //return j;
    }else{
        printf("pilha vazia");
        exit(1);
    }
}

int topoPilha(Pilha *p){
    if(p->numero!=-1){
        return p->carga[p->numero];
    }else
        printf("topo da pilha vazia");
}

void printPilha(Pilha *p){
    int i;
    printf("imprimindo pilha");
    if(p->numero!=-1){
        for(i=0;i<=p->numero;i++){
            printf(" %d/",p->carga[i]);
        }
        printf("\n");
    }
    else 
        printf("pilha vazia");
}

void descarrega(Pilha *p){
    free(p->carga);
    p->carga=NULL;
    printf("\ndescarregando pilha\n");
            
}