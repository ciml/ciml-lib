#include "eg.h"
#include "ag.h"
#include "parser.h"
#include "gramatica.h"
#include "utils.h"
#include <omp.h>
#include <stdlib.h>

int invalidos = 0;
double tempoTotalAvaliacao = 0;

int ObtemMelhor(individuo * pop){

    int indice_melhor=0;
	int i;

	for(i=1;i< TAMANHO_POPULACAO; i++){
        if(pop[i].aptidao>pop[indice_melhor].aptidao){
            indice_melhor=i;
        }
	}

	return indice_melhor;
}

void ImprimeIndividuo(individuo * pop, t_regra * gramatica, int id){

	//sprintf("indice melhor: %d\n", indice_melhor);
	
	t_item_programa programa[TAMANHO_MAX_PROGRAMA];

    //short genotipo_inteiro[TAMANHO_INDIVIDUO];
    //obtem_fenotipo_individuo(&pop[id], genotipo_inteiro);

	int program_ctr = Decodifica(gramatica, pop[id].genotipo, programa);

	ImprimePosfixa(programa);
	//ImprimeInfixa(programa);	
}

void ImprimeMelhor(individuo * pop, t_regra * gramatica){

	int indice_melhor = ObtemMelhor(pop);

	ImprimeIndividuo(pop, gramatica, indice_melhor);
}

void ImprimePopulacao(individuo * pop, t_regra * gramatica){

	int i;

    t_item_programa programa[TAMANHO_MAX_PROGRAMA];
    short genotipo_inteiro[TAMANHO_INDIVIDUO];

    int invalidos=0;

	for(i=0; i<TAMANHO_POPULACAO; i++){

		printf("%d - ", i);

        obtem_fenotipo_individuo(&pop[i], genotipo_inteiro);

        //int program_ctr = Decodifica(gramatica, pop[i].genotipo, programa);

        int program_ctr = Decodifica(gramatica, genotipo_inteiro, programa);

		if(program_ctr==-1){
		    invalidos++;
		}
		else{
            //ImprimeInfixa(programa);
            ImprimePosfixa(programa);
		}

		printf("==> %E\n\n", pop[i].aptidao);
	}

	printf("Inválidos: %d\n", invalidos);
}

float AvaliaBD(t_item_programa *programa, Database *db, int MODELO){
    float erro=0;
    int k;
    for(k=0; k < db->numRegistros; k++){
        erro += AvaliaSemMatrizConfusao(programa, db->registros, k, MODELO);
    }
    return erro;
}

void inicializaMatriz(int **M, int d){
    int i,j;
    for(i=0; i<d;i++)
        for(j=0; j<d;j++)
            M[i][j] = 0;
}

void Avaliacao(individuo * population, t_regra * gramatica, Database * db, int pop_size, long int *nodeEvaluations){
	
	double start = getTime();
	
    int i;

    invalidos = 0;

    t_prog programa;
    //short genotipo_inteiro[DIMENSOES_PROBLEMA];

    for(i=0; i < pop_size; i++){

       //obtem_fenotipo_individuo(&population[i], genotipo_inteiro);

       int program_ctr = Decodifica(gramatica, population[i].genotipo, programa.programa);

       //Programa inválido
       if(program_ctr==-1){
            population[i].valido = 0;
            population[i].aptidao = -999999999;
            invalidos++;
       }
       else{              
       
	       	(*nodeEvaluations) += program_ctr * db->numRegistros;
       
            population[i].valido = 1;

            //PreprocessProgram(programa.programa);
            float erro=0;
            int k;

            //ImprimePosfixa(programa.programa);

            /*arvore* arv = CriaArvore(programa.programa);
            population[i].profundidade = ObtemAlturaArvore(arv);
            population[i].tamanho = ObtemNumeroDeNosArvore(arv);
            DesalocaArvore(arv);
            */
            //inicializaMatriz(population[i].matrizDeConfusao, 3);

            for(k=0; k < db->numRegistros; k++){                
                //erro += Avalia(programa.programa, db->registros, k, 0);
                //erro += Avalia(programa.programa, db->registros, k, 1);
                                
                erro += pow(Avalia(programa.programa, db->registros, k, -1, population[i].matrizDeConfusao), 2);
            }

            //population[i].aptidao = erro*(-1.0)/db->numRegistros;
            population[i].aptidao = -erro;

            /*
            printf("Indivíduo %d:\n", i);
            ImprimeInfixa(&programa);
            printf("\nAptidão[%d] = %f\n", i, population[i].aptidao);
            printf("nodeEvaluations+= %li * %d\n", program_ctr, db->numRegistros);
            */
       }
    }
        
    tempoTotalAvaliacao  += getTime()-start;
}

void ReadIntegerPopFromFile(individuo *pop, char * fileName){

    int i=0,j=0;
    FILE *f = fopen(fileName, "r+");
    char str[16384];

    if(f == NULL){
        printf("Erro ao criar a população inicial: arquivo não encontrado.\n");
        exit(1);
    }

    i=0;
    while(fgets(str,16384,f) != NULL && i < TAMANHO_POPULACAO){
        j = 0;
        char *ch = strtok(str, " ");
        while (ch != NULL && *ch != '\0' && *ch != '\n') {
            pop[i].genotipo[j] = atoi(ch);
            j++;
            ch = strtok(NULL, " ");
        }
        i++;
    }

    for(i=0; i < TAMANHO_POPULACAO; i++){
        pop[i].matrizDeConfusao = malloc(sizeof(int*)*3);
        for(j=0;j<3;j++){
            pop[i].matrizDeConfusao[j] = malloc(sizeof(int)*3);
        }
    }
}

void ReadBinaryPopFromFile(individuo *pop, char * fileName, int count){

    int i=0, j=0;
    FILE *f = fopen(fileName, "r+");
    char str[16384];

    if(f == NULL){
        printf("Erro ao criar a população inicial: arquivo não encontrado.\n");
        exit(1);
    }

    i=0;
    while(fgets(str,16384,f) != NULL && i < count){ //TAMANHO_POPULACAO){

        //puts(str);
        for(j=0; j < TAMANHO_INDIVIDUO && str[j] != '\0'; j++){
            pop[i].genotipo[j] = str[j] == '1' ? 1 : 0;
        }
        i++;
    }

    for(; i < TAMANHO_POPULACAO; i++){
        for(j=0; j< TAMANHO_INDIVIDUO; j++){
            //pop[i].genotipo[j] = rand() % 256;
            pop[i].genotipo[j] = rand() % 2;
        }
    }

    for(i=0; i < TAMANHO_POPULACAO; i++){
        pop[i].matrizDeConfusao = malloc(sizeof(int*)*3);
        for(j=0;j<3;j++){
            pop[i].matrizDeConfusao[j] = malloc(sizeof(int)*3);
        }
    }
}

int NormaInfinitoMatriz(int M[][3], int n){

    int i,j;
    int norma = 0;

    for(i=0;i<n; i++){
        int aux = 0;
        for(j=0;j<n;j++){
            aux += abs(M[i][j]);
        }
        if(aux>norma)
            norma = aux;
    }

    return norma;
}

int Norma1(int M[][3], int n){

    int i,j;
    int norma = 0;

    for(i=0;i<n; i++){
        for(j=0;j<n;j++){
            norma += abs(M[i][j]);
        }
    }
    return norma;
}

void SubtraiMatrizes(int **A, int **B, int C[][3], int d){

    int i,j;
    for(i=0; i<d;i++)
        for(j=0; j<d;j++)
            C[i][j] = A[i][j] - B[i][j];
}

void ImprimeMatriz(int **A, int d){

    int i,j;

    printf("\n\n");

    for(i=0; i<d;i++){
        for(j=0; j<d;j++)
            printf("%d ", A[i][j]);
        printf("\n");
    }

    printf("\n\n");
}

void CalculaDistanciaMedia(individuo *pop, float *normaInfinito, float *norma1){

    int i,j, count=0;
    int M[3][3];
    float mediaNormaInfinito = 0, mediaNorma1 = 0;

    //printf("---------------------------------\n\n\n");

    for(i=0; i < TAMANHO_POPULACAO; i++){

        if(!pop[i].valido)
        {
            //printf("\n");
            continue;
        }

        for(j=i+1; j<TAMANHO_POPULACAO; j++){

            if(!pop[j].valido)
            {
                //printf("   ");
                continue;
            }

            SubtraiMatrizes(pop[i].matrizDeConfusao, pop[j].matrizDeConfusao, M, 3);

            //float norma = NormaInfinitoMatriz(M, 3);
            mediaNormaInfinito += NormaInfinitoMatriz(M, 3);
            mediaNorma1 += Norma1(M, 3);
            //printf("%.2f ", norma);

            //media += norma;
            count++;
        }
        //printf("\n");
    }

    *norma1 = mediaNorma1/(float)count;
    *normaInfinito = mediaNormaInfinito/(float)count;

    //printf("count = %d\n", count);
    //return media/(float)count;
}

void CalculaTamanhoEProfundidadeMedias(individuo *pop, float *tamanhoMedio, float *profundidadeMedia){

    int i, count=0;
    *tamanhoMedio = 0;
    *profundidadeMedia = 0;

    for(i=0; i < TAMANHO_POPULACAO; i++)
    {
        if(pop[i].valido)
        {
            *tamanhoMedio += pop[i].tamanho;
            *profundidadeMedia += pop[i].profundidade;
            count++;
        }
    }

    *tamanhoMedio = *tamanhoMedio/count;
    *profundidadeMedia = *profundidadeMedia/count;
}

void AvaliaMelhor(individuo * ind, t_regra * gramatica, Database * trainData, Database *testData)
{
    t_item_programa programa[TAMANHO_MAX_PROGRAMA];
    short genotipo_inteiro[TAMANHO_INDIVIDUO];

    int invalidos=0;

    obtem_fenotipo_individuo(ind, genotipo_inteiro);
    int program_ctr = Decodifica(gramatica, genotipo_inteiro, programa);

    PreprocessProgram(programa);

    float fitness_train_1 = AvaliaBD(programa, trainData, 0);
    float fitness_train_2 = AvaliaBD(programa, trainData, 1);
    float fitness_test_1  = AvaliaBD(programa, testData, 0);
    float fitness_test_2  = AvaliaBD(programa, testData, 1);

    printf("%f\t%f\t%f\t%f\t%f\t%f", fitness_train_1,
                                   fitness_train_2,
                                   fitness_train_1 + fitness_train_2,
                                   fitness_test_1,
                                   fitness_test_2,
                                   fitness_test_1 + fitness_test_2);

    //ImprimePosfixa(&programa);
    //printf("\t1000\n");
}

void AvaliaMelhorRegressao(individuo * ind, t_regra * gramatica, Database * trainData, Database *testData, long int *nodeEvaluations)
{
	double start = getTime();
		

    t_item_programa programa[TAMANHO_MAX_PROGRAMA];
    //short genotipo_inteiro[TAMANHO_INDIVIDUO];
    
	Database *data = trainData;
	
	if(testData != NULL){
		data = testData;
	}

    int invalidos=0;

    //obtem_fenotipo_individuo(ind, genotipo_inteiro);
    int program_ctr = Decodifica(gramatica, ind->genotipo, programa);
    
    (*nodeEvaluations) += program_ctr * data->numRegistros;
   
    float erro=0;
    int k;

    for(k=0; k < data->numRegistros; k++){
        erro += pow(Avalia(programa, data->registros, k, -1, ind->matrizDeConfusao), 2);
    }

    ind->aptidao = -erro;       
    
    tempoTotalAvaliacao  += getTime()-start;
}

void eg(t_regra *gramatica, Database *dataBase, Database *testData, char * pop_file, int pop_count){

    individuo * pop = (individuo*) malloc(sizeof(individuo)*TAMANHO_POPULACAO);
	individuo * newPop = (individuo*) malloc(sizeof(individuo)*TAMANHO_POPULACAO);

	int geracao = 0;
	long int nodeEvaluations = 0;

    if(pop_file != NULL && strlen(pop_file) > 0){
        //printf("Reading pop from file: %s\n", pop_file);
        ReadBinaryPopFromFile(pop, pop_file, pop_count);
    }
    else{
        CriaPopulacaoInicial(pop);
    }

    CriaPopulacaoInicial(newPop);
    
    Avaliacao(pop, gramatica, dataBase, TAMANHO_POPULACAO, &nodeEvaluations);	

	float distancia = 0;
	float tamanhoMedio = 0, alturaMedia = 0;
	float mediaNorma1 = 0, mediaNormaInfinito = 0;
	
	while(geracao < NUMERO_DE_GERACOES){

	    /*printf("-------------------------------------\n");
	    printf("Geração %d:\n", geracao);
		ImprimeMelhor(pop, gramatica);
		printf("\n");*/

        //CalculaDistanciaMedia(pop, &mediaNormaInfinito, &mediaNorma1);
        //CalculaTamanhoEProfundidadeMedias(pop, &tamanhoMedio, &alturaMedia);

		//printf("training\t%d\t", geracao);
		//ImprimeMelhor(pop, gramatica);

		int indice_melhor = ObtemMelhor(pop);

		//printf("\%f\t%f\t%f\t%f\t%f\t%d\n", pop[indice_melhor].aptidao, mediaNormaInfinito, mediaNorma1,
		//alturaMedia, tamanhoMedio, invalidos);

		CriaNovaPopulacao(pop, newPop);
    	Avaliacao(newPop, gramatica, dataBase, TAMANHO_POPULACAO, &nodeEvaluations);
		SubstituiPopulacao(pop, newPop);

		geracao++;
	}

	//printf("training\t%d\t", geracao);
    //ImprimeMelhor(pop, gramatica);
    //CalculaDistanciaMedia(pop, &mediaNormaInfinito, &mediaNorma1);
    //CalculaTamanhoEProfundidadeMedias(pop, &tamanhoMedio, &alturaMedia);

    int indice_melhor = ObtemMelhor(pop);    
    float trainingFitness, testFitness;
    trainingFitness = pop[indice_melhor].aptidao;
    
    printf("%E\t", trainingFitness);
    
    if(testData != NULL)
    {
    	AvaliaMelhorRegressao(&pop[indice_melhor], gramatica, dataBase, testData, &nodeEvaluations);
	    testFitness = pop[indice_melhor].aptidao;
	}
	else
	{
	    testFitness = trainingFitness;
	}
    
    printf("%E\t", testFitness);

    ImprimeIndividuo(pop, gramatica, indice_melhor);
    //printf("\n");
    
    printf("\t%f\t", ((float)nodeEvaluations) / 1000000000);
    
    printf("%lf\t0.0\t", tempoTotalAvaliacao);

    free(pop);
    free(newPop);
}
