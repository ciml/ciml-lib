/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: gabriel
 *
 * Created on 8 de Julho de 2016, 09:17
 */

#include "fila.h"
#include "pilha.h"
#include <stdio.h>
#include <stdlib.h>
#include "arvlin.h"

/*
 * 
 */
int main(int argc, char** argv) {
    Arvlin *arv=arvTest();
    //arvTest(&arv);
    arvPrintVetor(arv);
    printArvore(arv);
    
    return (EXIT_SUCCESS);
}

