#include "BaseClonalg.h"

BaseClonalg::BaseClonalg(int generations,
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
{
	m_generations = generations;
	m_popsize = popsize;
	m_dimensions = dimensions;
	m_bitsperdimension = bitsperdimension;
	m_bitsperparameter = bitsperparameter;
	m_mutationfactor = mutationfactor;
	m_cloningfactor  = cloningfactor;
	m_numclones = numclones;
	m_randominsertion = randominsertion;
	m_upperlim = upperlim;
	m_lowerlim = lowerlim;

	m_cromLen = m_dimensions * m_bitsperdimension;
	m_realLen = ceil(m_cromLen/BITS_PER_WORD);

	//TODO: Factory class for creating the objective function 
	m_objective = new OneMaxProblem(m_dimensions);

#ifdef verbose
	cout << "------------------------------------------------" << endl;
	cout << "Parameters:" << endl;
	cout << "generations:" << "\t" << m_generations  << endl;
	cout << "popsize:" << "\t" << m_popsize << endl;
	cout << "dimensions:" << "\t" << m_dimensions << endl;
	cout << "bits per dimension:" << "\t" << m_bitsperdimension << endl;
	cout << "bits per parameter:" << "\t" << m_bitsperparameter << endl;
	cout << "mutation factor:" << "\t" << m_mutationfactor << endl;
	cout << "cromlen:" << "\t" << m_cromLen << endl;
	cout << "realLen:" << "\t" << m_realLen << endl;
	cout << "numclones:" << "\t" << m_numclones << endl;
	cout << "upperlim:" << "\t" << m_upperlim << endl;
	cout << "lowerlim:" << "\t" << m_lowerlim << endl;
	cout << "------------------------------------------------" << endl;
#endif

}

BaseClonalg::~BaseClonalg(){
	delete m_objective;
}

float BaseClonalg::HipermutationRate(float affinity)
{
	return exp(-m_mutationfactor * affinity);
}
/*
void BaseClonalg::Decode(anticorpo *a, int *v)
{
	int count =0;
	for(int i=0; i < m_realLen; i++){
		for(int j=0; j < BITS_PER_WORD; j++){
			v[count++] = (a->representacao[i] & ( 1 << j)) != 0;
		}
	}
}*/
/*
void BaseClonalg::Decode(unsigned int *pop, int id, int *v)
{
	int count =0;
	for(int i=0; i < m_realLen; i++){

		unsigned val = GetWord(pop, id, i);

		for(int j=0; j < BITS_PER_WORD; j++){
			v[count++] = (val & ( 1 << j)) != 0;
		}
	}
}

*/

void BaseClonalg::Decode(unsigned int *individual, int *v)
{
	int count=0;

	for(int i=0; i < m_realLen; i++){

		unsigned val = individual[i];

		for(int j=0; j < BITS_PER_WORD; j++){
			v[count++] = (val & ( 1 << j)) != 0;
		}
	}
}


int BaseClonalg::CompareIndividuals(const void* a, const void* b){
    anticorpo* a1 = (anticorpo*)a;
    anticorpo* a2 = (anticorpo*)b;

    return a1->afinidade < a2->afinidade;
}

/*void BaseClonalg::Sort(anticorpo *pop){
	//vector<anticorpo> myvector (pop, pop+8);
	//sort (pop.begin(), pop.end(), CompareIndividuals);
	//qsort(pop, this->m_popsize, sizeof(anticorpo), (int(*)(const void*, const void*))BaseClonalg::CompareIndividuals);
}*/

void BaseClonalg::Sort(unsigned int *pop){
	//vector<anticorpo> myvector (pop, pop+8);
	//sort (pop.begin(), pop.end(), CompareIndividuals);
	//qsort(pop, this->m_popsize, sizeof(anticorpo), (int(*)(const void*, const void*))BaseClonalg::CompareIndividuals);
}

void BaseClonalg::PrintPop(){

	for(int i = 0; i< m_popsize; i++){

		cout << "#" << i << ": " << m_fitness[i] << endl;
		for(int j = 0; j< m_realLen; j++){

			//unsigned val = m_pop[m_realLen*i + j];
			unsigned val = GetWord(m_pop, i,j);

			for(int k = 0; k < BITS_PER_WORD; k++){
				cout << BIT_CHECK(val,k)/ pow(2,k);
			}
		}
		cout << endl;
	}
}
/*
void BaseClonalg::PrintPop(){

	for(int i = 0; i< m_popsize; i++){
		cout << "#" << i << ": " << m_pop[i].afinidade << endl;
		for(int j = 0; j< m_realLen; j++){

			unsigned val = m_pop[i].representacao[j];
			for(int k = 0; k < BITS_PER_WORD; k++){
				cout << BIT_CHECK(val,k)/ pow(2,k);
			}
		}
		cout << endl;
	}
}*/
