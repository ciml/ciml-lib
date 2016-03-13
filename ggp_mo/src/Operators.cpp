#include "Operators.h"

Operators* op;

void Operators::swap(No* p, No* n, Tree* a, Tree* b){
    if(p->fatherSon == -1){//é raiz
        if(n->fatherSon == -1){
            No* q = a->root;
            a->root = b->root;
            b->root = q;
        }
        else{
            n->father->sons[n->fatherSon] = a->root;
            a->setTree(n);
        }
    }
    else if(n->fatherSon == -1){//é raiz
        if(p->fatherSon == -1){
            No* q = a->root;
            a->root = b->root;
            b->root = q;
        }
        else{
            p->father->sons.at(p->fatherSon) = b->root;
            b->setTree(p);
        }
    }else{
//           cout << "pp:\n";
//           p->father->print(0);
//           cout << "nn:\n";
//           n->father->print(0);
//           cout << "P:\n";
//           p->father->sons[p->fatherSon]->print(0);
//           cout << "N:\n";
//           n->father->sons[n->fatherSon]->print(0);
        No* aux = p->father->sons[p->fatherSon];
        p->father->sons[p->fatherSon] = n->father->sons[n->fatherSon];
        n->father->sons[n->fatherSon] = aux;
//           cout << "P:\n";
//           p->father->sons[p->fatherSon]->print(0);
//           cout << "N:\n";
//           n->father->sons[n->fatherSon]->print(0);

//           No auxP = *p->father->sons[p->fatherSon];
//           No auxN = *n->father->sons[n->fatherSon];
//           cout << "auxP:\n";
//           auxP.print(0);
//           cout << "auxN:\n";
//           auxN.print(0);
//           cout << "P:\n";
//           p->father->sons[p->fatherSon]->print(0);
//           cout << "N:\n";
//           n->father->sons[n->fatherSon]->print(0);
//           *p->father->sons[p->fatherSon] = auxN;
//           *n->father->sons[n->fatherSon] = auxP;
//           cout << "auxP:\n";
//           auxP.print(0);
//           cout << "auxN:\n";
//           auxN.print(0);
//           cout << "P:\n";
//           p->father->sons[p->fatherSon]->print(0);
//           cout << "N:\n";
//           n->father->sons[n->fatherSon]->print(0);

//           cout << "p->father->sons[p->fatherSon]:\n";
//           p->father->sons[p->fatherSon]->print(0);
//           cout << "n->father->sons[n->fatherSon]:\n";
//           n->father->sons[n->fatherSon]->print(0);
//           No auxP = *p->father->sons[p->fatherSon];
//           No auxN = *n->father->sons[n->fatherSon];
//           *p->father->sons[p->fatherSon] = auxN;
//           *n->father->sons[n->fatherSon] = auxP;
//           cout << "new p->father->sons[p->fatherSon]:\n";
//           p->father->sons[p->fatherSon]->print(0);
//           cout << "new n->father->sons[n->fatherSon]:\n";
//           n->father->sons[n->fatherSon]->print(0);

//           aux.print(0);
//           cout << "aux2:\n";
//           aux.print(0);
    }
};

int Operators::randomI(int x){
    if(x <= 0){
        return 0;
    }
    return (rand() % x);
};

void Operators::Mutate(Tree* t){
    int x = randomI(t->countNT());
//       cout << "Mutar x = " << x << endl;
    No* n = t->getNo(x);
//       n->print(0);
    if(n != NULL){
        n->clean();
//           for(int i = 0; i < n->countSons; i++)
//               delete n->sons.at(i);
        gram->criaNo(n, n->deep);
        t->refresh();
//           cout << "Mut ok\n";
    }
    else{
//           cout << "Mut fail\n";
    }
};

void Operators::Cross(Tree* a, Tree* b, Tree* c, Tree* d){
//           cout << "Imprimindo A:\n";
//           a->print();
//           c->setTree(a->root->clone());
//           cout << "Imprimindo C:\n";
//           c->print();
//           cout << "Imprimindo B:\n";
//           b->root->print(0);
//           d->setTree(b->root->clone());
//           cout << "Imprimindo D:\n";
//           d->print();

        No* cRoot = new No();
        gram->criaNoGuiado(cRoot, a->root);
        No* dRoot = new No();
        gram->criaNoGuiado(dRoot, b->root);

        c->setTree(cRoot);
        d->setTree(dRoot);

//           a->print();
//           b->print();
//           c->print();
//           d->print();

        int sorted = randomI(c->countNT());
        No* n = c->getNo(sorted);
//           cout << "Sorteado:\n";
//           n->print(0);
        if(n != NULL){
            int candidates = d->countNTHigh(n->value, n->deep, n->highSubTree());
            if(candidates > 0){
                int candidate = randomI(candidates);
                No* p = d->getNoHigh(candidate, n->value, n->deep, n->highSubTree());
//                   cout << "Compativel:\n";
                if(p!= NULL){
//                       p->print(0);
//                       cout << n->mask << " " << p->mask;
                    swap(n, p, c, d);
//                       cout << "Cross ok\n";
                }
                else{
//                       cout << "Cross fail\n";
                }
            }
            else{
//                   cout << "Cross fail\n";
            }
        }
        else{
//               cout << "Cross fail\n";
        }
        c->refresh();
        d->refresh();
};

