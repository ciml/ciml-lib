#include "RKDParser.h"

RKDParser::RKDParser() {
    //ctor
}

void RKDParser::calcDerivadas(){
    cout << "Tam dataset " << tamDataset << endl;
    bancoDerivadas = new double*[tamDataset];


    for(int i = 0 ; i < tamDataset; i++){
        if(i < tamDataset ){
           // cout << "Derivada do registro " << i << endl;
            bancoDerivadas[i] = calcDerivadaRegistroI(i);
            //cin.get();
        }
        else
             bancoDerivadas[i] = calcDerivadaRegistroI(i-1);
    }

    cout << "\n\nBANCO DE DERIVADAS" << endl;

     for(int i = 0 ; i < tamDataset; i++){
        for(int j = 0; j < data->prediction+data->variables; j++){
            cout << bancoDerivadas[i][j] << " ";
        }
        cout << endl;
     }

     cin.get();
}

void RKDParser::setDataSet(double ** x, int tam){
    dataset = x;
    tamDataset = tam;
    calcDerivadas();
}


double RKDParser::Evaluate(Subject* s){
    /***
        Arrumar orientação mudar leastSquare para muitos lugares
    ***/
    double * r;
    s->fitness = 0;
    for(int arvore = 0; arvore < conf->numTree;arvore++){
        s->trees[arvore]->fitness = 0;
        r = AuxLSDEvaluate(s,arvore,dataset,tamDataset);
        for( int j = 0; j < tamDataset ; j++){ // para todos os dados do conjunto de treinamento
            if(isinf(r[j]) || isnan(r[j])){
                s->trees[arvore]->fitness = INFINITY;
                //s->trees[arvore]->root->print();
                break;
            }
            s->trees[arvore]->fitness += pow(r[j] - bancoDerivadas[j][data->variables + arvore], 2);
        }
        s->fitness += s->trees[arvore]->fitness;
        delete [] r;
    }



    return s->fitness;
}


double * RKDParser::calcDerivadaRegistroI(int registro){

    double *anterior, *proximo;
    double * derivadas = new double[data->prediction+data->variables];

    for(int i = 0; i < data->variables; i++){
        derivadas[i] = dataset[registro][i];
    }


    if(registro > 0 && registro < tamDataset -1){
        anterior = dataset[registro-1];
        proximo  = dataset[registro+1];

        for(int i = data->variables ; i <= data->variables + data->prediction - 1; i++){
            //cout << (proximo[i] - anterior[i])/(proximo[0] - anterior[0]);
            derivadas[i] = (proximo[i] - anterior[i])/(proximo[0] - anterior[0]);
            //cout << derivadas[i-data->variables];
        }

    }else if ( registro == 0){


        anterior = dataset[0];
        proximo  = dataset[1];

        for(int i = data->variables ; i <= data->variables + data->prediction - 1; i++){
            //cout << (proximo[i] - anterior[i])/(proximo[0] - anterior[0]) ;
            derivadas[i] = (proximo[i] - anterior[i])/(proximo[0] - anterior[0]);

        }
    }else if (registro == (tamDataset-1) ){

        anterior = dataset[tamDataset -2];
        proximo  = dataset[tamDataset -1];

        for(int i = data->variables; i < data->variables + data->prediction; i++){
           // cout << proximo[i] - anterior[i] ;
            derivadas[i] = (proximo[i] - anterior[i])/(proximo[0] - anterior[0]);
        }
    }

    for(int i = 0; i < data->variables + data->prediction ; i++){
        cout << derivadas[i] << " " ;
    }
//    cout << endl;
    return derivadas;

}

double * RKDParser::AuxLSDEvaluate(Subject* s, int model, double ** dat,int tam){
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
                stk.push(bancoDerivadas[i][(int)get<1>(t)]);
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
        b[i] = bancoDerivadas[i][model];
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

RKDParser::~RKDParser() {
    //dtor
}
