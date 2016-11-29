#ifndef CROSSOVER_H
#define CROSSOVER_H
#include "Subject.h"

class Crossover
{
    public:
        Crossover();
        /**
            Recebe um vetor de individuos e seu tamanho e deve aplicar cruzamento entre membros desse vetor
        **/
        virtual void cross(Subject ** vec_individuos, int num_individuos) = 0;
        virtual ~Crossover();

    protected:

    private:
};

#endif // CROSSOVER_H
