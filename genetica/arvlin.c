
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "arvlin.h"
#include "pilha.h"
#include <stdio.h>
#include <stdlib.h>
/*
void popula(Arvlin *arv,int N){//N= numero de elementos a se gerar
    int i,nivel=0,soma=0;
    Pilha pilha;
    iniPilha(&pilha,N);
    for(i=0;pilha->soma < N && i < N ;i++,nivel++){
        empilha(&pilha,sorteia(nivel));        
    }
    
        if(i==N && N!=0 && f==0){
            f=(rand()%2+1);
        }
        
        
        switch(f){
            case 2:{
                
            }
            case 1:
            case 0:
        }
        nivel++;
}


int sorteia(int nivel){
    return rand()%3;
}

int nivelArv(Arvlin *arv)
*/

void iniArvore(Arvlin *arv,int N){
    arv->Nfilhos=N;
    arv->elementos=malloc(sizeof(int)*N);
    arv->filhos=malloc(sizeof(int)*N);
}


void arvPrintVetor(Arvlin *arv){
    int i=0;
    printf(" VETOR DE ELEMENTOS \n ");
    for(i=0;i<arv->Nfilhos;i++){
        printf(" %d--(%d) ",i,arv->elementos[i]);
    }
    printf("\n");
    printf(" VETOR DE FILHOS \n ");
    for(i=0;i<arv->Nfilhos;i++){
        printf(" %d--(%d) ",i,arv->filhos[i]);
    }
    printf("\n");
}

void printNivel(Arvlin *arv,int nivel,int i){
    int k=0;
    for(;k<nivel;k++){
        printf("+");
    }
    printf("%d\n",arv->elementos[i]);
}

Arvlin* arvTest(){//preenche uma arvore test pre-definida;
    int i,Nel=12;
    Arvlin *arv=malloc(sizeof(Arvlin));
    int    testeFilhos[]={2,1,1,0,2,0,2,0,1,2,0,0};//{1,2,0,0};//{2,2,0,0,2,0,0};
    int testeElementos[]={1,2,4,5,3,6,7,8,9,10,11,23};//{1,2,3,4};//{1,2,4,5,3,6,7};
    iniArvore(arv,Nel);
    arv->Nfilhos=Nel;
    for(i=0;i< arv->Nfilhos;i++){
        arv->filhos[i]=testeFilhos[i];
        arv->elementos[i]=testeElementos[i];
    }
    return arv;
}

void printArvore(Arvlin *arv){
    int i,nivel;
    Pilha p;iniPilha(&p,arv->Nfilhos);
    for(i=0,nivel=0;i<arv->Nfilhos;i++){
        printNivel(arv,nivel,i);
        empilha(&p,i);
        if(arv->filhos[i]!=0){    
            nivel++;
        }else{
            if(arv->filhos[topoPilha(&p)]==2){
                printNivel(arv,nivel,++i);
                desempilha(&p);
                nivel--;                              
            }else{
                for(nivel++;arv->filhos[topoPilha(&p)]!=2;nivel--)
                    desempilha(&p);                        
            }
        }
    }
}
