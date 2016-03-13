#ifndef SUBJECT_H_INCLUDED
#define SUBJECT_H_INCLUDED

#include "Tree.h"
#include <vector>

class Subject{
    public:
        Tree* tree;

        double fitnessLS;
        double fitnessTestLS;
        double fitnessValidLS;

        int tt;// verdadeiro verdadeiro
        int ft;// falso verdadeiro
        int tf;// verdadeiro falso
        int ff;// falso falso

        int ranking = 0;

        bool printing;

        double crowdingDistance = 0;
        double* fitness;
        double* fitnessTest;

        Subject();
        Subject(Tree* n);
        double complexity();
        ~Subject();
        void print();
};

#endif // SUBJECT_H_INCLUDED
