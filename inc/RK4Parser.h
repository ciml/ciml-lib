#ifndef RK4PARSER_H
#define RK4PARSER_H

#include "Configures.h"
#include "Parser.h"
#include <vector>
#include <stack>
#include <tuple>
#include <cmath>

class RK4Parser : public Parser{
    public:
        RK4Parser();

        double Evaluate(Subject* s);
        double** Evaluate(Subject* s, double* initial, int vars, double h, int iterations);
        double Evaluate(Subject* s, int model, double* dat, double* increments);
        double * Evaluate(Subject* s, double ** dat,int tam);
        double* Operate(int tam, int opType, int opValue, double* a, double* b = NULL, double* c = NULL);
        double Operate(int opType, int opValue, double a, double b = NULL, double c = NULL);


        virtual ~RK4Parser();

    protected:

    private:
};

#endif // RK4PARSER_H
