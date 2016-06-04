#include <stdlib.h>
#include <iostream>
#include "Clonalg.h"

Clonalg::Clonalg(int generations,
				 int popsize,
				 int optimizationProblem,
				 int dimensions,
				 int bitsperdimension,
				 float mutationfactor,
				 float cloningfactor,
				 int numclones,
				 int randominsertion,
				 float upperlim,
				 float lowerlim)
				 : BaseClonalg(generations,
						 popsize,
						 optimizationProblem,
						 dimensions,
						 bitsperdimension,
						 mutationfactor,
						 cloningfactor,
						 numclones,
						 randominsertion,
						 upperlim,
						 lowerlim)
{
	m_v = new float[m_dimensions];
	m_aux_binary = new int[m_dimensions*m_bitsperdimension];

	m_clone = new unsigned[m_realLen];
	m_bestClone = new unsigned[m_realLen];
}

Clonalg::~Clonalg() {
	delete[] m_pop;
	delete[] m_fitness;
	delete[] m_fitnessNorm;
	delete[] m_v;
	delete[] m_aux_binary;

	delete [] m_clone;
	delete [] m_bestClone;
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


/*
 RealValue = infLimit + (supLimit - infLimit)/(2^k - 1) * IntegerValue
 */
void Clonalg::Decode(unsigned int *individual, float *v)
{

	/*int count=0;

	for(int i=0; i < m_realLen; i++){

		unsigned val = individual[i];

		for(int j=0; j < BITS_PER_WORD; j++){
			v[count++] = (val & ( 1 << j)) != 0;
		}
	}*/


	for(int k = 0; k < m_dimensions; k++){

		//cout << "k = " << k << endl;

		v[k] = 0;

		int index = (k * m_bitsperdimension) / BITS_PER_WORD;
		//cout << "index = " << index << endl;
		int val = individual[index];
		int start = k * m_bitsperdimension;
		int stop = start + m_bitsperdimension;
		double bitpow;

		int tp = val >> start;
		int count = 0, mask = 1;

		for(int j = start; j < stop; j++) {

			// test for current bit 0 or 1
			if((tp & mask) == 1) {
				bitpow = pow(2.0, (double)(count));
				v[k] += bitpow;
			}
			tp = tp >> 1;
			count++;
		}

		//cout << v[k] << " -> ";

		v[k] = m_lowerlim + (((m_upperlim - m_lowerlim)/((pow(2.0f, m_bitsperdimension)-1)))*v[k]);

		//cout << v[k] << endl;
	}
}

float Clonalg::Search()
{
	InitPopulation(&m_pop, &m_fitness, &m_fitnessNorm);
	EvaluatePop(m_pop);
	FindBestAndWorst();

	int gen=0;
	for(gen=0; gen< m_generations; gen++){
	//while(m_statistics.afinidadeMelhor < -0.00001){

		//Statistics(m_pop, m_fitness, gen);

		CalculateAffinity(m_pop, m_fitness, m_fitnessNorm);
		CloneAndHypermutate(m_pop, m_fitness, m_fitnessNorm);

		if(m_randominsertion > 0){
			RandomInsertion(m_pop, m_fitness);
		}
		FindBestAndWorst();
		//gen++;
	}

	return 0;
}

void Clonalg::CloneAndHypermutate(unsigned *pop, float *fitness, float * fitnessNorm)
{
	float mutationRate;
	float fitnessClone, fitnessBestClone;

	for(int i=0; i < m_popsize; i++){

		for(int k = 0; k < m_realLen; k++){
			m_bestClone[k] = GetWord(pop, i, k);
		}
		fitnessBestClone = fitness[i];

		mutationRate = HipermutationRate(fitnessNorm[i]);

		for(int j=0; j < m_numclones; j++){

			for(int k = 0; k < m_realLen; k++){
				m_clone[k] = GetWord(pop, i, k);
			}

		    Mutate(m_clone, mutationRate);
		    fitnessClone = Evaluate(m_clone);

			if(fitnessClone > fitnessBestClone){
				fitnessBestClone = fitnessClone;

				for(int k = 0; k < m_realLen; k++){
					m_bestClone[k] = m_clone[k];
				}
			}
		}

		if(fitnessBestClone > fitness[i]){

			for(int k = 0; k < m_realLen; k++)
			{
				pop[i*m_realLen+k] = m_bestClone[k];
			}
			fitness[i] = fitnessBestClone;
		}
	}
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
			n = 1-n;
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
	return m_objective->EvaluateFloat(m_v);
}

void Clonalg::EvaluatePop(unsigned *pop)
{
	for(int i=0; i< m_popsize; i++){
		m_fitness[i] = Evaluate(&pop[i*m_realLen]);
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
	cout << "Best #" << iterationNumber << ": " << m_statistics.afinidadeMelhor << endl;

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
