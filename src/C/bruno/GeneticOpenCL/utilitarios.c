#include "utilitarios.h"
#include "constantes.h"

int rand2(int *seed){
    int s  = *seed;
    s = abs((s * 16807) % 2147483647);//(int)(pown(2.0, 31)-1));
    *seed = s;
    return s;
}

float randomProb(int* seed){
    return (float)rand2(seed) / 2147483647;//pown(2.0, 31);
}


void atribuiSemente(int i){
    srand(i);
}

//float randomProb(){
//    return rand()/(float)(RAND_MAX+1);
//}

int randomInterval(int inferior, int superior){
    return rand() % (superior - inferior + 1) + inferior;
}

int randomSuperior(int limiteSuperior){
    if(limiteSuperior == 0) return 0;
    return rand() % (limiteSuperior + 1);
}

int randomTerminal(int N, int* seed){
    int valor = rand2(seed) % (2*(N-1));
    return valor;
}

///NAO ESTAO SENDO USADAS..............
int randomFunctionBin(int NUM_OPBIN){
    return rand() % NUM_OPBIN;
}
int randomFunctionUn(int NUM_OPUN){
    return rand() % NUM_OPUN;
}
///.....................................

//TODO(-): selecionar constante aleatoria de acordo com o maior e menor valor dos dados

float randomConst(int* seed){
    int superior = 6, inferior = 0;
   // printf("aa = %d ",(rand2(seed)% (superior - inferior + 1) + inferior));
   // float a = (rand2(seed)% (superior - inferior + 1) + inferior);
    //printf("a = %f", a);
    return 6;//(float)(2147483647/2));
}

int randomType(int NUM_OPBIN, int NUM_OPUN, int N, int* seed){
    int tipoAleatorio = rand2(seed)%(NUM_OPBIN+NUM_OPUN+(1)+N-1);
    return tipoAleatorio;
}


int randomLeafType(int NUM_OPBIN, int NUM_OPUN, int N, int* seed){
    int tipoAleatorio = (rand2(seed)%N) + NUM_OPBIN + NUM_OPUN;
    return tipoAleatorio;
}

int randomNodeType(int NUM_OPBIN, int NUM_OPUN, int N, int* seed){
    int tipoAleatorio = (rand2(seed)%(NUM_OPBIN+NUM_OPUN));
    return tipoAleatorio;
}


void fatal(char *msg) {
    printf("%s\n", msg);
    exit (1);
}


float** readTrainingData(int* M, int* N, int* NUM_CTES, int* NUM_OPBIN, int* NUM_OPUN, char*** LABELS,
                         int** conjuntoOpTerm, float* maxDados, float* minDados, char* filename){
    FILE *arq;
    float** dadosTreinamento;
    int i = 0, j;
    int k;
    printf("Lendo Dados Arquivo... %s\n",filename);

//    char nome[100];
//    printf("\nDigite nome do arquivo:");
//    gets(nome);
//    arq = fopen(nome, "r");
    arq = fopen(filename, "r");
    int boolLabel;
    //fscanf(arq, "%d %d %d", &numLinhas, &numColunas, &boolLabel);//, &numVariaveis);
    fscanf(arq, "%d %d %d", M, N, &boolLabel);//, &numVariaveis);

//    M = numLinhas;
//    N = numColunas;
//    printf("\nN = %d\n\n", *N);
//    printf("\nM = %d\n\n", *M);

    dadosTreinamento = malloc((*M)* sizeof(float*));
    for(k = 0; k < (*M); k++){
        (dadosTreinamento)[k] = malloc((*N)*sizeof(float));
    }

    //float dadosTreinamento[M][N];
    //printf("Inicializando Labels...\n");
    *LABELS = malloc((*N) * sizeof(char*));
//    for(k = 0; k < *N; k++){
//        (*LABELS)[k] = (char*)malloc(10*sizeof(char));
//    }


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

        while((fscanf(arq, format, word) == 1) && (i < (*N))) {
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
            (*LABELS)[i] = (char*)malloc((size));
            //printf("aaaaaaaaa");
            strcpy((*LABELS)[i], word);
            i = i + 1;

           // printf ("%s\n", word);
            //printf ("%s\n", LABELS[i]);
            fpos = ftell(arq);                               // mark file pos
        }
        fseek(arq, fpos, SEEK_SET);
        //printf("pos = %d\n", fpos);
        free(word);

    } else {
        for(k = 0; k < (*N); k++){
            (*LABELS)[k] = (char*)malloc(10*sizeof(char));
            (*LABELS)[k][0] = 'X';
            (*LABELS)[k][1] = '\0';

            char str[15];
            sprintf(str, "%d", k);
            strcat((*LABELS)[k], str);

        }
    }

//    for(i = 0; i < (*N); i++){
//        printf ("%s\n", (*LABELS)[i]);
//    }
    //printf("Lendo Dados...\n");
    for(i = 0; i < (*M); i++){
        for(j = 0; j < (*N); j++){
                //printf("a\n");
            if(!fscanf(arq, "%f", &dadosTreinamento[i][j]))
                break;
            //printf("%f ",(dadosTreinamento)[i][j]);
        }
        if(dadosTreinamento[i][j-1] > (*maxDados)){
            (*maxDados) = dadosTreinamento[i][j-1];
        } else if (dadosTreinamento[i][j-1] < (*minDados)){
            (*minDados) = dadosTreinamento[i][j-1];
        }
    }

    int boolOps;//, numOpBin, numOpUn;
    int tipo;
    int info;

    //printf("Inicializando conjunto de operações...\n");
    fscanf(arq, "%d", &boolOps);
    if(boolOps){
        fscanf(arq, "%d %d", NUM_OPBIN, NUM_OPUN);
//        NUM_OPBIN = numOpBin;
//        NUM_OPUN = numOpUn;

        //operacoesBin = malloc(numOpBin * sizeof(int));
        *conjuntoOpTerm = malloc(((*NUM_OPBIN)+(*NUM_OPUN)+(1)+(*N)-1) * sizeof(int));

        for (i = 0; i < (*NUM_OPBIN); i++){
            if(!fscanf(arq, "%d", &info))
                break;
            (*conjuntoOpTerm)[i] = packInt(FBIN, info);
        }

//        if(numOpUn!= 0){
            for (j = 0; j < (*NUM_OPUN); j++, i++){
                if(!fscanf(arq, "%d", &info))
                    break;
                (*conjuntoOpTerm)[i] = packInt(FUN, info);
            }
//        }

        for(j = 0; j < (*N)-1; j++, i++){
            (*conjuntoOpTerm)[i] = packInt(VAR, j);
        }

        (*conjuntoOpTerm)[i] = packFloat(CTE, 1.0);
        *NUM_CTES = 1;
    } else {

        *NUM_OPBIN = 4;
        *NUM_OPUN = 4;
        *NUM_CTES = 1;
        *conjuntoOpTerm = malloc(((*NUM_OPBIN)+(*NUM_OPUN)+(1)+(*N)-1) * sizeof(int));

        i = 0;

        (*conjuntoOpTerm)[i++] = packInt(FBIN, PLUS);
        (*conjuntoOpTerm)[i++] = packInt(FBIN, MIN);
        (*conjuntoOpTerm)[i++] = packInt(FBIN, MULT);
        (*conjuntoOpTerm)[i++] = packInt(FBIN, DIV);


        (*conjuntoOpTerm)[i++] = packInt(FUN, SIN);
        (*conjuntoOpTerm)[i++] = packInt(FUN, COS);
        (*conjuntoOpTerm)[i++] = packInt(FUN, SQR);
        (*conjuntoOpTerm)[i++] = packInt(FUN, EXP);


//        for (i = 0; i < (*NUM_OPBIN); i++){
//            (*conjuntoOpTerm)[i] = packInt(FBIN, i);
//        }
//        for (j = 0 ; j < (*NUM_OPUN); j++, i++){
//            (*conjuntoOpTerm)[i] = packInt(FUN, i);
//        }

        for(j = 0; j < (*N)-1; j++, i++){
            (*conjuntoOpTerm)[i] = packInt(VAR, j);
        }

        (*conjuntoOpTerm)[i] = packFloat(CTE, 1.0);

    }
    fclose(arq);

    return dadosTreinamento;

}


float proDiv(float num, float div){
    if(div == 0){
        return 1;
    } else {
        return (num/div);
    }
}

float infDiv(float num, float div){
    if(div == 0){
        return INFINITY;
    } else {
        return (num/div);
    }
}

float proSqrt(float num){
    if(num < 0){
        return 1;
    } else {
        return sqrt(num);
    }
}

float infSqrt(float num){
    if(num < 0){
        return INFINITY;
    } else {
        return sqrt(num);
    }
}

void imprimeParametros(int M, int N, int NUM_CTES, int NUM_OPBIN, int NUM_OPUN){
    printf("/*-----------------------------------------------------------------\n");
    printf("* NUMERO INDIVIDUOS: %d \t CROSS-OVER: %.2f\n", NUM_INDIV, PROB_CROSS);
    printf("* NUMERO GERACOES  : %d \t MUTACAO   : %.2f\n", NUM_GERACOES, PROB_MUT);
    printf("* NUM MAXIMO NOS   : %d \t ELITISMO  : %.2f\n", MAX_NOS, ELITISMO);
    printf("* PROFUNDIDADE MAX : %d \t \n", MAX_DEPTH);
    printf("* M: %d \t N: %d\n", M, N);
    printf("* NUM_OPBIN: %d \t NUM_OPUN: %d\n", NUM_OPBIN, NUM_OPUN);
    printf("* NUM_CTES: %d \t SEED: %d\n", NUM_CTES, SEED);
    printf("*------------------------------------------------------------------\n");
}

