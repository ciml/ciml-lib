#include "RK4LSIndividuo.h"

RK4LSIndividuo::RK4LSIndividuo(bool inicializa)
{
    if(inicializa == true){
        for(int i = 0 ; i < conf->numTree; i++){
            addTree(new Tree());
        }
    }
}

string RK4LSIndividuo::nameIndividual(){
    return "RK4LSIndividuo";
}


Subject* RK4LSIndividuo::clone()
{
    //cout << "CLONE:" << endl;
    Subject* k = new RK4LSIndividuo(false);



    for(int i = 0; i < conf->numTree; i++)
    {
        k->trees.push_back(trees.at(i)->clone());
    }
    k->fitness = fitness;

    return k;
}

void RK4LSIndividuo::print(){
      cout << " (" << fitness << ") " << endl;
        for(int m = 0; m < conf->numTree; m++){
            cout << "   f" << m << "() = (" << trees[m]->fitness << ") ";
            trees[m]->root->print();
            cout << "leastSquare=[";
            for(int j = 0; j < leastSquareSize[m]; j++)
                cout << leastSquare[m][j] << ", ";
            cout << "]";
        }
        cout << endl;
}

RK4LSIndividuo::~RK4LSIndividuo()
{

    if(leastSquare !=NULL){
		for (int i = 0; i < conf->numTree; i++)
	  	{
            if( leastSquare[i] != NULL )
                delete [] leastSquare[i];
	  	}
		delete[]leastSquare;

		//leastSquare = NULL;
    }

    if(leastSquareSize!=NULL){
        delete[]leastSquareSize;
        //leastSquareSize = NULL;
    }



    for(Tree* t : trees)
        delete t;
    trees.clear();

}
