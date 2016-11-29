#include "GenericParser.h"

GenericParser::GenericParser() {
    //ctor
}

double GenericParser::Evaluate(Subject* s){

}

double* GenericParser::Operate(int tam, int opType, int opValue, double* a, double* b, double* c){
    double* r = new double[tam];
    if(opType == conf->bynaryArithmeticOperators){
        if(opValue == 0)
            for(int i = 0; i < tam; i++)
                r[i] = a[i] + b[i];
        else if(opValue == 1)
            for(int i = 0; i < tam; i++)
                r[i] = a[i] - b[i];
        else if(opValue == 2)
            for(int i = 0; i < tam; i++)
                r[i] = a[i] * b[i];
        else if(opValue == 3)
            for(int i = 0; i < tam; i++){
                r[i] = a[i] / b[i];
                if(isnan(r[i]) || isinf(r[i])){
                    delete [] r;
                    return NULL;
                }
            }
        else if(opValue == 4){
            for(int i = 0; i < tam; i++){
                r[i] = pow(a[i], b[i]);
                if(isnan(r[i]) || isinf(r[i])){
                    delete [] r;
                    return NULL;
                }
            }
        }
    }
    else if(opType == conf->unaryArithmeticOperators){
        if(opValue == 0)
            for(int i = 0; i < tam; i++)
                r[i] = log(a[i]);
        else if(opValue == 1)
            for(int i = 0; i < tam; i++)
                r[i] = exp(a[i]);
        else if(opValue == 2)
            for(int i = 0; i < tam; i++)
                r[i] = sqrt(a[i]);
        else if(opValue == 3)
            for(int i = 0; i < tam; i++)
                r[i] = cos(a[i]);
        else if(opValue == 4)
            for(int i = 0; i < tam; i++)
                r[i] = sin(a[i]);
        else if(opValue == 5)
            for(int i = 0; i < tam; i++)
                r[i] = tan(a[i]);
        else if(opValue == 6)
            for(int i = 0; i < tam; i++)
                r[i] = acos(a[i]);
        else if(opValue == 7)
            for(int i = 0; i < tam; i++)
                r[i] = asin(a[i]);
        else if(opValue == 8)
            for(int i = 0; i < tam; i++)
                r[i] = atan(a[i]);
    }
    else if(opType == conf->binaryLogicalOperators){
        if(opValue == 0)
            for(int i = 0; i < tam; i++)
                r[i] = a[i] && b[i];
        else if(opValue == 1)
            for(int i = 0; i < tam; i++)
                r[i] = a[i] || b[i];
        else if(opValue == 2)
            for(int i = 0; i < tam; i++)
                r[i] = bool(a[i]) ^ bool(b[i]);
    }
    else if(opType == conf->unaryLogicalOperators){
        if(opValue == 0)
            for(int i = 0; i < tam; i++)
                r[i] = !bool(a[i]);
        else if(opValue == 1)
            for(int i = 0; i < tam; i++)
                r[i] = 1;
        else if(opValue == 2)
            for(int i = 0; i < tam; i++)
                r[i] = 0;
    }
    else if(opType == conf->comparisonOperators){
        if(opValue == 0)
            for(int i = 0; i < tam; i++)
                r[i] = a[i] < b[i];
        else if(opValue == 1)
            for(int i = 0; i < tam; i++)
                r[i] = a[i] <= b[i];
        else if(opValue == 2)
            for(int i = 0; i < tam; i++)
                r[i] = a[i] == b[i];
        else if(opValue == 3)
            for(int i = 0; i < tam; i++)
                r[i] = a[i] >= b[i];
        else if(opValue == 4)
            for(int i = 0; i < tam; i++)
                r[i] = a[i] > b[i];
        else if(opValue == 5)
            for(int i = 0; i < tam; i++)
                r[i] = a[i] != b[i];
    }

    return r;
}

double * GenericParser::Evaluate(Subject* s, int model, double ** dat,int tam){
    stack<double*> stk;
    for(tuple<double, double> t : s->trees[model]->linearModel){
        int var = (int)get<0>(t);
        if( var == conf->constant ) { // constant
            double* c = new double[tam];
            for(int d = 0; d < tam; d++)
                c[d] = get<1>(t);
            stk.push(c);
        }
        else if( var == conf->variable ){ // variable
            double* c = new double[tam];
            for(int d = 0; d < tam; d++)
                c[d] = dat[d][(int)get<1>(t)];
            stk.push(c);
        }
        else if( var == conf->bynaryArithmeticOperators){ // bynaryArithmeticOperators
            double* b = stk.top();
            stk.pop();
            double* a = stk.top();
            stk.pop();
            stk.push(Operate(tam, get<0>(t), get<1>(t), a, b));
            delete [] a;
            delete [] b;
        }
        else if( var ==  conf->unaryArithmeticOperators){ // unaryArithmeticOperators
            double* a = stk.top();
            stk.pop();
            stk.push(Operate(tam, get<0>(t), get<1>(t), a));
            delete [] a;
        }
        else if( var == conf->binaryLogicalOperators){ // binaryLogicalOperators
            double* b = stk.top();
            stk.pop();
            double* a = stk.top();
            stk.pop();
            stk.push(Operate(tam, get<0>(t), get<1>(t), a, b));
            delete [] a;
            delete [] b;
        }
        else if( var == conf->unaryLogicalOperators){ // unaryLogicalOperators
            double* a = stk.top();
            stk.pop();
            stk.push(Operate(tam, get<0>(t), get<1>(t), a));
            delete [] a;
        }
        else if( var == conf->comparisonOperators){ // comparisonOperators
            double* b = stk.top();
            stk.pop();
            double* a = stk.top();
            stk.pop();
            stk.push(Operate(tam, get<0>(t), get<1>(t), a, b));
            delete [] a;
            delete [] b;
        }
        else if( var == conf->programOperators){
//            programOperators
        }

        if(stk.top() == NULL){
            while(!stk.empty()){
                delete [] stk.top();
                stk.pop();
            }
            return NULL;
        }
    }

    return stk.top();

//    double * vec = new double[tam];
//    for(int i = 0; i < tam; i++)
//        vec[i] = rand() % 100;
//
//    return vec;

//
//    for(int i = 0; i < tam; i++){
//
//        stack<tuple<double, double> > stk;
//
//        for(tuple<double, double> t : s->trees[0]->linearModel){
//            int var = (int)get<0>(t);
//
//                if( var = conf->constant ) { // constant
//                    stk.push(t);
//                }else if( var = conf->variable ){ // variable
//                    stk.push(make_tuple(conf->constant, dat[i][(int)get<1>(t)]));
//                }else if( var = conf->bynaryArithmeticOperators){ // bynaryArithmeticOperators
//                    tuple<double, double> b = stk.top();
//                    stk.pop();
//                    tuple<double, double> a = stk.top();
//                    stk.pop();
//                    stk.push(Operate(a, b, t));
//                }else if( var =  conf->unaryArithmeticOperators){ // unaryArithmeticOperators
//                    tuple<double, double> a = stk.top();
//                    stk.pop();
//                    stk.push(Operate(a, t));
//                }else if( var = conf->binaryLogicalOperators){ // binaryLogicalOperators
//                    tuple<double, double> b = stk.top();
//                    stk.pop();
//                    tuple<double, double> a = stk.top();
//                    stk.pop();
//                    stk.push(Operate(a, b, t));
//                }else if( var = conf->unaryLogicalOperators){ // unaryLogicalOperators
//                    tuple<double, double> a = stk.top();
//                    stk.pop();
//                    stk.push(Operate(a, t));
//                }else if( var = conf->comparisonOperators){ // comparisonOperators
//                    tuple<double, double> b = stk.top();
//                    stk.pop();
//                    tuple<double, double> a = stk.top();
//                    stk.pop();
//                    stk.push(Operate(a, b, t));
//                }else if( var = conf->programOperators){
//                    //programOperators
//                }
//
//        }
//        std::cout << get<1>(stk.top()) << std::endl;
//        vec[i] = get<1>(stk.top());
//    }

//    double * vec = new double[tam];
//    stack<tuple<double, double> >* stk = new stack<tuple<double, double> >[tam];
//
//    for(tuple<double, double> t : s->trees[0]->linearModel){
//        int var = (int)get<0>(t);
//        if( var == conf->constant ) { // constant
//            for(int j = 0; j < tam; j++)
//                stk[j].push(t);
//        }else if( var == conf->variable ){ // variable
//            for(int j = 0; j < tam; j++)
//                stk[j].push(make_tuple(conf->constant, dat[j][(int)get<1>(t)]));
//        }else if( var == conf->bynaryArithmeticOperators){ // bynaryArithmeticOperators
//            for(int j = 0; j < tam; j++){
//                tuple<double, double> b = stk[j].top();
//                stk[j].pop();
//                tuple<double, double> a = stk[j].top();
//                stk[j].pop();
//                stk[j].push(Operate(a, b, t));
//            }
//        }else if( var == conf->unaryArithmeticOperators){ // unaryArithmeticOperators
//            for(int j = 0; j < tam; j++){
//                tuple<double, double> a = stk[j].top();
//                stk[j].pop();
//                stk[j].push(Operate(a, t));
//            }
//        }else if( var == conf->binaryLogicalOperators){ // binaryLogicalOperators
//            for(int j = 0; j < tam; j++){
//                tuple<double, double> b = stk[j].top();
//                stk[j].pop();
//                tuple<double, double> a = stk[j].top();
//                stk[j].pop();
//                stk[j].push(Operate(a, b, t));
//            }
//        }else if( var == conf->unaryLogicalOperators){ // unaryLogicalOperators
//            for(int j = 0; j < tam; j++){
//                tuple<double, double> a = stk[j].top();
//                stk[j].pop();
//                stk[j].push(Operate(a, t));
//            }
//        }else if( var == conf->comparisonOperators){ // comparisonOperators
//            for(int j = 0; j < tam; j++){
//                tuple<double, double> b = stk[j].top();
//                stk[j].pop();
//                tuple<double, double> a = stk[j].top();
//                stk[j].pop();
//                stk[j].push(Operate(a, b, t));
//            }
//        }else if( var == conf->programOperators){
//            //programOperators
//        }
//    }
//    for(int j = 0; j < tam; j++){
//        std::cout << get<1>(stk[j].top()) << std::endl;
//        vec[j] = get<1>(stk[j].top());
//    }
}

GenericParser::~GenericParser() {
    //dtor
}
