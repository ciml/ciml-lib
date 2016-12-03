#ifndef LEASTSQUAREINDIVIDUO_H
#define LEASTSQUAREINDIVIDUO_H
#include "Subject.h"

class LeastSquareIndividuo : public Subject
{
    public:
        double ** leastSquare = NULL;
        int * leastSquareSize = NULL;// vetor que contem os tamanhos de cada vetor leastSquare



        LeastSquareIndividuo(bool inicializa);
        virtual Subject* clone();// retorna uma copia da instancia atual
        virtual string nameIndividual();
        void print();

        virtual ~LeastSquareIndividuo();

    protected:

    private:
};

#endif // LEASTSQUAREINDIVIDUO_H
