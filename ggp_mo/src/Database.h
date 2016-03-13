#ifndef DATABASE_H_INCLUDED
#define DATABASE_H_INCLUDED
#include "Gramatica.h"
#include "Configures.h"

using namespace std;

class Database{
    public:
        int countVar; //quantidade de variaveis no arquivo texto
        std::string* vars;
        int countTestValues;
        double** values; //totalResultados x totalVariaveis
        int countResults;
        double* results;
        int* training;
        int trainCount;
        int* test;
        int testCount;
        int* validation;
        int validCount;

        Database(string base, string groups);
        void loadBase(string base);
        void loadGroups(string groups);
        void print();
        void selectData();
        double* getVars(int position);
};

extern Database* data;

#endif // DATABASE_H_INCLUDED
