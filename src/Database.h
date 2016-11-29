#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <cmath>
#include "Configures.h"

using namespace std;

class Database{
    public:
        Database(string arq);
        virtual ~Database();

        void loadBase(string base);
        void loadGroup(string group);
        void makeGroup(string group, int tr, int ts, int v);
        int getCode(string x);

        double* getRegister(int position);

        double* getTraining(int position);
        double* getTest(int position);
        double* getValidation(int position);

        double** getTraining();
        double** getTest();
        double** getValidation();
        double** getValues();

        void print();


//    protected:

//    private:
        int variables;
        int prediction;
        int size;
        string* map;
        double** values;

        int totalTraining, totalValidation, totalTest;
        double** training;
        double** validation;
        double** test;
};

extern Database* data;

#endif // DATABASE_H
