#include "utilitarios.h"

void atribuiSemente(int i){
    srand(i);
}

float randomProb(){
    return rand()/(float)(RAND_MAX+1);
}
