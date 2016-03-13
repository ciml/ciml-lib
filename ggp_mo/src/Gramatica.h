#ifndef GRAMATICA_H_INCLUDED
#define GRAMATICA_H_INCLUDED

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <vector>

#include "Tree.h"
#include "Configures.h"

using namespace std;
//NODE
class Node{
    public:
        double tipo;
        double val;
        string str;
        Node(int t, int v, string s){
            tipo = t;
            val = v;
            str = s;
        }
};
//PROD
class Prod{
    public:
        vector<Node*> produto;
        int qnt;
        int menorProfundidadeProd;
        Prod(){
            menorProfundidadeProd = -1;
            qnt = 0;
        }
        void add(Node* p){
            produto.push_back(p);
            qnt++;
        }
};
//REGRA
class Regra{
    public:
        vector<Prod*> Producao;
        int qnt;
        int menorProfundidadeRegra;
        int maiorProfundidadeRegra;
        Regra() {
            qnt = 0;
            menorProfundidadeRegra = -1;
            maiorProfundidadeRegra = -1;
        }
        void add(Prod* produto){
            Producao.push_back(produto);
            qnt++;
        }
};
//HASH
class Hash{
    public:
        char* s;
        int pos;

        char* getS(){
            return s;
        }
        int getPos(){
            return pos;
        }
};
//GRAMATICA
class Terminal{
    public:
        int Random(int x){
            return rand() % x;
        }

        Node* folha(char* s, int var){
            //bin
            if(strcmp(s, "+") == 0)
                return new Node(1, 0, " + ");
            else if(strcmp(s, "-") == 0)
                return new Node(1, 1, " - ");
            else if(strcmp(s, "*") == 0)
                return new Node(1, 2, " * ");
            else if(strcmp(s, "/") == 0)
                return new Node(1, 3, " / ");
            else if(strcmp(s, "pow") == 0)
                return new Node(1, 4, "^");
            else if(strcmp(s, "!") == 0)
                return new Node(1, 5, " ! ");
//            un
            else if(strcmp(s, "log") == 0)
                return new Node(2, 0, "log");
            else if(strcmp(s, "exp") == 0)
                return new Node(2, 1, "exp");
            else if(strcmp(s, "sqrt") == 0)
                return new Node(2, 2, "sqrt");
//            logic bin
            else if(strcmp(s, "and") == 0)
                return new Node(6, 0, " && ");
            else if(strcmp(s, "or") == 0)
                return new Node(6, 1, " || ");
            else if(strcmp(s, "xor") == 0)
                return new Node(6, 2, " xor ");
//            logic un
            else if(strcmp(s, "not") == 0)
                return new Node(7, 0, "not");
            else if(strcmp(s, "true") == 0)
                return new Node(7, 1, s);
            else if(strcmp(s, "false") == 0)
                return new Node(7, 2, s);
//            rel
            else if(strcmp(s, "<") == 0)
                return new Node(8, 0, " < ");
            else if(strcmp(s, "<=") == 0)
                return new Node(8, 1, " <= ");
            else if(strcmp(s, "==") == 0)
                return new Node(8, 2, " == ");
            else if(strcmp(s, ">=") == 0)
                return new Node(8, 3, " >= ");
            else if(strcmp(s, ">") == 0)
                return new Node(8, 4, " > ");
            else if(strcmp(s, "!=") == 0)
                return new Node(8, 5, " != ");
//            if-else
            else if(strcmp(s, "if-else") == 0)
                return new Node(9, 0, "if-else");
//            num
            else if(s[0] == '0' || atof(s))
                return new Node(5, atof(s), s);
            else{
                return new Node(3, var, s);
            }
        }
};

class Gramatica{
public:
    vector<Regra*> Regras;
    int totalRegra;
    vector<Hash*> Mapa;
    int totalMapa;
    vector<string> MapaVariaveis;
    int totalMapaVariaveis;

    vector<vector<char*> > rulesStrings;
    bool print;

    void addRegra(Regra* r);
    void addMap(char* s);
    int checkMap(char* s);
    void addMapV(char* s);
    int checkMapV(string s);

    Gramatica(string sarq);
    void ordenaProducoes();
    void imprimeMapa();
    void imprimeGramatica();
    void imprimeProfs();
    void alturaGramatica();
    int randint(int x);
    Tree* criaArvExpO();
    Tree* criaArvExpO(int valor);
    No* criaArvExp(float t, float v, int prof);
    void criaNoO(No* n);
    Tree* criaArvExp();
    void criaNo(No* n, int prof);
    void criaNoGuiado(No* n, No* q);
};

extern Gramatica* gram;

#endif // GRAMATICA_H_INCLUDED
