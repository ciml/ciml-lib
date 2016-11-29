#ifndef DATAMAKER_H
#define DATAMAKER_H

#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <fstream>

using namespace std;

class DataMaker
{
    public:
        DataMaker();
        virtual ~DataMaker();

        void MakeData(int variables, int predictions, int size, string masks, int groups, double tr, double te, double va, string sarq);

    protected:

    private:
};

#endif // DATAMAKER_H
