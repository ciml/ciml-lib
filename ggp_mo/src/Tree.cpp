#include "Tree.h"

Tree::Tree(){
//    temp = new No*[MAX];
    high = 0;
    counter = 0;
    tempCounter = 0;
    expCounter = 0;
    terminals = 0;
    exp = new double[0];
    root = NULL;
    alpha = NULL;
};

Tree::~Tree(){
    delete root;
//    for(int i = 0; i < tempCounter; i++)
//        delete temp[i];

    delete [] alpha;
    delete [] exp;
};

Tree* Tree::clone(){
    return new Tree(*this);
};

void Tree::setTree(No* n){
    root = n;
    refresh();
};

void Tree::refresh(){
    calc = true;
    high = -1;
    countLevels(root, 0);
    countHighs(root, high);
    terminals = countT();
    model = "";
    setExp();
    setSub();
};

void Tree::countLevels(No* n, int h){
    if(n != NULL){
//        cout << "Passando por: \n";
//        n->print(0);
        h++;
        n->deep = h;
        if(h > high){
            high = h;
        }
        for(int i = 0; i < n->countSons; i++){
            countLevels(n->sons[i], h);
        }
    }
};

void Tree::countHighs(No* n, int h){
    if(n != NULL){
        n->high = h;
        h--;
        for(int i = 0; i < n->countSons; i++){
            countHighs(n->sons[i], h);
        }
    }
};

void Tree::print(){
    if(root != NULL){
//        std::cout << "\nPrint Tree\n High: " << high << std::endl;
//        std::cout << std::endl;
//        for(int i = 0; i < expCounter; i++){
//            std::cout << exp[i] << " ";
//        }
//        std::cout << std::endl;
//        root->print(0);
//        root->print();
//        std::cout << std::endl;
        for(int i = 0; i < expCounter; i++)
            cout << exp[i] << " ";
        cout << endl;
    }else{
        cout << "Empty";
    }
};

string Tree::infixPrint(){
    if(model == ""){
        auxSubCounter = 0;
        auxInfix(root);
        model = p.top();
    }
    return model;
};

void Tree::printAlphas(){
    auxSubCounter = 0;
    cout << (conf->leastSquare == 0 ? 0 : subCounter) << " ";
    if(alpha != NULL){
        for(int i = 0; i < subCounter; i++){
            cout << alpha[i] << " ";
        }
    }
    cout << " ";
};

void Tree::auxInfix(No* no){
    switch((int)no->type){
        case 5:
        {
            p.push(no->mask);
        }
        break;
        case 3:
        {
            p.push(no->mask);
        }
        break;
        case 1:
        {
            if(no->value == 50){
//                if(auxSubCounter < subCounter){
//                    cout << alpha[0] << " | ";
//                    auxSubCounter++;
//                    cin.get();
//                }
//                if(auxSubCounter < subCounter){
//                    cout << alpha[1] << " | ";
//                    auxSubCounter++;
//                    cin.get();
//                }
//                string n = "(";
//                string b = p.top();
//                p.pop();
//                string d = p.top();
//                p.pop();
//                n.append(d);
//                n.append(no->mask);
//                n.append(b);
//                n.append(")");
//                p.push(n);
            }
            else{
                string n = "(";
                string b = p.top();
                p.pop();
                string d = p.top();
                p.pop();
                n.append(d);
                n.append(no->mask);
                n.append(b);
                n.append(")");
                p.push(n);
            }
        }
        break;
        case 2:
        {
            string n = no->mask;
            n.append("(");
            n.append(p.top());
            p.pop();
            n.append(")");
            p.push(n);
        }
        break;
        case 6:
        {

            string n = p.top();
            p.pop();
            string m = "(";
            m.append(p.top());
            p.pop();
            m.append(no->mask);
            m.append(n);
            m.append(")");
            p.push(m);
        }
        break;
        case 7:
        {
            string n = no->mask;
            n.append("(");
            n.append(p.top());
            p.pop();
            n.append(")");
            p.push(n);
        }
        break;
        case 8:
        {

            string n = p.top();
            p.pop();
            string m = "(";
            m.append(p.top());
            p.pop();
            m.append(no->mask);
            m.append(n);
            m.append(")");
            p.push(m);
        }
        break;
        case 9:
        {
            string ifelse = "(if";
            ifelse.append(p.top());
            p.pop();
            ifelse.append("{ ");
            string n = p.top();
            p.pop();
            string m = p.top();
            p.pop();
            ifelse.append(m);
            ifelse.append(" }else{ ");
            ifelse.append(n);
            ifelse.append(" })");
            p.push(ifelse);
        }
        break;
    }
    for(int i = 0; i < no->countSons; i++){
        auxInfix(no->sons.at(i));
    }
};

int Tree::countAll(){
    if(root == NULL){
        return 0;
    }else{
        return root->countAll();
    }
};

int Tree::countNT(){
    if(root == NULL){
        return 0;
    }else{
        return root->countNT();
    }
};

int Tree::countT(){
    if(root == NULL){
        return 0;
    }else{
        return root->countT();
    }
};

int Tree::countNTHigh(int value, int level, int sub){
    if(root == NULL){
        return 0;
    }else{
        countNTHigh(root, value, level, sub);
        return tempCounter;
    }
};

void Tree::countNTHigh(No* n, int v, int l, int sub){
    if(n->type == 0){
        if(n->value == v){
            if(l + n->highSubTree() <= conf->treeHigh && n->deep + sub <= conf->treeHigh){
//                temp[tempCounter] = n;
                tempCounter++;
            }
        }
        for(int i = 0; i < n->countSons; i++){
            countNTHigh(n->sons[i], v, l, sub);
        }
    }
};

No* Tree::getNo(int total){
    if(root == NULL){
        return NULL;
    }else{
        counter = 0;
        return auxGetNo(root, total);
    }
};

No* Tree::auxGetNo(No* n, int total){
    if(n != NULL){
        if(n->type == 0){
            counter++;
        }

        if(counter > total){
            return n;
        }else{
            for(int i = 0; i < n->countSons; i++){
                No* p = auxGetNo(n->sons[i], total);
                if(p != NULL){
                    return p;
                }
            }
            return NULL;
        }
    }
    return NULL;
};

No* Tree::getNoHigh(int total, int value, int level, int sub){
    if(root == NULL){
        return NULL;
    }else{
        counter = 0;
        return auxGetNoHigh(root, total, value, level, sub);
    }
};

No* Tree::auxGetNoHigh(No* n, int total, int v, int l, int sub){
    if(n->type == 0){
        if(n->value == v){
            if(l + n->highSubTree() <= conf->treeHigh && n->deep + sub <= conf->treeHigh){
                counter++;
            }
        }
        if(counter > total){
//            cout << "Retornar No: \n";
//            n->print(0);
            return n;
        }else{
            for(int i = 0; i < n->countSons; i++){
                No* p = auxGetNoHigh(n->sons[i], total, v, l, sub);
                if(p != NULL){
                    return p;
                }
            }
            return NULL;
        }
    }else{
        return NULL;
    }
};

void Tree::setExp(){
    delete [] exp;
    exp = new double[countT() * 2];
    expCounter = 0;
    getExp(root);
};

void Tree::getExp(No* n){
    if(n->countSons == 0){
//        std::cout << n->mask << " ";
        exp[expCounter] = n->type;
        exp[expCounter + 1] = n->value;
        expCounter += 2;
    }else{
        for(int i = 0; i < n->countSons; i++){
            getExp(n->sons[i]);
        }
    }
};

double Tree::treeResult(double* var, double* alpha, int alphaSize){
    if(alphaSize != 0){
        double result = 0;
        for(int i = 0; i < subCounter; i++){
            double* a = new double[sub.at(i).size()];
            for(int j = 0; j < sub.at(i).size(); j += 2){
                a[j] = sub.at(i).at(j);
                a[j + 1] = sub.at(i).at(j + 1);
                if(a[j] == 3.0){
                    a[j] = 0.0;
                    a[j + 1] = var[(int)a[j + 1]];
                }
                if(a[j] == 5.0){
                    a[j] = 0.0;
                }
            }
            result += alpha[i] * Avalia(a, sub.at(i).size());
            delete [] a;
        }
        return result;
    }
    else{
        double* a = new double[expCounter];
        for(int i = 0; i < expCounter; i += 2){
            a[i] = exp[i];
            a[i + 1] = exp[i + 1];
            if(a[i] == 3.0){
                a[i] = 0.0;
                a[i + 1] = var[(int)a[i + 1]];
            }
            if(a[i] == 5.0){
                a[i] = 0.0;
            }
        }

        double result = Avalia(a, expCounter);
        delete [] a;
        return result;
    }
};

double* Tree::getExp(){
    return exp;
};

void Tree::setSub(){
    subCounter = 0;
    sub.clear();
    findLS(root);
//    root->print(0);
//    cout << infixPrint() << endl;
//    this->print();
//    for(int i = 0; i < subCounter; i++){
//        for(int j = 0; j < sub.at(i).size(); j++)
//            cout << sub.at(i).at(j) << ", ";
//        cout << endl;
//    }
//    cout << endl;
};

void Tree::makeSub(No* n){
    if(n->type != 0){
        sub.at(subCounter - 1).push_back(n->type);
        sub.at(subCounter - 1).push_back(n->value);
    }
    else{
        for(int i = 0; i < n->countSons; i++)
            makeSub(n->sons.at(i));
    }
};

void Tree::findLS(No* n){
//    cout << "[" << n->mask << "] = " << n->sons.size() << " " << n->sons.at(2)->type << " " << n->sons.at(2)->value << endl;
    if(n->sons.size() == 3 && n->sons.at(2)->type == 1 && n->sons.at(2)->value == 5){
//        cout << "Tem LS pra baixo:" << endl;
//        n->print(0);
        findLS(n->sons.at(0));
        findLS(n->sons.at(1));
    }else{
        subCounter++;
        vector<double> a;
        sub.push_back(a);
        makeSub(n);
    }
//    for(int i = 0; i < n->countSons; i++)
//        findLS(n->sons.at(i));

//    if(n->type == 1 && n->value == 5){
//        if(n->father->sons.at(0)->sons.at(2)->type == 1 &&
//           n->father->sons.at(0)->sons.at(2)->value == 5){
//            findLS(n->father->sons.at(0));
//        }
//        else{
//            subCounter++;
//            vector<double> a;
//            sub.push_back(a);
//            makeSub(n->father->sons.at(0));
//        }
//
//        if(n->father->sons.at(1)->sons.at(2)->type == 1 &&
//           n->father->sons.at(1)->sons.at(2)->value == 5){
//            findLS(n->father->sons.at(1));
//        }
//        else{
//            subCounter++;
//            vector<double> a;
//            sub.push_back(a);
//            makeSub(n->father->sons.at(1));
//        }
//
//    }
//    else{
//        for(int i = 0; i < n->countSons; i++)
//            findLS(n->sons.at(i));
//    }
};

vector<double> Tree::getSub(int s){
    return sub.at(s);
};
