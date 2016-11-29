#include "No.h"

No::No(){
    //ctor
}

No::No(double t, double v, string m, int h){
    type = t;
    value = v;
    mask = m;
    deep = h;
}

void No::print(int format){
    for(int i = 0; i < format; i++)
        cout << " ";
    cout << height << " " << mask << endl;

    for(No* n : sons)
        n->print(format + 1);
}

void No::print(){
    if(type != -1)
        cout << mask << " ";

    for(No* n : sons)
        n->print();
}

void No::addSon(No* n){
    sons.push_back(n);
}

void No::erase(){
    for(No* n : sons)
        delete n;
    sons.clear();
}

No* No::clone(){
    No* n = new No(type, value, mask, deep);
    return n;
}

No::~No(){
    #ifdef debug
        cout << "DELETANDO NO" << endl;
        cin.get();
    #endif // debug

    for(No* n : sons)
        delete n;
    sons.clear();

}

void No::destrutor(No * p){
    if(p == NULL){
        delete p;
    }else{
        for(int i = 0; i < p->sons.size(); i++){
            p->sons[i]->father = NULL;
            destrutor(p->sons[i]);
        }
    }
}
