#ifndef OPERATORS_H_INCLUDED
#define OPERATORS_H_INCLUDED
#include <stdlib.h>
#include "Gramatica.h"
#include "Tree.h"
#include "No.h"
#include "Configures.h"

class Operators{

public:

    void swap(No* p, No* n, Tree* a, Tree* b);
    int randomI(int x);
    void Mutate(Tree* t);
    void Cross(Tree* a, Tree* b, Tree* c, Tree* d);
};

extern Operators* op;

#endif // OPERATORS_H_INCLUDED
