//
// Created by lucasmullers on 30/04/18.
//
#include <stdio.h>
#include <stdlib.h>

#ifndef CGP_REFATORADO_OPERACAOTXT_H
#define CGP_REFATORADO_OPERACAOTXT_H

void extraiTabelaVerdadeDoTxt(int numeroDeLinhas, int numeroDeColunas, int tabelaVerdade[], char *nomeDoArquivo);
void leituraDadosDaTabelaVerdade(int *numeroDeLinhas, int *numeroDeColunas, int *numeroDeEntradas, int *numeroDeSaidas, char *nomeDoArquivo);
void imprimePontuacaoEmEstatisticas(int pontuacaoFinal);
void imprimeEstatisticasNoTxt(double tempo, long int *geracaoInicial, long int *geracaoFinal, int seed);

#endif //CGP_REFATORADO_OPERACAOTXT_H
