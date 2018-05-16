unsigned int floatToInt(float floatVal)
{
    return (*(unsigned int*)(&floatVal) >> TIPO);
}

float intToFloat(unsigned int intVal)
{
    intVal = intVal << TIPO;
    return *(float*)(&intVal);
}

unsigned int packInfo(int tipo, int valor)
{
    unsigned int informacao = 0;
    informacao = (tipo << (32-TIPO)) | valor;
    return informacao;
}

unsigned int packInt(int tipo, int valor)
{
    unsigned int informacao = 0;
    informacao = (tipo << (32-TIPO)) | valor;
    return informacao;
}

unsigned int packFloat(int tipo, float valor)
{
    unsigned int informacao = 0;
    unsigned int valorInt = floatToInt(valor);

    informacao = (tipo << (32-TIPO)) | valorInt;
    return informacao;
}

unsigned int unpackTipo(unsigned int info)
{
    unsigned int tipo = (info>>(32-TIPO));
    return tipo;
}

unsigned int unpackInt(unsigned int info)
{
    int valor = (info << TIPO) >> TIPO;
    return valor;
}

float unpackFloat(unsigned int info)
{
    float valorF = intToFloat(info);
    return valorF;
}


int retornaTipo(__global Arvore* arv, int j)
{
    if(arv->numeroFilhos[j] == 0){
        return unpackTipo(arv->informacao[j]);
    } else {
        return unpackInt(arv->informacao[j]);
    }
}

float proDiv(float num, float div)
{
    if(div == 0){
        return 1;
    } else {
        return (num/div);
    }
}


float proSqrt(float num)
{
    if(num < 0){
        return 1;
    } else {
        return sqrt(num);
    }
}

int rand2(int *seed)
{
    
    int s  = *seed;
    s = (unsigned int)(s * 16807) % 2147483647;
    *seed = s;
    return s;

}

float randomProb(int* seed)
{
    return (float)rand2(seed) / 2147483647;
}

float randomConst(int* seed)
{
    float random = ((float)rand2(seed))/2147483647;
    float range = maxDados - minDados;
    float result = (range*random) + minDados;
    return result;
}

int randomType(int* seed)
{
    int tipoAleatorio = rand2(seed)%(NUM_OPBIN+NUM_OPUN+(1)+N-1);
    return tipoAleatorio;
}


int randomLeafType(int* seed)
{
    int tipoAleatorio = (rand2(seed)%N) + NUM_OPBIN + NUM_OPUN;
    return tipoAleatorio;
}

int randomNodeType(int* seed)
{
    int tipoAleatorio = (rand2(seed)%(NUM_OPBIN+NUM_OPUN));
    return tipoAleatorio;
}