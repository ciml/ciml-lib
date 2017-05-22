#ifndef CROSSOVER_H
#define CROSSOVER_H

/*Ao aumentar a quantidade de crossover deve 
										aumentar o vetor que realiza o sorteio*/

void opCrossover(int countInd, int father1, int father2);
void LOXcrossover(int countInd, int father1, int father2);
void PMXcrossover(int countInd, int father1, int father2);
void permutation(int firstCromossome, int i, int countInd, int father1, 
                                                            int father2, int j);

#endif // CROSSOVER_H