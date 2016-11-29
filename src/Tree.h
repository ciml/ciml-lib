#ifndef TREE_H
#define TREE_H

#include "No.h"
#include "Configures.h"
#include <iostream>
#include <vector>
#include <tuple>

class Tree{
    public:
        Tree();
        Tree(No* n);
        virtual ~Tree();
        No* root;
        vector<No*> nonTerminals;
        vector<No*> targetedNonTerminals;
        vector<tuple<double,double> > linearModel;
        double fitness;

        double * leastSquare;
        int leastSquareSize;

        No* subTree();
        No* targetSubTree(No* n);
        No* getSubTree(int i);
        void nonTerminalCount(No* n);
        void targetedNonTerminalCount(No* n, No* m);
        void update();
        void update(No* n);
        Tree * clone();
        No* clone(No * n);
        void print();

    protected:
    private:
};

#endif // TREE_H
