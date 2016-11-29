#ifndef PARSERDERIVADAS_H

#include "Configures.h"
#include "Parser.h"
#include <vector>
#include <stack>
#include <tuple>
#include <cmath>
#include "SimpleParser.h"


#define PARSERDERIVADAS_H


class ParserDerivadas : public SimpleParser{
    public:
        double ** bancoDerivadas;


        ParserDerivadas();
        double Evaluate(Subject* s);
        virtual string nameParser();



        virtual void setDataSet(double ** x, int tam);
        virtual ~ParserDerivadas();
    private:
        void calcDerivadas();
        double * calcDerivadaRegistroI(int registro);
        double AuxEvaluate(Subject* s, int model, double* dat);
};

#endif // PARSERDERIVADAS_H
