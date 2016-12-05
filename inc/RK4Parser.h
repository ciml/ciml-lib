#ifndef RK4PARSER_H
#define RK4PARSER_H

#include "Configures.h"
#include "SimpleParser.h"
#include "RK4LSIndividuo.h"
#include <vector>
#include <stack>
#include <tuple>
#include <cmath>

class RK4Parser : public Parser{
    public:
        RK4Parser();
/*
        virtual string nameParser();

        virtual void setDataSet(double ** x,int tam);

        double Evaluate(Subject* s);
*/
        virtual ~RK4Parser();

    protected:
    private:
        double AuxEvaluate(Subject* s, int model, double* dat, double* increments);
        double** RKEvaluate(Subject* s, double* initial, int vars, double h, int iterations);
        //double AuxEvaluate(Subject* s, int model, double* dat, double* increments);
};

#endif // RK4PARSER_H
