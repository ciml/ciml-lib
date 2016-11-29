#ifndef MUTATION_H
#define MUTATION_H
#include "Subject.h"

class Mutation
{
    public:
        /**
            Recebe um ponteiro para um individuo e deve aplicar mutação nesse
        **/
        virtual void mutate(Subject * individuo) = 0;
        virtual ~Mutation();

    protected:

    private:
};

#endif // MUTATION_H
