#include "Database.h"

Database* data;

Database::Database(string arq){
    loadBase(arq);
}

Database::~Database(){
    //dtor
}

void Database::print(){
    for(int i = 0; i < size; i++){
        int j = 0;
        for(j; j < variables; j++)
            cout << values[i][j] << " ";
        cout << "| ";
        for(j; j < variables + prediction; j++)
            cout << values[i][j] << " ";
        cout << endl;
    }
}

void Database::loadBase(string base){
    ifstream arq;

    arq.open(base);

    arq >> variables >> prediction >> size;

    map = new string[variables + prediction];
    for(int i = 0; i < variables + prediction; i++){
        arq >> map[i];
    }

    values = new double*[size];
    for(int i = 0; i < size; i++){
        values[i] = new double[variables + prediction];

        for(int j = 0; j < variables + prediction; j++){
            arq >> values[i][j];
        }
    }

    arq.close();
}

void Database::loadGroup(string group){
    ifstream arq;

    arq.open(group);

    arq >> totalTraining >> totalTest >> totalValidation;

    training = new double*[totalTraining];
    test = new double*[totalTest];
    validation = new double*[totalValidation];

    int p;
    for(int i = 0; i < totalTraining; i++){
        arq >> p;
        training[i] = values[p];
    }

    for(int i = 0; i < totalTest; i++){
        arq >> p;
        test[i] = values[p];
    }

    for(int i = 0; i < totalValidation; i++){
        arq >> p;
        validation[i] = values[p];
    }

    arq.close();
}

void Database::makeGroup(string group, int tr, int ts, int v){
    ofstream arq;

    arq.open(group);
    arq << round((tr * size)/100) << "  " << round((ts * size)/100) << "    " << round((v * size)/100) << endl;

    /** TODO: fix quantities **/

    vector<int> ordered;

    while(ordered.size() < size)
        ordered.push_back(ordered.size());

    int p;
    while(!ordered.empty()){
        p = rand() % ordered.size();
        arq << ordered.at(p) << "   ";
        ordered.erase(ordered.begin() + p);
    }

    arq.close();
}

double* Database::getRegister(int position){
    return position < size ? values[position] : NULL;
}

double** Database::getTraining(){
    return training;
}

double** Database::getTest(){
    return test;
}

double** Database::getValidation(){
    return validation;
}

double** Database::getValues(){
    return values;
}

double* Database::getTraining(int position){
    return position < totalTraining ? training[position] : NULL;
}

double* Database::getTest(int position){
    return position < totalTest ? test[position] : NULL;
}

double* Database::getValidation(int position){
    return position < totalValidation ? validation[position] : NULL;
}

int Database::getCode(string x){
    for(int i = 0; i < variables + prediction; i++){
        if(strcmp(map[i].c_str(), x.c_str()) == 0){
            return i;
        }
    }

    return -1;
}
