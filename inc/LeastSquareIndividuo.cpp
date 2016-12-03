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

//LeastSquareIndividuo::LeastSquareIndividuo(Subject * s){
//    //DerivedType *derived_ptr1=dynamic_cast<DerivedType*>(D_ptr);// works fine
////    LeastSquareIndividuo * x = dynamic_cast<LeastSquareIndividuo*>(s);
////    cout << "Trees size : " << x->trees.size() << endl;
////    cin.get()
////    trees = s->trees;
////    fitness = s->fitness;
////    leastSquare = new double*[conf->numTree];
////    leastSquareSize = new int[conf->numTree];
////    for(int i = 0; i < conf->numTree; i++)
////        leastSquareSize[i] = 0;
//
//}

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


   s->leastSquare = new double*[conf->numTree];
   s->leastSquareSize  = new int[conf->numTree];



    for(int i = 0; i < conf->numTree; i++){
        s->leastSquareSize[i] = leastSquareSize[i];
        s->leastSquare[i] = new double[s->leastSquareSize[i]];
    }

    for(int i = 0; i < conf->numTree; i++){
        for(int j = 0 ; j < s->leastSquareSize[i]; j++){
            s->leastSquare[i][j] = leastSquare[i][j];
        }
    }


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
