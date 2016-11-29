#ifndef NO_H
#define NO_H


#include <iostream>
#include <string>
#include <vector>

using namespace std;

class No {
    public:
        No();
        No(double t, double v, string m, int h);
        virtual ~No();

        void print(int format);
        void print();
        void addSon(No* n);
        void erase();
        void swap(No* m);
        No* clone();

        double type, value;
        string mask;
        int deep;
        int height;
        bool modificated;

        vector<No*> sons;
        No* father;
        int index;
        void destrutor(No * p);

    protected:
    private:
};

#endif // NO_H
