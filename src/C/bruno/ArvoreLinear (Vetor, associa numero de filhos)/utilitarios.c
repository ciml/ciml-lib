#include <stdio.h>
#include <stdlib.h>
#include "utilitarios.h"
#include "constantes.h"


void atribuiSemente(){
    srand(9);
}

float randomProb(){
    return rand()/(float)(RAND_MAX+1);
}

int randomInterval(int inferior, int superior){
    return rand() % (superior - inferior + 1) + inferior;
}

int randomSuperior(int limiteSuperior){
    if(limiteSuperior == 0) return 0;
    return rand() % (limiteSuperior + 1);
}

int randomTerminal(){
    int valor = rand() % (2*(N-1));
    return valor;
}

//TODO: sortear valores diferentes para funções unarias?
int randomFunctionBin(){
    return rand() % NUM_OPBIN;
}

int randomFunctionUn(){
    return NUM_OPBIN + rand() % NUM_OPUN;
}

//int randomLeafType(){
//    int i = rand()/(float)(RAND_MAX+1);
//    if(i > 0.5)
//        return CONST;
//    else
//        return VAR;
//}

float randomConst(){
    return (rand()/(float)(RAND_MAX+1))*100;
}




#include <string.h>

void fatal(char *msg) {
    printf("%s\n", msg);
    exit (1);
    }


float** readTrainingData(){
    FILE *arq;
    float** dadosTreinamento;
    int i = 0, j;
    int k;
    arq = fopen("dadosTreinamento3.txt", "r");
    int numLinhas, numColunas, boolLabel;
    fscanf(arq, "%d %d %d", &numLinhas, &numColunas, &boolLabel);//, &numVariaveis);

    M = numLinhas;
    N = numColunas;

    printf("\nN = %d\n\n", N);

    printf("\nM = %d\n\n", M);

    dadosTreinamento = malloc(M* sizeof(float*));
    for(k = 0; k < M; k++){
        (dadosTreinamento)[k] = malloc(N*sizeof(float));
    }

    //float dadosTreinamento[M][N];

    LABELS = malloc(numColunas*sizeof(char*));
    if(boolLabel){
        char *word = NULL;
        size_t size = 2;
        long fpos;
        fpos = ftell(arq);
        //printf("pos = %d\n", fpos);
        char format [32];
        //char* labels[n];

        if ((word = malloc(size)) == NULL)                  // word memory
            fatal("Failed to allocate memory");
        sprintf (format, "%%%us", (unsigned)size-1);        // format for fscanf''

        while((fscanf(arq, format, word) == 1) && (i < N)) {
            while (strlen(word) >= size-1) {                // is buffer full?
                size *= 2;                                  // double buff size
                //printf ("** doubling to %u **\n", (unsigned)size);
                if ((word = realloc(word, size)) == NULL)
                    fatal("Failed to reallocate memory");
                sprintf (format, "%%%us", (unsigned)size-1);// new format spec
                fseek(arq, fpos, SEEK_SET);                  // re-read the line
                if (fscanf(arq, format, word) == 0)
                    fatal("Failed to re-read file");

       // printf ("lido2= %s\n", word);
            }
            //printf("pos = %d\n", fpos);
            LABELS[i] = (char*)malloc((size));

            strcpy(LABELS[i], word);
            i = i + 1;

           // printf ("%s\n", word);
            //printf ("%s\n", LABELS[i]);
            fpos = ftell(arq);                               // mark file pos
        }
        fseek(arq, fpos, SEEK_SET);
        //printf("pos = %d\n", fpos);
        free(word);
    } else {
       //todo: definir
    }

//    for(i = 0; i < numColunas; i++){
//        printf ("%s\n", LABELS[i]);
//    }

    for(i = 0; i < numLinhas; i++){
        for(j = 0; j < numColunas; j++){
                //printf("a\n");
            if(!fscanf(arq, "%f", &dadosTreinamento[i][j]))
                break;
            //printf("%f ",(dadosTreinamento)[i][j]);
        }
    }


    fclose(arq);
    return dadosTreinamento;

}

//
//float** readTrainingData(){
//    FILE *arq;
//    int i, j;
//    arq = fopen("dadosTreinamento3.txt", "r");
//    int numLinhas, numColunas, numVariaveis;
//    fscanf(arq, "%d %d %d", &numLinhas, &numColunas, &numVariaveis);//, &numVariaveis);
//
//    fscanf(arq, "%s", LABELS);
//    //TODO: mudar para constantes;
//    //float dadosTreinamento[numLinhas][numColunas];
//
//    float** dadosTreinamento = (float **)malloc(numLinhas * sizeof(float*));
//    for(i = 0; i < numLinhas; i++)
//        dadosTreinamento[i] = (float *)malloc(numColunas * sizeof(float));
//
////    LABELS = (char **)malloc((numColunas+1) * sizeof(char*));
////    for(i = 0; i < numColunas+1; i++){
////        fgets(LABELS[i], 2, arq);
////         printf("%s", LABELS[i]);
////    }
//
//    for(i = 0; i < numLinhas; i++){
//        for(j = 0; j < numColunas; j++){
//            if(!fscanf(arq, "%f", &dadosTreinamento[i][j]))
//                break;
//        }
//    }
//
////    for(i = 0; i < numLinhas; i++){
////        for(j = 0; j < numColunas; j++){
////            printf("%.2f ", dadosTreinamento[i][j]);
////        }
////        printf("\n");
////    }
//
//    fclose(arq);
//    return dadosTreinamento;
//}
