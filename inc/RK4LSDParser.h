#ifndef RK4LSDPARSER_H
#define RK4LSDPARSER_H
#include "SimpleParser.h"
#include "RK4LSIndividuo.h"
#include "LeastSquareIndividuo.h"
#include "QRDecomposition.h"


class RK4LSDParser : public SimpleParser
{
    public:
        double ** bancoDerivadas;

        RK4LSDParser();

        virtual double Evaluate(Subject* s);
        virtual string nameParser(){
            return "RK4LSDParser";
        }

        void setDataSet(double ** x, int tam);

        virtual ~RK4LSDParser();

    protected:

    private:

        double** RKEvaluate(RK4LSIndividuo* s, double* initial, int vars, double h, int iterations);
        double   AuxRKEvaluate(RK4LSIndividuo* s, int model, double* dat, double* increments);
        double * AuxLSDEvaluate(RK4LSIndividuo* s, int model, double ** dat,int tam);

        double * calcDerivadaRegistroI(int registro);
        void calcDerivadas();
};

#endif // RK4LSDPARSER_H
