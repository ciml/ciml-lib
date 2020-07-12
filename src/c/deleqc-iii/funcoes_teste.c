#include "funcoes_teste.h"
//#include <iostream>
#include "stdio.h"
#include "math.h"
#include "stdlib.h"

void Problema_Teste(int prob, int *Restricoes, int *N_Coordenada, double *solucao, double *bounds, int nCoordenadasNova, int nRestricoesNova){
        
    switch(prob){
        case 1 :
            *Restricoes = 2;
            *N_Coordenada = 5;
            *solucao = 0;
            bounds[0] = -100;
            bounds[1] = 100;
             break;
        case 2 :
            *Restricoes = 2;
            *N_Coordenada = 5;
            *solucao = 0;
            bounds[0] = -100;
            bounds[1] = 100;
             break;
        case 3 :
            *Restricoes = 3;
            *N_Coordenada = 5;
            *solucao = 0;
            bounds[0] = -100;
            bounds[1] = 100;
             break;
        case 4 :
            *Restricoes = 3;
            *N_Coordenada = 5;
            *solucao = 0;
            bounds[0] = -100;
            bounds[1] = 100;
             break;
        case 5 :
            *Restricoes = 3;
            *N_Coordenada = 5;
            *solucao = 5.326647564;
            bounds[0] = -100;
            bounds[1] = 100;
             break;             
        case 6 :
            *Restricoes = 3;
            *N_Coordenada = 5;
            *solucao = 4.093023256;
            bounds[0] = -100;
            bounds[1] = 100;
             break;
        case 7 :
            if ( nCoordenadasNova == -1 && nRestricoesNova == -1){
                *Restricoes = 5;
                *N_Coordenada = 10;
                *solucao = 32.137;
            } else {
                *Restricoes = nRestricoesNova;
                *N_Coordenada = nCoordenadasNova;
                *solucao = 0.0;
            }
            bounds[0] = -100;
            bounds[1] = 100;
             break;
        case 8 :
            if ( nCoordenadasNova == -1 && nRestricoesNova == -1){
                *Restricoes = 5;
                *N_Coordenada = 10;
                *solucao = 35.377;
            } else {
                *Restricoes = nRestricoesNova;
                *N_Coordenada = nCoordenadasNova;
                *solucao = 0.0;
            }
            bounds[0] = -100;
            bounds[1] = 100;
             break;
        case 9 :
            if ( nCoordenadasNova == -1 && nRestricoesNova == -1){
                *Restricoes = 5;
                *N_Coordenada = 10;                
                *solucao = 36.975485;
            } else {
                *Restricoes = nRestricoesNova;
                *N_Coordenada = nCoordenadasNova;
                *solucao = 0.0;
            }
            bounds[0] = 2.56;
            bounds[1] = 5.12;
             break;
        case 10 :
            if ( nCoordenadasNova == -1 && nRestricoesNova == -1){
                *Restricoes = 5;
                *N_Coordenada = 10;
                //*solucao = 21485.3;
                *solucao = 21485.305028;
            } else {
                *Restricoes = nRestricoesNova;
                *N_Coordenada = nCoordenadasNova;
                *solucao = 0.0;
            }
            bounds[0] = -100;
            bounds[1] = 100;
             break;
        case 11 :
            if ( nCoordenadasNova == -1 && nRestricoesNova == -1){
                *Restricoes = 5;
                *N_Coordenada = 10;                
                *solucao = 0.150661;
            } else {
                *Restricoes = nRestricoesNova;
                *N_Coordenada = nCoordenadasNova;
                *solucao = 0.0;
            }
            bounds[0] = 300;
            bounds[1] = 600;
             break;
        case 12 :
            *Restricoes = 1;
            *N_Coordenada = 3;
            *solucao = 0;
            bounds[0] = -100;
            bounds[1] = 100;
             break;
        case 13 :
            *Restricoes = 14;
            *N_Coordenada = 30;
            *solucao = 0;
            bounds[0] = 0;
            bounds[1] = 100;
             break;
        case 14 : 
            if ( nCoordenadasNova == -1 && nRestricoesNova == -1){
                *Restricoes = 3;
                *N_Coordenada = 4;
                *solucao = 0;
            } else {
                *Restricoes = nRestricoesNova;
                *N_Coordenada = nCoordenadasNova;
                *solucao = 0.0;
            }
            bounds[0] = -100;
            bounds[1] = 100;
             break;
             
        case 19 : 
            *Restricoes = 8;
            *N_Coordenada = 16;
            //*solucao = 201.149429;  // solucao da referencia 244.899698
            *solucao = 200.731341;
            bounds[0] = 0;
            bounds[1] = 5;
             break;
             
        case 20 : 
            *Restricoes = 2;
            *N_Coordenada = 10;            
            *solucao = 0;
            bounds[0] = -100;
            bounds[1] = 100;
             break;
        
        case 21 : 
            *Restricoes = 8;
            *N_Coordenada = 10;            
            *solucao = 0;
            bounds[0] = -100;
            bounds[1] = 100;
             break;
             
        case 22 : 
            *Restricoes = 6;
            *N_Coordenada = 30;            
            *solucao = 0;
            bounds[0] = -100;
            bounds[1] = 100;
             break;
             
        case 23 : 
            *Restricoes = 24;
            *N_Coordenada = 30;            
            *solucao = 0;
            bounds[0] = -100;
            bounds[1] = 100;
             break;
             
        case 24 : 
            *Restricoes = 15;
            *N_Coordenada = 60;            
            *solucao = 0;
            bounds[0] = -100;
            bounds[1] = 100;
             break;
             
        case 25 : 
            *Restricoes = 45;
            *N_Coordenada = 60;            
            *solucao = 0;
            bounds[0] = -100;
            bounds[1] = 100;
             break;
        default:
            printf("Informe um problema va'lido!");
            break;
    }

    // Mesmos bounds para todos os problemas
    //bounds[0] = -100;
    //bounds[1] = 100;
   
   
}

             
void Avalia(double *id, int N_Coordenada, int prob, double **A, double *x){
    int i, j;
    double pi, soma, produto;

    pi = 3.1415926535;

    switch(prob){
        case 1:
            id[N_Coordenada] = pow((id[0] - 1), 2) + pow((id[1] - id[2]), 2) + pow((id[3] - id[4]), 2);
            break;
        case 2:
            id[N_Coordenada] = pow((id[0] - id[1]), 2) + pow((id[2] - 1), 2) + pow((id[3] - 1), 4) + pow((id[4] - 1), 6);
            break;
        case 3:
            id[N_Coordenada] = pow((id[0] - id[1]), 2) + pow((id[1] - id[2]), 2) + pow((id[2] - id[3]), 4) + pow((id[3] - id[4]), 2);
            break;
        case 4:
            id[N_Coordenada] = pow((id[0] - id[1]), 2) + pow((id[1] + id[2] - 2), 2) + pow((id[3] - 1),2) + pow((id[4] - 1),2);
            break;
        case 5:
            id[N_Coordenada] = pow((4*id[0] - id[1]), 2) + pow((id[1] + id[2] - 2), 2) + pow((id[3] - 1),2) + pow((id[4] - 1),2);
            break;
        case 6:
            id[N_Coordenada] = pow((id[0] - id[1]), 2) + pow((id[1] + id[2] - 2), 2) + pow((id[3] - 1),2) + pow((id[4] - 1),2);
            break;
        case 7:
            id[N_Coordenada] = 0.0;
            for(i = 0; i < N_Coordenada; i++){
                id[N_Coordenada] =  id[N_Coordenada] + pow(id[i], 2);
            }
            break;
        case 8:
            id[N_Coordenada] = 0.0;
            for(i = 0; i < N_Coordenada; i++){
                for(j = 0; j < N_Coordenada; j++){
                    id[N_Coordenada] = id[N_Coordenada] + exp(-pow((id[i] - id[j]),2))*id[i]*id[j];
                }
                id[N_Coordenada] = id[N_Coordenada] + id[i];
            }
            break;
        case 9:
            id[N_Coordenada] = 0.0;
            for(i = 0; i < N_Coordenada; i++){
                id[N_Coordenada] = id[N_Coordenada] + pow(id[i],2) + 10 - 10*cos(2*pi*id[i]);
            }
            break;
        case 10:
             id[N_Coordenada] = 0.0;
            for(i = 0; i < N_Coordenada - 1; i++){
                id[N_Coordenada] = id[N_Coordenada] + 100*pow((id[i+1] - pow(id[i],2)),2) + pow((id[i] - 1),2);
            }
            break;
        case 11:
            soma = 0.0;
            produto = 1.0;
            id[N_Coordenada] = 0.0;

            for(i = 0; i < N_Coordenada; i++){
                soma = soma + pow(id[i],2);
                produto = produto*cos(id[i]/sqrt(i+1));
            }
            id[N_Coordenada] = (1.0/4000.0)*soma - produto + 1.0;
            break;
        case 12:
            id[N_Coordenada] = id[0]*id[0] + id[1]*id[1] + id[2]*id[2] - 2*id[0]*id[1];
            break;
        case 13:
            id[N_Coordenada] = (0.5*((0.0625*id[0] - 24.24375)*(id[0] - 387.9) + (id[8] - 85.3)*(id[8] - 85.3) + 
                    (0.0625*id[1] - 24.425578125)*(id[1] - 390.80925) + (id[9] - 85.93975)*(id[9] -
                    85.93975) + (0.0625*id[2] - 24.6087699609375)*(id[2] - 393.740319375) + (id[10] -
                    86.584298125)*(id[10] - 86.584298125) + (0.0625*id[3] - 24.7933357356445)*(id[3] -
                    396.693371770313) + (id[11] - 87.2336803609375)*(id[11] - 87.2336803609375) + (
                    0.0625*id[4] - 24.9792857536619)*(id[4] - 399.66857205859) + (id[12] -
                    87.8879329636445)*(id[12] - 87.8879329636445) + (0.0625*id[5] - 25.1666303968143
                    )*(id[5] - 402.666086349029) + (id[13] - 88.5470924608719)*(id[13] -
                    88.5470924608719) + (0.0625*id[6] - 25.3553801247904)*(id[6] - 405.686081996647)
                    + (id[14] - 89.2111956543284)*(id[14] - 89.2111956543284) + (6.25*id[7] -
                    2554.55454757264)*(id[7] - 408.728727611622) + (100*id[15] - 8988.02796217359)*(
                    id[15] - 89.8802796217359)) + 0.5*((id[16] - 110.5)*(id[16] - 110.5) + (0.444*id[23]
                    - 65.3124)*(id[23] - 147.1) + (id[17] - 111.32875)*(id[17] - 111.32875) + (0.444*
                    id[24] - 65.802243)*(id[24] - 148.20325) + (id[18] - 112.163715625)*(id[18] -
                    112.163715625) + (0.444*id[25] - 66.2957598225)*(id[25] - 149.314774375) + (id[19]
                    - 113.004943492188)*(id[19] - 113.004943492188) + (0.444*id[26] -
                    66.7929780211688)*(id[26] - 150.434635182813) + (id[20] - 113.852480568379)*(id[20]
                    - 113.852480568379) + (0.444*id[27] - 67.2939253563275)*(id[27] -
                    151.562894946684) + (id[21] - 114.706374172642)*(id[21] - 114.706374172642) + (
                    0.444*id[28] - 67.7986297965)*(id[28] - 152.699616658784) + (id[22] -
                    115.566671978937)*(id[22] - 115.566671978937) + (0.444*id[29] - 68.3071195199738
                    )*(id[29] - 153.844863783725)));
            break;
        case 14: //sugerido pelo helio
            id[N_Coordenada] = 0.0;
            for(i = 0; i < N_Coordenada; i++){
                id[N_Coordenada] =  id[N_Coordenada] + pow(id[i], 2);
            }
            break;
            
        case 19: // Coville
            for(i = 0; i < N_Coordenada; i++){
                for(j = 0; j < N_Coordenada; j++){
                    id[N_Coordenada] += A[i][j]*( id[i]*id[i] + id[i] + 1 )*( id[j]*id[j] + id[j] + 1);
                }
            }
            break;
            
        default : // FUNCOES DE A PARTIR DE 20 Vaz09           
	    ; //This is an empty statement.
            double P1, P2, soma1, soma2, r, g1, g2;
            soma1 = soma2 = 0;
            r = 0.025;
            g1 = 1;
            g2 = 1;
            
            for (i = 0; i < N_Coordenada; i++){
                soma1 += (id[i] - x[i])*(id[i] - x[i]) ;
                soma2 += (id[i] - x[i]) ;                
            }
            P1 = soma1 + soma2;
            P2 = soma2;
                                
            id[N_Coordenada] = r*N_Coordenada*soma1 + (sin(g1*P1)*sin(g1*P1)) + (sin(g2*P2)*sin(g2*P2));

            break;
    }
}
