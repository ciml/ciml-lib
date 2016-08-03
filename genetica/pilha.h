/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   pilha.h
 * Author: gabriel
 *
 * Created on 8 de Julho de 2016, 09:19
 */

#ifndef PILHA_H
#define PILHA_H

#ifdef __cplusplus
extern "C" {
#endif

typedef  struct {
    int numero;
    int *carga;
    int max;
   // int soma;//adaptaçao para arvlin
}Pilha;

void iniPilha(Pilha *,int);
void empilha(Pilha *,int);
void desempilha(Pilha *);
void printPilha(Pilha *);
void descarrega(Pilha *);
int topoPilha(Pilha *);
#ifdef __cplusplus
}
#endif

#endif /* PILHA_H */

