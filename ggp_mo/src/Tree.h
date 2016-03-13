#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

#include <iostream>
#include <stack>
#include <cstdlib>
#include <string>

#include "Configures.h"
#include "No.h"
#include "Parser.h"

using namespace std;

class Tree{
    public:
        No* root;
        int high;
        int terminals;
        int counter;
        No** temp;
        int tempCounter;
        bool calc;

        string model;

        double* exp;
        int expCounter;
        stack<string> p;

        vector<vector<double> > sub;
        double* alpha;
        bool leastSquare;
        int subCounter;
        int auxSubCounter;

        Tree();
        ~Tree();
        virtual Tree* clone();
        void setTree(No* n);
        void refresh();
        void countLevels(No* n, int h);
        void countHighs(No* n, int h);
        void print();
        string infixPrint();
        void auxInfix(No* no);
        void printExp();
        void printAlphas();
        int countAll();
        int countNT();
        int countT();
        int countNTHigh(int value, int level, int sub);
        void countNTHigh(No* n, int v, int l, int sub);
        No* getNo(int total);
        No* auxGetNo(No* n, int total);
        No* getNoHigh(int total, int value, int level, int sub);
        No* auxGetNoHigh(No* n, int total, int v, int l, int sub);
        void setExp();
        void getExp(No* n);
        double treeResult(double* var, double* alpha = NULL, int alphaSize = 0);
        double* getExp();
        void setSub();
        void findLS(No* n);
        void newFindLS();
        void makeSub(No* n);
        vector<double> getSub(int s);

};

#endif // TREE_H_INCLUDED

