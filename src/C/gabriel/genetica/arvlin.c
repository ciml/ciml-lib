#include "arvlin.h"
#include "pilha.h"
#include <stdio.h>
#include <stdlib.h>
#define TAM_MAX_ARV 100

void iniArvore(Arvlin *arv,int N){
    arv->Nfilhos=0;
    arv->max=N;
    arv->elementos=malloc(sizeof(int)*N);
    arv->filhos=malloc(sizeof(int)*N);
}

void lenhador(Arvlin *arv){
    free(arv->elementos);
    free(arv->filhos);
    arv->elementos=0;
    arv->filhos=0;
    printf("\nÁrvore Podada\n");
}

void arvPrintVetor(Arvlin *arv){
    int i=0;
    printf(" VETOR DE ELEMENTOS \n ");
    for(i=0;i<arv->Nfilhos;i++){
        printf(" %d=(%d)",i,arv->elementos[i]);
    }
    printf("\n");
    printf(" VETOR DE FILHOS \n ");
    for(i=0;i<arv->Nfilhos;i++){
        printf(" %d=(%d)",i,arv->filhos[i]);
    }
    printf("\n");
}

void printNivel(Arvlin *arv,int nivel,int i){
    int k=0;
    printf("N(%2d)//I(%2d)||",nivel,i);
    for(;k<nivel && nivel>=0;k++){
        printf("+");
    }
    printf("%d\n",arv->elementos[i]);
}

Arvlin* arvTest(){//preenche uma arvore test pre-definida;
    int i;
    Arvlin *arv=malloc(sizeof(Arvlin));
    int testeElementos[]={1,2,3,4,5,6,7,8,9,10,11,12,13,14};
    int    testeFilhos[]={2,1,1,0,2,0,2,0,1,2,0,0,10};
    //int    testeFilhos[]={2,2,2,0,1,0,0,1,0,10};
    //int    testeFilhos[]={1,1,2,2,0,2,1,0,1,0,0,10};
    //int    testeFilhos[]={2,2,1,0,0,0,10};
    //int    testeFilhos[]={2,2,2,0,0,0,2,0,0,10};
    iniArvore(arv,20);
    for(i=0;testeFilhos[i]!=10;i++){
        arv->filhos[i]=testeFilhos[i];
        arv->elementos[i]=testeElementos[i];
    }
    arv->Nfilhos=i;
    printf("\n teste:%d",i);
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
        }else{
            //if(topoPilha(&p)==2){//resolver bug dos 3 zeros
              //      p.carga[p.numero]=1;
            //}else{
                for(; p.numero!=-1 && topoPilha(&p)==1;desempilha(&p));
                if(topoPilha(&p)==2)
                    p.carga[p.numero]=1;
            //}
        }
    }
    descarrega(&p);
}

void semeadora (Arvlin *arv,int Nmax){//geradora de arvores com nivel max
    int i=0;Pilha p;iniPilha(&p,Nmax);
    //for(i=0;1;i++){
    do{
        if(geraArv(arv,&i)!=0){//inverter a ordem de conferencia: testar se vai incluir para depois incluir
            if(p.numero+1!=Nmax-1){
                empilha(&p,arv->filhos[i]);
            }else{
                arv->filhos[i]=0;
                arv->elementos[i]=5;//trocar para rand;
                for(; p.numero!=-1 && topoPilha(&p)==1;desempilha(&p));
                if(topoPilha(&p)==2)
                    p.carga[p.numero]=1;
            }
        }else{
            for(; p.numero!=-1 && topoPilha(&p)==1;desempilha(&p));
            //TODO: verificar antes se a pilha está vazia
            if(topoPilha(&p)==2)
                p.carga[p.numero]=1;
        }
        i++;
    }while(p.numero!=-1);

    arv->Nfilhos=i;
    descarrega(&p);
}

int geraArv(Arvlin *arv,const int* indice){//retorna numero de filhos e preenche a arvore No a No
    arv->filhos[(*indice)]=rand()%3;
    arv->elementos[(*indice)]= arv->filhos[(*indice)] == 0 ? rand()%10 :
        arv->filhos[(*indice)] == 1 ? (rand()%10)+10 : (rand()%10)+20 ;
    return arv->filhos[(*indice)];
}

void mutaArv(Arvlin *arv){
    //TODO: trocar a ordem de conferência - gerar a arvore com profundidade maxima antes
    int i,tamPulo;Arvlin arvM;
    iniArvore(&arvM,TAM_MAX_ARV);
    semeadora(&arvM,(rand()%arv->Nfilhos));
    printArvore(&arvM);
    int Icol=(rand()%arv->Nfilhos),tamSubArv=calcTamSubArv(arv->filhos,&Icol);//indice onde colocar
    tamPulo = arvM.Nfilhos - tamSubArv;
    printf("==== vou colocar no indice %d, pulando %d ==== \n",Icol,tamPulo);
    if(arv->max >= tamPulo + arv->Nfilhos ){//cabe?
        skipElemArv(arv,&tamPulo,&Icol,&tamSubArv);
        copiaSubArv(arv,&arvM,Icol,0,&tamSubArv);
    }else{
        printf("impossivel realizar mutacao devido ao tamanho");
    }
    lenhador(&arvM);
}

void copiaSubArv(Arvlin* arv,Arvlin* arv2,int onde1, int onde2,const int* tamCopia ){
    int i;
    for(i=0; i < (*tamCopia) ;i++){
        arv->elementos[onde1+i]=arv2->elementos[onde2+i];//colocar ruido aqui;
        arv->filhos[onde1+i]=arv2->filhos[onde2+i];
    }
}

int calcTamSubArv(int *filhosArv,const int* indice){
    int i,tamSubArv=1;
    for(i=(*indice);tamSubArv!=0;tamSubArv--,i++){
        tamSubArv+=filhosArv[i];
    }
    return i - (*indice);
}

void skipElemArv(Arvlin *arv,const int* tamPulo,const int* onde,const int* tamSubArv){
    int i;
    if((*tamPulo)>=0){
        for(i=arv->Nfilhos-1;i > (*onde);i--){
            arv->elementos[i+(*tamPulo)]=arv->elementos[i];
            arv->filhos[i+(*tamPulo)]=arv->filhos[i];
        }
    }else{
        for(i=(*onde)+(*tamSubArv); i < arv->Nfilhos ;i++){
            arv->elementos[i+(*tamPulo)]=arv->elementos[i];
            arv->filhos[i+(*tamPulo)]=arv->filhos[i];
        }
    }
    arv->Nfilhos+=(*tamPulo);
}

void combinacao(Arvlin *arv1,Arvlin *arv2){//copia uma subarvore na outra
    int icol1=(rand()%arv1->Nfilhos), icol2=(rand()%arv2->Nfilhos);//indices das subarvores
    int tamSubArv1 = calcTamSubArv(arv1->filhos,&icol1);
    int tamSubArv2 = calcTamSubArv(arv2->filhos,&icol2);
    printf("tamsubarv 1 =%d , 2 = %d",tamSubArv1,tamSubArv2);
    int tamPulo1= (tamSubArv2 - tamSubArv1);
    int tamPulo2= (tamSubArv1 - tamSubArv2);
    printf("\n====vou colocar na arv1 ind= %d skip=%d na arv2 ind= %d skip=%d ====\n",
            icol1,tamPulo1,icol2,tamPulo2);
    //TODO: colocar em um loop para garantir que o cross-over aconteça
    if( (arv1->max >= (tamPulo1 + arv1->Nfilhos))
            && (arv2->max >= (tamPulo2 + arv2->Nfilhos))){//cabe nos dois

        if(tamSubArv1 >= tamSubArv2) {//achar menor sub para a copia paralela
            trocaSubArv(arv1,arv2,&icol1,&icol2,tamSubArv2);
            skipElemArv(arv2,&tamPulo2,&icol2,&tamSubArv2);
            if(tamSubArv1!=tamSubArv2)
                copiaSubArv(arv2,arv1,(icol2+tamSubArv2),(icol1+tamSubArv2),&tamPulo2);
            skipElemArv(arv1,&tamPulo1,&icol1,&tamSubArv1);

        }else{
            trocaSubArv(arv2,arv1,&icol2,&icol1,tamSubArv1);
            skipElemArv(arv1,&tamPulo1,&icol1,&tamSubArv1);
            copiaSubArv(arv1,arv2,icol1+tamSubArv1,icol2+tamSubArv1,&tamPulo1);
            skipElemArv(arv2,&tamPulo2,&icol2,&tamSubArv2);

        }
    }
}

void  trocaSubArv(Arvlin* arvM,Arvlin* arvm,int *icol1,int *icol2,int tamSubArvMenor){
    //TODO: pegar a ideia
    int i=0,aux;
    while(i<tamSubArvMenor){
        aux=arvM->elementos[*icol1+i];
        arvM->elementos[*icol1+i]=arvm->elementos[*icol2+i];
        arvm->elementos[*icol2+i]=aux;
        aux=arvM->filhos[*icol1+i];
        arvM->filhos[*icol1+i]=arvm->filhos[*icol2+i];
        arvm->filhos[*icol2+i]=aux;
        i++;
    }
}
