
/*
 Problemas de 1 a 6 mesma coisa do EPIA;
 Problemas de 7 a 11 mesma coisa do EPIA;
 + alterar dimensoes;
 20    5,10
 30    5,15
 40    5,20
 Problema do Colville 19.
 */

/*------------------------------------Importação das Bibliotecas------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <float.h>
#include "LU.h"
#include "funcoes_teste.h"

#define DELEQC_ONLY -1
#define DELEQC_TS 0
#define DELEQC_APM 1
#define TS_ONLY 2
#define APM_ONLY 3

#define BOUND_NONE -1
#define BOUND_TO_BOUND 0
#define BOUND_DIFF_SCALED 1
#define BOUND_DIFF_SCALED_RANDOMLY 2

/*------------------------------------Protótipo das Funções------------------------------------*/
double** Inicializa_Populacao(int Restricoes, int N_Coordenada, int N_Individuo, int prob, double** bounds, int nGs, int nHs, int* sizeOfIndividual);
void Avalia_Populacao(double **populacao, int N_Individuo, int N_Coordenada, int prob, double **bounds, double epsilonEqualityConstraints);
void Diferenciacao(double **populacao, double *novo_id, int id1, int id2, int N_Coordenada, double peso);
void Mutacao(double **populacao, double *novo_id, int idm, int N_Coordenada);
void Cross_Over(double **populacao, double *novo_id, int id);
/*void Print_Populacao(double **populacao, int N_Individuo, int N_Coordenada, int prob);*/
void Inicializa_Parametros(int argc, char **argv, int *n_pop, double *cross_prob, double *peso, int *n_gen, int *metodo, int *problema_numero, int *semente, double* epsilonCorrecao, int* nCoordenadasNova, int* nRestricoesNova, int* constraintHandler, int* boundHandler);
int procuraMelhorFactivel( double **populacao, int n_pop, int coordenadas, int restricoes, double epsilon );
int procuraMelhor( double **populacao, int n_pop, int coordenadas );
void imprimirIndividuo( double **populacao, int coordenadas, int id, int geracao, int restricoes, int nRep, int numberOfValuesInEachIndividual, int nGs, int nHs );
void imprimirIndividuoVetor( double *individuo, int coordenadas );
double Verifica_restricoes(int Restricoes, int N_Coordenada, double *ind);
void corrigirSolucoes(double **populacao, int N_Individuo, int Restricoes, int N_Coordenada);
void corrigirSolucao(double *solucao, int N_Individuo, int Restricoes, int N_Coordenada);
void corrigirSolucaoUsandoProjecao(double *solucao, int N_Individuo, int Restricoes, int N_Coordenada, double *v);
double normaInfinito(double *x, double *y, int n);
int individuoViolaRestricaoNormaInfinito( double **populacao, int coordenadas, int id, int geracao, int restricoes, double epsilon);
void calculateLinearEqualityConstraints( double **populacao, int coordenadas, int id, int restricoes, int nGs, int nHs, double epsilon );
void maxValues( double** population1, double** population2, double* maxV, int nIndividuals, int sizeIndividual );
int procuraMelhorRestricoesTS( double **populacao, int n_pop, int dimension, int idSumConstraints );
int bestIndividualObjectiveFunction( double* individual1, double* individual2, int dimension );
int bestIndividualTS( double* individual1, double* individual2, int dimension, int idSumConstraints );
void reduceVariationKeepingSolutionInFeasibleRegion(double *oldIndividual, double *newIndividual, int dimension, double** bounds, double* diff, double additionalScale);
double** createInitialPopulationSimple(int Restricoes, int N_Coordenada, int N_Individuo, int prob, double** bounds, int nGs, int nHs, int* numberOfValuesInEachIndividual);
void printPopulation( double **populacao, int coordenadas, int id, int geracao, int restricoes, int nReparos, int numberOfValuesInEachIndividual, int nGs, int nHs, int populationSize );
void APMCalculatePenaltyCoefficients( int populationSize, double** population1, double** population2, int dimension, int nLECs, int nGs, int nHs, double* penaltyCoefficients, double* averageObjectiveFunctionValues, int idGConstraints, int idBoundConstraints, int idLinearEqualityConstraints, int sizeOfIndividual );
double APMcalculateFitness( double* individual, int dimension, int nLECs, int nGs, int nHs, double* penaltyCoefficients, double averageObjectiveFunctionValues, int idGConstraints,	int idBoundConstraints,	int idLinearEqualityConstraints, int constraintHandler);
int procuraMelhorAPM( double **populacao, int n_pop, int idFitnessAPM );
int bestIndividualAPM( double* individual1, double* individual2, int idFitnessAPM );

double **Matriz_Coef = NULL;
double **Matriz_E = NULL;
double **Matriz_E_Transposto = NULL;
double **Matriz_M = NULL;
double *b = NULL;
double *base = NULL;
int *p = NULL;

int main( int argc, char** argv ) {

	double **populacao = NULL;
	int prob, N_Individuo, N_Geracoes, Restricoes, N_Coordenada,  N_CoordenadaNova = -1, RestricoesNova = -1;
	int nGs = 0, nHs = 0; //number of constraintts other than the linear equality ones
	double peso; // valor de F no DE
	double cross_prob; //parametro utilizado apenas no DE+APM e DE+Deb pois essa implementacao de DE nao possui recombinacao
	int metodo; //parametro necessario na implementacao em Java pois ha mais de um metodo a ser selecionado
	int semente;
	double melhorF;
	//double epsilon = 0.0001;
	int idMelhorIndividuo;
	int idAntigoMelhorIndividuo;
	double epsilonCorrecao = 0.001;
	int nReparos = 0;
	int numberOfValuesInEachIndividual;
	int constraintHandler = -1;
	int boundHandler = -1;
	int g,i;
	int id1, id2, id3;
	int j;
	int best = -1;
	double* maxConstraints = NULL;
	double* penaltyCoefficients = NULL;
	int idSumViolations = -1;
	int idLinearEqualityConstraints = -1;
	int idBoundConstraints = -1;
	int idGConstraints = -1;
	int idSumBoundViolations = -1;
	int idSumViolationsWithoutLE = -1;
	int idFitnessAPM = -1;
	double averageObjectiveFunctionValues = 0;

	Inicializa_Parametros(argc, argv, &N_Individuo, &cross_prob, &peso, &N_Geracoes, &metodo, &prob, &semente, &epsilonCorrecao, &N_CoordenadaNova, &RestricoesNova, &constraintHandler, &boundHandler);

	//inicializa o gerador de numero aleatorio
	srand( semente );

    double **bounds = NULL;
    double *diff = NULL; //auxiliary vector used for keeping the solutions in the feasible region

	//Problema_Teste(prob, &Restricoes, &N_Coordenada, &melhorF, bounds);
	Problema_Teste(prob, &Restricoes, &N_Coordenada, &melhorF, &bounds, N_CoordenadaNova, RestricoesNova, &nGs, &nHs);

	//printf("constraint handler: %d\n", constraintHandler);

	if (constraintHandler == DELEQC_ONLY || constraintHandler == DELEQC_TS || constraintHandler == DELEQC_APM ) {
		populacao = Inicializa_Populacao(Restricoes, N_Coordenada, N_Individuo, prob, bounds, nGs, nHs, &numberOfValuesInEachIndividual);
	} else if (constraintHandler == TS_ONLY || constraintHandler == APM_ONLY) {
		//printf("-----------------OK---------------------");
		populacao = createInitialPopulationSimple(Restricoes, N_Coordenada, N_Individuo, prob, bounds, nGs, nHs, &numberOfValuesInEachIndividual);
	} else {
		printf("It is not implemented yet.");
		exit(4);
	}

	

	Avalia_Populacao(populacao, N_Individuo, N_Coordenada, prob, bounds, epsilonCorrecao);

	diff = (double*) malloc( N_Coordenada * sizeof( double ) );
	idSumViolations = N_Coordenada + 1 + nGs + nHs + N_Coordenada + 1;
	idLinearEqualityConstraints = N_Coordenada + 1 + nGs + nHs + N_Coordenada + 1 + 1 + 1;
	idBoundConstraints = N_Coordenada + 1 + nGs + nHs;
	idGConstraints = N_Coordenada + 1;
	idSumBoundViolations = N_Coordenada + 1 + nGs + nHs + N_Coordenada;
	idSumViolationsWithoutLE = N_Coordenada+1+nGs+nHs+N_Coordenada+1+1+1+Restricoes;
	idFitnessAPM = N_Coordenada + 1 + nGs + nHs + N_Coordenada + 1 + 1;

	if (constraintHandler == TS_ONLY || constraintHandler == DELEQC_TS || constraintHandler == APM_ONLY || constraintHandler == DELEQC_APM) {

		for(i=0; i<N_Individuo; i++) {
			boundConstraints(populacao[i], N_Coordenada, nGs, nHs, bounds);
			calculateLinearEqualityConstraints(populacao, N_Coordenada, i, Restricoes, nGs, nHs, epsilonCorrecao);
		}

		if (constraintHandler == TS_ONLY || constraintHandler == DELEQC_TS) {
			maxConstraints = (double*) malloc(numberOfValuesInEachIndividual * sizeof(double));
			maxValues( populacao, NULL, maxConstraints, N_Individuo, numberOfValuesInEachIndividual);
			for(i=0; i<N_Individuo; i++) {
				sumViolation(populacao[i], N_Coordenada, nGs, nHs, bounds, Restricoes, maxConstraints);
			}
		} else if (constraintHandler == APM_ONLY || constraintHandler == DELEQC_APM) {
			penaltyCoefficients = (double*) malloc(numberOfValuesInEachIndividual * sizeof(double));
			APMCalculatePenaltyCoefficients(N_Individuo, populacao, NULL, N_Coordenada, Restricoes, nGs, nHs, penaltyCoefficients, &averageObjectiveFunctionValues, idGConstraints, idBoundConstraints, idLinearEqualityConstraints, numberOfValuesInEachIndividual);
			//printf("Initial population: \n");
			for(i=0; i<N_Individuo; i++) {
				sumViolation(populacao[i], N_Coordenada, nGs, nHs, bounds, Restricoes, NULL);
				populacao[i][ idFitnessAPM ] = APMcalculateFitness(populacao[i], N_Coordenada, Restricoes, nGs, nHs, penaltyCoefficients, averageObjectiveFunctionValues, idGConstraints, idBoundConstraints, idLinearEqualityConstraints, constraintHandler);
				//printf("populacao[%d] = %f\n", i, populacao[i][ idFitnessAPM ]);
			}
		}
	}

	//imprimir populacao inicial

	/*int l;
        for(l=0; l<N_Individuo; l++) {
            imprimirIndividuo(populacao, N_Coordenada, l, 0, Restricoes);
	}*/
//        getchar();

		//exit(0);

		if ( constraintHandler == DELEQC_ONLY ) {
			idMelhorIndividuo = procuraMelhor( populacao, N_Individuo, N_Coordenada );
		} else if ( constraintHandler == DELEQC_TS ) {
			idMelhorIndividuo = procuraMelhorRestricoesTS( populacao, N_Individuo, N_Coordenada, idSumViolationsWithoutLE );
		} else if ( constraintHandler == TS_ONLY ) {
			idMelhorIndividuo = procuraMelhorRestricoesTS( populacao, N_Individuo, N_Coordenada, idSumViolations );
		} else if ( constraintHandler == APM_ONLY || constraintHandler == DELEQC_APM ) {
			idMelhorIndividuo = procuraMelhorAPM( populacao, N_Individuo, idFitnessAPM );
		} else {
			printf("It was not implemented yet.");
			exit(1);
		}

		//printf("Best ID: %d\n", idMelhorIndividuo);

		//printPopulation(populacao, N_Coordenada, idMelhorIndividuo, 0, Restricoes, nReparos, numberOfValuesInEachIndividual, nGs, nHs, N_Individuo);

		imprimirIndividuo(populacao, N_Coordenada, idMelhorIndividuo, 0, Restricoes, nReparos, numberOfValuesInEachIndividual, nGs, nHs);

        double **novos_ind = (double**) malloc(N_Individuo * sizeof(double*));
        double *melhor_ind = (double*) malloc((numberOfValuesInEachIndividual) * sizeof(double));

        for(i=0; i< N_Individuo; i++) {
            novos_ind[i] = (double*) malloc((numberOfValuesInEachIndividual) * sizeof(double));
            for(j=0; j<numberOfValuesInEachIndividual; j++) {
                novos_ind[i][j] = 0;
            }
        }
        int geracoes = 1;

        //g eh usado para indicar o criterio de parada com base no numero de calculos de funcao objetivo
        //geracoes indica o numero efetivo de geracoes que foram percorridas

        for(g = 1; g < N_Geracoes; g++){ // && populacao[idMelhorIndividuo][N_Coordenada] - melhorF > epsilon ; g++){
        //for(g = 1; g < N_Geracoes && populacao[idMelhorIndividuo][N_Coordenada] - melhorF > epsilon ; g++){
        //printf("Entrou no for g\n");

        //printf("N_Individuo: %d\n", N_Individuo);

            for (i = 0; i < N_Individuo; ++i) {
                //printf("Entrou no for i\n");

                id1 = rand() % N_Individuo;
                while(id1 == i){
                    id1 = rand() % N_Individuo;
                }
                id2 = rand() % N_Individuo;
                while((id2 == i) || (id2 == id1)){
                    id2 = rand() % N_Individuo;
                }
                id3 = rand() % N_Individuo;
                while((id3 == i) || (id3 == id1) || (id3 == id2)){
                     id3 = rand() % N_Individuo;
                }

                //int jRand = rand()%N_Coordenada;
                //imprimirIndividuoVetor(novo_id, N_Coordenada);

                for(j = 0; j < N_Coordenada; j++){
                    //printf("Entrou no for j\n");

                    //mutacao
                    switch(metodo){
                        case 1: //DE/rand/1/bin
                            novos_ind[i][j] = populacao[id1][j] + peso*(populacao[id2][j] - populacao[id3][j]);
                            break;
                        case 2: //DE/best/1/bin
                            novos_ind[i][j] = populacao[idMelhorIndividuo][j] + peso*(populacao[id1][j] - populacao[id2][j]);
                            break;
                        case 3: //DE/target-to-rand/1/bin
                            novos_ind[i][j] = populacao[i][j] + peso*(populacao[id1][j] - populacao[i][j]) + peso*(populacao[id2][j] - populacao[id3][j]);
                            break;
                        case 4: //DE/target-to-best/1/bin
                            novos_ind[i][j] = populacao[i][j] + peso*(populacao[idMelhorIndividuo][j] - populacao[i][j]) + peso*(populacao[id2][j] - populacao[id3][j]);
                            break;
                        default:
                            printf("O metodo selecionado nao estah entre as possibilidades disponiveis (1-4).\n");
                            return EXIT_FAILURE; // termina a execucao com erro
                    }

                    //crossover via combinacao convexa usando o CR como parametro que define a proximidade com o vetor gerado
                    novos_ind[i][j] = populacao[i][j]*(1-cross_prob) + cross_prob*novos_ind[i][j];

                }

                ///fix bound constraints violations

                if ( boundHandler == BOUND_TO_BOUND ) {
					if ( constraintHandler == TS_ONLY || constraintHandler == APM_ONLY ) {
						//if Deb's TS or APM, then the bounds can be fixed
						//we assume the bound when the current value is larger or smaller than it
						for(j = 0; j < N_Coordenada; j++) {
							if ( novos_ind[i][j] < bounds[j][0] ) {
								novos_ind[i][j] = bounds[j][0];
							} else if ( novos_ind[i][j] > bounds[j][1] ) {
								novos_ind[i][j] = bounds[j][1];
							}
						}
					} else {
						printf("DELEqC methods cannot use this bound handling strategy.");
						exit(6);
					}
				} else if ( boundHandler == BOUND_DIFF_SCALED || boundHandler == BOUND_DIFF_SCALED_RANDOMLY ) {

					boundConstraints(novos_ind[i], N_Coordenada, nGs, nHs, bounds);

					if ( boundHandler == BOUND_DIFF_SCALED ) {
						if ( novos_ind[ i ][ idSumBoundViolations ] > 0 && populacao[ i ][ idSumBoundViolations ] == 0 ) {
							//if DELEqC+TS is used, and (i) the new individual violates the bound constraints and (ii) the old individual is feasible with respect to the bound constraints, then fix the new individual
							reduceVariationKeepingSolutionInFeasibleRegion( populacao[i], novos_ind[i], N_Coordenada, bounds, diff, 1 );
						}
					} else {
						if ( novos_ind[ i ][ idSumBoundViolations ] > 0 && populacao[ i ][ idSumBoundViolations ] == 0 ) {
							//if DELEqC+TS is used, and (i) the new individual violates the bound constraints and (ii) the old individual is feasible with respect to the bound constraints, then fix the new individual
							reduceVariationKeepingSolutionInFeasibleRegion( populacao[i], novos_ind[i], N_Coordenada, bounds, diff, ( (rand()%1000000) / 1000000.f ) );
						}
					}

				} else if ( boundHandler != BOUND_NONE ) {
					printf("This bound handling is not implemented.");
					exit(7);
				}

                Avalia(novos_ind[i], N_Coordenada, prob, Matriz_Coef, bounds, epsilonCorrecao);
                //CEC2018
                //evaluate the bound and linear violations: APM and DEb's TS
                if (constraintHandler != DELEQC_ONLY) {
					boundConstraints(novos_ind[i], N_Coordenada, nGs, nHs, bounds);
					calculateLinearEqualityConstraints(novos_ind, N_Coordenada, i, Restricoes, nGs, nHs, epsilonCorrecao);
				}

            }

            //atualiza o bkp do melhor individuo
            idAntigoMelhorIndividuo = idMelhorIndividuo;
            //for (j = 0; j <= N_Coordenada; ++j) {
            for (j = 0; j < numberOfValuesInEachIndividual; ++j) {
                melhor_ind[j] = populacao[idAntigoMelhorIndividuo][j];
            }

			//substitution of the population
			//some cases are introduced in the paper submitted to CEC2018
			if ( constraintHandler == DELEQC_ONLY ) {
				for (i = 0; i < N_Individuo; ++i) {
					if(novos_ind[i][N_Coordenada] < populacao[i][N_Coordenada]){

						// a dimensao N_Coordenada carrega a funcao de avaliacao
						//for (j = 0; j <= N_Coordenada; ++j) {
						for (j = 0; j < numberOfValuesInEachIndividual; ++j) {
								populacao[i][j] = novos_ind[i][j];
						}

							//imprimirIndividuo(populacao, N_Coordenada, i, g);
					}
				}
			} else if ( constraintHandler == DELEQC_TS || constraintHandler == TS_ONLY ) {
				//printf("\n\nMAX\n\n");
				maxValues( populacao, novos_ind, maxConstraints, N_Individuo, numberOfValuesInEachIndividual);
				for(i=0; i<N_Individuo; i++) {
					sumViolation(populacao[i], N_Coordenada, nGs, nHs, bounds, Restricoes, maxConstraints);
				}
				for(i=0; i<N_Individuo; i++) {
					sumViolation(novos_ind[i], N_Coordenada, nGs, nHs, bounds, Restricoes, maxConstraints);
				}
				//replacement based on Deb's Tournament Selection
				if ( constraintHandler == DELEQC_TS ) {
					for (i = 0; i < N_Individuo; ++i) {
						///replacement
						best = bestIndividualTS(novos_ind[i], populacao[i], N_Coordenada, idSumViolationsWithoutLE);
						if ( best == 0 ) {
							for (j = 0; j < numberOfValuesInEachIndividual; ++j) {
								populacao[i][j] = novos_ind[i][j];
							}
						}
						/*if (novos_ind[i][idSumViolations] < populacao[i][idSumViolations]) {
							//replace population[i] by new_ind[i] when the violation of the last is smaller
							for (j = 0; j < numberOfValuesInEachIndividual; ++j) {
									populacao[i][j] = novos_ind[i][j];
							}
						} else if (novos_ind[i][idSumViolations] == populacao[i][idSumViolations] && novos_ind[i][N_Coordenada] < populacao[i][N_Coordenada]) {
							//replace population[i] by new_ind[i] when the violations are equal and the object function of the last one is smaller (minimization problems)
							for (j = 0; j < numberOfValuesInEachIndividual; ++j) {
									populacao[i][j] = novos_ind[i][j];
							}
						}*/
						//otherwise, the current individual 'i' is better than the new one
					}
				} else if (constraintHandler == TS_ONLY) {
					for (i = 0; i < N_Individuo; ++i) {
						///replacement
						best = bestIndividualTS(novos_ind[i], populacao[i], N_Coordenada, idSumViolations);
						if ( best == 0 ) {
							for (j = 0; j < numberOfValuesInEachIndividual; ++j) {
								populacao[i][j] = novos_ind[i][j];
							}
						}
					}
				} else {
					printf("It is not implemented yet.");
					exit(5);
				}
			} else if ( constraintHandler == APM_ONLY || constraintHandler == DELEQC_APM ) {

				APMCalculatePenaltyCoefficients(N_Individuo, populacao, novos_ind, N_Coordenada, Restricoes, nGs, nHs, penaltyCoefficients, &averageObjectiveFunctionValues, idGConstraints, idBoundConstraints, idLinearEqualityConstraints, numberOfValuesInEachIndividual);
				for(i=0; i<N_Individuo; i++) {
					sumViolation(populacao[i], N_Coordenada, nGs, nHs, bounds, Restricoes, NULL);
					populacao[i][ idFitnessAPM ] = APMcalculateFitness(populacao[i], N_Coordenada, Restricoes, nGs, nHs, penaltyCoefficients, averageObjectiveFunctionValues, idGConstraints, idBoundConstraints, idLinearEqualityConstraints, constraintHandler);
					//printf("populacao[%d] = %f\n", i, populacao[i][ idFitnessAPM ]);
				}
				for(i=0; i<N_Individuo; i++) {
					sumViolation(novos_ind[i], N_Coordenada, nGs, nHs, bounds, Restricoes, NULL);
					novos_ind[i][ idFitnessAPM ] = APMcalculateFitness(novos_ind[i], N_Coordenada, Restricoes, nGs, nHs, penaltyCoefficients, averageObjectiveFunctionValues, idGConstraints, idBoundConstraints, idLinearEqualityConstraints, constraintHandler);
					//printf("novos_ind[%d] = %f\n", i, novos_ind[i][ idFitnessAPM ]);
				}
				
				//replacement
				for(i=0; i<N_Individuo; i++) {
					if(novos_ind[i][idFitnessAPM] < populacao[i][idFitnessAPM]){

						for (j = 0; j < numberOfValuesInEachIndividual; ++j) {
								populacao[i][j] = novos_ind[i][j];
						}

					}
				}

			} else {
				printf("It is not implemented yet.");
				exit(9);
			}

            //printf("Menor Diferenca: %f\n", moduloMenorDiferenca);

			//CEC2018: if DELEqC is used then it is required to fix the numerical error (DELEqC-II)
			if ( constraintHandler == DELEQC_ONLY || constraintHandler == DELEQC_TS || constraintHandler == DELEQC_APM ) {

				if ( constraintHandler == DELEQC_ONLY ) {
					//acha o melhor individuo
					idMelhorIndividuo = procuraMelhor( populacao, N_Individuo, N_Coordenada );

				} else if ( constraintHandler == DELEQC_TS ) {

					idMelhorIndividuo = procuraMelhorRestricoesTS( populacao, N_Individuo, N_Coordenada, idSumViolationsWithoutLE );

				} else if ( constraintHandler == DELEQC_APM ) {

					idMelhorIndividuo = procuraMelhorAPM( populacao, N_Individuo, idFitnessAPM );

				} else {

					printf("Not implemented yet.");
					exit(1);

				}

				//printf("\n\nidMelhor: %d\n\n", idMelhorIndividuo);

				if ( individuoViolaRestricaoNormaInfinito( populacao, N_Coordenada, idMelhorIndividuo, g, Restricoes, epsilonCorrecao ) == 0 ) {

					//atualiza g pois tem que avaliar toda a populacao
					g++;
					//verifica se ha orcamento disponivel
					if ( g < N_Geracoes ) {
						//corrige todas as solucoes
						for (i = 0; i < N_Individuo; ++i) {
							corrigirSolucao(populacao[i], N_Individuo, Restricoes, N_Coordenada);
							Avalia(populacao[i], N_Coordenada, prob, Matriz_Coef, bounds, epsilonCorrecao);
							//CEC2018
							//evaluate the bound and linear violations: APM and DEb's TS
							if (constraintHandler != DELEQC_ONLY) {
								boundConstraints(populacao[i], N_Coordenada, nGs, nHs, bounds);
								calculateLinearEqualityConstraints(populacao, N_Coordenada, i, Restricoes, nGs, nHs, epsilonCorrecao);
							}
						}
						nReparos += N_Individuo;

						//acha o melhor individuo
						//idMelhorIndividuo = procuraMelhor( populacao, N_Individuo, N_Coordenada );

						//acha o melhor individuo
						if ( constraintHandler == DELEQC_ONLY ) {
							idMelhorIndividuo = procuraMelhor( populacao, N_Individuo, N_Coordenada );
							best = bestIndividualObjectiveFunction( melhor_ind, populacao[ idMelhorIndividuo ], N_Coordenada );
						} else if ( constraintHandler == DELEQC_TS ) {
							for(i=0; i<N_Individuo; i++) {
								sumViolation(populacao[i], N_Coordenada, nGs, nHs, bounds, Restricoes, maxConstraints);
							}
							idMelhorIndividuo = procuraMelhorRestricoesTS( populacao, N_Individuo, N_Coordenada, idSumViolationsWithoutLE );
							best = bestIndividualTS( melhor_ind, populacao[ idMelhorIndividuo ], N_Coordenada, idSumViolationsWithoutLE );
						} else if ( constraintHandler == DELEQC_APM ) {
							for(i=0; i<N_Individuo; i++) {
								sumViolation(populacao[i], N_Coordenada, nGs, nHs, bounds, Restricoes, NULL);
								populacao[i][ idFitnessAPM ] = APMcalculateFitness(populacao[i], N_Coordenada, Restricoes, nGs, nHs, penaltyCoefficients, averageObjectiveFunctionValues, idGConstraints, idBoundConstraints, idLinearEqualityConstraints, constraintHandler);
							}
							idMelhorIndividuo = procuraMelhorAPM( populacao, N_Individuo, idFitnessAPM );
							best = bestIndividualAPM( melhor_ind, populacao[ idMelhorIndividuo ], idFitnessAPM );
						} else {
							printf("It was not implemented yet.");
							exit(3);
						}

						// se o melhor individuo piorar
						//if ( melhor_ind[ N_Coordenada ] < populacao[ idMelhorIndividuo ][ N_Coordenada ] ) {
						if ( best == 0 ) {
							/// the new best individual is worse than the previous one ==>
							/// the previous best individual is put in the new population
							/// thus, the quality of the best solution never decreases
							//volta com o antigo melhor individuo para a populacao na mesma posicao que ele estava
							//isso garante que nao ha piora do melhor individuo ao londo do processo evolutivo
							idMelhorIndividuo = idAntigoMelhorIndividuo;
							//for (j = 0; j <= N_Coordenada; ++j) {
							for (j = 0; j < numberOfValuesInEachIndividual; ++j) {
								populacao[idMelhorIndividuo][j] = melhor_ind[j];
							}
						}
						imprimirIndividuo(populacao, N_Coordenada, idMelhorIndividuo, geracoes, Restricoes, nReparos, numberOfValuesInEachIndividual, nGs, nHs);
					}

				} else {
					//caso contrario nada deve ser feito e o melhor individuo deve ser impresso
					imprimirIndividuo(populacao, N_Coordenada, idMelhorIndividuo, geracoes, Restricoes, nReparos, numberOfValuesInEachIndividual, nGs, nHs);
				}

			} else if ( constraintHandler == TS_ONLY ) {
				idMelhorIndividuo = procuraMelhorRestricoesTS( populacao, N_Individuo, N_Coordenada, idSumViolations );
				imprimirIndividuo(populacao, N_Coordenada, idMelhorIndividuo, geracoes, Restricoes, nReparos, numberOfValuesInEachIndividual, nGs, nHs);
			}  else if ( constraintHandler == APM_ONLY ) {
				idMelhorIndividuo = procuraMelhorAPM( populacao, N_Individuo, idFitnessAPM );
				imprimirIndividuo(populacao, N_Coordenada, idMelhorIndividuo, geracoes, Restricoes, nReparos, numberOfValuesInEachIndividual, nGs, nHs);
			} else {
				printf("It was not implemented yet.");
				exit(10);
			}

			printf("populacao[ best ][ %d ] = %f\n", idMelhorIndividuo, populacao[idMelhorIndividuo][ idFitnessAPM ] );

			//printPopulation(populacao, N_Coordenada, idMelhorIndividuo, geracoes, Restricoes, nReparos, numberOfValuesInEachIndividual, nGs, nHs, N_Individuo);

            geracoes++;

            //-- EXIBE O VALOR DA VIOLACAO DAS RESTRICOES --------------------
            //if (prob == 7 || prob == 8 || prob == 9 || prob == 10 || prob == 11){
            //    double rest;
            //    rest = Verifica_restricoes(Restricoes, N_Coordenada,  populacao[idMelhorIndividuo]);
            //    printf(" %f ", rest);
            //}
        }

        for(i=0; i< N_Individuo; i++) {
            free( novos_ind[i] );
        }
        free(novos_ind);

    //desnecessario
	//Print_Populacao(populacao, N_Individuo, N_Coordenada, prob);

	for (i = 0; i < N_Individuo; ++i) {
		free(populacao[i]);
	}

	free(populacao);

	//libera memoria das matrizes

	if ( Matriz_E != NULL ) {
		for (i = 0; i < Restricoes; i++) {
			free(Matriz_E[i]);
		}
		free(Matriz_E);
	}

	if ( Matriz_M != NULL ) {
		for (i = 0; i < Restricoes; i++) {
			free(Matriz_M[i]);
		}
		free(Matriz_M);
	}

	if ( Matriz_E_Transposto != NULL ) {
		for (i = 0; i < N_Coordenada; i++) {
			free(Matriz_E_Transposto[i]);
		}
		free(Matriz_E_Transposto);
	}

    if (Matriz_Coef != NULL) {
		for (i = 0; i < N_Coordenada; i++) {
			free(Matriz_Coef[i]);
		}
		free(Matriz_Coef);
	}

    if ( base != NULL ) {
		free(base);
	}

    if ( p != NULL ) {
		free(p);
	}

    for (i = 0; i < N_Coordenada; i++) {
        free(bounds[i]);
    }

    free(bounds);

    if ( b != NULL ) {
		free(b);
	}
    free(melhor_ind);
    free(diff);

    ///free variables used by each constraint handling
    if (constraintHandler == TS_ONLY || constraintHandler == DELEQC_TS) {
		free( maxConstraints );
	} else if (constraintHandler == APM_ONLY || constraintHandler == DELEQC_APM) {
		free(penaltyCoefficients);
	}

    //desnecessario
	//printf(" Problema %d Resolvido com Sucesso!\n\n\n", prob);

	return EXIT_SUCCESS;

}

// VERIFICA RESTRICOES DOS PROBLEMAS 7 A 11 ------------------
double Verifica_restricoes(int Restricoes, int N_Coordenada, double *ind){

     double EPS = 0.0001;
     double rest = 0.0;
     double v = 0.0;

     v = fabs( -3*ind[1] - ind[2] + 2*ind[5] - 6*ind[6] - 4*ind[8] - 2*ind[9] - 3);
     if ( v > EPS ){
         rest += v;
     }
     v = fabs(-ind[0] - 3*ind[1] - ind[2] - 5*ind[6] - ind[7] - 7*ind[8] - 2*ind[9] );
     if ( v > EPS ){
         rest += v;
     }
     v = fabs(ind[2] + ind[5] + 3*ind[6] - 2*ind[8] + 2*ind[9] - 9);
     if ( v > EPS ){
         rest += v;
     }
     v = fabs(2*ind[0] + 6*ind[1] + 2*ind[2] + 2*ind[3] + 4*ind[6] + 6*ind[7] + 16*ind[8] + 4*ind[9] + 16);
     if ( v > EPS ){
         rest += v;
     }
     v = fabs( -ind[0] - 6*ind[1] - ind[2] - 2*ind[3] - 2*ind[4] + 3*ind[5] -  6*ind[6] -  5*ind[7] - 13*ind[8] - 4*ind[9] - 30);
     if ( v > EPS ){
         rest += v;
     }

     return rest;
}


/*
double** Inicializa_Populacao(int Restricoes, int N_Coordenada, int N_Individuo, int prob, double* bounds){

    double **populacao;
    int i, j, k ,m;
    int p[Restricoes];
    double b[Restricoes], z[Restricoes], vetor_v[N_Coordenada], base[N_Coordenada], solucao[N_Coordenada], d[N_Coordenada];
    double Matriz_E[Restricoes][N_Coordenada], Matriz_E_Transposto[N_Coordenada][Restricoes], **Matriz_M;
    double *resFx, *vetor_u;
    FILE *arq;

    switch(prob){
        case 1:
            arq = fopen("Entrada1.txt", "r");
            if (arq == NULL){
            printf("Problemas na LEITURA do arquivo\n");
            }
            break;
        case 2:
            arq = fopen("Entrada2.txt", "r");
            if (arq == NULL){
            printf("Problemas na LEITURA do arquivo\n");
            }
            break;
        case 3:
            arq = fopen("Entrada3.txt", "r");
            if (arq == NULL){
            printf("Problemas na LEITURA do arquivo\n");
            }
            break;
        case 4:
            arq = fopen("Entrada4.txt", "r");
            if (arq == NULL){
            printf("Problemas na LEITURA do arquivo\n");
            }
            break;
        case 5:
            arq = fopen("Entrada5.txt", "r");
            if (arq == NULL){
            printf("Problemas na LEITURA do arquivo\n");
            }
            break;
        case 6:
            arq = fopen("Entrada6.txt", "r");
            if (arq == NULL){
            printf("Problemas na LEITURA do arquivo\n");
            }
            break;
        case 7:
            arq = fopen("Entrada7.txt", "r");
            if (arq == NULL){
            printf("Problemas na LEITURA do arquivo\n");
            }
            break;
        case 8:
            arq = fopen("Entrada8.txt", "r");
            if (arq == NULL){
            printf("Problemas na LEITURA do arquivo\n");
            }
            break;
        case 9:
            arq = fopen("Entrada9.txt", "r");
            if (arq == NULL){
            printf("Problemas na LEITURA do arquivo\n");
            }
            break;
        case 10:
            arq = fopen("Entrada10.txt", "r");
            if (arq == NULL){
            printf("Problemas na LEITURA do arquivo\n");
            }
            break;
        case 11:
            arq = fopen("Entrada11.txt", "r");
            if (arq == NULL){
            printf("Problemas na LEITURA do arquivo\n");
            }
            break;
        case 12:
            arq = fopen("Entrada12.txt", "r");
            if (arq == NULL){
            printf("Problemas na LEITURA do arquivo\n");
            }
            break;
        default:
            printf("Informe o numero do problema correto! ");
    }

    Matriz_M = (double**) malloc (Restricoes*sizeof(double*));
    for(i=0; i<Restricoes; i++){
        Matriz_M[i] = (double*) malloc (Restricoes*sizeof(double));
    }

    for(i = 0; i < Restricoes; i++){
        for(j = 0; j < N_Coordenada; j++){
            fscanf(arq,"%lf",&Matriz_E[i][j]);
            //printf(" %f ", Matriz_E[i][j]);
            Matriz_E_Transposto[j][i] = Matriz_E[i][j];
        }
        printf("\n");
    }
    for(i = 0; i < Restricoes; i++){
       fscanf(arq,"%lf", &b[i]);
    }

    fclose(arq);

    for(i = 0; i < Restricoes; i++){
        for(j = 0; j < Restricoes; j++){
            Matriz_M[i][j] = 0.0;
            for(k = 0; k < N_Coordenada; k++){
                Matriz_M[i][j]=  Matriz_M[i][j] + Matriz_E[i][k]*Matriz_E_Transposto[k][j];
            }
        }
    }

    for(i=0; i<Restricoes; i++){
        p[i] = i;
    }

    lu(Matriz_M,&p[0],Restricoes);	//Encontra as matrizes 'L' e 'U'
    resFx = lu_solve(Matriz_M,p,b,Restricoes);	//Encontra o resultado do sistema Mx=b

  	populacao = (double**) malloc(N_Individuo *sizeof(double*));

	for(i = 0; i < N_Individuo; i++){
          populacao[i]= (double*)  malloc((N_Coordenada+1) *sizeof(double));
    }

    for(j = 0; j < N_Coordenada; j++){
        base[j] = 0;
        for(m = 0; m < Restricoes; m++){
            base[j] = base[j]+ Matriz_E_Transposto[j][m]*resFx[m];
        }
    }

    k = 0;
    //srand(time(NULL)); <-- nao deve-se iniciar o gerador novamente na mesma execucao independente
    while(k < N_Individuo){
        for(j = 0; j < N_Coordenada; j++){
           //d[j] =  rand()%10;
           //d[j] =  ( (rand()%1000000) / 1000000.f ) * 2000 - 1000;  //assumindo bounds [-1000; 1000]
           d[j] =  ( (rand()%1000000) / 1000000.f ) * ( bounds[1]-bounds[0] ) + bounds[0];
        }
        for(j = 0; j < Restricoes; j++){
            z[j] = 0.0;
            for(m = 0; m < N_Coordenada; m++){
                z[j]=  z[j] + Matriz_E[j][m]*d[m];
            }
        }

        vetor_u = lu_solve(Matriz_M,p,z,Restricoes);

        for(j = 0; j < N_Coordenada; j++){
            vetor_v[j] = 0;
            for(m = 0; m < Restricoes; m++){
                vetor_v[j]  = vetor_v[j]  + Matriz_E_Transposto[j][m]*vetor_u[m];
            }
        }
        if(k < (N_Individuo/2)){
            for(i = 0; i < N_Coordenada; i++){
                solucao[i] = base[i] + d[i] - vetor_v[i];
            }
        }
        else{
            for(i = 0; i < N_Coordenada; i++){
                solucao[i] = base[i] - d[i] + vetor_v[i];
            }
        }

        for (i = 0; i < N_Coordenada; i++){
            populacao[k][i] = solucao[i];
        }

        k = k + 1;

	}

	return populacao;
}
*/

double** Inicializa_Populacao(int Restricoes, int N_Coordenada, int N_Individuo, int prob, double** bounds, int nGs, int nHs, int* sizeOfIndividual){

    double **populacao;
    int i, j, k ,m;
    //int p[Restricoes];
    //double b[Restricoes], z[Restricoes], vetor_v[N_Coordenada], base[N_Coordenada], solucao[N_Coordenada], d[N_Coordenada];
    //double Matriz_E[Restricoes][N_Coordenada], Matriz_E_Transposto[N_Coordenada][Restricoes], **Matriz_M;//, **Matriz_Z;
    //int *p;
    double /* *b ,*/ *z, *vetor_v, /* *base,*/ *solucao, *d;
    double /* **Matriz_E ,*/ /* **Matriz_E_Transposto,*/ /* **Matriz_M ,*/ **Matriz_Z;
    double *resFx, *vetor_u;
    //double *solucao_x_til;
    FILE *arq = NULL;
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
        case 19: // PROBLEMA 119 DO COLVILLE
            arq = fopen("input_data/Entrada19.txt", "r");
            if (arq == NULL){
            printf("Problemas na LEITURA do arquivo\n");
            }
            break;
        case 20:
        case 101:
        case 102:
        case 103:
        case 104:
        case 105:
        case 106:
			///problem with no file to describe the linear equality constraints
			break;
        default:
            printf("Informe o numero do problema correto! ");
            exit(2);
    }

    // ALOCANDO MEMORIA -------------------------------------------------------
    p = (int*) malloc( Restricoes * sizeof(int) );
    b = (double*) malloc( Restricoes * sizeof(double) );
    z = (double*) malloc( Restricoes * sizeof(double) );
    vetor_v = (double*) malloc( N_Coordenada * sizeof(double) );
    base = (double*) malloc( N_Coordenada * sizeof(double) );
    solucao = (double*) malloc( N_Coordenada * sizeof(double) );
    d = (double*) malloc( N_Coordenada * sizeof(double) );

    Matriz_M = (double**) malloc (Restricoes*sizeof(double*));
    for(i=0; i<Restricoes; i++){
        Matriz_M[i] = (double*) malloc (Restricoes*sizeof(double));
    }

    Matriz_E = (double**) malloc( Restricoes * sizeof(double*) );
    for(i=0; i<Restricoes; i++) {
        Matriz_E[i] = (double*) malloc( N_Coordenada * sizeof(double) );
    }

    Matriz_E_Transposto = (double**) malloc( N_Coordenada * sizeof(double*) );
    for(i=0; i<N_Coordenada; i++) {
        Matriz_E_Transposto[i] = (double*) malloc( Restricoes * sizeof(double) );
    }

    // MATRIZ 'Z' PARA A BASE DO NUCLEO DE 'E'
    Matriz_Z = (double**) malloc( N_Coordenada * sizeof(double*) );
    for(i=0; i<N_Coordenada; i++) {
        Matriz_Z[i] = (double*) malloc( (N_Coordenada-Restricoes) * sizeof(double) );
    }

    // MATRIZ DE COEFICIENTES DO PROBLEMA 19
    Matriz_Coef = (double**) malloc (N_Coordenada*sizeof(double*));
    for(i=0; i<N_Coordenada; i++){
        Matriz_Coef[i] = (double*) malloc (N_Coordenada*sizeof(double));
    }
    //-------------------------------------------------------------------------


    // ESTES SAO OS PROBLEMAS QUE PODEM SER ESCALONADOS -----------------------
    if (prob == 7 || prob == 8 || prob == 9 || prob == 10 || prob == 11){
        restOriginal = 5;
        coordOriginal = 10;

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
                //printf(" %.3f ", Matriz_EOr[i][j]);
            }
            //printf("\n");
        }
        //printf("\n");
        for(i = 0; i < restOriginal; i++){
           fscanf(arq,"%lf", &bOr[i]);
           //printf(" %.3f ", bOr[i]);
        }
        //printf("\n\n");

        for(i = 0; i < coordOriginal; i++){
            for(j = 0; j < (coordOriginal-restOriginal); j++){
                fscanf(arq,"%lf",&Matriz_ZOr[i][j]);
                //printf(" %f ", Matriz_ZOr[i][j]);
            }
            //printf("\n");
        }
        //printf("\n");

        // SE A DIMENSAO FOR A ORIGINAL (N = 10 E M = 5), PREENCHER OS VALORES
        if (N_Coordenada == coordOriginal && Restricoes == restOriginal){
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

            int pM_rest = Restricoes / 5;

            for(i = 0; i < Restricoes; i++){
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

/*
            for(i = 0; i < Restricoes; i++){
                for(j = 0; j < N_Coordenada; j++){
                    printf( " %.3f ", Matriz_E[i][j]);
                }
                printf("\n");
            }
*/

            //REPLICAR OS VALORES DO VETOR 'bOr' PARA O VETOR 'b' -------------
            cont_i = 0;
            for (cont_p = 0 ; cont_p < pM_rest; cont_p++){
                cont_i = cont_p * restOriginal;
                for(i = 0; i < restOriginal; i++, cont_i++){
                    b[cont_i] = bOr[i];
                    //printf(" %.3f ", b[cont_i]);
                }
            }
            //printf("\n\n");

            //REPLICAR OS VALORES DA MATRIZ 'ZOr' PARA 'Z' --------------------
            cont_i = 0;
            cont_j = 0;
            int pM_coord = Restricoes / restOriginal;

            for (i = 0 ; i < N_Coordenada ; i++){
                for (j = 0 ; j < (N_Coordenada-Restricoes) ; j++){
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
                for( j = Restricoes; j < (N_Coordenada-Restricoes); j++){
                    if ( (i-Restricoes) == j ){
                        Matriz_Z[i][j]= 1.0;
                    } else {
                        Matriz_Z[i][j]= 0.0;
                    }
                }
            }
/*
            for(i = 0; i < N_Coordenada; i++){
                for(j = 0; j < (N_Coordenada-Restricoes); j++){
                    printf( " %.f ", Matriz_Z[i][j]);
                }
                printf("\n");
            }
*/
        }

        //getchar();

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

        for(i = 0; i < Restricoes; i++){
            for(j = 0; j < N_Coordenada; j++){
                fscanf(arq,"%lf",&Matriz_E[i][j]);
                //printf(" %f ", Matriz_E[i][j]);
                Matriz_E_Transposto[j][i] = Matriz_E[i][j];
            }
            //printf("\n");
        }
        for(i = 0; i < Restricoes; i++){
           fscanf(arq,"%lf", &b[i]);
           //printf(" %f ", b[i]);
        }
        //printf("\n");

        for(i = 0; i < N_Coordenada; i++){
            for(j = 0; j < N_Coordenada; j++){
                fscanf(arq,"%lf",&Matriz_Coef[i][j]);
                //printf(" %.2f ", Matriz_Coef[i][j]);
            }
            //printf("\n");
        }
        //getchar();

        for(i = 0; i < N_Coordenada; i++){
            for(j = 0; j < (N_Coordenada-Restricoes); j++){
                fscanf(arq,"%lf",&Matriz_Z[i][j]);
                //printf(" %f ", Matriz_Z[i][j]);
            }
            //printf("\n");
        }

    // DEMAIS PROBLEMAS -------------------------------------------------------
    } else {

		///Problems with several types of constraints
		if ( prob > 100 || prob == 20 ) {

            linearConstraintsData(prob, Matriz_E, Matriz_E_Transposto, b);
            //printf("Data loaded.\n");

		///the other problems
		} else {

			for(i = 0; i < Restricoes; i++){
				for(j = 0; j < N_Coordenada; j++){
					fscanf(arq,"%lf",&Matriz_E[i][j]);
					//printf(" %f ", Matriz_E[i][j]);
					Matriz_E_Transposto[j][i] = Matriz_E[i][j];
				}
				//printf("\n");
			}
			for(i = 0; i < Restricoes; i++){
			   fscanf(arq,"%lf", &b[i]);
			   //printf(" %f ", b[i]);
			}
			//printf("\n");

			for(i = 0; i < N_Coordenada; i++){
				for(j = 0; j < (N_Coordenada-Restricoes); j++){
					fscanf(arq,"%lf",&Matriz_Z[i][j]);
					//printf(" %f ", Matriz_Z[i][j]);
				}
				//printf("\n");
			}
		}

    }

	if (arq != NULL) {
		fclose(arq);
	}

    /* codigo da Jaque
    // INICIO DA CONSTRUCAO DE SOLUCAO INICIAL VIAVEL -------------------------
    for(i = 0; i < Restricoes; i++){
        for(j = 0; j < Restricoes; j++){
            Matriz_M[i][j] = 0.0;
            for(k = 0; k < N_Coordenada; k++){
                Matriz_M[i][j]=  Matriz_M[i][j] + Matriz_E[i][k]*Matriz_E_Transposto[k][j];
            }
        }
    }

    for(i=0; i<Restricoes; i++){
        p[i] = i;
    }

    lu(Matriz_M,&p[0],Restricoes);	//Encontra as matrizes 'L' e 'U'
    resFx = lu_solve(Matriz_M,p,b,Restricoes);	//Encontra o resultado do sistema Mx=b

    populacao = (double**) malloc(N_Individuo *sizeof(double*));
    for(i = 0; i < N_Individuo; i++){
        populacao[i]= (double*)  malloc((N_Coordenada+1) *sizeof(double));
    }

    for(j = 0; j < N_Coordenada; j++){
        base[j] = 0;
        for(m = 0; m < Restricoes; m++){
            base[j] = base[j]+ Matriz_E_Transposto[j][m]*resFx[m];
        }
    }

    // GERAR UM INDIVIDUO x^til, ARMAZENADO EM 'solucao[]' --------------------
    for(j = 0; j < N_Coordenada; j++){
       d[j] =  ( (rand()%1000000) / 1000000.f ) * ( bounds[1]-bounds[0] ) + bounds[0];
    }
    for(j = 0; j < Restricoes; j++){
        z[j] = 0.0;
        for(m = 0; m < N_Coordenada; m++){
            z[j]=  z[j] + Matriz_E[j][m]*d[m];
        }
    }

    vetor_u = lu_solve(Matriz_M,p,z,Restricoes);

    for(j = 0; j < N_Coordenada; j++){
        vetor_v[j] = 0;
        for(m = 0; m < Restricoes; m++){
            vetor_v[j]  = vetor_v[j]  + Matriz_E_Transposto[j][m]*vetor_u[m];
        }
    }

    //printf("\n Solucao Inicial -- x_til \n");
    solucao_x_til = (double*) malloc((N_Coordenada+1) * sizeof(double));

    for(i = 0; i < N_Coordenada; i++){
        solucao[i] = base[i] + d[i] - vetor_v[i];
        solucao_x_til[i] = solucao[i];
        //printf (" %f ", solucao[i]);
    }
    //printf("\n\n");


    //-------------------------------------------------------------------------
    // NOVO CODIGO PARA A CRIACAO DA POPULACAO 'P'
    // ------------------------------------------------------------------------
    populacao_p = (double**) malloc(N_Individuo *sizeof(double*));
    for(i = 0; i < N_Individuo; i++){
        populacao_p[i]= (double*)  malloc((N_Coordenada-Restricoes) *sizeof(double));
    }

    for ( k = 0 ; k < N_Individuo; k++){
        for (j = 0; j < N_Coordenada-Restricoes; j++){
            //populacao_p[k][j] = ( (rand()%1000000) / 1000000.f ) * ( bounds[1]-bounds[0] ) + bounds[0];
            populacao_p[k][j] = ( (rand()%1000000) / 1000000.f ) * ( (100)-(-100) ) + (-100); // SEM BOUNDS
        }
    }

    for (k = 0 ; k < N_Individuo; k++){

            for ( i = 0; i < N_Coordenada; i++){
                double Zp = 0;
                for (j = 0 ; j < N_Coordenada-Restricoes; j++){
                    Zp += Matriz_Z[i][j] * populacao_p[k][j];
                }
                populacao[k][i] = solucao[i] + Zp;
            }
    }
    */


    /*
    Codigo do Rodrigo -- DELEqC
    */
    for(i = 0; i < Restricoes; i++){
        for(j = 0; j < Restricoes; j++){
            Matriz_M[i][j] = 0.0;
            for(k = 0; k < N_Coordenada; k++){
                Matriz_M[i][j]=  Matriz_M[i][j] + Matriz_E[i][k]*Matriz_E_Transposto[k][j];
            }
        }
    }

    for(i=0; i<Restricoes; i++){
        p[i] = i;
    }

    lu(Matriz_M,&p[0],Restricoes);	//Encontra as matrizes 'L' e 'U'
    resFx = lu_solve(Matriz_M,p,b,Restricoes);	//Encontra o resultado do sistema Mx=b

  	populacao = (double**) malloc(N_Individuo *sizeof(double*));

  	///more general version: used in the test-problems after the paper submitted to CEC 2018
    ///the individuals are composed by
    ///n values: the design variables
    ///1 value: the objective function value
    ///nGs values: the constraints violations in the form g(x) <= 0
    ///nHs values: the non-linear constraints violations in the form h(x) = 0
    ///n values: the bound violations, as xL_i <= x_i <= xU_i
    ///1 value: the sum of the bound constraint violations
    ///1 value: the sum of the normalized constraint violation values -- used by Feasible Tournament Selection (Deb)
    ///1 value: the fitness value -- used by APM
    ///m values: the violations of the linear constraints in the form h(x) = 0
    ///1 value: the sum of the normalyzed constraint violation values, except the linear equality ones -- used by DELEqC+TS
    ///1 value: the sum of the constraint violation values (PRINT)
  	*sizeOfIndividual = N_Coordenada+1+nGs+nHs+N_Coordenada+1+1+1+Restricoes + 1 + 1;

	for(i = 0; i < N_Individuo; i++) {
		///version solving only linear equality constraints; old version
		//populacao[i]= (double*)  malloc((N_Coordenada+1) *sizeof(double));


		populacao[i]= (double*)  malloc( (*sizeOfIndividual) *sizeof(double));
		for(j=0; j< *sizeOfIndividual; j++) {
            populacao[i][j] = 0;
		}
    }

    for(j = 0; j < N_Coordenada; j++){
        base[j] = 0;
        for(m = 0; m < Restricoes; m++){
            base[j] = base[j]+ Matriz_E_Transposto[j][m]*resFx[m];
        }
    }

    k = 0;
    //srand(time(NULL)); <-- nao deve-se iniciar o gerador novamente na mesma execucao independente
    while(k < N_Individuo){
        for(j = 0; j < N_Coordenada; j++){
           //d[j] =  rand()%10;
           //d[j] =  ( (rand()%1000000) / 1000000.f ) * 2000 - 1000;  //assumindo bounds [-1000; 1000]
           d[j] =  ( (rand()%1000000) / 1000000.f ) * ( bounds[j][1]-bounds[j][0] ) + bounds[j][0];
        }
        for(j = 0; j < Restricoes; j++){
            z[j] = 0.0;
            for(m = 0; m < N_Coordenada; m++){
                z[j]=  z[j] + Matriz_E[j][m]*d[m];
            }
        }

        vetor_u = lu_solve(Matriz_M,p,z,Restricoes);

        for(j = 0; j < N_Coordenada; j++){
            vetor_v[j] = 0;
            for(m = 0; m < Restricoes; m++){
                vetor_v[j]  = vetor_v[j]  + Matriz_E_Transposto[j][m]*vetor_u[m];
            }
        }
        if(k < (N_Individuo/2)){
            for(i = 0; i < N_Coordenada; i++){
                solucao[i] = base[i] + d[i] - vetor_v[i];
            }
        }
        else{
            for(i = 0; i < N_Coordenada; i++){
                solucao[i] = base[i] - d[i] + vetor_v[i];
            }
        }

        for (i = 0; i < N_Coordenada; i++){
            populacao[k][i] = solucao[i];
        }

        k = k + 1;
        //desaloca o vetor_u que eh a solucao do sistema linear
        free(vetor_u);

	}


    //desalocando o que foi alocado dinamicamente e que nao eh mais usado
    free(z);
    free(vetor_v);
    free(solucao);
    free(d);
    free(resFx);
    for(i=0; i<N_Coordenada; i++) {
        free(Matriz_Z[i]);
    }
    free(Matriz_Z);




/*
    for ( k = 0 ; k < N_Individuo; k++){
        printf(" %d ", k, "pop: ");
        for ( i = 0; i < N_Coordenada; i++){
            printf(" %f ", populacao[k][i]);
        }
        printf("   pop_p:");
        for ( i = 0; i < N_Coordenada-Restricoes; i++){
            printf(" %f ", populacao_p[k][i]);
        }
        printf("\n");
    }
    getchar();
 */

	return populacao;
}


void Avalia_Populacao(double **populacao, int N_Individuo, int N_Coordenada, int prob, double **bounds, double epsilon){
	int i;
	for(i = 0; i < N_Individuo; i++){
        //Avalia(populacao[i], N_Coordenada, prob);
        Avalia(populacao[i], N_Coordenada, prob, Matriz_Coef, bounds, epsilon);
	}
}

/*
void Avalia(double *id, int N_Coordenada, int prob){
    int i, j;
    double pi, soma, produto;

    pi = 3.1415926535;

    switch(prob){
        case 1:
            id[N_Coordenada] = pow((id[0] - 1), 2) + pow((id[1] - id[2]), 2) + pow((id[3] - id[4]), 2);
            break;
        case 2:
            id[N_Coordenada] = pow((id[0] - id[1]), 2) + pow((id[2] - 1), 2) + pow((id[3] - 1), 4) + pow((id[4] - 1), 6);
            break;
        case 3:
            id[N_Coordenada] = pow((id[0] - id[1]), 2) + pow((id[1] - id[2]), 2) + pow((id[2] - id[3]), 4) + pow((id[3] - id[4]), 2);
            break;
        case 4:
            id[N_Coordenada] = pow((id[0] - id[1]), 2) + pow((id[1] + id[2] - 2), 2) + pow((id[3] - 1),2) + pow((id[4] - 1),2);
            break;
        case 5:
            id[N_Coordenada] = pow((4*id[0] - id[1]), 2) + pow((id[1] + id[2] - 2), 2) + pow((id[3] - 1),2) + pow((id[4] - 1),2);
            break;
        case 6:
            id[N_Coordenada] = pow((id[0] - id[1]), 2) + pow((id[1] + id[2] - 2), 2) + pow((id[3] - 1),2) + pow((id[4] - 1),2);
            break;
        case 7:
            id[N_Coordenada] = 0.0;
            for(i = 0; i < N_Coordenada; i++){
                id[N_Coordenada] =  id[N_Coordenada] + pow(id[i], 2);
            }
            break;
        case 8:
            id[N_Coordenada] = 0.0;
            for(i = 0; i < N_Coordenada; i++){
                for(j = 0; j < N_Coordenada; j++){
                    id[N_Coordenada] = id[N_Coordenada] + exp(-pow((id[i] - id[j]),2))*id[i]*id[j];
                }
                id[N_Coordenada] = id[N_Coordenada] + id[i];
            }
            break;
        case 9:
            id[N_Coordenada] = 0.0;
            for(i = 0; i < N_Coordenada; i++){
                id[N_Coordenada] = id[N_Coordenada] + pow(id[i],2) + 10 - 10*cos(2*pi*id[i]);
            }
            break;
        case 10:
             id[N_Coordenada] = 0.0;
            for(i = 0; i < N_Coordenada - 1; i++){
                id[N_Coordenada] = id[N_Coordenada] + 100*pow((id[i+1] - pow(id[i],2)),2) + pow((id[i] - 1),2);
            }
            break;
        case 11:
            soma = 0.0;
            produto = 1.0;
            id[N_Coordenada] = 0.0;

            for(i = 0; i < N_Coordenada; i++){
                soma = soma + pow(id[i],2);
                produto =   produto*cos(id[i]/sqrt(i+1));
            }
            id[N_Coordenada] = (1.0/4000.0)*soma - produto + 1.0;
            break;
        case 12:
            id[N_Coordenada] = id[0]*id[0] + id[1]*id[1] + id[2]*id[2] - 2*id[0]*id[1];
            break;
    }
}
*/

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

/*
void Print_Populacao(double **populacao, int N_Individuo, int N_Coordenada, int prob){
	int i,j ;
	FILE *arq1;

	switch(prob){
	    case 1:
            arq1 = fopen("Solucao_Problema_1.txt", "w");
            break;
        case 2:
            arq1 = fopen("Solucao_Problema_2.txt", "w");
            break;
        case 3:
            arq1 = fopen("Solucao_Problema_3.txt", "w");
            break;
        case 4:
            arq1 = fopen("Solucao_Problema_4.txt", "w");
            break;
        case 5:
            arq1 = fopen("Solucao_Problema_5.txt", "w");
            break;
        case 6:
            arq1 = fopen("Solucao_Problema_6.txt", "w");
            break;
        case 7:
            arq1 = fopen("Solucao_Problema_7.txt", "w");
            break;
        case 8:
            arq1 = fopen("Solucao_Problema_8.txt", "w");
            break;
        case 9:
            arq1 = fopen("Solucao_Problema_9.txt", "w");
            break;
        case 10:
            arq1 = fopen("Solucao_Problema_10.txt", "w");
            break;
        case 11:
            arq1 = fopen("Solucao_Problema_11.txt", "w");
            break;
        case 12:
            arq1 = fopen("Solucao_Problema_12.txt", "w");
            break;
        case 13:
            arq1 = fopen("Solucao_Problema_13.txt", "w");
            break;
	}

	if (arq1 == NULL){
        printf("Problemas na CRIACAO do arquivo\n");
        return;
    }
    fprintf(arq1,"######################## Algoritmo Evolucao Diferencial ########################\n ");
    fprintf(arq1,"######################## Geracao de Populacao Viavel ########################\n ");


	for (i = 0; i < N_Individuo; ++i) {
		fprintf(arq1, "Individuo %d\n", i+1);
            for (j = 0; j < N_Coordenada; ++j) {
                fprintf(arq1, "\t x[%d] = %0.8lf\n", j+1, populacao[i][j]);
            }
			fprintf(arq1, "\t FO= %0.8lf\n", populacao[i][N_Coordenada]);
	}

	fclose(arq1);
}
* */

//void Inicializa_Parametros(int argc, char **argv, int *n_pop, double *cross_prob, double *peso, int *n_gen, int *metodo, int *problema_numero, int *semente){
void Inicializa_Parametros(int argc, char **argv, int *n_pop, double *cross_prob, double *peso, int *n_gen, int *metodo, int *problema_numero, int *semente, double* epsilonCorrecao, int* nCoordenadasNova, int* nRestricoesNova, int* constraintHandler, int* boundHandler){

    //printf("Numero de argumentos: %d\n", argc);

//    printf("Informe o numero de individuos na populacao: ");
//	scanf("%d",&(*n_pop));
//	printf("Informe o peso para mutacao: ");
//	scanf("%lf",&(*peso));
//	printf("Informe o numero de geracoes: ");
//	scanf("%d",&(*n_gen));
//	printf("Informe o numero do problema: ");
//	scanf("%d",&(*prob));

     if(argc >= 9) {
        //carrega parametros
        *n_pop = atoi(argv[1]);
        *cross_prob = atof(argv[2]);
        *peso = atof(argv[3]);
        *n_gen = atoi(argv[4]);
        *problema_numero = atoi(argv[5]);
        *metodo = atoi(argv[6]);
        *semente = atoi(argv[7]);
        *epsilonCorrecao = atof(argv[8]);
        //printf("cross: %f\n", *cross_prob);
        //printf("Epsilon: %f\n", *epsilonCorrecao);
        //printf("Problem: %d\n", *problema_numero);

        if(argc == 11 && ( *problema_numero <= 100 && *problema_numero != 20 ) ) { // UTILIZADOS APENAS NOS PROBLEMAS 7 a 11
            *nCoordenadasNova = atoi(argv[9]);
            *nRestricoesNova = atoi(argv[10]);
        } else if ( argc == 11 && ( *problema_numero > 100 || *problema_numero == 20 ) ) { // USED IN TEST-PROBLEMS 101-...
			*constraintHandler = atoi(argv[9]); // 0- DELEqC+TS, 1- DELEqC+APM, 2- TS, 3- APM
			*boundHandler = atoi(argv[10]); //
			//printf("CH: %d\nBH: %d\n", *constraintHandler, *boundHandler);
        }

        //printf("nCoordenadas: %d\n", *nCoordenadasNova);
        //printf("nRestricoes: %d\n", *nRestricoesNova);

    } else {
        //numero de parametros estah incorreto
        printf("O numero de parametros estah incorreto!\n");
        exit( EXIT_FAILURE );
    }

}

int procuraMelhorFactivel( double **populacao, int n_pop, int coordenadas, int restricoes, double epsilon ) {
    //acha alguem factivel
    int melhor = -1;
    int k = 0;
    int i;
    int j;
    double value;
    int viola = 0;
    for(i=0; i<restricoes && viola == 0; i++) {
        value = 0;
        for(j=0; j<coordenadas; j++) {
            value += Matriz_E[i][j] * populacao[ k ][ j ];
        }
        if ( fabs(value-b[i]) > epsilon ) {
            viola = 1;
        }
    }
    k++;
    while (viola == 1 && k<n_pop) {
        viola = 0;
        for(i=0; i<restricoes && viola == 0; i++) {
            value = 0;
            for(j=0; j<coordenadas; j++) {
                value += Matriz_E[i][j] * populacao[ k ][ j ];
            }
            if ( fabs(value-b[i]) > epsilon ) {
                viola = 1;
            }
        }
        k++;
    }

    if (k == n_pop) {
        //nao ha individuos factiveis na populacao
        return -1;
    } else {

        melhor = k-1;

        //k indica o individuo apos o primeiro nao viola as restricoes
        for(; k<n_pop; k++ ) {

            viola = 0;
            for(i=0; i<restricoes && viola == 0; i++) {
                value = 0;
                for(j=0; j<coordenadas; j++) {
                    value += Matriz_E[i][j] * populacao[ k ][ j ];
                }
                if ( fabs(value-b[i]) > epsilon ) {
                    viola = 1;
                }
            }
            if ( viola == 0 && populacao[k][coordenadas] < populacao[melhor][coordenadas] ) {
                melhor = k;
            }
        }
        return melhor;
    }
}

int procuraMelhor( double **populacao, int n_pop, int coordenadas ) {

    int melhor = 0;
    int k;

    for(k = 1; k<n_pop; k++ ) {

        if ( populacao[k][coordenadas] < populacao[melhor][coordenadas] ) {
            melhor = k;
        }

    }
    return melhor;
}


int procuraMelhorRestricoesTS( double **populacao, int n_pop, int dimension, int idSumConstraints ) {

    int melhor = 0;
    int k = 0;

    for(k = 1; k<n_pop; k++ ) {

		if ( populacao[k][idSumConstraints] < populacao[melhor][idSumConstraints] ) {
			melhor = k;
		} else if ( populacao[k][idSumConstraints] == populacao[melhor][idSumConstraints] && populacao[k][dimension] < populacao[melhor][dimension] ) {
            melhor = k;
        }

    }

    return melhor;
}

int procuraMelhorAPM( double **populacao, int n_pop, int idFitnessAPM ) {

    int melhor = 0;
    int k;

    for(k = 1; k<n_pop; k++ ) {

        if ( populacao[k][idFitnessAPM] < populacao[melhor][idFitnessAPM] ) {
            melhor = k;
        }

    }
    return melhor;
}

void printPopulation( double **populacao, int coordenadas, int id, int geracao, int restricoes, int nReparos, int numberOfValuesInEachIndividual, int nGs, int nHs, int populationSize ) {

	printf("------------------------------------\nPopulation at generation %d:\n", geracao);
	for(int i=0; i<populationSize; i++) {
		imprimirIndividuo( populacao, coordenadas, i, geracao, restricoes, nReparos, numberOfValuesInEachIndividual, nGs, nHs);
	}
	printf("------------------------------------\n\n");

}

void imprimirIndividuo( double **populacao, int coordenadas, int id, int geracao, int restricoes, int nReparos, int numberOfValuesInEachIndividual, int nGs, int nHs ) {

	int idSumViolations = coordenadas+1+nGs+nHs+coordenadas+1+1+1+restricoes+1;

    printf("%d | %f | %e", geracao, populacao[id][coordenadas], populacao[id][idSumViolations]); // aptidao na ultima posicao
    int i,j;
    double value;
    double somaViolacoes = 0;
    double violacao;
    for(i=0; i<coordenadas; i++) {
        printf(" | %f", populacao[id][i]);
    }

    //produto matriz-vetor para imprimir as violações de restrição
    printf(" | lec");
    for(i=0; i<restricoes; i++) {
		value = 0;
		for(j=0; j<coordenadas; j++) {
			value += Matriz_E[i][j] * populacao[ id ][ j ];
		}
		violacao = fabs(value-b[i]);
		printf(" | %.16f", violacao);
		somaViolacoes += violacao;
	}

    printf(" | %.16f", somaViolacoes);

	printf(" | other violations %d", coordenadas+1 );

    for(i = coordenadas+1; i<numberOfValuesInEachIndividual; i++) {
        printf(" | %.16f", populacao[id][i]);
    }

    printf(" | %d", nReparos);

    //printf(" | %f", somaViolacoes);
    printf("\n");

    //printf("Size of the individuals: %d\n", numberOfValuesInEachIndividual);

    //o codigo que segue eh necessario quando ha individuos factiveis na populacao (nao eh o caso aqui)
//    System.out.printf(" | %b | %f", melhor.isFeasible(), melhor.getAvaliacaoPenalizada());
//    for(float xi: melhor.getRests()) {
//        System.out.printf(" | %f", xi);
//    }
//    System.out.println();

}

int individuoViolaRestricaoNormaInfinito( double **populacao, int coordenadas, int id, int geracao, int restricoes, double epsilon) {

    int i,j;
    double value;

    for(i=0; i<restricoes; i++) {
		value = 0;
		for(j=0; j<coordenadas; j++) {
			value += Matriz_E[i][j] * populacao[ id ][ j ];
		}
		if ( fabs(value-b[i]) > epsilon ) {   ///h(x)=0 ==> | h(x) | <= epsilon
            return 0;
		}
	}

    return 1;

}

void calculateLinearEqualityConstraints( double **populacao, int coordenadas, int id, int restricoes, int nGs, int nHs, double epsilon ) {

    int i,j;
    double value, violation;
    int idLinearEqualityConstraints = coordenadas+1+nGs+nHs+coordenadas+1+1+1;

    for(i=0; i<restricoes; i++) {
		value = 0;
		for(j=0; j<coordenadas; j++) {
			value += Matriz_E[i][j] * populacao[ id ][ j ];
		}
		violation = fabs(value-b[i]) - epsilon; ///h(x)=0 ==> | h(x) | - epsilon <= 0
		populacao[ id ][ idLinearEqualityConstraints ] = violation <= 0? 0: violation;
		//populacao[ id ][ idLinearEqualityConstraints ] = 0;
		idLinearEqualityConstraints++;
	}
}

void imprimirIndividuoVetor( double *individuo, int coordenadas ) {

    printf("-- | %f", individuo[coordenadas]); // aptidao na ultima posicao
    int i;
    for(i=0; i<coordenadas; i++) {
        printf(" | %f", individuo[i]);
    }
    printf("\n");

}


//projeta todas as solucoes no espaco das solucoes viaveis
void corrigirSolucoes(double **populacao, int N_Individuo, int Restricoes, int N_Coordenada) {
    int j, m;
    double *z = (double*) malloc( Restricoes * sizeof(double) );
    double *vetor_u;
    double *vetor_v = (double*) malloc( N_Coordenada * sizeof(double) );
    //double *solucaoOriginal = (double*) malloc( N_Coordenada * sizeof(double) );

    for(int k=0; k < N_Individuo; k++) {

        //printf("Solucao candidata original:\n");
        //imprimirIndividuoVetor(populacao[k], N_Coordenada);

        //for(j = 0; j < N_Coordenada; j++){
        //    solucaoOriginal[j] = populacao[k][j];
        //}

        for(j = 0; j < Restricoes; j++){
            z[j] = 0.0;
            for(m = 0; m < N_Coordenada; m++){
                z[j]=  z[j] + Matriz_E[j][m]*populacao[k][m];
            }
        }

        vetor_u = lu_solve(Matriz_M,p,z,Restricoes); //aloca um vetor solucao

        for(j = 0; j < N_Coordenada; j++){
            vetor_v[j] = 0;
            for(m = 0; m < Restricoes; m++){
                vetor_v[j]  = vetor_v[j]  + Matriz_E_Transposto[j][m]*vetor_u[m];
            }
        }
        for(j = 0; j < N_Coordenada; j++){
            populacao[k][j] = base[j] + populacao[k][j] - vetor_v[j];
        }

        free(vetor_u); //desaloca o vetor solucao

        //printf("Solucao candidata projetada no espaco factivel:\n");
        //imprimirIndividuoVetor(populacao[k], N_Coordenada);

        //double diff = normaInfinito(solucaoOriginal, populacao[k], N_Coordenada);
        //if (diff > 0) {
        //    printf("Differenca entre as solucoes: %.16f\n", diff);
        //}

	}

	//free(solucaoOriginal);
	free(z);
	free(vetor_v);
}


void corrigirSolucao(double *solucao, int N_Individuo, int Restricoes, int N_Coordenada) {
    int j, m;
    double *z = (double*) malloc( Restricoes * sizeof(double) );
    double *vetor_u;
    double *vetor_v = (double*) malloc( N_Coordenada * sizeof(double) );

    for(j = 0; j < Restricoes; j++){
        z[j] = 0.0;
        for(m = 0; m < N_Coordenada; m++){
            z[j]=  z[j] + Matriz_E[j][m]*solucao[m];
        }
    }

    vetor_u = lu_solve(Matriz_M,p,z,Restricoes); //aloca um vetor solucao

    for(j = 0; j < N_Coordenada; j++){
        vetor_v[j] = 0;
        for(m = 0; m < Restricoes; m++){
            vetor_v[j]  = vetor_v[j]  + Matriz_E_Transposto[j][m]*vetor_u[m];
        }
    }
    for(j = 0; j < N_Coordenada; j++){
        solucao[j] = base[j] + solucao[j] - vetor_v[j];
    }

    free(vetor_u); //desaloca o vetor solucao

	free(z);
	free(vetor_v);
}

void corrigirSolucaoUsandoProjecao(double *solucao, int N_Individuo, int Restricoes, int N_Coordenada, double *v) {
    int j;
    double produtoEscalar = 0;
    double somaModuloQuadradosComponentes = 0;

    for(j = 0; j < N_Coordenada; j++) {
        produtoEscalar += solucao[ j ] * v[ j ];
        somaModuloQuadradosComponentes += v[ j ] * v[ j ];
    }

    for(j = 0; j < N_Coordenada; j++) {
		solucao[ j ] = v[ j ] * ( produtoEscalar / somaModuloQuadradosComponentes );
	}

}


double normaInfinito(double *x, double *y, int n) {
    double maxDiff = fabs(x[0] - y[0]);
    double diff;

    for(int i=1; i<n; i++) {
        diff = fabs(x[i] - y[i]);
        if (diff > maxDiff) {
            maxDiff = diff;
        }
    }

    return maxDiff;
}

///maxV is filled with the max values observed in two populations
///this procedure is used in Deb's Tournament Selection
void maxValues( double** population1, double** population2, double* maxV, int nIndividuals, int sizeIndividual ) {
	int i, j;

	for(j=0; j<sizeIndividual; j++) {
		maxV[j] = population1[0][j];
	}
	for(i=1; i<nIndividuals; i++) {
		for(j=0; j<sizeIndividual; j++) {
			if (maxV[j] < population1[i][j]) {
				maxV[j] = population1[i][j];
			}
		}
	}
	if ( population2 != NULL ) {
		for(i=0; i<nIndividuals; i++) {
			for(j=0; j<sizeIndividual; j++) {
				if (maxV[j] < population2[i][j]) {
					maxV[j] = population2[i][j];
				}
			}
		}
	}

	//print max values
	/*printf("MAX");
	for(j=0; j<sizeIndividual; j++) {
		printf(" | %f", maxV[ j ]);
	}
	printf("\n");*/
}

int bestIndividualObjectiveFunction( double* individual1, double* individual2, int dimension ) {
	return individual1[ dimension ] < individual2[ dimension ] ? 0: 1;
}

int bestIndividualTS( double* individual1, double* individual2, int dimension, int idSumConstraints ) {
	if ( individual1[idSumConstraints] < individual2[idSumConstraints] ) {
		return 0;
	} else if ( individual1[idSumConstraints] > individual2[idSumConstraints] ) {
		return 1;
	} else if ( individual1[dimension] < individual2[dimension] ) {
		return 0;
	} else {
		return 1;
	}
}

int bestIndividualAPM( double* individual1, double* individual2, int idFitnessAPM ) {
	if ( individual1[idFitnessAPM] < individual2[idFitnessAPM] ) {
		return 0;
	} else {
		return 1;
	}
}

void reduceVariationKeepingSolutionInFeasibleRegion(double *oldIndividual, double *newIndividual, int dimension, double** bounds, double* diff, double additionalScale) {
	int i;
	double alpha = 1;
	double f;

	//printf("Fixing the violation of the bound constraints:\n");

	for (i = 0; i < dimension; ++i) {
		diff[i] = newIndividual[ i ] - oldIndividual[ i ];
		if (diff[i] != 0) {
			if ( bounds[ i ][ 1 ] < newIndividual[ i ] ) {
				f = ( bounds[ i ][ 1 ] - oldIndividual[ i ] ) / diff[i];
				if ( alpha > f ) {
                    alpha = f;
                }
                //printf("diff[%d] = %f\n", i, diff[ i ]);
				//printf("bound[%d][1] = %f\n", i, bounds[ i ][ 1 ]);
				//printf("old value[%d] = %f\n", i, oldIndividual[ i ]);
				//printf("value[%d] = %f\n", i, newIndividual[ i ]);
			} else if ( bounds[ i ][ 0 ] > newIndividual[ i ] ) {
				f = ( bounds[ i ][ 0 ] - oldIndividual[ i ] ) / diff[i];
				if ( alpha > f ) {
                    alpha = f;
                }
                //printf("diff[%d] = %f\n", i, diff[ i ]);
				//printf("bound[%d][0] = %f\n", i, bounds[ i ][ 0 ]);
				//printf("old value[%d] = %f\n", i, oldIndividual[ i ]);
				//printf("value[%d] = %f\n", i, newIndividual[ i ]);
			}
            /*f = ( bounds[ i ][ 1 ] - newIndividual[ i ] ) / fabs( diff[i] );
            if ( f < 0 ) {
				printf("diff[%d] = %f\n", i, diff[ i ]);
				printf("bound[%d][1] = %f\n", i, bounds[ i ][ 1 ]);
				printf("old value[%d] = %f\n", i, oldIndividual[ i ]);
				printf("value[%d] = %f\n", i, newIndividual[ i ]);
                if ( alpha > -f ) {
                    alpha = -f;
                }
            } else {
                f = ( bounds[i][0] - newIndividual[i] ) / fabs( diff[i] );
                if ( f > 0 ) {
					printf("diff[%d] = %f\n", i, diff[ i ]);
					printf("bound[%d][0] = %f\n", i, bounds[ i ][ 0 ]);
					printf("old value[%d] = %f\n", i, oldIndividual[ i ]);
					printf("value[%d] = %f\n", i, newIndividual[ i ]);
                    if ( alpha > f ) {
                        alpha = f;
                    }
                }
            }*/
		}
	}
	//printf("Alpha: %f\n", alpha);
	//printf("Original ");
	//for (i = 0; i < dimension; ++i) {
        //printf(" | %f", newIndividual[i]);
    //}
    //printf("\nFixed ");
    double scale = alpha * additionalScale;
	for (i = 0; i < dimension; ++i) {
        newIndividual[i] = oldIndividual[i] + scale * diff[i];
    //    printf(" | %f", newIndividual[i]);
    }
    //printf("\n");
}

double** createInitialPopulationSimple(int Restricoes, int N_Coordenada, int N_Individuo, int prob, double** bounds, int nGs, int nHs, int* numberOfValuesInEachIndividual) {
	int i, j;

	double** populacao = (double**) malloc(N_Individuo *sizeof(double*));

  	///more general version: used in the test-problems after the paper submitted to CEC 2018
    ///the individuals are composed by
    ///n values: the design variables
    ///1 value: the objective function value
    ///nGs values: the constraints violations in the form g(x) <= 0
    ///nHs values: the non-linear constraints violations in the form h(x) = 0
    ///n values: the bound violations, as xL_i <= x_i <= xU_i
    ///1 value: the sum of the bound constraint violations
    ///1 value: the sum of the constraint violation values -- used by Feasible Tournament Selection (Deb)
    ///1 value: the fitness value -- used by APM
    ///m values: the violations of the linear constraints in the form h(x) = 0
    ///1 value: the sum of the constraint violation values, except the linear equality ones -- used by DELEqC+TS
    ///+1
  	*numberOfValuesInEachIndividual = N_Coordenada+1+nGs+nHs+N_Coordenada+1+1+1+Restricoes + 1 + 1;

	//printf("populacao\n");
	for(i = 0; i < N_Individuo; i++) {
		populacao[i]= (double*)  malloc( (*numberOfValuesInEachIndividual) *sizeof(double));
		for(j=0; j< N_Coordenada; j++) {
            populacao[i][j] = ( (rand()%1000000) / 1000000.f ) * ( bounds[j][1]-bounds[j][0] ) + bounds[j][0];
            //printf("| %f", populacao[i][j]);
		}
		for(; j< *numberOfValuesInEachIndividual; j++) {
			populacao[i][j] = 0;
			//printf("| %f", populacao[i][j]);
		}
		//printf("\n");
    }

    b = (double*) malloc( Restricoes * sizeof(double) );
    Matriz_E = (double**) malloc( Restricoes * sizeof(double*) );
    for(i=0; i<Restricoes; i++) {
        Matriz_E[i] = (double*) malloc( N_Coordenada * sizeof(double) );
    }
    linearConstraintsData(prob, Matriz_E, NULL, b);

    return populacao;
}


/* APM
 * this implementation was adapted from that available at github
 */
void APMCalculatePenaltyCoefficients(
	int populationSize,
	double** population1,
	double** population2,
	int dimension,
	int nLECs,
	int nGs,
	int nHs,
	double* penaltyCoefficients,
	double* averageObjectiveFunctionValues,
	int idGConstraints,
	int idBoundConstraints,
	int idLinearEqualityConstraints,
	int sizeOfIndividual) {

	int i;
	int j;
	int l;
	double sumObjectiveFunction = 0;
	//int numberOfConstraints = nGs + nHs + dimension + nLECs;

	//foreach candidate solution
	for (i = 0; i < populationSize; i++) {

		printf("population1[ %d ] = %f\n", i, population1[ i ][ dimension ]);
		sumObjectiveFunction += population1[ i ][ dimension ];

	}
	
	printf("1-sumObjectiveFunction: %f\n", sumObjectiveFunction);

	if ( population2 != NULL ) {
        for (i = 0; i < populationSize; i++) {
            sumObjectiveFunction += population2[ i ][ dimension ];
        }
	}
	printf("2-sumObjectiveFunction: %f\n", sumObjectiveFunction);
	
	//the absolute of the sumObjectiveFunction
	if (sumObjectiveFunction < 0) {
		sumObjectiveFunction = -sumObjectiveFunction;
	}

	//the average of the objective function values
	*averageObjectiveFunctionValues = population2 == NULL? sumObjectiveFunction / populationSize: sumObjectiveFunction / ( 2* populationSize);

	//the denominator of the equation of the penalty coefficients
	double denominator = 0;
	//the sum of the constraint violation values
	//these values are recorded to be used in the next situation
	double* sumViolation = (double*) malloc(sizeOfIndividual * sizeof ( double));
	//sum Gs + Hs (non-linear)
	for (l = idGConstraints; l < idGConstraints + nGs + nHs; l++) {

		sumViolation[ l ] = 0;
		for (i = 0; i < populationSize; i++) {
			sumViolation[ l ] += population1[ i ][ l ] > 0? population1[ i ][ l ]: 0;
		}
		denominator += sumViolation[ l ] * sumViolation[ l ];
	}

	//bound constraints
	for (l = idBoundConstraints; l < idBoundConstraints + dimension; l++) {

		sumViolation[ l ] = 0;
		for (i = 0; i < populationSize; i++) {
			sumViolation[ l ] += population1[ i ][ l ] > 0? population1[ i ][ l ]: 0;
		}
		denominator += sumViolation[ l ] * sumViolation[ l ];
	}

	//linear equality constraints
	for (l = idLinearEqualityConstraints; l < idLinearEqualityConstraints + nLECs; l++) {

		sumViolation[ l ] = 0;
		for (i = 0; i < populationSize; i++) {
			sumViolation[ l ] += population1[ i ][ l ] > 0? population1[ i ][ l ]: 0;
		}
		denominator += sumViolation[ l ] * sumViolation[ l ];

	}

	if ( population2 != NULL ) {
        //sum Gs + Hs (non-linear)
        for (l = idGConstraints; l < idGConstraints + nGs + nHs; l++) {

            sumViolation[ l ] = 0;
            for (i = 0; i < populationSize; i++) {
                sumViolation[ l ] += population2[ i ][ l ] > 0? population2[ i ][ l ]: 0;
            }
            denominator += sumViolation[ l ] * sumViolation[ l ];
        }

        //bound constraints
        for (l = idBoundConstraints; l < idBoundConstraints + dimension; l++) {

            sumViolation[ l ] = 0;
            for (i = 0; i < populationSize; i++) {
                sumViolation[ l ] += population2[ i ][ l ] > 0? population2[ i ][ l ]: 0;
            }
            denominator += sumViolation[ l ] * sumViolation[ l ];
        }

        //linear equality constraints
        for (l = idLinearEqualityConstraints; l < idLinearEqualityConstraints + nLECs; l++) {

            sumViolation[ l ] = 0;
            for (i = 0; i < populationSize; i++) {
                sumViolation[ l ] += population2[ i ][ l ] > 0? population2[ i ][ l ]: 0;
            }
            denominator += sumViolation[ l ] * sumViolation[ l ];

        }
	}

	//the penalty coefficients are calculated
	//sum Gs + Hs (non-linear)
	for (j = idGConstraints; j < idGConstraints + nGs + nHs; j++) {

		penaltyCoefficients[ j ] = denominator == 0? 0: (sumObjectiveFunction / denominator) * sumViolation[ j ];
		//printf("penaltyCoefficients[%i]=%f\n", j, penaltyCoefficients[j]);
		
	}

	//bound constraints
	for (j = idBoundConstraints; j < idBoundConstraints + dimension; j++) {

		penaltyCoefficients[ j ] = denominator == 0? 0: (sumObjectiveFunction / denominator) * sumViolation[ j ];
		//printf("penaltyCoefficients[%i]=%f\n", j, penaltyCoefficients[j]);
		
	}

	//linear equality constraints
	for (j = idLinearEqualityConstraints; j < idLinearEqualityConstraints + nLECs; j++) {

		printf("(%f / %f) * %f\n", sumObjectiveFunction, denominator, sumViolation[ j ]);
		penaltyCoefficients[ j ] = denominator == 0? 0: (sumObjectiveFunction / denominator) * sumViolation[ j ];
		printf("penaltyCoefficients[%i]=%f\n", j, penaltyCoefficients[j]);

	}

	//remove auxiliary variables
	free(sumViolation);

}

double APMcalculateFitness(
	double* individual,
	int dimension,
	int nLECs,
	int nGs,
	int nHs,
	double* penaltyCoefficients,
	double averageObjectiveFunctionValues,
	int idGConstraints,
	int idBoundConstraints,
	int idLinearEqualityConstraints,
	int constraintHandler) {

	//indicates if the candidate solution is infeasible
	int infeasible;
	int j;
	//the penalty value
	double penalty;

	//the candidate solutions are assumed feasible
	infeasible = 0;
	penalty = 0;

	for (j = idGConstraints; j < idGConstraints + nGs + nHs; j++) {

		//printf("individual[ %d ] = %f\n", j, individual[ j ]);
		
		if ( individual[ j ] > 0 ) {
			//the candidate solution is infeasible if some constraint is violated
			infeasible = 1;
			//the penalty value is updated
			penalty += penaltyCoefficients[ j ] * individual[ j ];
		}

	}

	for (j = idBoundConstraints; j < idBoundConstraints + dimension; j++) {

		//printf("individual[ %d ] = %f\n", j, individual[ j ]);

		if ( individual[ j ] > 0 ) {
			//the candidate solution is infeasible if some constraint is violated
			infeasible = 1;
			//the penalty value is updated
			penalty += penaltyCoefficients[ j ] * individual[ j ];
		}

	}

	if (constraintHandler == APM_ONLY) { //otherwise, the linear equality constraints are handled by DELEqC
        for (j = idLinearEqualityConstraints; j < idLinearEqualityConstraints + nLECs; j++) {

			//printf("individual[ %d ] = %f\n", j, individual[ j ]);

            if ( individual[ j ] > 0 ) {
                //the candidate solution is infeasible if some constraint is violated
                infeasible = 1;
                //the penalty value is updated
                penalty += penaltyCoefficients[ j ] * individual[ j ];
            }
        }
	}
	
	printf("penalty: %f\n", penalty);
	if (infeasible == 1) {
		printf("Unfeasible.\nFitness: %f\n", (individual[ dimension ] > averageObjectiveFunctionValues? individual[ dimension ] + penalty: averageObjectiveFunctionValues + penalty));
	} else {
		printf("Feasible.\n");
	}

	//the fitness is the sum of the objective function and penalty values
	//if the candidate solution is infeasible
	//otherwise, it is only the objective function value
	return infeasible == 1 ?
		(individual[ dimension ] > averageObjectiveFunctionValues? individual[ dimension ] + penalty: averageObjectiveFunctionValues + penalty) :
		individual[ dimension ];

}
