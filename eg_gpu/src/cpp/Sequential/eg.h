#ifndef EG_H_INCLUDED
#define EG_H_INCLUDED

#include "representacao.h"
#include "gramatica.h"
#include "parser.h"

void eg(t_regra *gramatica, Database *dataBase, Database *testData, char * pop_file, int pop_count);



#endif // EG_H_INCLUDED
