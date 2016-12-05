#ifndef RK4SPARSER_H
#define RK4SPARSER_H
#include "SimpleParser.h"

class RK4sParser : public SimpleParser
{
    public:
        RK4sParser();
        virtual double Evaluate(Subject* s);
        virtual string nameParser();
        virtual void setDataSet(double ** x,int tam);
        virtual ~RK4sParser();

    protected:

    private:
        double AuxEvaluate(Subject* s, int model, double* dat, double* increments);
        double** RKEvaluate(Subject* s, double* initial, int vars, double h, int iterations);

};

#endif // RK4SPARSER_H
