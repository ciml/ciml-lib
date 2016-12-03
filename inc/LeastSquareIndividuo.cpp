#include "LeastSquareIndividuo.h"
#include "Tree.h"

LeastSquareIndividuo::LeastSquareIndividuo(bool inicializa)
{
    if(inicializa == true){
        leastSquare = new double*[conf->numTree];
        leastSquareSize = new int[conf->numTree];
        for(int i = 0 ; i < conf->numTree; i++)
        {
            leastSquareSize[i] = 0;
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
    LeastSquareIndividuo* s = new LeastSquareIndividuo(false);//cria um invididuo 'vazio'

    for(int i = 0; i < conf->numTree; i++)
    {
        s->addTree(trees.at(i)->clone());
    }
    s->fitness = fitness;


    double ** mat = new double*[conf->numTree];
    int * vet_tam = new int[conf->numTree];

    for(int i = 0; i < conf->numTree; i++){
            vet_tam[i] = leastSquareSize[i];
            mat[i] = new double[vet_tam[i]];
    }

    for(int i = 0; i < conf->numTree; i++){
        for(int j = 0 ; j < vet_tam[i]; j ++){
            mat[i][j] = leastSquare[i][j];
        }
    }
    s->leastSquare = mat;
    s->leastSquareSize = vet_tam;

    return s;
}

LeastSquareIndividuo::~LeastSquareIndividuo()
{

    if ( leastSquare != NULL)
	{
//		for (int i = 0; i < conf->numTree; i++)
//	  	{
//            if( leastSquare[i] != NULL )
//                delete [] leastSquare[i];
//	  	}
		delete[]leastSquare;

		//leastSquare = NULL;

	}
	if(leastSquareSize != NULL){
        delete[]leastSquareSize;
        //leastSquareSize = NULL;
	}


//    for(Tree* t : trees)
//        delete t;
//    trees.clear();

}
