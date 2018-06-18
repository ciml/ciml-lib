#include <stdlib.h>
#include "arvlin.h"
#include <stdio.h>
int main(int argc, char** argv) {
    srand(14);
    printf("inciando programa...\n");
    //Arvlin *arv=arvTest();
    Arvlin a,b;
    iniArvore(&a,100);
    iniArvore(&b,100);
    //arvTest(&arv);
    semeadora(&a,7);
    semeadora(&b,7);
    printf("printando vetor...\n");
    //arvPrintVetor(&a);
    //arvPrintVetor(&b);
    printf("printando arvore...\n");
    //printArvore(arv);   
    printArvore(&a);
    printArvore(&b);
    //printf("mutaçao");
    //mutaArv(&a);
    printf("combinando\n");
    combinacao(&a,&b);
    //arvPrintVetor(&a);
    printArvore(&a);
    //arvPrintVetor(&b);
    printArvore(&b);
    return (EXIT_SUCCESS);
}
