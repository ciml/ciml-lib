#ifndef NO_H_INCLUDED
#define NO_H_INCLUDED
#include <iostream>
#include <vector>

#include "Configures.h"

class No{
    public:
        double type;
        double value;
        std::string mask;
        int derivation;
        No* father;
        int fatherSon;
        int countSons;
        int high;
        int deep;
        std::vector<No*> sons;

        No();
        ~No();
        virtual No* clone();
        virtual No* clone(No* n);
        void clean();
        void setNo(double t, double v, std::string m);
        void cpyNo(No* n);
        void setFather(No* n, int fs);
        void setSon(No* n);
        int highSubTree();
        int NTValue(int v);
        int NTValueHigh(int v, int n, int sub);
        int countAll();
        int countNT();
        int countT();
        void print(int tab);
        void print();
};

#endif // NO_H_INCLUDED
