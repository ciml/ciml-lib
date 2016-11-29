#ifndef GENERICPARSER_H
#define GENERICPARSER_H

#include "Configures.h"
#include "Parser.h"
#include <vector>
#include <stack>
#include <tuple>
#include <cmath>

class GenericParser{
    public:
        GenericParser();

        double Evaluate(Subject* s);
        double * Evaluate(Subject* s, int model, double** dat,int tam);
        double * Evaluate(Subject* s, double* dat);
        double* Operate(int tam, int opType, int opValue, double* a, double* b = NULL, double* c = NULL);

        virtual ~GenericParser();
    protected:
    private:
};

#endif // GENERICPARSER_H
