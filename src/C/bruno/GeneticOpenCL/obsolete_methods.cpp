void selection(Arvore pop[], int k){
    int inicio = 0, fim = NUM_INDIV-1;

    while(inicio < fim){
        int r = inicio, w = fim;
        float meio = pop[(r+w)/2].aptidao;

        while(r < w){
            if(pop[r].aptidao <= meio){
                Arvore tmp = pop[w];
                pop[w] = pop[r];
                pop[r] = tmp;
                w--;
            } else {
                r++;
            }
        }
            if(pop[r].aptidao < meio)
                r--;

            if(k<=r){
                fim = r;
            }else{
                inicio = r+1;
            }
    }
    return;
}

void fatal(char *msg) {
    printf("%s\n", msg);
    exit (1);
}

float fabs2(float val1, float val2){
    if(val1-val2 < 0){
        return (-1* (val1-val2));
    } else {
        return (val1-val2);
    }
}
//if(fabs2(pop[indice].aptidao, pop[indiceMelhor].aptidao) < 0.0000001){
//printf("entrouTorneio %d\n", indiceMelhor);

