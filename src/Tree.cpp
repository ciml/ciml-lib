#include "Tree.h"

Tree::Tree(){
    No* n = new No(-1, 0, "<expr>", 0);
    grammar->derivate(n);
    root = n;
//    cout << "Criando arvore com raiz " << root->mask << endl;
    update();
    fitness = 0;
}
Tree::Tree(No* n){
    //ctor
    root = n;
    update();
    fitness = 0;
}

void Tree::print(){
    root->print();
}

No* Tree::subTree(){
    return nonTerminals.at(rand() % nonTerminals.size()); // returns a random subtree
};

No* Tree::targetSubTree(No* n){
    targetedNonTerminalCount(n, root);
    if(targetedNonTerminals.size() > 0)
        return targetedNonTerminals.at(rand() % targetedNonTerminals.size());
    return NULL;
};

void Tree::targetedNonTerminalCount(No* n, No* m){
    if(n->type == m->type &&
        n->value == m->value &&
        n->deep + m->height < conf->MAXDEEP &&
        m->deep + n->height < conf->MAXDEEP)
        targetedNonTerminals.push_back(m);
    for(No* p : m->sons)
        targetedNonTerminalCount(n, p);
}

void Tree::nonTerminalCount(No* n){
    if(n->type == NONTERMINAL){
        nonTerminals.push_back(n);
    }else{
        linearModel.push_back(make_tuple(n->type,n->value));
    }
    for(No* m : n->sons)
        nonTerminalCount(m);
}

Tree * Tree::clone(){
    No * n = clone(root);
    Tree * t = new Tree(n);
    t->update();
    return t;
}

void Tree::update(){
    nonTerminals.clear();
    linearModel.clear();
    update(root);
}

void Tree::update(No* n){
    if(n->type == NONTERMINAL){
        nonTerminals.push_back(n);
        n->height = 0;

        for(No* m : n->sons){
            m->deep = n->deep + 1;
            update(m);

            if(m->height + 1 > n->height)
                n->height = m->height + 1;
        }
    }
    else{
        n->height = 0;
        linearModel.push_back(make_tuple(n->type,n->value));
    }
}

No* Tree::clone(No* n){
    No* m = n->clone();

    for(No* f : n->sons){
        m->addSon(clone(f));
    }

    return m;
}

Tree::~Tree(){
    //dtor
     delete root;
     #ifdef debug
            cout << "deletando ARVORE" << endl;
            cin.get();
        #endif // debug
    nonTerminals.clear();
    targetedNonTerminals.clear();
    linearModel.clear();
//    if(leastSquare!=NULL)
//        delete [] leastSquare;

}

