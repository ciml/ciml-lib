#ifndef INDIVIDUOBUILDER_H
#define INDIVIDUOBUILDER_H
#include "Subject.h"
#include <string>

class IndividuoBuilder
{
    public:
        IndividuoBuilder();
        virtual Subject * generateIndividuo() = 0;
        virtual string nameIndividuo() = 0;
        virtual ~IndividuoBuilder();

    protected:

    private:
};

#endif // INDIVIDUOBUILDER_H
