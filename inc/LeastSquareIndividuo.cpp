#include "LeastSquareIndividuo.h"

LeastSquareIndividuo::LeastSquareIndividuo(bool inicializa)
{
    if(inicializa == true){
        //leastSquare = new double*[conf->numTree];
        for(int j = 0 ; j < conf->numTree; j++)
        {
            addTree(new Tree());
        }
    }else{
//        cout << "Inicializa == false " << endl;
//        cin.get();
    }
}

LeastSquareIndividuo::LeastSquareIndividuo(Subject * s){
    trees = s->trees;
    fitness = s->fitness;
    leastSquare = new double*[conf->numTree];
    leastSquareSize = new int[conf->numTree];
}

string LeastSquareIndividuo::nameIndividual(){
    return "LeastSquareIndividuo";
}

void LeastSquareIndividuo::print(){

}

Subject* LeastSquareIndividuo::clone()
{

     if(conf->numTree  !=trees.size()){
        cout << "Clone " << endl;
        cout << "num trees "<< conf->numTree << endl;
        cout << "trees size " <<trees.size() << endl;

        cout << "algo de muito errado com o clone do individuo" <<endl;
        cin.get();
     }

    LeastSquareIndividuo* s = new LeastSquareIndividuo(false);

    for(int i = 0; i < conf->numTree; i++)
    {
        s->addTree(trees.at(i)->clone());
    }
    s->fitness = fitness;


    if(leastSquare != NULL){
        int auxLSSize[conf->numTree];
        for(int i = 0; i < conf->numTree;i++){
            auxLSSize[i] = leastSquareSize[i];
        }
        double ** auxLS = new double*[conf->numTree];

        for(int i = 0 ; i < conf->numTree; i++){
            auxLS[i] = new double[leastSquareSize[i]];
        }


        for(int i = 0; i < conf->numTree; i++){
            for(int j = 0; j < leastSquareSize[i]; j++){
                auxLS[i][j] = leastSquare[i][j];
            }

        }
        s->leastSquareSize = auxLSSize;
        s->leastSquare = auxLS;
    }
    //cout << "clone fit\n";

    return s;
}

LeastSquareIndividuo::~LeastSquareIndividuo()
{
//    if(leastSquareSize != NULL){
//        cout << "DESTRUTOR1\n";
//        cin.get();
//        for(int i = 0 ; i < conf->numTree;i++){
//            delete [] leastSquare[i];
//        }
//
//        delete [] leastSquareSize;
//        cout << "DESTRUTOR2\n";
//        cin.get();
//    }

}
