#ifndef SELECTION_H
#define SELECTION_H
#include "Subject.h"

class Selection
{
    public:
        int tam_selection = 2;//numero de individuos retornados pela seleção
        Selection();
        virtual Subject * selects(Subject ** pop) = 0;
        virtual ~Selection();

    protected:

    private:
};

#endif // SELECTION_H
