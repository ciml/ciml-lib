/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   arvlin.h
 * Author: gabriel
 *
 * Created on 8 de Julho de 2016, 09:46
 */

#ifndef ARVLIN_H
#define ARVLIN_H

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct{
      int Nfilhos;
      int *elementos;
      int *filhos;
    }Arvlin;
    void iniArvore(Arvlin *arv,int N);
    void arvPrintVetor(Arvlin *arv);
    void printNivel(Arvlin *arv,int nivel,int i);
    Arvlin *arvTest();
    void printArvore(Arvlin *arv);




#ifdef __cplusplus
}
#endif

#endif /* ARVLIN_H */

