#include "Grammar.h"
#include "Configures.h"

Grammar* grammar;

Grammar::Grammar(string gram){
    makeTerminals();

    ifstream arq;
    string line;
    vector<string> tokens;

    arq.open(gram.c_str());

    int nt = 0;
    while(getline(arq, line)){
        vector<string> rule;
        tokenize(line,rule, " ::= ");
        nonTerminals.push_back(rule.at(0));
        tokens.push_back(rule.at(1));
        //create a rule node
        Node* n = new Node();
        n->type = -1;
        n->value = nt;
        n->mask = nonTerminals.at(nt);
        gg.push_back(n);
        nt++;
    }

    for(int i = 0; i < tokens.size(); i++){
        //n is the ist rule
        Node* n = gg.at(i);

        vector<string> production;
        tokenize(tokens.at(i),production, " | ");
        for(int j = 0; j < production.size(); j++){
            //create every production in a rule
            Node* nn = new Node();
            n->productions.push_back(nn);

            vector<string> product;
            tokenize(production.at(j),product, " ");

            for(int k = 0; k < product.size(); k++){
                //Create the node of every product in a production
                auto t = getString(product.at(k));
                Node* nnn;
                if(get<0>(t) == -1){
                    nnn = gg.at(get<1>(t));
                }
                else{
                    nnn = new Node();
                    nnn->type = get<0>(t);
                    nnn->value = get<1>(t);
                    nnn->mask = get<2>(t);
                }
                nn->productions.push_back(nnn);
            }
        }
    }

    arq.close();
    this->update();
}

void Grammar::makeTerminals(){
    //0
    vector<string> constants;
    terminals.push_back(constants);
    conf->constant = terminals.size() - 1;

    //1
    vector<string> variables;
    terminals.push_back(variables);
    conf->variable = terminals.size() - 1;
    //2
    vector<string> bynaryArithmeticOperators;
    bynaryArithmeticOperators.push_back("+");
    bynaryArithmeticOperators.push_back("-");
    bynaryArithmeticOperators.push_back("*");
    bynaryArithmeticOperators.push_back("/");
    bynaryArithmeticOperators.push_back("^");
    terminals.push_back(bynaryArithmeticOperators);
    conf->bynaryArithmeticOperators = terminals.size() - 1;
    //3
    vector<string> unaryArithmeticOperators;
    unaryArithmeticOperators.push_back("log");
    unaryArithmeticOperators.push_back("exp");
    unaryArithmeticOperators.push_back("sqrt");
    unaryArithmeticOperators.push_back("cos");
    unaryArithmeticOperators.push_back("sin");
    unaryArithmeticOperators.push_back("tan");
    unaryArithmeticOperators.push_back("acos");
    unaryArithmeticOperators.push_back("asin");
    unaryArithmeticOperators.push_back("atan");
    terminals.push_back(unaryArithmeticOperators);
    conf->unaryArithmeticOperators = terminals.size() - 1;
    //4
    vector<string> binaryLogicalOperators;
    binaryLogicalOperators.push_back("and");
    binaryLogicalOperators.push_back("or");
    binaryLogicalOperators.push_back("xor");
    terminals.push_back(binaryLogicalOperators);
    conf->binaryLogicalOperators = terminals.size() - 1;
    //5
    vector<string> unaryLogicalOperators;
    unaryLogicalOperators.push_back("not");
    unaryLogicalOperators.push_back("true");
    unaryLogicalOperators.push_back("false");
    terminals.push_back(unaryLogicalOperators);
    conf->unaryLogicalOperators = terminals.size() - 1;
    //6
    vector<string> comparisonOperators;
    comparisonOperators.push_back("<");
    comparisonOperators.push_back("<=");
    comparisonOperators.push_back("==");
    comparisonOperators.push_back(">=");
    comparisonOperators.push_back(">");
    comparisonOperators.push_back("!=");
    terminals.push_back(comparisonOperators);
    conf->comparisonOperators = terminals.size() - 1;
    //7
    vector<string> leastSquareOperator;
    leastSquareOperator.push_back("!");
    terminals.push_back(leastSquareOperator);
    conf->leastSquareOperator = terminals.size() - 1;
    //8
    vector<string> programOperators;
    comparisonOperators.push_back("if-else");
    comparisonOperators.push_back("if-elseif*-else");
    comparisonOperators.push_back("for");
    comparisonOperators.push_back("while");
    comparisonOperators.push_back("do-while");
    terminals.push_back(comparisonOperators);
    conf->comparisonOperators = terminals.size() - 1;
}

tuple<double, double, string> Grammar::getString(string s){
    tuple<double, double, string> t;
    if(isNonTerminal(s)){
        double nt = getNonTerminal(s);
        if(nt != -1)
            return make_tuple(-1.0, nt, s);
    }
    else{
        t = getTerminal(s);
        if(get<0>(t) != -2)
            return t;
    }

    return make_tuple(-2.0, -2.0, "huebr2016");
}

bool Grammar::isNonTerminal(string s){
    if(s.front() == '<' && s.back() == '>')
        return true;
    return false;
}

Grammar::~Grammar(){
    //dtor
}

double Grammar::getNonTerminal(string nt){
    for(int i = 0; i < nonTerminals.size(); i++){
        if(nt.compare(nonTerminals.at(i)) == 0)
            return i;
    }
    return -1;
}

tuple<double, double, string> Grammar::getTerminal(string t){
    int i = 0;
    for(vector<string> type : terminals){
        int j = 0;
        for(string value : type){
            if(value.compare(t) == 0)
                return make_tuple(i, j, t);
            j++;
        }
        i++;
    }

    bool number = true;
//    for(char c : t)
//        if(!isdigit(c))
//            number = false;

    int var = data->getCode(t);
    if(var != -1)
        return make_tuple(1, var, t);

    if(number)
        return make_tuple(0, atof(t.c_str()), t);

    /** TODO - It should stop here case it gets here **/
    cout << "--- The grammar is syntactically wrong! A Non Terminal has no rule." << endl;
    cin.get();
    exit(-1);
    return make_tuple(-2, -2, t);
}

void Grammar::print(){
    for(Node* n : gg){
        cout << n->mask << "(" << n->high << ")" << " ::= ";
//        cout << n->mask << " ::= ";

        for(Node* nn : n->productions){
            for(Node* nnn : nn->productions){
                cout << "(" << nnn->type << "," << nnn->value << "," << nnn->mask << "," << nnn->high << ") ";
//                cout << nnn->mask << " ";
            }
            cout << nn->high << "| ";
//            cout << "| ";
        }

        cout << endl;
    }
}

void Grammar::tokenize(std::string str, std::vector<string> &token_v, string DELIMITER){
    size_t start = str.find_first_not_of(DELIMITER), end=start;

    while (start != std::string::npos){
        // Find next occurence of delimiter
        end = str.find(DELIMITER, start);
        // Push back the token found into vector
        token_v.push_back(str.substr(start, end-start));
        // Skip all occurences of the delimiter to find new start
        start = str.find_first_not_of(DELIMITER, end);
    }
}

void Grammar::update(){
    high1(gg.at(0));  //set rules minor high and some productions (must have 1 value for each rule, otherwise the grammar is not CFG)
    high2();          // set every production high
    for(Node* n : gg) // sort the productions by minimum high it require
        stable_sort(n->productions.begin(), end(n->productions), sortNode);
}

bool Grammar::sortNode(Node* a, Node* b){
    int aa = 0, bb = 0;

    for(Node* n : a->productions)
        if(aa < n->high)
            aa = n->high;

    for(Node* n : b->productions)
        if(bb < n->high)
            bb = n->high;

    return aa < bb;
}

void Grammar::high1(Node* n){
    n->visited = true;
    n->high = INFINITY;

    if(n->type > -1)
        n->high = 0;

    for(Node* nn : n->productions){
        bool invalid = false;
        for(Node* nnn : nn->productions){
            int h = 0;
            if(!nnn->visited){
                high1(nnn);
                h = nnn->high + 1;
                if(n->high > h)
                    n->high = h;
            }
            else{
                invalid = true;
            }
            if(invalid)
                n->high = INFINITY;
        }
    }

}

void Grammar::high2(){
    bool allSet;
//    do{
        allSet = true;
        for(Node* n : gg){
            int min = INFINITY;
            for(Node* nn : n->productions){
                int max = 0;
                for(Node* nnn : nn->productions)
                    if(nnn->high > max)
                        max = nnn->high;
                nn->high = max;

                if(min > max)
                    min = max;
            }
//            if(n->high == )
        }
//    }while(!allSet);
}

No* Grammar::derivate(){
    Node* node = gg.at(0);
    delete node;
    No* n = new No(node->type, node->value, node->mask, 0);
    derivate(n);
    return n;
}

int Grammar::derivate(No* n){
    if(n->type == NONTERMINAL){
        n->height = 0;
        int i = 0;
        for(Node* p : gg.at(n->value)->productions){//para todas as produções de n
            if(p->high + n->deep + 1 <= conf->MAXDEEP)
                i++;
            else
                break;
        }
        if(i == 0){
            cout << "ALTURA MÍNIMA DE ÁRVORE EXCEPTION" << endl;
            exit(43);
        }
        int production = rand() % i;

        for(Node* m : gg.at(n->value)->productions.at(production)->productions){
            No* p = new No(m->type, m->value, m->mask, n->deep + 1);

            p->index = n->sons.size();
            p->father = n;
            n->addSon(p);


            int h = derivate(p);

            if(h >= n->height)
                n->height = h;
        }
    }
    else{
        n->height = 0;
    }

    return n->height + 1;
}
