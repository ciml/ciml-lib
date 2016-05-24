//2^31
#define CONST1 2147483648.0
//2^31 -1
#define CONST2 2147483647

#define MASK 2147483648

#define BIT_SET(a,b) ((a) |= (1<<(b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1<<(b)))
#define BIT_FLIP(a,b) ((a) ^= (1<<(b)))
#define BIT_CHECK(a,b) ((a) & (1<<(b)))

int rand(int *seed){

    int s  = *seed;
    s = abs((s * 16807) % CONST2);    
    *seed = s;        
    return s; 
}

float u_rand(int * seed){
    return (float)rand(seed) / CONST1;
}

int binario_para_decimal(unsigned binario[], int inicio, int fim){

    int i,n=1; int valorNumerico=0;

    for(i=fim-1; i>=inicio; i--, n=n<<1){
        valorNumerico += n*((int)binario[i]);
    }
    return valorNumerico;
}


void gray_para_binario(__global unsigned gray[], unsigned binarios[]){

    int i,j;

    for(i=0; i< TAMANHO_ANTICORPO; i++){
        binarios[i] = gray[i];
    }

    int start;
    int end = 0;
    for (j = 0; j < DIMENSOES; j++) {
        start = end;
        end += BITS_POR_DIMENSAO;
        for (i = start + 1; i < end; i++) {
            binarios[i] = binarios[i - 1] ^ binarios[i];
        }
    }
}

void gray_para_binario_l(__local unsigned gray[], __local unsigned binarios[]){

    int i,j;

    for(i=0; i< TAMANHO_ANTICORPO; i++){
        binarios[i] = gray[i];
    }

    int start;
    int end = 0;
    for (j = 0; j < DIMENSOES; j++) {
        start = end;
        end += BITS_POR_DIMENSAO;
        for (i = start + 1; i < end; i++) {
            binarios[i] = binarios[i - 1] ^ binarios[i];
        }
    }
}

void gray_para_binario_p(unsigned gray[], unsigned binarios[]){

    int i,j;

    for(i=0; i< TAMANHO_ANTICORPO; i++){
        binarios[i] = gray[i];
    }

    int start;
    int end = 0;
    for (j = 0; j < DIMENSOES; j++) {
        start = end;
        end += BITS_POR_DIMENSAO;
        for (i = start + 1; i < end; i++) {
            binarios[i] = binarios[i - 1] ^ binarios[i];
        }
    }
}
