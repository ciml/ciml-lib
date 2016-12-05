#ifndef RK4LSDerivadasParser_H
#define RK4LSDerivadasParser_H

#include "Configures.h"
#include "Parser.h"
#include "QRDecomposition.h"
#include <vector>
#include <stack>
#include <tuple>
#include <cmath>
#include "RK4LSIndividuo.h"
#include "SimpleParser.h"

class RK4LSDerivadasParser: public SimpleParser{
    public:
        RK4LSDerivadasParser();

        double Evaluate(Subject* s);
        double* calcDerivadaRegistroI(int registro);

        double ** banco_derivadas;
        virtual void setDataSet(double ** x, int tam);

        virtual ~RK4LSDerivadasParser();
    protected:
    private:
            double * AuxEvaluate(RK4LSIndividuo * s, int model, double ** dat,int tam);
            void calcDerivadas();
};

#endif // RK4LSDerivadasParser_H
