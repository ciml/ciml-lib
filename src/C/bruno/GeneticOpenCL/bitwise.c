#include <stdio.h>
#include <stdlib.h>
#include "bitwise.h"
#include "constantes.h"

//typedef union {
//    float f;
//    unsigned int i;
//} FloatBits;

/*
*Funcao para retornar um valor Int que representa um Float
*@param floatVal  : valor a ser transformado
*
*@return unsigned int: o inteiro desejado deslocado "TIPO" bits para a direita
*********************************************************/
unsigned int floatToInt(float floatVal){
    return (*(unsigned int*)(&floatVal) >> TIPO);
}


/*
*Funcao para retornar um valor Float que estava salvo em um Int
*@param intVal  : valor a ser transformado
*
*@return float: o float desejado
*********************************************************/
float intToFloat(unsigned int intVal){
    intVal = intVal << TIPO;
    return *(float*)(&intVal);
}


/*
*Funcao para armazenar duas informações em um unico Int
*@param tipo  : primeira informação que deve ter no máxima um tamanho "TIPO" em bits
*       valor : segunda informação (int ou float já transformado em int e deslocado "TIPO" vezes
*
*@return unsigned int: a informação codificada
*********************************************************/
unsigned int packInfo(int tipo, int valor){
    unsigned int informacao = 0;
    informacao = (tipo << (32-TIPO)) | valor;
    return informacao;
}


/*
*Funcao para armazenar duas informações em um unico Int
*@param tipo  : primeira informação que deve ter no máxima um tamanho "TIPO" em bits
*       valor : segunda informação (int)
*
*@return unsigned int: a informação codificada
*********************************************************/
unsigned int packInt(int tipo, int valor){
    unsigned int informacao = 0;
    informacao = (tipo << (32-TIPO)) | valor;
    return informacao;
}

/*
*Funcao para armazenar duas informações em um unico Int
*@param tipo  : primeira informação que deve ter no máxima um tamanho "TIPO" em bits
*       valor : segunda informação (float)
*
*@return unsigned int: a informação codificada
*********************************************************/
unsigned int packFloat(int tipo, float valor){
    unsigned int informacao = 0;
    unsigned int valorInt = floatToInt(valor);
    //if(i == 1) printf("tipo ini = %d\n", tipo);

    informacao = (tipo << (32-TIPO)) | valorInt;
    //if(i == 1) printf("tipo ini = %d\n", unpackTipo(informacao));
    return informacao;
}

/*
*Funcao para retornar o 'tipo' de uma informação codificada
*@param info  : informação codificada
*
*@return int: o tipo
*********************************************************/
unsigned int unpackTipo(unsigned int info){
    unsigned int tipo = (info>>(32-TIPO));
    return tipo;
}

/*
*Funcao para retornar um Int de uma informação codificada
*@param info  : informação codificada
*
*@return int: a informação
*********************************************************/
unsigned int unpackInt(unsigned int info){
    int valor = (info << TIPO) >> TIPO;
    return valor;
}

/*
*Funcao para retornar um Float de uma informação codificada
*@param info  : informação codificada
*
*@return float: a informação
*********************************************************/
float unpackFloat(unsigned int info){
    //info = info;
    float valorF = intToFloat(info);
    return valorF;
}
