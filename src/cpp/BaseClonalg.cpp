#include "BaseClonalg.h"

BaseClonalg::BaseClonalg(int generations,
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
{
	m_generations = generations;
	m_popsize = popsize;
	m_optimizationProblem = optimizationProblem;
	m_dimensions = dimensions;
	m_bitsperdimension = bitsperdimension;
	m_mutationfactor = mutationfactor;
	m_cloningfactor  = cloningfactor;
	m_numclones = numclones;
	m_randominsertion = randominsertion;
	m_upperlim = upperlim;
	m_lowerlim = lowerlim;

	m_cromLen = m_dimensions * m_bitsperdimension;
	m_realLen = ceil(m_cromLen/BITS_PER_WORD);

	m_objective = ProblemFactory::CreateProblem(m_optimizationProblem, m_dimensions);

	m_pop = NULL;

#ifdef VERBOSE
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

int  BaseClonalg::BinaryToDecimal(int *binary, int begin, int end){

    int i, n=1, decimal=0;

    for(i=end-1; i>=begin; i--, n=n<<1){
    	decimal += n*binary[i];
    }
    return decimal;
}

void BaseClonalg::Sort(unsigned int *pop){
}

void BaseClonalg::PrintPop(){

	for(int i = 0; i< m_popsize; i++){

		cout << "#" << i << ": " << m_fitness[i] << endl;
		for(int j = 0; j< m_realLen; j++){

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
