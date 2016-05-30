#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_INDIV 10
#define MUT_PROB 0.3
#define CROSS_PROB 0.7

void inicializaPopulacao(float pop[], int tam){
    int i;
    for(i = 0; i < tam; i++){
        //pop[i] = (rand())/(float)(RAND_MAX)*20.0 - 10;
        pop[i] = (rand()%1000)/(float)(1001) * 20  - 10;
    }
}

float funcaoPol(float x){
    return x*x - 4;
}

void avaliaFilhos(float pop[], float fpop[], int tam){
    int i;
    for(i = 0; i < tam; i++){
        fpop[i] = funcaoPol(pop[i]);
    }
}

void avaliaIndiv(float pop[], float fpop[], int tam, int it){
    int i;
    printf(" Geração %d: \n", it);
    for(i = 0; i<tam; i++){
        fpop[i] = funcaoPol(pop[i]);
        printf("x%d = %f - f(x%d) = %f \n",i, pop[i], i, fpop[i]);
    }
    // cria vetor com os resultados da função para cada indivíduo;
}

float selecionaElite(float pop[], float fpop[], int tam){
    float minimo = fpop[0];
    int ind = 0;
    int i;
    for(i = 1; i < tam; i++){
        if(fpop[i] < minimo){
            ind = i;
            minimo = fpop[i];
        }
    }

    return pop[ind];
}


void crossOver(float x1, float x2, float filhos[], int* ind){
    float j = (float)(rand())/(float)(RAND_MAX);
    if(j <= CROSS_PROB){
        filhos[*ind] = 0.3 * x1 + 0.7 * x2;
        (*ind)++;

        if((*ind)== NUM_INDIV)
            return;

        filhos[*ind] = 0.7 * x1 + 0.3 * x2;
        (*ind)++;
    } else {
        filhos[*ind] = x1;
        (*ind)++;

        if((*ind)== NUM_INDIV)
            return;

        filhos[*ind] = x2;
        (*ind)++;
    }


}

void mutacao(float filhos[], int indice){
    int i = rand()%2;
    if(i == 1)
        filhos[indice] += 0.1;
    else
        filhos[indice] -= 0.1;
}

void evolucao(float pop[], float fpop[], int tam){
    float popfilhos[NUM_INDIV];
    float fpopfilhos[NUM_INDIV];

    float j;

    int i;
    int indiceAtual = 0;

    int indAleatorio1;
    int indAleatorio2;

    while(indiceAtual < NUM_INDIV){
        indAleatorio1 = rand()%NUM_INDIV;
        indAleatorio2 = rand()%NUM_INDIV;

        crossOver(pop[indAleatorio1], pop[indAleatorio2], popfilhos, &indiceAtual);
    }


    for(i = 0; i < NUM_INDIV; i++){
        j = (float)(rand())/(float)(RAND_MAX);
        if(j <= MUT_PROB){
            mutacao(popfilhos, i);
        }
    }

    avaliaFilhos(popfilhos, fpopfilhos, NUM_INDIV);

    float elitePais = selecionaElite(pop, fpop, tam);

    float maximof = fpopfilhos[0];
    int ind = 0;

    for(i = 1; i<tam; i++){
        if(fpopfilhos[i] > maximof){
            ind = i;
            maximof = fpopfilhos[i];
        }
    }

    popfilhos[ind] = elitePais;

    for(i = 0; i<tam; i++){
        pop[i] = popfilhos[i];
        if(pop[i]>10)
            pop[i] = 10;
        if(pop[i]<-10)
            pop[i] = -10;
    }
}

int main()
{
    srand(4);
    float populacao[NUM_INDIV];
    float fpopulacao[NUM_INDIV];
    float melhor;

    int i;
    int iteracoes = 0;

    inicializaPopulacao(populacao, NUM_INDIV);

    for(i = 0; i < NUM_INDIV; i++){
        printf("%f \n", populacao[i]);
    }

    avaliaIndiv(populacao, fpopulacao, NUM_INDIV, iteracoes);

    while(iteracoes < 200){
        evolucao(populacao, fpopulacao, NUM_INDIV);
        avaliaIndiv(populacao, fpopulacao, NUM_INDIV, iteracoes);
        melhor = selecionaElite(populacao, fpopulacao, NUM_INDIV);

        printf("Melhor valor x = %f \n", melhor);
        iteracoes++;
    }

}
