#include "RK4LSDParser.h"

RK4LSDParser::RK4LSDParser()
{
    //ctor
}

double RK4LSDParser::Evaluate(Subject* s){
    RK4LSIndividuo * s1 = dynamic_cast<RK4LSIndividuo*>(s);
    s1->leastSquareSize = new int[conf->numTree];
    s1->leastSquare = new double*[conf->numTree];

    for(int i  = 0; i < conf->numTree; i++){
        s1->leastSquare[i] = NULL;
    }

    for(int i  = 0; i < conf->numTree; i++){
        delete [] AuxLSDEvaluate(s1,i,bancoDerivadas,tamDataset);
//        s1->trees[i]->print();
//        cout << s1->leastSquareSize[i] << endl;
//        cin.get();
    }

    double** rk4 = RKEvaluate(s1, dataset[0], data->variables + data->prediction, dataset[1][0]-dataset[0][0], tamDataset);
//    for(int i = 0; i < tamDataset; i++){
//        for(int j = 0; j < data->variables + data->prediction; j++)
//            cout << rk4[i][j] << " ";
//        cout << endl;
//    }
//    cin.get();



    for(int m = 0; m < conf->numTree; m++){
        s1->trees[m]->fitness = 0;
        for(int j = 0; j < tamDataset; j++){
            s1->trees[m]->fitness  += double(pow(dataset[j][data->variables + m] - rk4[j][data->variables + m], 2)/tamDataset);
        }
        if(isinf(s1->trees[m]->fitness) || isnan(s1->trees[m]->fitness))
            s1->trees[m]->fitness = INFINITY;
        s1->fitness += double(s1->trees[m]->fitness/conf->numTree);
//              cout << " ===== " << s1->trees[m]->fitness << endl;
    }

    for(int i = 0; i < tamDataset; i++)
        delete [] rk4[i];

    delete [] rk4;
    return s1->fitness;
}

double * RK4LSDParser::AuxLSDEvaluate(RK4LSIndividuo * s, int model, double ** dat,int tam){

    //LeastSquareIndividuo * s1 = s;
    //s->trees[model]->root->print();
//    cout << endl;
//    s->trees[model]->root->print();
//   cout << "________________________" << endl;
   //cin.get();

    /// Find base functions
    int sub = 1;
    for(int i = 0; i < s->trees[model]->linearModel.size(); i++)
        if((int)get<0>(s->trees[model]->linearModel.at(i)) == conf->leastSquareOperator)
            sub++;
    s->leastSquareSize[model] = sub;

//    cout <<"LeastSquareSize : " ;
//    cout << s->leastSquareSize[model];
//    cout << " TAM : " << tam << endl;
//    cin.get();

    double** subMatrix = new double*[tam];
    for(int i = 0; i < tam; i++){
        subMatrix[i] = new double[sub];
    }

    //cout << " subMatrix criada\n";
//    cin.get();

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

    //cout <<"Pilha operada \n";
//    cin.get();

//////    /// Solve Least Square
    QRDecomposition* qrDec = new QRDecomposition(subMatrix, tam, sub);
    double* b = new double[tam];
    for(int i = 0; i < tam; i++){
        b[i] = dat[i][data->variables + model];
        //cout << b[i] << endl;
    }

    //cout <<"Solve LeastSquare\n";
//    cin.get();

    double * coef = qrDec->solveLeastSquares(b, tam);
    s->leastSquareSize[model] = sub;

    //cout <<"Bora calcular o erro?\n";
//    cin.get();

//    s->trees[model]->leastSquare = NULL;
    if(coef != NULL){
        //cout << "deu" << endl;
    }
    else{
//        cout << "nao deu" << endl;
        coef = new double[sub];
        for(int j = 0; j < sub; j++){
            coef[j] = 1;
        }
    }

//    cout << "model " ;
//    s->trees[model]->root->print();
    /// Calculate Error
    double* r = new double[tam];
    for(int i = 0; i < tam; i++){
        r[i] = 0;
        for(int j = 0; j < sub; j++){
//            if(j == 0){
//            cout << "Coeficiente : " << s->leastSquare[model][j] <<endl;
//            cout << "valor : " << subMatrix[i][j] << endl;
//            //cin.get();
//            }
  //          cout << "\n[" << i  <<  ","<< model << "," << j <<"] ";
//            cout << " LS : " << double(coef[j]) ;
    //        cout <<" subMatriz " << double(subMatrix[i][j]) << endl;
            //cin.get();
            r[i] += double(coef[j]) * double(subMatrix[i][j]);
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

    //cout << "Todos calcs realizados\n";
   // cin.get();
   s->leastSquare[model] = coef;
   //delete [] coef;
    for(int i = 0; i < tam; i++){
        delete [] subMatrix[i];
    }
    delete [] subMatrix;

    delete [] b;
    delete qrDec;
//    cout << "vetor r : ";
//    for(int i = 0; i < tam; i++){
//                cout << r[i] << " ";
//    }
//    cout << endl;
//    cin.get();
//    delete []s->leastSquare;
//    delete []s->leastSquareSize;

   // cout << "Finalizado retornando \n";
    //cin.get();
    return r;
}

double** RK4LSDParser::RKEvaluate(RK4LSIndividuo* s, double* initial, int vars, double h, int iterations){
//    cout << "h = " << h << endl;
//    for(int i = 0; i < vars - 1; i++){
//        cout << "f" << i << " = ";
//        s->trees[i]->root->print();
//        cout << endl;
//    }

//    double k[vars][4];
    double k[vars][4];
    double** r = new double*[iterations];
    for(int i = 0; i < iterations; i++)
        r[i] = new double[vars];

    for(int i = 0; i < vars; i++){
        r[0][i] = initial[i];
//        cout << r[0][i] << " ";
    }
//    cout << endl;

    //assuming initial[0] is x and initial[1 ... vars - 1] is f0(x) ... fn(x)

    for(int i = 0; i < iterations - 1; i++){
        double* increments = new double[vars];
        r[i + 1][0] = r[i][0] + h;

        // k0
        increments[0] = 0;
        for(int j = 1; j < vars; j++)
            increments[j] = 0;
        for(int j = 1; j < vars; j++) // s: modelos j:especific model r[i]:variables increments:vector
            k[j][0] = h * AuxRKEvaluate(s, j, r[i], increments);

        // k1
        increments[0] = h/2.0;
        for(int j = 1; j < vars; j++)
            increments[j] = k[j][0]/2.0;
        for(int j = 1; j < vars; j++)
            k[j][1] = h * AuxRKEvaluate(s, j, r[i], increments);

        // k2
        increments[0] = h/2.0;
        for(int j = 1; j < vars; j++)
            increments[j] = k[j][1]/2.0;
        for(int j = 1; j < vars; j++)
            k[j][2] = h * AuxRKEvaluate(s, j, r[i], increments);

        // k3
        increments[0] = h;
        for(int j = 1; j < vars; j++)
            increments[j] = k[j][2];
        for(int j = 1; j < vars; j++)
            k[j][3] = h * AuxRKEvaluate(s, j, r[i], increments);

        delete [] increments;

//        for(int m = 1; m < vars; m++){
//            cout << "   ";
//            for(int l = 0; l < 4; l++)
//                cout << k[m][l] << " ";
//            cout << endl;
//        }


        for(int j = 1; j < vars; j++){
//            cout << r[i][j] + ((k[j][0] + 2*(k[j][1] + k[j][2]) + k[j][3]))/6.0 << " ";
            r[i + 1][j] = r[i][j] + ((k[j][0] + 2*(k[j][1] + k[j][2]) + k[j][3]))/6.0;
        }
//        cout << endl;

//        for(int j = 0; j < vars; j++)
//            cout << r[i + 1][j] << " ";
//        cout << endl;

//        cout << r[i + 1][0] << " ";
//
//        for(int j = 1; j < vars; j++){ // for every fn(x) calculate
//            k[0] = Evaluate(s, j, r[i], 0, 0);
//            k[1] = Evaluate(s, j, r[i], 0.5 * h, 0.5 * k[0]);
//            k[2] = Evaluate(s, j, r[i], 0.5 * h, 0.5 * k[1]);
//            k[3] = Evaluate(s, j, r[i], h, k[2]);
//
////            cout << "    For " << j << " " << k[0] << " " << k[1] << " " << k[2] << " " << k[3] << endl;
////            cout << "K[1...4] = " << k[0] << " " << k[1] << " " << k[2] << " " << k[3] << endl;
//
//            r[i + 1][j] = r[i][j] + (h * (k[0] + 2*(k[1] + k[2]) + k[3]))/6.0;
//            cout << r[i + 1][j] << " ";
//        }
//        cout << endl;
    }

//    for(int i = 0; i < iterations; i++)
//        delete [] r[i];
//    delete [] r;
    return r;
}

double RK4LSDParser::AuxRKEvaluate(RK4LSIndividuo* s, int model, double* dat, double* increments){
    stack<double> stk;
    for(tuple<double, double> t : s->trees[model - 1]->linearModel){// -1 because model is the n-1 tree of s and n y ou fn(x)
//        cout << get<0>(t)<< " , " << get<1>(t) << endl;
        int var = (int)get<0>(t);
        if( var == conf->constant ) { // constant
            stk.push(get<1>(t));
        }
        else if( var == conf->variable ){ // variable;
            stk.push(dat[(int)get<1>(t)] + increments[(int)get<1>(t)]);
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
            //programOperators
        }
    }

    double result = 0;
    int j = s->leastSquareSize[model - 1] - 1;
    while(!stk.empty()){
//        result += stk.top();
        result += stk.top() * s->leastSquare[model - 1][j];
        stk.pop();
        j--;
    }

    return result;
}

void RK4LSDParser::calcDerivadas(){
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

double * RK4LSDParser::calcDerivadaRegistroI(int registro){

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

void RK4LSDParser::setDataSet(double ** x, int tam){
    dataset = x;
    tamDataset = tam;
    calcDerivadas();
}

RK4LSDParser::~RK4LSDParser()
{
    //dtor
}
