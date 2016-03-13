#ifndef SEARCH_H_INCLUDED
#define SEARCH_H_INCLUDED

#include "Population.h"
#include "Subject.h"
#include "Database.h"
#include "Gramatica.h"
#include "Operators.h"
#include "QRDecomposition.h"
#include <cmath>
#include <vector>
#include <limits>

class Search{
    public:
        Population* pop0;
        Population* pop1;
        vector<Subject*> popaux;
        Subject** pop;
        int* aux;
        int iterations;
        int errors;
        int objectives;

        double MAX = std::numeric_limits<double>::max();

        double* convergence;

        double testError;
        double testTr;
        string testBest;

        int bestHigh = conf->treeHigh;
        int worstHigh = conf->treeHigh;

        float peso0;
        float peso1;

        Search();
        ~Search();
        void doMonoSearch();
        void doMultiSearch();
        int tournamentMono(int a, int b);
        int tournamentMulti(int a, int b);
        void Operate();
        void updateParetoAux(Subject* a);
        void calcRank(int size);
        void calcRankTest(int size);
        void calcRankValid(int size);
        void crowdingDistanceFitness(int i, int f);
        void crowdingDistanceFitnessTest(int i, int f);
        void crowdingDistanceFitnessValid(int i, int f);
        void crowdingDistanceHigh(int i, int f);
        void crowdingDistanceComplexity(int i, int f);
        void crowdingDistanceSize(int i, int f);
        bool dominate(Subject* a, Subject* b);
        bool dominateTest(Subject* a, Subject* b);
        bool dominateValid(Subject* a, Subject* b);
        void calcFitness(Subject* s);
        void calcFitnessLS(Subject* s);
        void calcFitnessTest(Subject* s);
        void calcFitnessTestLS(Subject* s);
        void calcFitnessValidLS(Subject* s);
        bool static sortFit(Subject* a, Subject* b);
        bool static sortFitTest(Subject* a, Subject* b);
        bool static sortFitValid(Subject* a, Subject* b);
        bool static sortHigh(Subject* a, Subject* b);
        bool static sortComplexity(Subject* a, Subject* b);
        bool static sortSize(Subject* a, Subject* b);

        void Replace();

};

#endif // SEARCH_H_INCLUDED

