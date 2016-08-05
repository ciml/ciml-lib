#include <stdlib.h>
#include "arvlin.h"

int main(int argc, char** argv) {
    srand(9);
    printf("inciando programa...\n");
    Arvlin *arv=arvTest();
    Arvlin a;
    iniArvore(&a,100);
    //arvTest(&arv);
    semeadora(&a,4);
    printf("printando vetor...\n");
    arvPrintVetor(arv);
    printf("printando arvore...\n");
    printArvore(arv);
    
    return (EXIT_SUCCESS);
}
