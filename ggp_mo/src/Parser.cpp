#include <iostream>
#include <stack>
#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include <sstream>
#include "Parser.h"

using namespace std;

int quantidade(int vet[])
{
    int n=0;
    for(int i=0; vet[i] != NULL; i++)
        n++;

    return n+1;
}

void imprimeEquacao(double* expr, int tam){
    cout << " Expressao: ";
    for(int i = 0; i < tam; i+=2)
    {
        if(expr[i] == 0)
            cout << expr[i+1];
        else
            cout << converte(expr[i],expr[i+1]);
        cout << " ";
    }
    cout << "\n";
};

string converteInfixo(double x, double y){
    if(x == 1){
        if(y == 0)
            return "+";
        if(y == 1)
            return "-";
        if(y == 2)
            return "*";
        if(y == 3)
            return "/";
        if(y == 4)
            return "^";
    }
    if(x == 2){
        if(y == 0)
            return "log(";
        if(y == 1)
            return "exp(";
        if(y == 2)
            return "sqrt(";
    }
}

void imprimeEquacaoInfixa(double* expr, int tam){
    stack<string> p;
    cout << endl;
    for(int i = 0; i < tam; i+=2)
    {
        switch((int)expr[i]){
            case 0:
            {
                stringstream s;
                s << expr[i + 1];
                p.push(s.str());
            }
            break;
            case 5:
            {
                stringstream s;
                s << expr[i + 1];
                p.push(s.str());
            }
            break;
            case 3:
            {
                p.push("x");
            }
            break;
            case 1:
            {
                if(expr[i + 1] == 0 || expr[i + 1] == 1){
                    string n = p.top();
                    p.pop();
                    string m = p.top();
                    p.pop();
                    m.append(converteInfixo(expr[i], expr[i + 1]));
                    m.append(n);
                    p.push(m);
                }
                else{
                    string n = "(";
                    string m = p.top();
                    p.pop();
                    string mm = p.top();
                    p.pop();
                    n.append(mm);
                    n.append(converteInfixo(expr[i], expr[i + 1]));
                    n.append(m);
                    n.append(")");
                    p.push(n);
                }
            }
            break;
            case 2:
            {
                string n = converteInfixo(expr[i], expr[i + 1]);
                n.append(p.top());
                p.pop();
                n.append(")");
                p.push(n);
            }
            break;
        }
    }
    cout << p.top();
};

char converte(double x, double y){
    if((double)x == 1)
        if(y == 0)
            return '+';
        if(y == 1)
            return '-';
        if(y == 2)
            return '*';
        if(y == 3)
            return '/';
    if(x == 2)
        if(y == 0)
            return '(';
        if(y == 1)
            return ')';
}

int verifica(double x){
    if(x == 0)//int
        return 0;
    if(x == 1)//ope
        return 1;
    if(x == 2)//paren
        return 2;
    if(x == 3)//var
        return 3;
}

int prio_vet(double x, double y){
    if(x == 2 && y == 0)// '('
        return 1;
    if(x == 1)
        if(y == 0 || y == 1)// + or -
            return 2;
    if(x == 1)
        if(y == 2 || y == 3)// * or /
            return 3;
}

//1
double opera(double a, double b, int x){
    if(x == 0)
        return a+b;
    else if(x == 1)
        return a-b;
    else if(x == 2)
        return a*b;
    else if(x == 3){
//        return b == 0 ? a/b : INFINITY;
        return a/b;
    }
    else if(x == 4)
        return pow(a, b);
//        return a > 0 ? pow(a, b) : INFINITY;
    else if(x == 5)
        return a + b;
}
//2
double opera(double a, int x){
    if(x == 0) //log
//        return a >= 0 ? log(a) : INFINITY;
        return log(a);
    else if(x == 1) //exp
        return exp(a);
    else if(x == 2)//sqrt
//        return a > 0 ? sqrt(a) : INFINITY;
        return sqrt(a);
}
//6
double operaLogic(double a, double b, int x){
    if(x == 0)
        return a && b;
    else if(x == 1)
        return a || b;
    else if (x == 2)
        return (bool)a ^ (bool)b;
}
//7
double operaLogic(double a, int x){
    if(x == 0)
        return !(bool)a;
    else if(x == 1)
        return 1;
    else if(x == 2)
        return 0;
}
//8
double operaComp(double a, double b, int x){
    if(x == 0)
        return a < b;
    else if(x == 1)
        return a <= b;
    else if(x == 2)
        return a == b;
    else if(x == 3)
        return a >= b;
    else if(x == 4)
        return a > b;
    else if(x == 5)
        return a != b;
}
//9
double operaIfElse(double a, double b, int c){
    if(a == 0){
        return b;
    }
    else{
        return c;
    }
}

double Avalia(double *expr, int tam)
{
    stack<double> q;
    int aux;
    int countAlpha = 0;
    for(int i = 0; i < tam; i+=2){
        double result;
        switch((int)expr[i]){
            case 0:
            {
                q.push(expr[i + 1]);
            }
            break;
            case 1:// + - * / pow !
            {
                double b = q.top();
                q.pop();
                double d = q.top();
                q.pop();
                result = opera(d, b, expr[i+1]);
                q.push(result);
            }
            break;
            case 2:// log exp sqrt
            {
                double n = q.top();
                q.pop();
                result = opera(n, expr[i+1]);
                q.push(result);
            }
            break;
            case 6:// && || xor
            {
                double a = q.top();
                q.pop();
                double b = q.top();
                q.pop();
                result = operaLogic(b, a, expr[i+1]);
                q.push(result);
            }
            break;
            case 7:
            {
                double a = q.top();
                q.pop();
                result = operaLogic(a, expr[i+1]);
                q.push(result);
            }
            break;
            case 8:
            {
                double a = q.top();
                q.pop();
                double b = q.top();
                q.pop();
                result = operaComp(b, a, expr[i+1]);
                q.push(result);
            }
            break;
            case 9:
            {
                double a = q.top();
                q.pop();
                double b = q.top();
                q.pop();
                double c = q.top();
                q.pop();
                result = operaIfElse(a, c, b);
                q.push(result);
            }
        }
        if(isnan(result) || isinf(result)){
            return INFINITY;
        }
    }

    double resultado = q.top();

    return resultado;
}
