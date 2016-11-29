#ifndef SIMPLEPARSER_H
#define SIMPLEPARSER_H

#include "Configures.h"
#include "Parser.h"
#include <vector>
#include <stack>
#include <tuple>
#include <cmath>

class SimpleParser : public Parser{
    public:
        SimpleParser();
        virtual string nameParser();
        virtual double Evaluate(Subject* s);
        virtual double Operate(int opType, int opValue, double a, double b = NULL, double c = NULL);


        virtual void setDataSet(double ** x,int tam){
            tamDataset = tam;
            dataset = x;
        }

        ~SimpleParser();
    protected:
        double** dataset;
        int tamDataset;
    private:
        double AuxEvaluate(Subject* s, int model, double* dat);
};

#endif // SIMPLEPARSER_H
