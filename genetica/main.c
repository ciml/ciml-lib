#include <stdlib.h>
#include "arvlin.h"
#include <stdio.h>
int main(int argc, char** argv) {
    srand(9);
    printf("inciando programa...\n");
    Arvlin *arv=arvTest();
    Arvlin a;
    iniArvore(&a,100);
    //arvTest(&arv);
    semeadora(&a,5);
    printf("print vetor...\n");
    arvPrintVetor(&a);
    printf("print arvore...\n");
    printArvore(&a);
    
    return (EXIT_SUCCESS);
}
