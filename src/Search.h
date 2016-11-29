#ifndef SEARCH_H
#include "Subject.h"
#include "Grammar.h"
#include "Tree.h"
#include "Configures.h"
#include "Parser.h"
#include "SimpleParser.h"
#include <cmath>
#include <algorithm>
#include <exception>
#include "Selection.h"
#include "TournamentSelection.h"
#include "PopulationReplacement.h"
#include "SimpleReplace.h"
#include "IndividuoBuilder.h"
#include "SimpleIndividuoBuilder.h"

#include "Mutation.h"
#include "SimpleMutation.h"

#include "Crossover.h"
#include "SimpleCrossover.h"

# include <omp.h>

#define SEARCH_H

class Search
{
public:
    Subject** pop;
    Subject** tmpPop;
    bool stepByStep = false;
    /**
        Tolerância de otimização. Quando o fitness não está melhorando
        pelo menos {tolerance} para duas iterações consecutivas a população é
        considera adaptada.
    **/
    double tolerance = 0.0001;// quando o fitnesse é menor que tolerance, o individuo está adaptado

    IndividuoBuilder * individuoBuilder;
    Selection * seletor;
    Parser * parser;
    PopulationReplacement * replacer;
    Mutation * mutationOperator;
    Crossover * crossoverOperator;

    Search();
    Search(Parser * parser = NULL, PopulationReplacement * populationreplacement = NULL,IndividuoBuilder * individuobuilder = NULL);

    void Operate();
    void Replace();
    void evolve();
    virtual ~Search();

    /** Print functions **/
    void printResult();
    void printParameters();
    void printBestIndividuo();

    /**Set GP Parameters*/
    void setCrossover(Crossover * opCrossover);
    void setMutation(Mutation * opMutation);
    void setIndividuoBuilder(IndividuoBuilder * indBuilder);
    void setPopulationReplace(PopulationReplacement * popReplace);
    void setParser(Parser * parser);
    void setSelection(Selection * seletor);
protected:

private:
    void EvaluatePopulation(int initialIndex, int finalIndex);
    void DetailedEvaluatePopulation(Subject* s);
    static bool SortPopulationFitness(Subject* a, Subject* b);
    void createsInitialPopulation();
};

#endif // SEARCH_H
