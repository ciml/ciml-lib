#include "ParserDerivadas.h"

ParserDerivadas::ParserDerivadas() {
    //
}

void ParserDerivadas::calcDerivadas(){
    cout << tamDataset << endl;
    bancoDerivadas = new double*[tamDataset];


    for(int i = 0 ; i < tamDataset; i++){
           bancoDerivadas[i] = calcDerivadaRegistroI(i);
    }

    cout << "\n\nBANCO DE DERIVADAS" << endl;

     for(int i = 0 ; i < tamDataset; i++){
        cout << i << " - " ;
        for(int j = 0; j < data->prediction; j++){
            cout << bancoDerivadas[i][j] << " ";
        }
        cout << endl;
     }

   cin.get();
}


void ParserDerivadas::setDataSet(double ** x, int tam){
    dataset = x;
    tamDataset = tam;
    calcDerivadas();
}

string ParserDerivadas::nameParser(){
    return "Parser Derivadas";
}

double * ParserDerivadas::calcDerivadaRegistroI(int registro){

    double *anterior, *proximo;
    double * derivadas = new double[data->prediction];


    if(registro > 0 && registro < tamDataset -1){
        anterior = dataset[registro-1];
        proximo  = dataset[registro+1];

        for(int i = data->variables ; i <= data->variables + data->prediction - 1; i++){
            //cout << (proximo[i] - anterior[i])/(proximo[0] - anterior[0]);
            derivadas[i-data->variables] = (proximo[i] - anterior[i])/(proximo[0] - anterior[0]);
            //cout << derivadas[i-data->variables];
        }

    }else if ( registro == 0){

        anterior = dataset[0];
        proximo  = dataset[1];

        for(int i = data->variables ; i <= data->variables + data->prediction - 1; i++){
            //cout << (proximo[i] - anterior[i])/(proximo[0] - anterior[0]) ;
            derivadas[i-data->variables] = (proximo[i] - anterior[i])/(proximo[0] - anterior[0]);

        }
    }else if (registro == (tamDataset-1) ){

        anterior = dataset[tamDataset -2];
        proximo  = dataset[tamDataset -1];

        for(int i = data->variables; i <= data->variables + data->prediction - 1; i++){
           // cout << proximo[i] - anterior[i] ;
            derivadas[i-data->variables] = (proximo[i] - anterior[i])/(proximo[0] - anterior[0]);
        }
    }

    for(int i = data->variables; i <= data->variables + data->prediction - 1; i++){
        cout << derivadas[i-data->variables];
        if(i != data->prediction)
                    cout << " - ";
    }
//    cout << endl;
    return derivadas;

}

double ParserDerivadas::Evaluate(Subject* s){

    double r;
    s->fitness = 0;
    for(int arvore = 0; arvore < conf->numTree;arvore++){
        s->trees[arvore]->fitness = 0;
        for( int j = 0; j < tamDataset; j++){ // para todos os dados do conjunto de treinamento
            //break;
            r = AuxEvaluate(s,arvore,bancoDerivadas[j]);
            if(isinf(r) || isnan(r)){
                  s->trees[arvore]->fitness = INFINITY;
//                s->trees[arvore]->root->print();
//                cin.get();
//                break;
            }
            double fit = pow(r - dataset[j][data->variables + arvore], 2);
            s->trees[arvore]->fitness += fit;
        }
        s->fitness += s->trees[arvore]->fitness;
    }


}

double ParserDerivadas::AuxEvaluate(Subject* s, int model, double* dat){
    stack<double> stk;
    for(tuple<double, double> t : s->trees[model]->linearModel){
        int var = (int)get<0>(t);
        if( var == conf->constant ) { // constant
            stk.push(get<1>(t));
        }
        else if( var == conf->variable ){ // variable
            stk.push(dat[(int)get<1>(t)]);
        }
        else if( var == conf->bynaryArithmeticOperators){ // bynaryArithmeticOperators
            double b = stk.top();
            stk.pop();
            double a = stk.top();
            stk.pop();
            stk.push(Operate(get<0>(t), get<1>(t), a, b));
        }
        else if( var ==  conf->unaryArithmeticOperators){ // unaryArithmeticOperators
            double a = stk.top();
            stk.pop();
            stk.push(Operate(get<0>(t), get<1>(t), a));
        }
        else if( var == conf->binaryLogicalOperators){ // binaryLogicalOperators
            double b = stk.top();
            stk.pop();
            double a = stk.top();
            stk.pop();
            stk.push(Operate(get<0>(t), get<1>(t), a, b));
        }
        else if( var == conf->unaryLogicalOperators){ // unaryLogicalOperators
            double a = stk.top();
            stk.pop();
            stk.push(Operate(get<0>(t), get<1>(t), a));
        }
        else if( var == conf->comparisonOperators){ // comparisonOperators
            double b = stk.top();
            stk.pop();
            double a = stk.top();
            stk.pop();
            stk.push(Operate(get<0>(t), get<1>(t), a, b));
        }
        else if( var == conf->programOperators){
//            programOperators
        }

//        if(stk.top() == NULL){
//            while(!stk.empty()){
//                stk.pop();
//            }
//            cout << "   retornando NULL" << endl;
//            return NULL;
//        }
//        cout << "   " << stk.top() << endl;
    }

    return stk.top();
}

ParserDerivadas::~ParserDerivadas() {
    //dtor
}
