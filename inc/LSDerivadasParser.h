#ifndef LSDERIVADASPARSER_H
#define LSDERIVADASPARSER_H

#include "Configures.h"
#include "Parser.h"
#include "QRDecomposition.h"
#include <vector>
#include <stack>
#include <tuple>
#include <cmath>
#include "LeastSquareIndividuo.h"
#include "SimpleParser.h"

class LSDerivadasParser: public SimpleParser{
    public:
        LSDerivadasParser();

        double Evaluate(Subject* s);
        double* calcDerivadaRegistroI(int registro);

        double ** banco_derivadas;
        virtual void setDataSet(double ** x, int tam);

        virtual ~LSDerivadasParser();
    protected:
    private:
            double * AuxEvaluate(LeastSquareIndividuo * s, int model, double ** dat,int tam);
            //double * AuxEvaluate(Subject* s, int model, double ** dat,int tam);
            void calcDerivadas();
};

#endif // LSDERIVADASPARSER_H
