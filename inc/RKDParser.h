#ifndef RKDPARSER_H
#define RKDPARSER_H


#include "Configures.h"
#include "Parser.h"
#include <vector>
#include <stack>
#include <tuple>
#include <cmath>
#include "QRDecomposition.h"
#include "SimpleParser.h"


class RKDParser : public SimpleParser{
    public:
        RKDParser();
        double Evaluate(Subject* s);
        virtual string nameParser();



        virtual void setDataSet(double ** x, int tam);
        virtual~RKDParser();
        double ** bancoDerivadas;
    protected:

    private:
        void calcDerivadas();
        double * AuxLSDEvaluate(Subject* s, int model, double ** dat,int tam);
        void AuxRk4Evaluete();
        double * calcDerivadaRegistroI(int registro);
};

#endif // RKDPARSER_H
