#ifndef DATAMAKER_INCLUDED
#define DATAMAKER_INCLUDED

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string>
#include <math.h>
#include <vector>

using namespace std;

void makeData(string base, string groups, int dataSize, int variables, string vars){
    ofstream arq;
    arq.open(base);
    arq << variables << "    " << dataSize << endl;
    arq << vars << endl;

    for(int i = 0; i < dataSize; i++){
        double a, b, c, d, e, res;
        a = rand() % 100;
        b = rand() % 100;
        c = rand() % 100;
        d = rand() % 100;
        e = rand() % 100;
//        res = a + b * c + (d / e); //grupo1
//        res = a * b * c * d * e; //grupo2
//        res = a < b ? c + d : d - e; //grupo3
//        res = .1 * (a + b) + .3 * c + .4 * (d + e); //grupo4
        res = a < b ? c < d ? 0 : 1 : d < e ? 0 : 1; //grupo5
        if(!isinf(res) && !isnan(res)){
            arq << a << " " << b << " " << c << " " << d << " " << e << " ";
            arq << res << endl;
        }
        else
            i--;
    }

    arq.close();

    vector<int> all;
    int i = 0;
    while(all.size() < dataSize)
        all.push_back(i++);
    vector<int> test;
    vector<int> train;
    vector<int> valid;
    int pos;
    for(int i = 0; i < dataSize * .9; i++){
        pos = rand() % all.size();
        train.push_back(all.at(pos));
        swap(all.at(pos), all.at(all.size() - 1));
        all.pop_back();
    }
    for(int i = 0; i < dataSize * .05; i++){
        pos = rand() % all.size();
        test.push_back(all.at(pos));
        swap(all.at(pos), all.at(all.size() - 1));
        all.pop_back();
    }
    for(int i = 0; i < dataSize * .05; i++){
        pos = rand() % all.size();
        valid.push_back(all.at(pos));
        swap(all.at(pos), all.at(all.size() - 1));
        all.pop_back();
    }


    arq.open(groups);
    arq << dataSize * .9 << "   " << dataSize * .05 << "    " << dataSize * .05 << "    " << 1<< "    " << 1 << endl;
    arq << "treino" << endl;
    for(int i : train)
        arq << i << " ";
    arq << endl << "teste" << endl;
    for(int i : test)
        arq << i << " ";
    arq << endl << "valida" << endl;
    for(int i : valid)
        arq << i << " ";
    arq << endl;
}

#endif // DATAMAKER_INCLUDED
