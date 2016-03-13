#include "No.h"

using namespace std;

No::No(){
    countSons = 0;
    fatherSon = -1;
    countSons = 0;
    high = 0;
    deep = 0;
    father = NULL;
//    sons = new No*[MAX_SONS];
//    sons.resize(10);
};

No::~No(){
//    cout << "deletando " << mask << " " << countSons << " " << sons.size() << " " << fatherSon << endl;
    if(type == 0){
        for(int i = 0; i < countSons; i++){
            delete sons.at(i);
        }
    }
};

No* No::clone(){
////    No* n = new No(*this);
////    sons.at(0)->print(0);
//    No* n = new No();
//    n->type = type;
//    n->value = value;
////    cout << type << endl << value << endl;
//    n->countSons = countSons;
//    n->mask = mask;
////    n->father = father;
//    n->fatherSon = fatherSon;
//    n->sons.resize(sons.size());
////    cout << "aqui: " << countSons << n->countSons << endl;
////    cout << type << endl << value << endl;
////    print(0);
//    for(int i = 0; i < countSons; i++){
////        n->sons[i]->father = n;
//        n->sons[i] = sons[i]->clone();
////        n->sons.push_back(sons.at(i)->clone());
//    }
//    return n;
};

No* No::clone(No* n){
//    cout << "Clonando " << mask << " " << countSons << " " << sons.size() << " " << fatherSon << endl;
    No* p = new No();
    p->setNo(type, value, mask);
    p->setFather(n, fatherSon);
    p->sons.resize(sons.size());
    for(int i = 0; i < countSons; i++){
        p->setSon(sons.at(i)->clone(p));
    }
    return p;
};

void No::clean(){
    for(int i = 0; i < countSons; i++)
        delete sons.at(i);
    countSons = 0;
};

void No::setFather(No* n, int fs){
    father = n;
    fatherSon = fs;
};

void No::setNo(double t, double v, string m){
    type = t;
    value = v;
    mask = m;
};

void No::cpyNo(No* n){
//    cout << "coipando no " << n->mask;
//    type = n->type;
//    value = n->value;
//    countSons = n->countSons;
//    cout << " " << type << " " << value << " " << countSons << endl;
//    mask = n->mask;
//    for(int i = 0; i < n->countSons; i++){
//        sons[i]->cpyNo(n->sons[i]);
//    }
};

void No::setSon(No* n){
//    sons.resize(countSons + 1);
    sons.at(countSons) = n;
//    sons.push_back(n);
    countSons++;
};

int No::highSubTree(){
    if(countSons == 0){
        return 0;
    }else{
        int alt = 0;
        for(int i = 0; i < countSons; i++){
            int aux = sons.at(i)->highSubTree();
            if(aux > alt){
                alt = aux;
            }
        }
        return alt + 1;
    }
};

int No::NTValue(int v){
    int total = 0;
    if(type == 0){
        if(value == v){
            total++;
        }
        for(int i = 0; i < countSons; i++)
                total += sons.at(i)->NTValue(v);
    }
    return total;
};

int No::NTValueHigh(int v, int n, int sub){
    int total = 0;
    if(type == 0){
        if(value == v){
            if(n + highSubTree() <= conf->treeHigh && deep + sub <= conf->treeHigh){
                total++;
            }
        }
        for(int i = 0; i < countSons; i++)
                total += sons.at(i)->NTValueHigh(v, n, sub);
    }
    return total;
};

int No::countAll(){
    int total = 1;
    for(int i = 0; i < countSons; i++)
        total += sons.at(i)->countAll();
    return total;
};

int No::countNT(){
    int total = 0;
    if(type == 0){
        total++;
        for(int i = 0; i < countSons; i++)
            total += sons.at(i)->countNT();
    }
    return total;
};

int No::countT(){
    int total = 0;
    if(type == 0){
        for(int i = 0; i < countSons; i++){
            total += sons.at(i)->countT();
        }
    }else{
        total++;
    }
    return total;
};

void No::print(int tab){
//    if(type != 0) cout << mask << " ";
    for(int i = 0; i < tab; i++) {cout << " ";}
    cout << deep << " " << mask << " " << endl;
//    cout << tab << " " << mask << "{" << type << "," << value << "}\n";
    tab++;
    for(int i = 0; i < countSons; i++){
        sons.at(i)->print(tab);
    }
};

void No::print(){
    if(type == 0){
        if(value == 0 && countSons > 1){
//            cout << "( ";
            for(int i = 0; i < countSons; i++){
                sons.at(i)->print();
            }
//            cout << ")";
        }
        else{
            for(int i = 0; i < countSons; i++){
                sons.at(i)->print();
            }
        }
    }else{
        cout << mask << " ";
    }
};
