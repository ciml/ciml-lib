#include <iostream>
#include "Population.h"
#include "Subject.h"
#include "Gramatica.h"

Population::Population(Population* p){
    for(int i = 0; i < conf->popSize; i++){
//        pop[i] = new Subject(objectives);
//        p->pop[i]->tree->print();
        pop.push_back(new Subject());
        pop[i]->tree->setTree(p->pop[i]->tree->root->clone(NULL));
    }
};

Population::Population(){
    for(int i = 0; i < conf->popSize; i++){
//        pop[i] = new Subject(gram->criaArvExp(), objectives);
        pop.push_back(new Subject(gram->criaArvExp()));
    }
};

Population::~Population(){
    pop.clear();
//    for(int i = 0; i < conf->popSize; i++){
////        delete pop[i];
//        pop.pop_back();
//    }
};

void Population::print(){
//    double v[] = {1.0};
    for(int i = 0; i < conf->popSize; i++){
//        pop[i]->tree->print();
        cout << pop[i]->fitness << " ";
    }
    cout << endl;
};
