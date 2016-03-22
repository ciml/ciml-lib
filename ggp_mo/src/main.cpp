#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>

#include "Datamaker.h"
#include "No.h"
#include "Tree.h"
#include "Database.h"
#include "Gramatica.h"
#include "Configures.h"
#include "Operators.h"
#include "Search.h"


using namespace std;


int main(int argc, char* args[]){

    int seed = atoi(args[1]);
    string gramatica = args[2];
    string dados = args[3];
    string grupo = args[4];
    int geracoes = atoi(args[5]);
    int populacao = atoi(args[6]);
    int altura = atoi(args[7]);
    int complexidade = atoi(args[8]);

    srand(seed);
    cout.precision(7);

    //configuracoes
    conf = new Configures();
    conf->treeHigh = altura;
    conf->popSize = populacao;
    conf->iterations = geracoes;
    conf->leastSquare = 1;
    conf->elitism = 0.1;
    conf->mono = 0;
    conf->complexity = complexidade; // 0 = high 1 = terminals

    //gramatica
//    gram = new Gramatica("read/gram5.txt");

/** Juliana e Rogério **/
    gram = new Gramatica(gramatica);
    gram->imprimeGramatica(); // estrutura da gramática
    // Mostra cada valor encontrado no arquivo com seu tipo, valor e máscara

    gram->imprimeMapa(); // elementos encontrados
    // Mostra os elementos identificados na gramática que não são pré definidos

    gram->imprimeProfs(); // Profundidades
    // Mostra a profundidade mínima necessária para cada produção, permitindo a esolha da mais formidável
/**                 **/
    //operadores
//    op = new Operators();

    //dados
//    data = new Database("read/base5.txt", "read/grupo5.txt");
//    data = new Database(dados, grupo);
//    data->print();

    //busca
//    Search* s = new Search();
//    delete s;

    /************************************
            No Args
    ************************************/

//    srand(int(args[1]));
//    cout.precision(6);
//
//    //configuracoes
//    conf = new Configures();
//    conf->treeHigh = 20;
//    conf->popSize = 500;
//    conf->iterations = 5;
//    conf->leastSquare = 1;
//    conf->elitism = 0.1;
//    conf->mono = 1;
//    conf->complexity = 1; // 0 = high 1 = terminals
//
//    //gramatica
////    gram = new Gramatica("read/gram5.txt");
//    gram = new Gramatica("read/virus2.txt");
//    //operadores
//    op = new Operators();
//
//    //dados
////    data = new Database("read/base5.txt", "read/grupo5.txt");
//    data = new Database("read/SPT.csv", "read/SPT1.csv");
//
//    //busca
//    Search* s = new Search();
//    delete s;

    /************************************
            Tests
    ************************************/
     //datamaker
//    makeData("read/base1.txt", "read/grupo1.txt", 100, 5, "a b c d e");
//    makeData("read/base2.txt", "read/grupo2.txt", 100, 5, "a b c d e");
//    makeData("read/base3.txt", "read/grupo3.txt", 10000, 5, "a b c d e");
//    makeData("read/base4.txt", "read/grupo4.txt", 100, 5, "a b c d e");
//    makeData("read/base5.txt", "read/grupo5.txt", 100, 5, "a b c d e");

    //seed
//    srand(12);
//    srand(time(NULL));

    //gramatica
//    gram = new Gramatica("read/gram1.txt");
//    gram->imprimeGramatica();
//    cout << gram->checkMapV("c") << endl;
//    cout << gram->totalMapaVariaveis << endl;
//    for(string s : gram->MapaVariaveis)
//        cout << s << " ";
//    cout << endl;

    //tree
//    for(long int i = 0; i < 10; i++){
//        Tree* t = gram->criaArvExp();
//        cout << t->infixPrint() << endl;
//        delete t;
//    }

    //operators
//    op = new Operators();
    //  Cross
//    Tree* a = gram->criaArvExp();
//    Tree* b = gram->criaArvExp();
//    Tree* c = new Tree();
//    Tree* d = new Tree();
//    cout << a->infixPrint() << endl;
//    cout << b->infixPrint() << endl;
//    op->Cross(a, b, c, d);
//    cout << c->infixPrint() << endl;
//    cout << d->infixPrint() << endl;
    //  Mutation
//    Tree* t = gram->criaArvExp();
//    cout << t->infixPrint() << endl;
//    op->Mutate(t);
//    cout << t->infixPrint() << endl;

//    database
//    Database* data = new Database("read/base1.txt", "read/grupo1.txt", gram);
//    data->print();

    return 0;
}

