#ifndef SIMPLECROSSOVER_H
#define SIMPLECROSSOVER_H
#include "Crossover.h"

class SimpleCrossover : public Crossover
{
    public:
        SimpleCrossover();
        virtual void cross(Subject ** vec_individuos, int num_individuos);
        virtual ~SimpleCrossover();

    protected:

    private:

        void treeCrossover(Tree* a, Tree* b);
        bool checkRate(double rate);
};

#endif // SIMPLECROSSOVER_H
