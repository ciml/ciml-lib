#ifndef CROSSOVER2_H
#define CROSSOVER2_H

void opCrossover(int countInd, int *geneFather1, int *geneFather2);
void LOXcrossover(int countInd, int *geneFather1, int *geneFather2);
void PMXcrossover(int countInd, int *geneFather1, int *geneFather2);
void permutation(int firstCromossome, int i, int countInd, int *geneFather1, 
                                                            int *geneFather2, int j);

#endif // CROSSOVER2_H
