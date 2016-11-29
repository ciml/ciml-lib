#include "SimpleParser.h"

SimpleParser::SimpleParser() {

}

string SimpleParser::nameParser(){
    return "SimpleParser";
}

double SimpleParser::Evaluate(Subject* s){
    //return  1;
    double r;
    s->fitness = 0;
    for(int arvore = 0; arvore < conf->numTree;arvore++){
        s->trees[arvore]->fitness = 0;
        for( int j = 0; j <tamDataset; j++){ // para todos os dados do conjunto de treinamento
            //break;
            r = AuxEvaluate(s,arvore,dataset[j]);
            if(isinf(r) || isnan(r)){
                s->trees[arvore]->fitness = INFINITY;
                s->trees[arvore]->root->print();
                break;
            }
            double fit = pow(r - dataset[j][data->variables + arvore], 2);
            s->trees[arvore]->fitness += fit;
        }
        s->fitness += s->trees[arvore]->fitness;
    }
    return s->fitness;

}

double SimpleParser::Operate(int opType, int opValue, double a, double b, double c){
    double r;
    if(opType == conf->bynaryArithmeticOperators){
        if(opValue == 0)
            r = a + b;
        else if(opValue == 1)
            r = a - b;
        else if(opValue == 2)
            r = a * b;
        else if(opValue == 3)
            r = a / b;
        else if(opValue == 4)
            r = pow(a, b);
    }
    else if(opType == conf->unaryArithmeticOperators){
        if(opValue == 0)
                r = log(a);
        else if(opValue == 1)
                r = exp(a);
        else if(opValue == 2)
                r = sqrt(a);
        else if(opValue == 3)
                r = cos(a);
        else if(opValue == 4)
                r = sin(a);
        else if(opValue == 5)
                r = tan(a);
        else if(opValue == 6)
                r = acos(a);
        else if(opValue == 7)
                r = asin(a);
        else if(opValue == 8)
                r = atan(a);
    }
    else if(opType == conf->binaryLogicalOperators){
        if(opValue == 0)
                r = a && b;
        else if(opValue == 1)
                r = a || b;
        else if(opValue == 2)
                r = bool(a) ^ bool(b);
    }
    else if(opType == conf->unaryLogicalOperators){
        if(opValue == 0)
                r = !bool(a);
        else if(opValue == 1)
                r = 1;
        else if(opValue == 2)
                r = 0;
    }
    else if(opType == conf->comparisonOperators){
        if(opValue == 0)
                r = a < b;
        else if(opValue == 1)
                r = a <= b;
        else if(opValue == 2)
                r = a == b;
        else if(opValue == 3)
                r = a >= b;
        else if(opValue == 4)
                r = a > b;
        else if(opValue == 5)
                r = a != b;
    }

    if(isnan(r) || isinf(r)){
//        cout << "      " << opType << " " << opValue << " " << a << " " << b << " " << r << endl;
        return NULL;
    }
//    cout << "      " << r << endl;
    return r;
}

double SimpleParser::AuxEvaluate(Subject* s, int model, double* dat){

    //s->trees[model]->root->print();
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
    double resp = 0;
//    if(stk.size()!=1){
//        cout << "DEUM MT MERDA" << endl;
//        cout << "TAM " << stk.size() << endl;
//        cin.get();
//    }
     resp += stk.top();
   while (!stk.empty())
  {
     //resp += stk.top();
     stk.pop();
  }
//    if(stk.size()!=0){
//        //cout << "Deu ruim no parser";
//        //cout << "Tam da pilha " << stk.size();
//        cin.get();
//    }
    return resp;
}

SimpleParser::~SimpleParser() {
    //dtor
}


