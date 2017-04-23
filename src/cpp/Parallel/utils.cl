
int rand2(int *seed){

    int s  = *seed;

    s = abs((s * 16807) % (int)(pown(2.0, 31)-1));
    
    *seed = s;
        
    return s;
}

float u_rand2(int * seed){
    return (float)rand2(seed) / pown(2.0, 31);
}


int binario_para_decimal(__local short binario[], int inicio, int fim){

    int i,n=1; int valorNumerico=0;

    for(i=fim-1; i>=inicio; i--, n=n<<1){
        valorNumerico += n*((int)binario[i]);
    }

    return valorNumerico;
}

void gray_para_binario(short gray[], __local short binarios[]){

        int i,j;

        for(i=0; i< TAMANHO_INDIVIDUO; i++){
            binarios[i] = gray[i];
        }

        int start;
        int end = 0;
        for (j = 0; j < DIMENSOES_PROBLEMA; j++) {
            start = end;
            end += TAMANHO_VALOR;
            for (i = start + 1; i < end; i++) {
                binarios[i] = binarios[i - 1] ^ binarios[i];
            }
        }
}
