#ifndef PARSER_H
#include "Configures.h"
#define PARSER_H

#include "Subject.h"
#include <string>

class Parser {
    public:
        Parser(){}
        virtual double Evaluate(Subject* s) = 0;
        virtual string nameParser() = 0;
        ~Parser(){}
    protected:
    private:

};

//extern Parser* parser;

#endif // PARSER_H
