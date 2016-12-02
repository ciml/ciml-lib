#ifndef LEASTSQUAREINDIVIDUOBUILDER_H
#define LEASTSQUAREINDIVIDUOBUILDER_H
#include "IndividuoBuilder.h"
#include "LeastSquareIndividuo.h"

class LeastSquareIndividuoBuilder : public IndividuoBuilder
{
    public:
        LeastSquareIndividuoBuilder();
        virtual Subject * generateIndividuo(){
            return new LeastSquareIndividuo(true);
        }
        virtual string nameIndividuo(){
            return "LeastSquareIndividuo";
        }
        virtual ~LeastSquareIndividuoBuilder();

    protected:

    private:
};

#endif // LEASTSQUAREINDIVIDUOBUILDER_H
