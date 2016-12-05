#ifndef RK4LSINDIVIDUO_H
#define RK4LSINDIVIDUO_H
#include "Subject.h"

class RK4LSIndividuo : public Subject
{
    public:
        double ** leastSquare = NULL;
        int * leastSquareSize = NULL;// vetor que contem os tamanhos de cada vetor leastSquare


        RK4LSIndividuo(bool inicializa);
        virtual Subject* clone();// retorna uma copia da instancia atual
        virtual string nameIndividual();
        void print();

        virtual ~RK4LSIndividuo();

    protected:

    private:
};

#endif // RK4LSINDIVIDUO_H
