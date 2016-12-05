#ifndef SIMPLEINDIVIDUO_H
#define SIMPLEINDIVIDUO_H
#include "Subject.h"

class SimpleIndividuo : public Subject {
    public:
        SimpleIndividuo(bool inicializa = true);
        virtual Subject* clone();// retorna uma copia da instancia atual
        virtual string nameIndividual();
        virtual void print();
        virtual ~SimpleIndividuo();


    protected:

    private:
};

#endif // SIMPLEINDIVIDUO_H
