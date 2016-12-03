#include "LeastSquareIndividuo.h"

LeastSquareIndividuo::LeastSquareIndividuo(bool inicializa)
{
    if(inicializa == true){
        leastSquare = new double*[conf->numTree];
        leastSquareSize = new int[conf->numTree];
        for(int j = 0 ; j < conf->numTree; j++)
        {
            addTree(new Tree());
        }
        for(int i = 0; i < conf->numTree; i++)
            leastSquareSize[i] = 0;
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
    for(int i = 0; i < conf->numTree; i++)
        leastSquareSize[i] = 0;
}

string LeastSquareIndividuo::nameIndividual(){
    return "LeastSquareIndividuo";
}

void LeastSquareIndividuo::print(){

}

Subject* LeastSquareIndividuo::clone()
{

    LeastSquareIndividuo* s = new LeastSquareIndividuo(false);

    for(int i = 0; i < conf->numTree; i++)
    {
        s->addTree(trees.at(i)->clone());
    }
    s->fitness = fitness;

//    cout << "Clone"<< endl;
    //cin.get();

    double ** mat = new double*[conf->numTree];
    int * vet_tam = new int[conf->numTree];
    //cin.get();
    for(int i = 0; i < conf->numTree; i++){
//            cout << " : " << leastSquareSize[i] << " : \n";
            vet_tam[i] = leastSquareSize[i];
            mat[i] = new double[vet_tam[i]];

    }
    s->leastSquare = mat;
    s->leastSquareSize = vet_tam;
//    cout << "saiu";
//    cin.get();
    return s;
}

LeastSquareIndividuo::~LeastSquareIndividuo()
{

    if (leastSquare != NULL)
	{
//        cout << "DESTRUTOR1\n";
//        cin.get();
		for (int i = 0; i < conf->numTree; i++)
	  	{
  			delete[]leastSquare[i];
	  	}
		delete[]leastSquare;

		leastSquare = NULL;

//		cout << "DESTRUTOR2\n";
//        cin.get();
	}
	if(leastSquareSize != NULL){
        delete[]leastSquareSize;
        leastSquareSize = NULL;
	}
//
//    for(Tree* t : trees)
//        delete t;
//    trees.clear();

}
