#ifndef TOURNAMENTSELECTION_H
#define TOURNAMENTSELECTION_H
#include "Selection.h"
#include "Subject.h"

class TournamentSelection : public Selection {
    public:
        TournamentSelection();
        virtual Subject * selects(Subject ** pop){
               Subject * x = pop[rand()%conf->popSize];
               Subject * y = pop[rand()%conf->popSize];

               if(x->fitness > y->fitness){
                    return x;
               }
               return y;
        }
        virtual ~TournamentSelection();

    protected:

    private:
};

#endif // TOURNAMENTSELECTION_H
