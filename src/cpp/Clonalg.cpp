#include <stdlib.h>
#include <iostream>
#include "Clonalg.h"

Clonalg::Clonalg(int generations,
				 int popsize,
				 int dimensions,
				 int bitsperdimension,
				 int bitsperparameter,
				 float mutationfactor,
				 float cloningfactor,
				 int numclones,
				 int randominsertion,
				 float upperlim,
				 float lowerlim)
				 : BaseClonalg(generations,
						 popsize,
						 dimensions,
						 bitsperdimension,
						 bitsperparameter,
						 mutationfactor,
						 cloningfactor,
						 numclones,
						 randominsertion,
						 upperlim,
						 lowerlim)
{
	m_v = new int[m_dimensions];
}

Clonalg::~Clonalg() {
	delete[] m_pop;
	delete[] m_fitness;
	delete[] m_fitnessNorm;
	delete[] m_v;
}

void Clonalg::InitPopulation(unsigned ** population, float ** fitness, float ** fitnessNorm){

	unsigned * pop = new unsigned[m_popsize*m_realLen];
	*fitness = new float[m_popsize];
	*fitnessNorm = new float[m_popsize];

	for(int i=0; i < m_popsize; i++){

		(*fitness)[i] = 0.0f;
		(*fitnessNorm)[i] = 0.0f;

		for(int l=0; l < m_realLen; l++){

			unsigned val = 0;

			for(int k=0;k<BITS_PER_WORD;k++){
				if(rand() % 2){
					BIT_SET(val, k);
				}
			}
			pop[i*m_realLen+l] = val;
		}
	}

	*population = pop;
}

float Clonalg::Search()
{
	InitPopulation(&m_pop, &m_fitness, &m_fitnessNorm);
	EvaluatePop(m_pop);
	FindBestAndWorst();
	//PrintPop();

	int i=0;
	for(i=0; i< m_generations; i++){
	//while(m_statistics.afinidadeMelhor < 0.0){
		Statistics(m_pop, m_fitness, i);
		CalculateAffinity(m_pop, m_fitness, m_fitnessNorm);
		CloneAndHypermutate(m_pop, m_fitness, m_fitnessNorm);

		if(m_randominsertion > 0){
			RandomInsertion(m_pop, m_fitness);
		}
		FindBestAndWorst();
		//i++;
	}

	return 0;
	//PrintPop();*/
}

void Clonalg::CloneAndHypermutate(unsigned *pop, float *fitness, float * fitnessNorm)
{
	float mutationRate;
	unsigned *clone, *bestClone;

	clone = new unsigned[m_realLen];
	bestClone = new unsigned[m_realLen];
	float fitnessClone, fitnessBestClone;

	for(int i=0; i < m_popsize; i++){

		for(int k = 0; k < m_realLen; k++){
			bestClone[k] = GetWord(pop, i, k);
		}
		fitnessBestClone = fitness[i];

		mutationRate = HipermutationRate(fitnessNorm[i]);

		for(int j=0; j < m_numclones; j++){

			for(int k = 0; k < m_realLen; k++){
				clone[k] = GetWord(pop, i, k);
			}

		    Mutate(clone, mutationRate);
		    fitnessClone = Evaluate(clone);

			if(fitnessClone > fitnessBestClone){
				fitnessBestClone = fitnessClone;

				for(int k = 0; k < m_realLen; k++){
					bestClone[k] = clone[k];
				}
			}
		}

		if(fitnessBestClone > fitness[i]){

			for(int k = 0; k < m_realLen; k++){
				pop[i*m_realLen+k] = bestClone[k];
			}
			fitness[i] = fitnessBestClone;
		}
	}

	delete clone;
	delete bestClone;
}

void Clonalg::CalculateAffinity(unsigned *pop, float *fitness, float *fitnessNorm)
{
	float max = m_statistics.afinidadeMelhor,
		  min = m_statistics.afinidadePior;

	for(int i=0; i< m_popsize; i++){

		if(max-min == 0){
			fitnessNorm[i] = 0.0f;
		}
		else{
			float n = (fitness[i]- min) / (max-min);
			n = (MAXIMIZACAO == 0) ? 1-n : n;
			fitnessNorm[i] = n;
		}
	}
}

void Clonalg::Mutate(unsigned *clone, float mutationRate)
{
	int i,j;
	unsigned val;

	for(i=0; i < m_realLen; i++){

		val = clone[i];

		int count = 0;

		for(j=0; j< BITS_PER_WORD; j++){

			float random = (float)rand()/RAND_MAX;

			if(random < mutationRate){
				BIT_FLIP(val, j);
				count++;
			}
		}
		clone[i] = val;
	}
}

float Clonalg::Evaluate(unsigned *individual)
{
	Decode(individual, m_v);
	return m_objective->Evaluate(m_v);
}

void Clonalg::EvaluatePop(unsigned *pop)
{
	for(int i=0; i< m_popsize; i++){
		m_fitness[i] = Evaluate(&pop[i]);
	}
}

void Clonalg::RandomInsertion(unsigned *pop, float * fitness)
{
	float worstFitness = fitness[0];
	int index = 0;

	for(int i = 1; i< m_popsize; i++){
		if(fitness[i] < worstFitness){
			worstFitness = fitness[i];
			index = i;
		}
	}

	for(int j=0;j< m_realLen; j++){

		unsigned val = 0;

		for(int k = 0; k < BITS_PER_WORD; k++){
			if(rand() % 2){
				BIT_SET(val,k);
			}
		}

		pop[index*m_realLen+j] = val;
	}

	fitness[index] = Evaluate(&pop[index]);

	/*for(int i=m_popsize - m_randominsertion; i < m_popsize; i++){

		for(int j=0;j< m_realLen; j++){

			unsigned val = 0;

			for(int k = 0; k < BITS_PER_WORD; k++){
				if(rand() % 2){
					BIT_SET(val,k);
				}
			}

			pop[i*m_realLen+j] = val;
		}

		fitness[i] = Evaluate(&pop[i]);
	}*/
}

void Clonalg::FindBestAndWorst()
{
	m_statistics.afinidadeMelhor = m_fitness[0];
	m_statistics.indiceMelhor = 0;
	m_statistics.afinidadePior = m_fitness[0];
	m_statistics.indicePior = 0;

	for(int i = 1; i< m_popsize; i++){
		if(m_fitness[i] > m_statistics.afinidadeMelhor){
			m_statistics.afinidadeMelhor = m_fitness[i];
			m_statistics.indiceMelhor = i;
		}
		else if(m_fitness[i] < m_statistics.afinidadePior){
			m_statistics.afinidadePior = m_fitness[i];
			m_statistics.indicePior = i;
		}
	}
}

void Clonalg::Statistics(unsigned *pop, float * fitness, int iterationNumber)
{
	//cout << "Best #" << iterationNumber << ": " << m_statistics.afinidadeMelhor << endl;

	/*for(j=0; j<TAMANHO_ANTICORPO; j++){
		printf("%d ", populacao[0].representacao[j]);
	}
	printf("\nBinário:\n");

	short binario[TAMANHO_ANTICORPO];
	grayParaBinario(populacao[0].representacao, binario);

	for(j=0;j<TAMANHO_ANTICORPO;j++){
		printf("%d ", binario[j]);
	}
	printf("\n");*/
}
