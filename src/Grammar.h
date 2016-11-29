#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <tuple>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

#include "Database.h"
#include "No.h"
#include "Configures.h"


using namespace std;

class Node{
    public:
        vector<Node*> productions;

        bool visited;
        int high;
        double type;
        double value;
        string mask;

        Node(){visited = false; high = -1;}
};

class Product{
    public:
        int rule;

        Product(int r){
            rule = r;
        }

        Product(){}
};

class Production{
    public:
        vector<Product*> products;
        int high;

        Production(){}
};

class Rule{
    public:
        vector<Production*> productions;
        int high;
        double type;
        double value;
        string mask;

        Rule(double type, double value, string mask){
            this->type = type;
            this->value = value;
            this->mask = mask;
            high = INFINITY;
        }

        Rule();
};

class Grammar{
    public:
        Grammar(string gram);
        virtual ~Grammar();

        void tokenize(std::string str, std::vector<string> &token_v, string DELIMITER);
        double getNonTerminal(string nt);
        tuple<double, double, string> getTerminal(string t);
        tuple<double, double, string> getString(string s);
        bool isNonTerminal(string s);
        void makeTerminals();
        void print();
        void update();
        void high1(Node* n);
        void high2();
        static bool sortNode(Node* a, Node* b);
        void calcHigh();

        int derivate(No* n);
        No* derivate();

    protected:
    private:

        vector<vector<vector<tuple<double, double, string> > > > grammar;
        vector<Rule*> grammar2;
        vector<Node*> gg;
        vector<vector<vector<int> > > higths;
        vector<string> nonTerminals;
        vector<vector<string> > terminals;

};

extern Grammar* grammar;

#endif // GRAMMAR_H
