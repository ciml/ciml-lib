#ifndef SUBJECT_H
#define SUBJECT_H
#include <iostream>
#include <vector>
#include "Tree.h"
#include <string>

using namespace std;

class Subject{
    public:
        double fitness;//representa o nivel de aptidão do inviduo
        vector<Tree*> trees;//representa a solução do problema

        Subject();
        void addTree(Tree* t);
        virtual string nameIndividual() = 0; // deve printar o nome do individuo
        virtual Subject* clone() = 0;
        virtual ~Subject();


    protected:
    private:
};

#endif // SUBJECT_H
