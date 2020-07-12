/*---------------------- Importacao das Bibliotecas --------------------------*/
#include <stdio.h>
#include "string.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "LU.h"
#include "funcoes_teste.h"

// Example of command to execute:
// ./bin/Release/DE -pop 100 -CR 0.8 -F 0.8 -gen 100 -func 14 -var 3 -seed 1 -N 10 -M 5

// Example using valgrind (memory verification)
// valgrind --leak-check=full -v ./bin/Debug/DE -pop 100 -CR 0.8 -F 0.8 -gen 100 -func 7 -var 3 -seed 2 -N 40 -M 20

/*---------------------- Variaveis globais ------------------------------------*/
double **Matriz_Z;
double **Matriz_Coef;
double **populacao_p;
double *solucao_x_til;
double **Matriz_E;
double *b;
double *x_star;

/*---------------------- Prototipo das Funcoes--------------------------------*/
double** Inicializa_Populacao(int N_Restricoes, int N_Coordenada, int N_Individuo, int prob, double* bounds);
void Inicializa_Parametros(int argc, char **argv, int *pop, double *cr, double *f, int *gen, int *var, int *func, int *seed, int *nCoordenadasNova, int *nRestricoesNova);
void Avalia_Populacao(double **populacao, int N_Individuo, int N_Coordenada, int prob);
int procuraMelhor( double **populacao, int n_pop, int coordenadas );
void imprimirIndividuo( double **populacao, int coordenadas, int id, int geracao, int restricoes );
void imprimir_pop(double **pop, double **pop_p, int N_Individuo, int N_Coordenada, int N_Restricoes);
void imprimirInd( double *populacao, int coordenadas, int restricoes );


/*----------------------- main -----------------------------------------------*/
int main( int argc, char** argv ) {
    
    int FUNC, VAR, SEED, N_Individuo, N_Geracoes, N_Restricoes, N_Coordenada;
    int N_CoordenadaNova = -1, N_RestricoesNova = -1;
    double F, CR;    
    double melhorF;
    double *bounds;
    
    Inicializa_Parametros(argc, argv, &N_Individuo, &CR, &F, &N_Geracoes, &VAR, &FUNC, &SEED, &N_CoordenadaNova, &N_RestricoesNova);
                       
    srand( SEED );
    bounds = (double*) malloc( 2*sizeof(double) );
    
    Problema_Teste(FUNC, &N_Restricoes, &N_Coordenada, &melhorF, bounds, N_CoordenadaNova, N_RestricoesNova);
    
    //---------------------- DE() ----------------------------------------------
    
    double **populacao;                  
    int idMelhorIndividuo;
    
    populacao = Inicializa_Populacao(N_Restricoes, N_Coordenada, N_Individuo, FUNC, bounds);                   
    Avalia_Populacao(populacao, N_Individuo, N_Coordenada, FUNC);
    idMelhorIndividuo = procuraMelhor( populacao, N_Individuo, N_Coordenada );
            
    int g,i;
    for(g = 1; g < N_Geracoes; g++){
    //for(g = 1; g < N_Geracoes && populacao[idMelhorIndividuo][N_Coordenada] - melhorF > epsilon ; g++){
        
        //idMelhorIndividuo = procuraMelhor( populacao, N_Individuo, N_Coordenada );
        //printf("Melhor na iteracao: \n");
        //imprimirIndividuo(populacao, N_Coordenada, idMelhorIndividuo, 0, N_Restricoes);
        //printf("\n");

        for (i = 0; i < N_Individuo; ++i) {

            int id1 = rand() % N_Individuo;
            while(id1 == i){
                id1 = rand() % N_Individuo;
            }
            int id2 = rand() % N_Individuo;
            while((id2 == i) || (id2 == id1)){
                id2 = rand() % N_Individuo;
            }
            int id3 = rand() % N_Individuo;
            while((id3 == i) || (id3 == id1) || (id3 == id2)){
                 id3 = rand() % N_Individuo;
            }
            //printf("id1: %d, id2: %d, id3: %d \n", id1, id2, id3);

            double *novo_id, *novo_id_p;
            novo_id = (double*) malloc((N_Coordenada+1) * sizeof(double)); // dimensao original 'n'
            novo_id_p = (double*) malloc((N_Coordenada-N_Restricoes) * sizeof(double)); // dimensao reduzida 'n-m = p'

            int jRand = rand()%(N_Coordenada-N_Restricoes);
            int j;
            for( j = 0; j < (N_Coordenada-N_Restricoes); j++){

                if (j == jRand || rand()/(float)RAND_MAX < CR){

                    switch(VAR){
                    case 1: //DE/rand/1/bin                            
                        novo_id_p[j] = populacao_p[id1][j] + F*(populacao_p[id2][j] - populacao_p[id3][j]);
                        break;
                    case 2: //DE/best/1/bin                            
                        novo_id_p[j] = populacao_p[idMelhorIndividuo][j] + F*(populacao_p[id1][j] - populacao_p[id2][j]);
                        break;
                    case 3: //DE/target-to-rand/1/bin                        
                        novo_id_p[j] = populacao_p[i][j] + F*(populacao_p[id1][j] - populacao_p[i][j]) + F*(populacao_p[id2][j] - populacao_p[id3][j]);
                        break;
                    case 4: //DE/target-to-best/1/bin                        
                        novo_id_p[j] = populacao_p[i][j] + F*(populacao_p[idMelhorIndividuo][j] - populacao_p[i][j]) + F*(populacao_p[id2][j] - populacao_p[id3][j]);
                        break;
                    }

                } else {
                        novo_id_p[j] = populacao_p[i][j];
                }                                        
            }

            int l;                
            //printf("Novo_id_p: ");
            //for (j = 0 ; j < (N_Coordenada-N_Restricoes); j++){
            //    printf(" %f ", novo_id_p[j]);
            //}
            //printf("\n");                
            //printf("Novo_id:");                
            for ( l = 0; l < N_Coordenada; l++){
                double Zp = 0;
                for (j = 0 ; j < (N_Coordenada-N_Restricoes) ; j++){
                    Zp += Matriz_Z[l][j] * novo_id_p[j];
                }             
                //printf(" [%f] ",solucao_x_til[l]);
                novo_id[l] = solucao_x_til[l] + Zp;   
                //printf(" %f ", novo_id[l]);
            }           

            //printf(" aptidao: ");                
            Avalia(novo_id, N_Coordenada, FUNC, Matriz_Coef, x_star);
            //printf(" %f ", novo_id[N_Coordenada]);                
            //printf("\n");                                                
            //getchar();                

            if(novo_id[N_Coordenada] < populacao[i][N_Coordenada]){
                    int j;
                    for (j = 0; j <= N_Coordenada; ++j) {
                            populacao[i][j] = novo_id[j];
                    }
                    for (j = 0 ; j < (N_Coordenada-N_Restricoes); j++){
                            populacao_p[i][j] = novo_id_p[j];
                    }
            }

            free(novo_id);
            free(novo_id_p);
        }

        //imprimir_pop(populacao, populacao_p, N_Individuo, N_Coordenada, N_Restricoes);
        
        idMelhorIndividuo = procuraMelhor( populacao, N_Individuo, N_Coordenada );
        if ( g % 1000 == 0 ){
            imprimirIndividuo(populacao, N_Coordenada, idMelhorIndividuo, g, N_Restricoes);
            printf("\n");
        }
        //getchar();
    }
    
    

    //Print_Populacao(populacao, N_Individuo, N_Coordenada, prob);
    //printf(" Problema %d Resolvido com Sucesso!\n\n\n", prob);

    // LIBERANDO MEMORIA
    for (i = 0; i < N_Individuo; i++) {
        free(populacao[i]);
        free(populacao_p[i]);
    }
    free(populacao);
    free(populacao_p);
    free(solucao_x_til);  
    free(b);
    free(x_star);
    free(bounds);
    for (i = 0; i < N_Coordenada; i++) {
        free(Matriz_Z[i]);
        free(Matriz_Coef[i]);        
    }
    for (i = 0; i < N_Restricoes; i++){
        free(Matriz_E[i]);
    }
    free(Matriz_Z);
    free(Matriz_Coef);
    free(Matriz_E);
    

    return EXIT_SUCCESS;


}


void Inicializa_Parametros(int argc, char **argv, int *pop, double *cr, double *f, int *gen, int *var, int *func, int *seed, int *nCoordenadasNova, int *nRestricoesNova){
       
    if(argc >= 15) {
    
        int i;
        for( i = 0; i < argc; i++){
            if (strcmp(argv[i], "-gen") == 0){
                    *gen  = atoi(argv[++i]);
            } else if (strcmp(argv[i], "-pop") == 0){
                    *pop = atoi(argv[++i]);            
            } else if (strcmp(argv[i], "-func") == 0){
                    *func = atoi(argv[++i]);
            } else if (strcmp(argv[i], "-F") == 0){
                    *f = atof(argv[++i]);
            } else if (strcmp(argv[i], "-CR") == 0){
                    *cr = atof(argv[++i]);
            } else if (strcmp(argv[i], "-var") == 0){
                    *var = atoi(argv[++i]);                       
            } else if (strcmp(argv[i], "-seed") == 0){
                    *seed = atoi(argv[++i]);            
            }  else if (strcmp(argv[i], "-N") == 0){
                    *nCoordenadasNova = atoi(argv[++i]);            
            } else if (strcmp(argv[i], "-M") == 0){
                    *nRestricoesNova = atoi(argv[++i]);            
            }
        }       
             
    } else {
        //numero de parametros estah incorreto
        printf("Numero de parametros incorreto!");
        exit( EXIT_FAILURE );
               
    }   

}

double** Inicializa_Populacao(int N_Restricoes, int N_Coordenada, int N_Individuo, int prob, double* bounds){
    
    double **populacao;    
    int i, j, k ,m;
    //int p[N_Restricoes];
    //double b[N_Restricoes], z[N_Restricoes], vetor_v[N_Coordenada], base[N_Coordenada], solucao[N_Coordenada], d[N_Coordenada];
    //double Matriz_E[N_Restricoes][N_Coordenada], Matriz_E_Transposto[N_Coordenada][N_Restricoes], **Matriz_M;//, **Matriz_Z;    
    int *p;
    double *z, *vetor_v, *vetor_u, *base, *d, *resp;
    double **Matriz_E_Transposto, **Matriz_M;
    double *resFx;       
    FILE *arq;           
    int restOriginal, coordOriginal;       
    
    switch(prob){
        case 1:
            arq = fopen("input_data/Entrada1.txt", "r");
            if (arq == NULL){
            printf("Problemas na LEITURA do arquivo\n");
            }
            break;
        case 2:
            arq = fopen("input_data/Entrada2.txt", "r");
            if (arq == NULL){
            printf("Problemas na LEITURA do arquivo\n");
            }
            break;
        case 3:
            arq = fopen("input_data/Entrada3.txt", "r");
            if (arq == NULL){
            printf("Problemas na LEITURA do arquivo\n");
            }
            break;
        case 4:
            arq = fopen("input_data/Entrada4.txt", "r");
            if (arq == NULL){
            printf("Problemas na LEITURA do arquivo\n");
            }
            break;
        case 5:
            arq = fopen("input_data/Entrada5.txt", "r");
            if (arq == NULL){
            printf("Problemas na LEITURA do arquivo\n");
            }
            break;
        case 6:
            arq = fopen("input_data/Entrada6.txt", "r");
            if (arq == NULL){
            printf("Problemas na LEITURA do arquivo\n");
            }
            break;
        case 7:
            arq = fopen("input_data/Entrada7.txt", "r");
            if (arq == NULL){
            printf("Problemas na LEITURA do arquivo\n");
            }
            break;
        case 8:
            arq = fopen("input_data/Entrada8.txt", "r");
            if (arq == NULL){
            printf("Problemas na LEITURA do arquivo\n");
            }
            break;
        case 9:
            arq = fopen("input_data/Entrada9.txt", "r");
            if (arq == NULL){
            printf("Problemas na LEITURA do arquivo\n");
            }
            break;
        case 10:
            arq = fopen("input_data/Entrada10.txt", "r");
            if (arq == NULL){
            printf("Problemas na LEITURA do arquivo\n");
            }
            break;
        case 11:
            arq = fopen("input_data/Entrada11.txt", "r");
            if (arq == NULL){
            printf("Problemas na LEITURA do arquivo\n");
            }
            break;
        case 12:
            arq = fopen("input_data/Entrada12.txt", "r");
            if (arq == NULL){
            printf("Problemas na LEITURA do arquivo\n");
            }
            break;   
        case 13:
            arq = fopen("input_data/Entrada13.txt", "r");
            if (arq == NULL){
            printf("Problemas na LEITURA do arquivo\n");
            }
            break;  
        case 14:
            arq = fopen("input_data/Entrada14.txt", "r");
            if (arq == NULL){
            printf("Problemas na LEITURA do arquivo\n");
            }
            break;  
        case 19: // PROBLEMA 119 DO COLVILLE
            arq = fopen("input_data/Entrada19.txt", "r");
            if (arq == NULL){
            printf("Problemas na LEITURA do arquivo\n");
            }
            break;   
        case 20: // PROBLEMAS GERADOS POR Vaz09
            arq = fopen("input_data/Entrada20.txt", "r");
            if (arq == NULL){
            printf("Problemas na LEITURA do arquivo\n");
            }
            break;  
        case 21: 
            arq = fopen("input_data/Entrada21.txt", "r");
            if (arq == NULL){
            printf("Problemas na LEITURA do arquivo\n");
            }
            break;  
        case 22: 
            arq = fopen("input_data/Entrada22.txt", "r");
            if (arq == NULL){
            printf("Problemas na LEITURA do arquivo\n");
            }
            break;  
        case 23: 
            arq = fopen("input_data/Entrada23.txt", "r");
            if (arq == NULL){
            printf("Problemas na LEITURA do arquivo\n");
            }
            break;  
        case 24: 
            arq = fopen("input_data/Entrada24.txt", "r");
            if (arq == NULL){
            printf("Problemas na LEITURA do arquivo\n");
            }
            break;              
        case 25: 
            arq = fopen("input_data/Entrada25.txt", "r");
            if (arq == NULL){
            printf("Problemas na LEITURA do arquivo\n");
            }
            break;  
        default:
            printf("\n Informe o numero do problema correto! \n");
    }                           
    
    // ALOCANDO MEMORIA -------------------------------------------------------
    p = (int*) malloc( N_Restricoes * sizeof(int) );
    b = (double*) malloc( N_Restricoes * sizeof(double) );
    z = (double*) malloc( N_Restricoes * sizeof(double) );
    vetor_v = (double*) malloc( N_Coordenada * sizeof(double) );
    vetor_u = (double*) malloc( N_Coordenada * sizeof(double) );
    base = (double*) malloc( N_Coordenada * sizeof(double) );
    resp = (double*) malloc( N_Coordenada * sizeof(double) );
    d = (double*) malloc( N_Coordenada * sizeof(double) );  
    
    x_star = (double*) malloc( N_Coordenada * sizeof(double) ); 
    
    Matriz_M = (double**) malloc (N_Restricoes*sizeof(double*));    
    for(i=0; i<N_Restricoes; i++){
        Matriz_M[i] = (double*) malloc (N_Restricoes*sizeof(double));
    }
    
    Matriz_E = (double**) malloc( N_Restricoes * sizeof(double*) );    
    for(i=0; i<N_Restricoes; i++) {
        Matriz_E[i] = (double*) malloc( N_Coordenada * sizeof(double) );        
    }
    
    Matriz_E_Transposto = (double**) malloc( N_Coordenada * sizeof(double*) );
    for(i=0; i<N_Coordenada; i++) {
        Matriz_E_Transposto[i] = (double*) malloc( N_Restricoes * sizeof(double) );
    }
    
    // MATRIZ 'Z' PARA A BASE DO NUCLEO DE 'E'
    Matriz_Z = (double**) malloc( N_Coordenada * sizeof(double*) );    
    for(i=0; i<N_Coordenada; i++) {
        Matriz_Z[i] = (double*) malloc( (N_Coordenada-N_Restricoes) * sizeof(double) );        
    }   
    
    // MATRIZ DE COEFICIENTES DO PROBLEMA 19
    Matriz_Coef = (double**) malloc (N_Coordenada*sizeof(double*));
    for(i=0; i<N_Coordenada; i++){
        Matriz_Coef[i] = (double*) malloc (N_Coordenada*sizeof(double));
    }  
    //-------------------------------------------------------------------------
    
    
    // ESTES SAO OS PROBLEMAS QUE PODEM SER ESCALONADOS ----------------------- 
    if (prob == 7 || prob == 8 || prob == 9 || prob == 10 || prob == 11 || prob == 14){
        restOriginal = 5;
        coordOriginal = 10;
        if (prob == 14){
            restOriginal = 3;
            coordOriginal = 4;
        }
        
        // MATRIZES E VETOR COM A DIMENSAO ORIGINAL
        double* bOr = (double*) malloc( restOriginal * sizeof(double) );
        double** Matriz_EOr = (double**) malloc( restOriginal * sizeof(double*) );
        for(i=0; i < restOriginal; i++) {        
            Matriz_EOr[i] = (double*) malloc( coordOriginal * sizeof(double) );
        }
        double** Matriz_ZOr = (double**) malloc (coordOriginal*sizeof(double*));    
        for(i=0; i < coordOriginal ; i++){
            Matriz_ZOr[i] = (double*) malloc ((coordOriginal-restOriginal)*sizeof(double));
        }
    
        //PREENCHER 'EOr', 'ZOr' E 'bOr' COM OS DADOS DO ARQUIVO DE ENTRADA
        for(i = 0; i < restOriginal; i++){
            for(j = 0; j < coordOriginal; j++){
                fscanf(arq,"%lf",&Matriz_EOr[i][j]);
//                printf(" %.3f ", Matriz_EOr[i][j]);                
            }
//            printf("\n");
        }
//        printf("\n");
        for(i = 0; i < restOriginal; i++){
           fscanf(arq,"%lf", &bOr[i]);
//           printf(" %.3f ", bOr[i]);
        }
//        printf("\n\n");
                       
        for(i = 0; i < coordOriginal; i++){
            for(j = 0; j < (coordOriginal-restOriginal); j++){
                fscanf(arq,"%lf",&Matriz_ZOr[i][j]);
//                printf(" %f ", Matriz_ZOr[i][j]);            
            }
//            printf("\n");
        }   
//        printf("\n");
                
        // SE A DIMENSAO FOR A ORIGINAL (N = 10 E M = 5), PREENCHER OS VALORES
        if (N_Coordenada == coordOriginal && N_Restricoes == restOriginal){
            for ( i = 0 ; i < restOriginal ; i++){
                for ( j = 0 ; j < coordOriginal ; j++){                    
                    Matriz_E[i][j] = Matriz_EOr[i][j];               
                    Matriz_E_Transposto[j][i] = Matriz_E[i][j];                                                          
                }               
            }
            for(i = 0; i < restOriginal; i++){
                b[i] = bOr[i];           
            }
            for(i = 0; i < coordOriginal; i++){
                for(j = 0; j < (coordOriginal-restOriginal); j++){
                    Matriz_Z[i][j]= Matriz_ZOr[i][j];                              
                }               
            }               
            
        // SE DIMENSAO DIFERENTE DA ORIGINAL, REPLICAR OS VALORES DA MATRIZ 
        // 'EOr' PARA A MATRIZ ESCALONADA 'E'  
        } else {            
                   
            int pM_rest = N_Restricoes / 5;
            if (prob == 14){
                pM_rest = N_Restricoes / 3;
            }
            
            for(i = 0; i < N_Restricoes; i++){
                for(j = 0; j < N_Coordenada; j++){                               
                        Matriz_E[i][j] = 0.0;                                 
                        Matriz_E_Transposto[j][i] = Matriz_E[i][j];               
                }                           
            }

            int cont_p;
            int cont_i = 0;
            int cont_j = 0;        
            for (cont_p = 0 ; cont_p < pM_rest; cont_p++){
                cont_i = cont_p * restOriginal;                
                for ( i = 0 ; i < restOriginal ; i++, cont_i++ ){
                    cont_j = cont_p * coordOriginal;                    
                    
                    for ( j = 0 ; j < coordOriginal ; j++, cont_j++){                    
                        Matriz_E[cont_i][cont_j] = Matriz_EOr[i][j];                              
                        Matriz_E_Transposto[cont_j][cont_i] = Matriz_E[cont_i][cont_j];                        
                    }                                      
                }
            }


            for(i = 0; i < N_Restricoes; i++){
                for(j = 0; j < N_Coordenada; j++){                               
//                    printf( " %.3f ", Matriz_E[i][j]);                         
                }            
//                printf("\n");
            }
//            printf("\n\n");
           
                        
            //REPLICAR OS VALORES DO VETOR 'bOr' PARA O VETOR 'b' -------------
            cont_i = 0;
            for (cont_p = 0 ; cont_p < pM_rest; cont_p++){
                cont_i = cont_p * restOriginal;
                for(i = 0; i < restOriginal; i++, cont_i++){
                    b[cont_i] = bOr[i];
//                    printf(" %.3f ", b[cont_i]);
                }
            }

//            printf("\n\n");
//            getchar();

                      
            //REPLICAR OS VALORES DA MATRIZ 'ZOr' PARA 'Z' --------------------            
            cont_i = 0;
            cont_j = 0;     
            int pM_coord = N_Restricoes / restOriginal;             
            
            for (i = 0 ; i < N_Coordenada ; i++){
                for (j = 0 ; j < (N_Coordenada-N_Restricoes) ; j++){                                       
                    Matriz_Z[i][j] = 0.0;                                                      
                }
            }
            
            for (cont_p = 0 ; cont_p < pM_coord; cont_p++){
                cont_i = cont_p * coordOriginal;
                 for ( i = 0 ; i < coordOriginal ; i++, cont_i++ ){
                    cont_j = cont_p * (coordOriginal-restOriginal);
                    for ( j = 0 ; j < (coordOriginal-restOriginal) ; j++, cont_j++){                    
                        Matriz_Z[cont_i][cont_j] = Matriz_ZOr[i][j]; 
                    }
                }
            }  
            
            for( i = coordOriginal*pM_coord ; i < N_Coordenada; i++){
                for( j = N_Restricoes; j < (N_Coordenada-N_Restricoes); j++){
                    if ( (i-N_Restricoes) == j ){                        
                        Matriz_Z[i][j]= 1.0;   
                    } else {
                        Matriz_Z[i][j]= 0.0;   
                    }
                }
            }            

            /*
            for(i = 0; i < N_Coordenada; i++){
                for(j = 0; j < (N_Coordenada-N_Restricoes); j++){                               
                    printf( " %.f ", Matriz_Z[i][j]);                         
                }            
                printf("\n");
            }
            getchar(); 
            */
        }
    
               
               
        // LIBERAR MEMORIA ----------------------------------------------------
        for (i = 0; i < restOriginal ; i++) {
            free(Matriz_EOr[i]);           
        }
        for (i = 0; i < coordOriginal ; i++) {
            free(Matriz_ZOr[i]);           
        }
        
        free(bOr);
        free(Matriz_EOr);
        free(Matriz_ZOr);    
        
    // SE PROBLEMA 19, LER DADOS DA MATRIZ DE COEFICIENTES -------------------    
    } else if (prob == 19){
        
        //printf("\n\n MATRIZ E \n");
        for(i = 0; i < N_Restricoes; i++){
            for(j = 0; j < N_Coordenada; j++){
                fscanf(arq,"%lf",&Matriz_E[i][j]);
        //        printf(" %f ", Matriz_E[i][j]);
                Matriz_E_Transposto[j][i] = Matriz_E[i][j];
            }
        //    printf("\n");
        }
        
        //printf("\n\n VETOR b \n");
        for(i = 0; i < N_Restricoes; i++){
           fscanf(arq,"%lf", &b[i]);
        //   printf(" %f ", b[i]);
        }
        //printf("\n");
                
        //printf("\n\n MATRIZ DE COEFICIENTES \n");
        for(i = 0; i < N_Coordenada; i++){
            for(j = 0; j < N_Coordenada; j++){
                fscanf(arq,"%lf",&Matriz_Coef[i][j]);
        //       printf(" %.2f ", Matriz_Coef[i][j]);           
            }
        //    printf("\n");
        }           
        
        //printf("\n\n MATRIZ Z - BASE NUCLEO DE E \n");
        for(i = 0; i < N_Coordenada; i++){
            for(j = 0; j < (N_Coordenada-N_Restricoes); j++){
                fscanf(arq,"%lf",&Matriz_Z[i][j]);
        //        printf(" %f ", Matriz_Z[i][j]);            
            }
        //    printf("\n");
        }    
        
    // DEMAIS PROBLEMAS -------------------------------------------------------    
    } else {  

        for(i = 0; i < N_Restricoes; i++){
            for(j = 0; j < N_Coordenada; j++){
                fscanf(arq,"%lf",&Matriz_E[i][j]);
                //printf(" %f ", Matriz_E[i][j]);
                Matriz_E_Transposto[j][i] = Matriz_E[i][j];
            }
            //printf("\n");
        }
        for(i = 0; i < N_Restricoes; i++){
           fscanf(arq,"%lf", &b[i]);
           //printf(" %f ", b[i]);
        }
        //printf("\n");
                
        for(i = 0; i < N_Coordenada; i++){
            for(j = 0; j < (N_Coordenada-N_Restricoes); j++){
                fscanf(arq,"%lf",&Matriz_Z[i][j]);
                //printf(" %f ", Matriz_Z[i][j]);            
            }
            //printf("\n");
        }    
        
        // -- SE PROBLEMAS A PARTIR DE 20, LER VETOR x^*
        // x^* eh necessario para o calculo da funcao objetivo
        if (prob >= 20){
            for(i = 0; i < N_Coordenada; i++){
               fscanf(arq,"%lf", &x_star[i]);
               //printf(" %f ", x_star[i]);
            }
            //printf("\n");
        }
        
        
/*
        Avalia(x_star, N_Coordenada, prob, Matriz_Coef, x_star);
        imprimirInd(x_star,N_Coordenada,N_Restricoes);
        getchar();
*/
        
    }

    fclose(arq);           
   
    // INICIO DA CONSTRUCAO DE SOLUCAO INICIAL VIAVEL -------------------------
    /*
    Codigo do Rodrigo -- DELEqC
    */
    for(i = 0; i < N_Restricoes; i++){
        for(j = 0; j < N_Restricoes; j++){
            Matriz_M[i][j] = 0.0;
            for(k = 0; k < N_Coordenada; k++){
                Matriz_M[i][j] =  Matriz_M[i][j] + Matriz_E[i][k]*Matriz_E_Transposto[k][j];
            }
        }
    }

    for(i=0; i<N_Restricoes; i++){
        p[i] = i;
    }
    
    lu(Matriz_M,&p[0],N_Restricoes); //Encontra as matrizes 'L' e 'U'
    resFx = lu_solve(Matriz_M,p,b,N_Restricoes); //Encontra o resultado do sistema Mx=b

    populacao = (double**) malloc(N_Individuo *sizeof(double*));
    for(i = 0; i < N_Individuo; i++){
        populacao[i]= (double*) malloc((N_Coordenada+1) *sizeof(double));
    }

    for(j = 0; j < N_Coordenada; j++){
        base[j] = 0;
        for(m = 0; m < N_Restricoes; m++){
            base[j] = base[j]+ Matriz_E_Transposto[j][m]*resFx[m];
        }
    }

/*    
    //----- GERAR UM INDIVIDUO x_i -----
    for(j = 0; j < N_Coordenada; j++){        
       d[j] =  ( (rand()%1000000) / 1000000.f ) * ( bounds[1]-bounds[0] ) + bounds[0];
    }
    for(j = 0; j < N_Restricoes; j++){
        z[j] = 0.0;
        for(m = 0; m < N_Coordenada; m++){
            z[j]=  z[j] + Matriz_E[j][m]*d[m];
        }
    }

    vetor_u = lu_solve(Matriz_M,p,z,N_Restricoes);

    for(j = 0; j < N_Coordenada; j++){
        vetor_v[j] = 0;
        for(m = 0; m < N_Restricoes; m++){
            vetor_v[j]  = vetor_v[j]  + Matriz_E_Transposto[j][m]*vetor_u[m];
        }
        resp[j] = base[j] + d[j] - vetor_v[j];
    }
            
    //imprimirInd( resp, N_Coordenada, N_Restricoes ); //TODO -- comentei essa linha mas nao sei se podia
    //printf("\n\n");
    //getchar();
    //---------------
 */
    
    //printf("\n Solucao Inicial -- x_til \n");
    solucao_x_til = (double*) malloc((N_Coordenada) * sizeof(double));

    for(i = 0; i < N_Coordenada; i++){ 
        //No Algoritmo GENERATE INITIAL POPULATION        
        solucao_x_til[i] = base[i]; //x_til eh o x_0
        //solucao_x_til[i] = resp[i]; // x_til eh um x_i (descomentar -GERAR UM INDIVIDUO x_i-)
    }          
    
    //imprimirInd( solucao_x_til, N_Coordenada, N_Restricoes ); //TODO -- comentei essa linha mas nao sei se podia
    //printf("\n\n");
    //getchar();
    
    //-------------------------------------------------------------------------
    // NOVO CODIGO PARA A CRIACAO DA POPULACAO 'P' 
    // ------------------------------------------------------------------------
    populacao_p = (double**) malloc(N_Individuo *sizeof(double*));
    for(i = 0; i < N_Individuo; i++){
        populacao_p[i]= (double*)  malloc((N_Coordenada-N_Restricoes) *sizeof(double));
    }
        
    for ( k = 0 ; k < N_Individuo; k++){
        for (j = 0; j < N_Coordenada-N_Restricoes; j++){
            populacao_p[k][j] = ( (rand()%1000000) / 1000000.f ) * ( bounds[1]-bounds[0] ) + bounds[0];                        
        }
    }
               
    double* dd = (double*)  malloc((N_Coordenada) *sizeof(double));
    
    for (k = 0 ; k < N_Individuo; k++){

            for ( i = 0; i < N_Coordenada; i++){
                double Zp = 0;
                for (j = 0 ; j < N_Coordenada-N_Restricoes; j++){
                    Zp += Matriz_Z[i][j] * populacao_p[k][j];                    
                }              
                dd[i] = Zp;
                //printf(" %f  ", dd[i]);
                populacao[k][i] = solucao_x_til[i] + Zp;                
            }            
    }

    //for ( k = 0 ; k < N_Individuo; k++){        
    //    imprimirIndividuo(populacao, N_Coordenada, k, 0, N_Restricoes);     
    //}        
    //getchar();
    
    for (i = 0; i < N_Restricoes; i++){
        free(Matriz_M[i]); 
    }
    free(Matriz_M);
    for (i = 0; i < N_Coordenada; i++){
        free(Matriz_E_Transposto[i]);
    }
    free(Matriz_E_Transposto);
    free(p);
    free(z);
    free(vetor_v);
    free(base);
    free(d);    
    free(resFx); 
    free(dd);
            
    return populacao;
    
    for (i = 0; i < N_Individuo; i++){
        free(populacao[i]); 
    }
    free(populacao);
}

void Avalia_Populacao(double **populacao, int N_Individuo, int N_Coordenada, int prob){
	int i;
	for(i = 0; i < N_Individuo; i++){
        Avalia(populacao[i], N_Coordenada, prob, Matriz_Coef, x_star);
	}
}

/*
void Diferenciacao(double **populacao, double *novo_id, int id1, int id2, int N_Coordenada, double peso){
	int i;
	for (i = 0; i < N_Coordenada; ++i) {
		novo_id[i] = peso*(populacao[id1][i] - populacao[id2][i]);
	}

}

void Mutacao(double **populacao, double *novo_id, int idm, int N_Coordenada){
	int i;
	for (i = 0; i < N_Coordenada; ++i) {
		novo_id[i] = novo_id[i] + populacao[idm][i];
	}
}
*/


int procuraMelhor( double **populacao, int n_pop, int coordenadas ) {
    int melhor = 0;
    int i;
    for( i=1; i<n_pop; i++ ) {
        if ( populacao[i][coordenadas] < populacao[melhor][coordenadas] ) {
            melhor = i;
        }
    }
    return melhor;
}



//-- IMPRIMIR POP E POP_P
void imprimir_pop(double **pop, double **pop_p, int N_Individuo, int N_Coordenada, int N_Restricoes){
        
    int i,j;
    for (i  = 0 ; i < N_Individuo ; i++){
        
        imprimirIndividuo(pop, N_Coordenada, i, 0, N_Restricoes);
  
        printf(" || ");
        for (j = 0 ; j < N_Coordenada-N_Restricoes; j++){
                printf(" %0.6lf ", pop_p[i][j]);
        }
            printf(" || ");                        
            printf("\n\n");
 
    }   
    
}

//--- IMPRIMIR UM INDIVIDUO ---
void imprimirInd( double *populacao, int coordenadas, int restricoes ) {
    
    printf("%d | %f", populacao[coordenadas]); // aptidao na ultima posicao
    int i, j;
    double value;
    double somaViolacoes = 0;
    for(i=0; i<coordenadas; i++) {
        printf(" | %.10f", populacao[i]);
    }
    
    //produto matriz-vetor para imprimir as violacoes de restricao
    //printf(" | violacoes");
    for(i=0; i<restricoes; i++) {
		value = 0;
		for(j=0; j<coordenadas; j++) {
			value += Matriz_E[i][j] * populacao[ j ];
		}
		//printf(" | %f", fabs(value-b[i]));
		somaViolacoes += fabs(value-b[i]);
	}
    
    printf(" | %e", somaViolacoes);
    printf("\n");
    //o codigo que segue eh necessario quando ha individuos factiveis na populacao (nao eh o caso aqui)
//    System.out.printf(" | %b | %f", melhor.isFeasible(), melhor.getAvaliacaoPenalizada());
//    for(float xi: melhor.getRests()) {
//        System.out.printf(" | %f", xi);
//    }
//    System.out.println();

}

//--- IMPRIMIR POPULACAO ---
void imprimirIndividuo( double **populacao, int coordenadas, int id, int geracao, int restricoes ) {
    
    printf("%d | %f", geracao, populacao[id][coordenadas]); // aptidao na ultima posicao
    int i, j;
    double value;
    double somaViolacoes = 0;
    for(i=0; i<coordenadas; i++) {
        printf(" | %.10f", populacao[id][i]);
    }
    
    //produto matriz-vetor para imprimir as violacao de restricao
    //printf(" | violacoes");
    for(i=0; i<restricoes; i++) {
		value = 0;
		for(j=0; j<coordenadas; j++) {
			value += Matriz_E[i][j] * populacao[ id ][ j ];
		}
		//printf(" | %f", fabs(value-b[i]));
		somaViolacoes += fabs(value-b[i]);
	}
    
    printf(" | %e", somaViolacoes);
    printf("\n");
    //o codigo que segue eh necessario quando ha individuos factiveis na populacao (nao eh o caso aqui)
//    System.out.printf(" | %b | %f", melhor.isFeasible(), melhor.getAvaliacaoPenalizada());
//    for(float xi: melhor.getRests()) {
//        System.out.printf(" | %f", xi);
//    }
//    System.out.println();

}

