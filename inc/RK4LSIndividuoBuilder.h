#ifndef RK4LSINDIVIDUOBUILDER_H
#define RK4LSINDIVIDUOBUILDER_H
#include "IndividuoBuilder.h"
#include "RK4LSIndividuo.h"

class RK4LSIndividuoBuilder : public IndividuoBuilder
{
    public:
        RK4LSIndividuoBuilder();
        virtual Subject * generateIndividuo(){
            return new RK4LSIndividuo(true);
        }
        virtual string nameIndividuo(){
            return "RK4LSIndividuo";
        }
        virtual ~RK4LSIndividuoBuilder();

    protected:

    private:
};

#endif // RK4LSINDIVIDUOBUILDER_H
