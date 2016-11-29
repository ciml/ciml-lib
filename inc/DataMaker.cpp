#include "DataMaker.h"

DataMaker::DataMaker()
{
    //ctor
}

DataMaker::~DataMaker()
{
    //dtor
}

double f1(double* vars, int size){
    cout << vars[0]/((vars[1] + vars[2]) * vars[3]);
}

void DataMaker::MakeData(int variables, int predictions, int size, string masks, int groups, double tr, double te, double va, string sarq){
    stringstream datas;
    datas << "input/data/" << sarq << ".dat";

    ofstream out;
    out.open(datas.str(), std::ofstream::out | std::ofstream::app);

    out << variables << " " << predictions << " " << size << endl;
    out << masks << endl;

    for(int i = 0; i < size; i++){
        double* vars = new double[variables];
        for(int j = 0; j < variables; j++){
            vars[j] = (rand() % 100) / (rand() % 100);
            out << vars[j] << " ";
        }

        out << f1(vars, variables) << endl;
    }
    out.close();

//    vector<int> total;
//    vector<int> validation;
//    for(int i = 0; i < size; i++)
//        total.push_back(i);
//    for(int i = 0; i < size * va; i++){
//        int pos = rand() % total.size();
//        validation.push_back(total.at(pos));
//        total.erase(pos);
//    }
//
//    for(int i = 0; i < groups; i++){
//        stringstream grp;
//        grp << "input/data/" << sarq << ".grp";
//
//        ofstream out;
//        out.open(grp.str(), std::ofstream::out | std::ofstream::app);
//
//        cout <<
//    }
}
