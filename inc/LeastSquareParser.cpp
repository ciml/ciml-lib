#include "LeastSquareParser.h"

LeastSquareParser::LeastSquareParser() {
    //ctor
}

double LeastSquareParser::Evaluate(Subject* s){
    /***
        Arrumar orientação mudarleastSquare para muitos lugares
    ***/
    double * r;
    s->fitness = 0;
    for(int arvore = 0; arvore < conf->numTree;arvore++){
        s->trees[arvore]->fitness = 0;
        r = AuxEvaluate(s,arvore,dataset,tamDataset);
        for( int j = 0; j < tamDataset ; j++){ // para todos os dados do conjunto de treinamento
            if(isinf(r[j]) || isnan(r[j])){
                s->trees[arvore]->fitness = INFINITY;
                //s->trees[arvore]->root->print();
                break;
            }
            s->trees[arvore]->fitness += pow(r[j] - dataset[j][data->variables + arvore], 2);
        }
        s->fitness += s->trees[arvore]->fitness;
    }
    return s->fitness;

}

double * LeastSquareParser::AuxEvaluate(Subject* s, int model, double ** dat,int tam){
//    s->trees[model]->root->print();
//    cout << endl;
//    s->trees[model]->root->print(0);
//    cout << "________________________" << endl;

    /// Find base functions
    int sub = 1;
    for(int i = 0; i < s->trees[model]->linearModel.size(); i++)
        if((int)get<0>(s->trees[model]->linearModel.at(i)) == conf->leastSquareOperator)
            sub++;

    double** subMatrix = new double*[tam];
    for(int i = 0; i < tam; i++){
        subMatrix[i] = new double[sub];
    }

//    for(tuple<double, double> t : s->trees[model]->linearModel)
//        cout << "(" << get<0>(t) << "," << get<1>(t) << ")";
//    cout << endl << "tam = " << tam << " sub = " << sub << endl;

    /// Operate with a Stack
    int j = 0;
    for(int i = 0; i < tam; i++){
        stack<double> stk;
//        cout << "  i = " << i;
        for(tuple<double, double> t : s->trees[model]->linearModel){
            int var = (int)get<0>(t);
            if( var == conf->constant ) { // constant
                stk.push(get<1>(t));
            }
            else if( var == conf->variable ){ // variable
                stk.push(dat[i][(int)get<1>(t)]);
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
//                programOperators
            }
            else if(var == conf->leastSquareOperator){
//                subMatrix[i][j] = stk.top();
//                cin.get();
//                stk.pop();
            }
        }

//        if(stk.size() != sub){
//            cout << "Teste " << stk.size() << "  " << sub << endl;
//            for(tuple<double, double> tt : s->trees[model]->linearModel)
//                cout << "(" << get<0>(tt) << "," << get<1>(tt) << ")";
//            cout << endl;
//            cin.get();
//        }

        int j = sub - 1;
        while(!stk.empty() && j >= 0){
            subMatrix[i][j] = stk.top();
//            cout << subMatrix[i][j] << ", ";
//            cout << stk.top() << ", ";
            stk.pop();
//            cout << " j = " << j << " pilha = " << stk.size() << " ";
            j--;
        }
//        cout << endl;
    }

//////    /// Solve Least Square
    QRDecomposition* qrDec = new QRDecomposition(subMatrix, tam, sub);
    double* b = new double[tam];
    for(int i = 0; i < tam; i++){
        b[i] = dat[i][data->variables + model];
    }

    s->trees[model]->leastSquare = qrDec->solveLeastSquares(b, tam);
    s->trees[model]->leastSquareSize = sub;
//    s->trees[model]->leastSquare = NULL;
    if(s->trees[model]->leastSquare != NULL){
//        cout << "deu" << endl;
    }
    else{
//        cout << "nao deu" << endl;
        s->trees[model]->leastSquare = new double[sub];
        for(int j = 0; j < sub; j++){
            s->trees[model]->leastSquare[j] = 1;
        }
    }

    /// Calculate Error
    double* r = new double[tam];
    for(int i = 0; i < tam; i++){
        r[i] = 0;
        for(int j = 0; j < sub; j++){
            r[i] += s->trees[model]->leastSquare[j] * subMatrix[i][j];
            if(isnan(r[i]) || isinf(r[i]))
                r[i] = INFINITY;
        }
    }

//    for(int i = 0; i < tam; i++){
//        for(int j = 0; j < sub; j++)
//            cout << subMatrix[i][j] << " ";
//            cout << s->trees[model]->leastSquare[j] << " ";
//        cout << endl;
//    }
//    cin.get();

    for(int i = 0; i < tam; i++){
        delete [] subMatrix[i];
    }
    delete [] subMatrix;

    delete [] b;
    delete qrDec;

//    cin.get();
    return r;
}

LeastSquareParser::~LeastSquareParser() {
    //dtor
}
