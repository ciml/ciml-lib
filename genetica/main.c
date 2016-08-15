#include <stdlib.h>
#include "arvlin.h"
#include <stdio.h>
int main(int argc, char** argv) {
    srand(30);
    printf("inciando programa...\n");
    //Arvlin *arv=arvTest();
    Arvlin a,b;
    iniArvore(&a,100);
    iniArvore(&b,10);
    //arvTest(&arv);
    semeadora(&a,7);
    semeadora(&b,3);
    printf("printando vetor...\n");
    arvPrintVetor(&a);
    arvPrintVetor(&b);
    printf("printando arvore...\n");
    //printArvore(arv);   
    printArvore(&a);
    printArvore(&b);
    printf("mutaçao");
    mutaArv(&a,&b);
    arvPrintVetor(&a);
    printArvore(&a);
    return (EXIT_SUCCESS);
}
