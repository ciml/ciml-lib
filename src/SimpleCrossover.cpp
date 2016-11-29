#include "SimpleCrossover.h"

SimpleCrossover::SimpleCrossover()
{
    //ctor
}

bool SimpleCrossover::checkRate(double rate){
    return (rand()%100 < rate*100);
}

void SimpleCrossover::cross(Subject ** vec_individuos, int num_individuos){

    for(int i = 1; i < num_individuos; i+= 2){
        for( int j = 0 ; j < vec_individuos[i]->trees.size(); j++){
            if(checkRate(conf->crossoverRate)){
                treeCrossover( vec_individuos[i-1]->trees[j] , vec_individuos[i]->trees[j] );
            }
        }
    }

}


void SimpleCrossover::treeCrossover(Tree* a, Tree* b){
    No* n = a->subTree();
    No* m = b->targetSubTree(n);

    if(n == NULL || m == NULL){
        return;
    }
    swap(n->sons, m->sons);
    swap(n->father, m->father);

    a->update();
    b->update();
}

SimpleCrossover::~SimpleCrossover()
{
    //dtor
}
