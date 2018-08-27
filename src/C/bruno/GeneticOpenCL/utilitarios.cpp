#include "utilitarios.h"
#include "constantes.h"

int rand2(int *seed){
    int s  = *seed;
    s = ((unsigned int)(s * 16807) % 2147483647);//(int)(pown(2.0, 31)-1));
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

int randomFunctionBin(int NUM_OPBIN){
    return rand() % NUM_OPBIN;
}
int randomFunctionUn(int NUM_OPUN){
    return rand() % NUM_OPUN;
}

float randomConst(int* seed, float maxDados, float minDados){
    float random = ((float)rand2(seed))/2147483647;
    float range = maxDados - minDados;
    float result = (range*random)+ minDados;

    return result;
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
    printf("* NUMERO GERACOES  : %d \t MUTACAO   : %.2f\n", CARGA/NUM_INDIV, PROB_MUT);
    printf("* NUM MAXIMO NOS   : %d \t ELITISMO  : %.2f\n", MAX_NOS, ELITISMO);
    printf("* PROFUNDIDADE MAX : %d \t \n", MAX_DEPTH);
    printf("* M: %d \t N: %d\n", M, N);
    printf("* NUM_OPBIN: %d \t NUM_OPUN: %d\n", NUM_OPBIN, NUM_OPUN);
    printf("* NUM_CTES: %d \t SEED: %d\n", NUM_CTES, SEED);
    printf("*------------------------------------------------------------------\n");
}

float** readTrainingData(int *M, int *N, int *NUM_CTES, int *NUM_OPBIN, int *NUM_OPUN, char ***LABELS,
                          int **conjuntoOpTerm, float *maxDados, float *minDados, char *filename){

    std::fstream arq;
    float** dadosTreinamento;
    int i, j, k;
    int readLabel;
    int readOps;//, numOpBin, numOpUn;
    int info;

    printf("Lendo Dados Arquivo... %s\n",filename);
    arq.open(filename, std::fstream::in);

    arq >> (*M);
    arq >> (*N);
    arq >> (readLabel);

    dadosTreinamento = new float* [(*M)];
    for(i = 0; i < (*M); i++){
        dadosTreinamento[i] = new float [(*N)];
    }

    (*LABELS) = new char* [*(N)];
    for(i = 0; i < (*N); i++){
        (*LABELS)[i] = new char [10];
    }

    if(readLabel){
        for(i = 0; i < (*N); i ++){
            arq >> (*LABELS)[i];
        }
    } else {
        for(i = 0; i < (*N); i ++){
            std::stringstream ss;
            std::string str;
            char lab;

            if(i < (*N)/2)
                lab = (char)('a' + i);
            else
                lab = (char)('A' + i - ((*N)/2));
            ss << lab;
            ss >> str;

            (*LABELS)[i] = (char*)(str.c_str());
        }
    }

    for(i = 0; i < (*M); i++){
        for(j = 0; j < (*N); j++){
            arq >> dadosTreinamento[i][j];

        }
    }

    arq >> readOps;

    if(readOps){
        arq >> (*NUM_OPBIN);
        arq >> (*NUM_OPUN);

        (*conjuntoOpTerm) = new int [(*NUM_OPBIN)+(*NUM_OPUN)+(1)+(*N)-(1)];

        for (i = 0; i < (*NUM_OPBIN); i++){
            arq >> info;
            (*conjuntoOpTerm)[i] = packInt(FBIN, info);
        }
        for (j = 0; j < (*NUM_OPUN); j++, i++){
            arq >> info;
            (*conjuntoOpTerm)[i] = packInt(FUN, info);
        }

        for(j = 0; j < (*N)-(1); j++, i++){
            (*conjuntoOpTerm)[i] = packInt(VAR, j);
        }

        (*conjuntoOpTerm)[i] = packInt(CTE, 1);
        *NUM_CTES = 1;
    } else {
        *NUM_OPBIN = 4;
        *NUM_OPUN = 4;
        *NUM_CTES = 1;

        (*conjuntoOpTerm) = new int [(*NUM_OPBIN)+(*NUM_OPUN)+(1)+(*N)-(1)];
        //*conjuntoOpTerm = (int*)malloc(((*NUM_OPBIN)+(*NUM_OPUN)+(1)+(*N)-1) * sizeof(int*));

        i = 0;

        (*conjuntoOpTerm)[i++] = packInt(FBIN, PLUS);
        (*conjuntoOpTerm)[i++] = packInt(FBIN, MIN);
        (*conjuntoOpTerm)[i++] = packInt(FBIN, MULT);
        (*conjuntoOpTerm)[i++] = packInt(FBIN, DIV);


        (*conjuntoOpTerm)[i++] = packInt(FUN, SIN);
        (*conjuntoOpTerm)[i++] = packInt(FUN, COS);
        (*conjuntoOpTerm)[i++] = packInt(FUN, SQR);
        (*conjuntoOpTerm)[i++] = packInt(FUN, EXP);


        for(j = 0; j < (*N)-(1); j++, i++){
            (*conjuntoOpTerm)[i] = packInt(VAR, j);
        }

        (*conjuntoOpTerm)[i] = packFloat(CTE, 1.0);


    }
    return dadosTreinamento;
}

bool IsPowerOf2( int n ){
   return (n & -n) == n;
}

unsigned NextPowerOf2( unsigned n ){
   n--;
   n |= n >> 1;  // handle  2 bit numbers
   n |= n >> 2;  // handle  4 bit numbers
   n |= n >> 4;  // handle  8 bit numbers
   n |= n >> 8;  // handle 16 bit numbers
   n |= n >> 16; // handle 32 bit numbers
   n++;

   return n;
}
std::string ToString( float t ){
      std::stringstream ss; ss << std::setprecision(32) << t; return ss.str();
}


float mysqr(float f){
    f *= f;
    return f;
}
