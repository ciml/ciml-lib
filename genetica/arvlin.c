#include "arvlin.h"
#include "pilha.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
void semeadora(Arvlin *arv,int nivelMax){//ainda nao funciona...
    int i=0,aux,nivel;Pilha p;
    iniPilha(&p,nivelMax);
    for(i=0,nivel=0;i < arv->max;i++){
        if(nivel<nivelMax){            
            if(geraArv(arv,i)!=0){
                empilha(&p,i);
                nivel++;
            }else{
                while(topoPilha(&p)!=2){
                    if(p.numero != -1)
                        desempilha(&p);
                    else
                        break;
                    if(p.numero != -1)
                        desempilha(&p);
                    nivel--;
                }
            }
        }else{
            arv->filhos[i]=0;
            arv->elementos[i]=rand()%10;
            while(topoPilha(&p)!=2){
                if(p.numero != -1)
                    desempilha(&p);
                else
                    break;
                if(p.numero != -1)
                        desempilha(&p);
                nivel--;
            }
        }
    }
    arv->Nfilhos=i;
    descarrega(&p);
}

int geraArv(Arvlin *arv,int indice){//retorna numero de filhos e preenche a arvore No a No
    arv->filhos[indice]=rand()%3;
    arv->elementos[indice]= arv->filhos[indice] == 0 ? rand()%10 : 
        arv->filhos[indice] == 1 ? (rand()%10)+10 : (rand()%10)+20 ;
    return arv->filhos[indice];
}


void iniArvore(Arvlin *arv,int N){
    arv->Nfilhos=0;
    arv->max=N;
    arv->elementos=malloc(sizeof(int)*N);
    arv->filhos=malloc(sizeof(int)*N);
}

void arvPrintVetor(Arvlin *arv){
    int i=0;
    printf(" VETOR DE ELEMENTOS \n ");
    for(i=0;i<arv->Nfilhos;i++){
        printf(" %d=(%d) \t",i,arv->elementos[i]);
    }
    printf("\n");
    printf(" VETOR DE FILHOS \n ");
    for(i=0;i<arv->Nfilhos;i++){
        printf(" %d=(%d) \t",i,arv->filhos[i]);
    }
    printf("\n");
}

void printNivel(Arvlin *arv,int nivel,int i){
    int k=0;
    printf("N(%2d)//I(%2d)",nivel,i);
    for(;k<nivel;k++){
        printf("+");
    }
    printf("%d\n",arv->elementos[i]);
}

Arvlin* arvTest(){//preenche uma arvore test pre-definida;
    int i,Nel=14;
    Arvlin *arv=malloc(sizeof(Arvlin));
    //int    testeFilhos[]={2,2,2,0,0,0,2,0,0};//{2,1,1,0,2,0,2,0 ,1,2,0,0};////////{1,2,0,0};;;{2,2,0,0,2,0,0};/////
    int testeElementos[]={1,2,3,4,5,6,7,8,9,10,11,12,13,14};//{1,2,3,6,7,8,9,10,11,13,14,12};//{0,1,2,3,4,5,6,7,8,9,10};////{1,2,3,4};{1,2,4,5,3,6,7};//{1,2,4,5,3,6,7};//{1,2,3,6,7,8,9,10,11,13,14,12};////
    int    testeFilhos[]={2,1,1,0,2,0,2,0 ,1,2,0,0};
    //int    testeFilhos[]={1,1,2,2,0,2,1,0,1,0,0};
    //int    testeFilhos[]={1,2,1,0,0};   
    //int    testeFilhos[]={2,2,2,0,0,0,2,0,0};
    iniArvore(arv,Nel);
    arv->Nfilhos=Nel;
    for(i=0;i< arv->Nfilhos;i++){
        arv->filhos[i]=testeFilhos[i];
        arv->elementos[i]=testeElementos[i];
    }
    return arv;
}

void printArvore(Arvlin *arv){
    int i;
    Pilha p;iniPilha(&p,arv->Nfilhos);
    for(i=0;i<arv->Nfilhos;i++){
        printNivel(arv,p.numero+1,i);
        //empilha(&p,i);
        if(arv->filhos[i]!=0){                    
            empilha(&p,arv->filhos[i]);            
        }else
            if(topoPilha(&p)==2){//resolver bug dos 3 zeros               
                    p.carga[p.numero]=1;
            }else{              
                for(; p.numero!=-1 && topoPilha(&p)==1;desempilha(&p));
            }
    }
    descarrega(&p);
}