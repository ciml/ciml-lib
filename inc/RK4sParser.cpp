#include "RK4sParser.h"

RK4sParser::RK4sParser()
{
    //ctor
}
string RK4sParser::nameParser(){
    return "RK4sParser";
}

void RK4sParser::setDataSet(double ** x,int tam){
    tamDataset = tam;
    dataset = x;
}


double RK4sParser::Evaluate(Subject* s){
    RK4LSIndividuo * s1 = dynamic_cast<RK4LSIndividuo*>(s);

    double ** serieRK = RKEvaluate(s1,dataset[0],data->prediction + data->variables,dataset[1][0] - dataset[0][0],tamDataset);
    s1->fitness  = 0;
    for(int m = 0; m < conf->numTree; m++){
                    s1->trees[m]->fitness = 0;
                    for(int j = 0; j < tamDataset; j++){
                        s1->trees[m]->fitness  += double(pow(dataset[j][data->variables + m] - serieRK[j][data->variables + m], 2)/tamDataset);
                    }
                    if(isinf(s1->trees[m]->fitness) || isnan(s1->trees[m]->fitness))
                        s1->trees[m]->fitness = INFINITY;
                    s1->fitness += double(s1->trees[m]->fitness/conf->numTree);
    //                cout << " ===== " << s1->trees[m]->fitness << endl;
    }

//    for(int i = 0 ; i < tamDataset; i++){
//        for(int j = 0 ; j < data->prediction + data->variables; j++){
//            cout << serieRK[i][j] << " ";
//        }
//        cout << endl;
//
//    }
//    cin.get();


    for(int i = 0 ; i < tamDataset; i++)
        delete [] serieRK[i];
    delete [] serieRK;
    return s1->fitness;
}

double** RK4sParser::RKEvaluate(Subject* s, double* initial, int vars, double h, int iterations){
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
            k[j][0] = h * AuxEvaluate(s, j, r[i], increments);

        // k1
        increments[0] = h/2.0;
        for(int j = 1; j < vars; j++)
            increments[j] = k[j][0]/2.0;
        for(int j = 1; j < vars; j++)
            k[j][1] = h * AuxEvaluate(s, j, r[i], increments);

        // k2
        increments[0] = h/2.0;
        for(int j = 1; j < vars; j++)
            increments[j] = k[j][1]/2.0;
        for(int j = 1; j < vars; j++)
            k[j][2] = h * AuxEvaluate(s, j, r[i], increments);

        // k3
        increments[0] = h;
        for(int j = 1; j < vars; j++)
            increments[j] = k[j][2];
        for(int j = 1; j < vars; j++)
            k[j][3] = h * AuxEvaluate(s, j, r[i], increments);

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



double RK4sParser::AuxEvaluate(Subject* s, int model, double* dat, double* increments){
//    for(int i = 0; i < 3; i++)
//        cout << increments[i] << " e ";
//    cout << endl;

    stack<double> stk;
    for(tuple<double, double> t : s->trees[model - 1]->linearModel){// -1 because model is the n-1 tree of s and n y ou fn(x)
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
    return stk.top();
}
RK4sParser::~RK4sParser()
{
    //dtor
}
