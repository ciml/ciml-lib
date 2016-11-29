#ifndef SIMPLEINDIVIDUOBUILDER_H
#define SIMPLEINDIVIDUOBUILDER_H
#include "IndividuoBuilder.h"
#include "SimpleIndividuo.h"

class SimpleIndividuoBuilder : public IndividuoBuilder
{
    public:
        SimpleIndividuoBuilder();
        virtual Subject * generateIndividuo(){
            return new SimpleIndividuo(true);
        }
        virtual string nameIndividuo(){
            return "SimpleIndividuo";
        }
        virtual ~SimpleIndividuoBuilder();

    protected:

    private:
};

#endif // SIMPLEINDIVIDUOBUILDER_H
