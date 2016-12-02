#ifndef LEASTSQUAREPARSER_H
#define LEASTSQUAREPARSER_H

#include "Configures.h"
#include "Parser.h"
#include "../inc/QRDecomposition.h"
#include <vector>
#include <stack>
#include <tuple>
#include <cmath>
#include "SimpleParser.h"
#include "LeastSquareIndividuo.h"

class LeastSquareParser : public SimpleParser {
    public:
        LeastSquareParser();

        virtual double Evaluate(Subject* s);
        virtual string nameParser(){
            return "LeastSquareParser";
        }

        virtual ~LeastSquareParser();
    protected:
    private:
        double * AuxEvaluate(LeastSquareIndividuo* s, int model, double ** dat,int tam);
};

#endif // LEASTSQUAREPARSER_H
