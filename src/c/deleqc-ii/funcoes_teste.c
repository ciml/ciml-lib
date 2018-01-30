#include "funcoes_teste.h"
//#include <iostream>
#include "stdio.h"
#include "math.h"
#include "stdlib.h"
#include <float.h>


#ifndef VARIABLES
#define VARIABLES
double P20_A[6] = {8, 24, 20, 24, 16, 12};
double P20_B[4] = {29, 41, 13, 21};
double P20_C[6][4] ={{300, 270, 460, 800},
					 {740, 600, 540, 380},
					 {300, 490, 380, 760},
					 {430, 250, 390, 600},
					 {210, 830, 470, 680},
					 {360, 290, 400, 310}};
double P20_D[6][4] ={{-7, -4, -6, -8},
					 {-12, -9, -14, -7},
					 {-13, -12, -8, -4},
					 {-7, -9, -16, -8},
					 {-4, -10, -21, -13},
					 {-17, -9, -8, -4}};

double P106_A[4][2] =	{{0.23947, 0.75835},
						 {-0.0139904, -0.0661588},
						 {0.0093514, 0.0338147},
						 {0.0077308, 0.0373349}};
double P106_B[2][2] =	{{-0.0005719, 0.0016371},
						 {0.0042656, 0.0288996}};
#endif //variables


void Problema_Teste(int prob, int *Restricoes, int *N_Coordenada, double *solucao, double ***boundsMain, int nCoordenadasNova, int nRestricoesNova, int* nGs, int* nHs){

    int i;
    double** bounds = NULL;
    switch(prob){
        case 1 :
            *Restricoes = 2;
            *N_Coordenada = 5;
            *solucao = 0;
            bounds = (double**) malloc( (*N_Coordenada)*sizeof(double*) );
            for(i=0; i< *N_Coordenada; i++) {
                bounds[i] = (double*) malloc( 2*sizeof(double) );
                bounds[i][0] = -100;
                bounds[i][1] = 100;
            }
             break;
        case 2 :
            *Restricoes = 2;
            *N_Coordenada = 5;
            *solucao = 0;
            bounds = (double**) malloc( (*N_Coordenada)*sizeof(double*) );
            for(i=0; i< *N_Coordenada; i++) {
                bounds[i] = (double*) malloc( 2*sizeof(double) );
                bounds[i][0] = -100;
                bounds[i][1] = 100;
            }
             break;
        case 3 :
            *Restricoes = 3;
            *N_Coordenada = 5;
            *solucao = 0;
            bounds = (double**) malloc( (*N_Coordenada)*sizeof(double*) );
            for(i=0; i< *N_Coordenada; i++) {
                bounds[i] = (double*) malloc( 2*sizeof(double) );
                bounds[i][0] = -100;
                bounds[i][1] = 100;
            }
             break;
        case 4 :
            *Restricoes = 3;
            *N_Coordenada = 5;
            *solucao = 0;
            bounds = (double**) malloc( (*N_Coordenada)*sizeof(double*) );
            for(i=0; i< *N_Coordenada; i++) {
                bounds[i] = (double*) malloc( 2*sizeof(double) );
                bounds[i][0] = -100;
                bounds[i][1] = 100;
            }
             break;
        case 5 :
            *Restricoes = 3;
            *N_Coordenada = 5;
            *solucao = 5.326647564;
            bounds = (double**) malloc( (*N_Coordenada)*sizeof(double*) );
            for(i=0; i< *N_Coordenada; i++) {
                bounds[i] = (double*) malloc( 2*sizeof(double) );
                bounds[i][0] = -100;
                bounds[i][1] = 100;
            }
             break;
        case 6 :
            *Restricoes = 3;
            *N_Coordenada = 5;
            *solucao = 4.093023256;
            bounds = (double**) malloc( (*N_Coordenada)*sizeof(double*) );
            for(i=0; i< *N_Coordenada; i++) {
                bounds[i] = (double*) malloc( 2*sizeof(double) );
                bounds[i][0] = -100;
                bounds[i][1] = 100;
            }
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
            bounds = (double**) malloc( (*N_Coordenada)*sizeof(double*) );
            for(i=0; i< *N_Coordenada; i++) {
                bounds[i] = (double*) malloc( 2*sizeof(double) );
                bounds[i][0] = -100;
                bounds[i][1] = 100;
            }
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
            bounds = (double**) malloc( (*N_Coordenada)*sizeof(double*) );
            for(i=0; i< *N_Coordenada; i++) {
                bounds[i] = (double*) malloc( 2*sizeof(double) );
                bounds[i][0] = -100;
                bounds[i][1] = 100;
            }
             break;
        case 9 :
            if ( nCoordenadasNova == -1 && nRestricoesNova == -1){
                *Restricoes = 5;
                *N_Coordenada = 10;
                //*solucao = 36.975;
                *solucao = 36.975485;
            } else {
                *Restricoes = nRestricoesNova;
                *N_Coordenada = nCoordenadasNova;
                *solucao = 0.0;
            }
            bounds = (double**) malloc( (*N_Coordenada)*sizeof(double*) );
            for(i=0; i< *N_Coordenada; i++) {
                bounds[i] = (double*) malloc( 2*sizeof(double) );
                bounds[i][0] = 2.56;
                bounds[i][1] = 5.12;
            }
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
            bounds = (double**) malloc( (*N_Coordenada)*sizeof(double*) );
            for(i=0; i< *N_Coordenada; i++) {
                bounds[i] = (double*) malloc( 2*sizeof(double) );
                bounds[i][0] = -100;
                bounds[i][1] = 100;
            }
             break;
        case 11 :
            if ( nCoordenadasNova == -1 && nRestricoesNova == -1){
                *Restricoes = 5;
                *N_Coordenada = 10;
                //*solucao = 0.151;
                *solucao = 0.150661;
            } else {
                *Restricoes = nRestricoesNova;
                *N_Coordenada = nCoordenadasNova;
                *solucao = 0.0;
            }
            bounds = (double**) malloc( (*N_Coordenada)*sizeof(double*) );
            for(i=0; i< *N_Coordenada; i++) {
                bounds[i] = (double*) malloc( 2*sizeof(double) );
                bounds[i][0] = 300;
                bounds[i][1] = 600;
            }
             break;
        case 12 :
            *Restricoes = 1;
            *N_Coordenada = 3;
            *solucao = 0;
            bounds = (double**) malloc( (*N_Coordenada)*sizeof(double*) );
            for(i=0; i< *N_Coordenada; i++) {
                bounds[i] = (double*) malloc( 2*sizeof(double) );
                bounds[i][0] = -100;
                bounds[i][1] = 100;
            }
             break;
        case 13 :
            *Restricoes = 14;
            *N_Coordenada = 30;
            *solucao = 0;
            bounds = (double**) malloc( (*N_Coordenada)*sizeof(double*) );
            for(i=0; i< *N_Coordenada; i++) {
                bounds[i] = (double*) malloc( 2*sizeof(double) );
                bounds[i][0] = 0;
                bounds[i][1] = 100;
            }
             break;
        case 19 :
            *Restricoes = 8;
            *N_Coordenada = 16;
            //*solucao = 201.149429;  // solucao da referencia 244.899698
            *solucao = 200.731341;
            bounds = (double**) malloc( (*N_Coordenada)*sizeof(double*) );
            for(i=0; i< *N_Coordenada; i++) {
                bounds[i] = (double*) malloc( 2*sizeof(double) );
                bounds[i][0] = 0;
                bounds[i][1] = 5;
            }
             break;

        case 20: /// Floudas e Pardalos (1990), problem 2.8.1
			*Restricoes = 10;
			//*Restricoes = 9; // removing one of the 10 equality constraints; according to the authors, 1 constraint is redundant
			//*N_Coordenada = 9; // 9 are the number of solutions of the best solutions presented
			*N_Coordenada = 24; //the number of variables are mxn = 6x4 = 24
			*solucao = 15639;

			bounds = (double**) malloc( (*N_Coordenada)*sizeof(double*) );
			for (int i = 0; i < *N_Coordenada; i++) {
				bounds[i] = (double*) malloc( 2*sizeof(double) );
				bounds[i][0] = 0;
				bounds[i][1] = 30; //these bound constraints are not in the original formulation
			}

			*nGs = 0;
			*nHs = 0;

			break;

        ///Test-problems used in CEC 2018
        case 101 : ///G15
            *Restricoes = 1; ///number of linear constraints in the form h(x)=0
            *N_Coordenada = 3;
            *solucao = 961.715022289961;

            bounds = (double**) malloc( (*N_Coordenada)*sizeof(double*) );
            for(i=0; i< *N_Coordenada; i++) {
                bounds[i] = (double*) malloc( 2*sizeof(double) );
                bounds[i][0] = 0;
                bounds[i][1] = 10;
            }

            *nGs = 0; ///number of constraints in the form g(x) <= 0 ; bound contraints are not included here
            *nHs = 1; ///number of non-linear constraints in the form h(x) = 0
            break;

        case 102: ///G23
			*Restricoes = 3;
			*N_Coordenada = 9;
			*solucao = -400.055099999999584;

			bounds = (double**) malloc( (*N_Coordenada)*sizeof(double*) );
			for (int i = 0; i < *N_Coordenada; i++) {
				bounds[i] = (double*) malloc( 2*sizeof(double) );
			}
			bounds[0][0] = 0;
			bounds[0][1] = 300;
			bounds[1][0] = 0;
			bounds[1][1] = 300;
			bounds[5][0] = 0;
			bounds[5][1] = 300;
			bounds[2][0] = 0;
			bounds[2][1] = 100;
			bounds[4][0] = 0;
			bounds[4][1] = 100;
			bounds[6][0] = 0;
			bounds[6][1] = 100;
			bounds[3][0] = 0;
			bounds[3][1] = 200;
			bounds[7][0] = 0;
			bounds[7][1] = 200;
			bounds[8][0] = 0.01;
			bounds[8][1] = 0.03;

			*nGs = 2;
			*nHs = 1;

			break;

		case 103: /// Pooling problem from Ryoo and Sahinidis (1995).
			*Restricoes = 4;
			*N_Coordenada = 10;
			*solucao = -400.0;

			bounds = (double**) malloc( (*N_Coordenada)*sizeof(double*) );
			for (int i = 0; i < *N_Coordenada; i++) {
				bounds[i] = (double*) malloc( 2*sizeof(double) );
			}
			bounds[0][0] = 0;
			bounds[0][1] = 300;
			bounds[1][0] = 0;
			bounds[1][1] = 300;
			bounds[5][0] = 0;
			bounds[5][1] = 300;
			bounds[2][0] = 0;
			bounds[2][1] = 100;
			bounds[4][0] = 0;
			bounds[4][1] = 100;
			bounds[6][0] = 0;
			bounds[6][1] = 100;
			bounds[3][0] = 0;
			bounds[3][1] = 200;
			bounds[7][0] = 0;
			bounds[7][1] = 200;
			bounds[8][0] = 0;
			bounds[8][1] = 200;
			bounds[9][0] = 1;
			bounds[9][1] = 3;

			*nGs = 2;
			*nHs = 1;

			break;

		case 104: /// From Ryoo and Sahinidis (1995).
			*Restricoes = 3; //4?
			*N_Coordenada = 6;
			*solucao = -13.401904;

			bounds = (double**) malloc( (*N_Coordenada)*sizeof(double*) );
			for (int i = 0; i < *N_Coordenada; i++) {
				bounds[i] = (double*) malloc( 2*sizeof(double) );
			}
			bounds[0][0] = 0;
			bounds[0][1] = 3;
			bounds[1][0] = 0;
			bounds[1][1] = 4;
			bounds[2][0] = 0;
			bounds[2][1] = 4;
			bounds[3][0] = 0;
			bounds[3][1] = 2;
			bounds[4][0] = 0;
			bounds[4][1] = 2;
			bounds[5][0] = 0;
			bounds[5][1] = 6;

			*nGs = 3;
			*nHs = 0;

			break;

		case 105: /// Floudas e Pardalos (1990)
			*Restricoes = 3;
			*N_Coordenada = 9;
			*solucao = -600.0;

			bounds = (double**) malloc( (*N_Coordenada)*sizeof(double*) );
			for (int i = 0; i < *N_Coordenada; i++) {
				bounds[i] = (double*) malloc( 2*sizeof(double) );
			}
			bounds[0][0] = 0;
			bounds[0][1] = 800;
			bounds[1][0] = 0;
			bounds[1][1] = 800;
			bounds[2][0] = 0;
			bounds[2][1] = 600;
			bounds[3][0] = 0;
			bounds[3][1] = 200;
			bounds[4][0] = 0;
			bounds[4][1] = 600;
			bounds[5][0] = 0;
			bounds[5][1] = 200;
			bounds[6][0] = 0;
			bounds[6][1] = 600;
			bounds[7][0] = 0;
			bounds[7][1] = 200;
			bounds[8][0] = 1;
			bounds[8][1] = 3;

			*nGs = 2;
			*nHs = 1;

			break;

		case 106: /// Floudas e Pardalos (1990)
		{
			int i;
			*Restricoes = 13;
			*N_Coordenada = 48; //a candidate solution is represented as F1-F20, fa(5, 13), fb(5, 13), fc(5, 13), xa(5, 6, 9, 13, 14, 18), xb(5, 6, 9, 13, 14, 18), xc(5, 6, 9, 13, 14, 18), ra1, rb1, rb2, rc2
			*solucao = 1.5671;

			bounds = (double**) malloc( (*N_Coordenada)*sizeof(double*) );
			//bound constraints for Fs, fs and xs
			//these bound constraints are not in the reference
			for (i = 0; i < (*N_Coordenada)-4; i++) {
				bounds[i] = (double*) malloc( 2*sizeof(double) );
				bounds[i][0] = 0.0;
                //bounds[i][1] = 200;
                bounds[i][1] = 150.0;
			}
			//bound constraints for rs
			for(; i< *N_Coordenada; i++) {
                bounds[i] = (double*) malloc( 2*sizeof(double) );
                bounds[i][0] = 0.85;
                bounds[i][1] = 1.0;
            }

			*nGs = 0;
			*nHs = 25;

			break;
		}
        default:
            printf("Informe um problema va'lido!");
            break;

    }

    if (bounds != NULL) {
        *boundsMain = bounds;
    }

    // Mesmos bounds para todos os problemas
    //bounds[0] = -100;
    //bounds[1] = 100;

}

void Avalia(double *id, int N_Coordenada, int prob, double **A, double** bounds, double epsilonEqualityConstraints) {
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
                produto =   produto*cos(id[i]/sqrt(i+1));
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
        case 19:
            for(i = 0; i < N_Coordenada; i++){
                for(j = 0; j < N_Coordenada; j++){
                    id[N_Coordenada] += A[i][j]*( id[i]*id[i] + id[i] + 1 )*( id[j]*id[j] + id[j] + 1);
                }
            }
            break;

        case 20:
			{
				int m = 6;
				int n = 4;
				int k;
				for(i = 0; i < m; i++){
					for(j = 0; j < n; j++){
						k = i * n + j;
						id[N_Coordenada] += P20_C[i][j] * id[ k ] + P20_D[i][j] * id[ k ] * id[ k ];
					}
				}
			}
            break;
        case 101: /// G15
            id[N_Coordenada] = 1000.0 - pow(id[0], 2.0) - 2.0 * id[1] * id[1] - id[2] * id[2] - id[0] * id[1] - id[0] * id[2];

            id[N_Coordenada+1] = pow(id[0], 2.0) + pow(id[1], 2.0) + pow(id[2], 2.0) - 25.0;

            //equality constraints
            for (j = N_Coordenada+1+0; j < N_Coordenada+1+0+1; j++) {
                id[ j ] = fabs(id[j]) - epsilonEqualityConstraints;
            }

            //id[N_Coordenada+1] = 0;
            break;

		case 102: /// G23

			id[N_Coordenada] = -9.0 * id[4] - 15.0 * id[7] + 6.0 * id[0] + 16.0 * id[1] + 10.0 * (id[5] + id[6]);

			id[N_Coordenada+1] = id[8] * id[2] + 0.02 * id[5] - 0.025 * id[4];
			id[N_Coordenada+2] = id[8] * id[3] + 0.02 * id[6] - 0.015 * id[7];
			id[N_Coordenada+3] = 0.03 * id[0] + 0.01 * id[1] - id[8] * (id[2] + id[3]);


			//equality constraints
            for (j = N_Coordenada+1+2; j < N_Coordenada+1+2+1; j++) {
                id[ j ] = fabs(id[j]) - epsilonEqualityConstraints;
            }

            break;

        case 103:

			id[N_Coordenada] = -9.0 * id[4] - 15.0 * id[8] + 6.0 * id[0] + 16.0 * id[1] + 10.0 * id[5];

			id[N_Coordenada+1] = id[9] * id[2] + 2 * id[6] - 2.5 * id[4];
			id[N_Coordenada+2] = id[9] * id[3] + 2 * id[7] - 1.5 * id[8];
			id[N_Coordenada+3] = 3 * id[0] + id[1] - id[9] * id[2] - id[9] * id[3];

			//equality constraints
            for (j = N_Coordenada+1+2; j < N_Coordenada+1+2+1; j++) {
                id[ j ] = fabs(id[j]) - epsilonEqualityConstraints;
            }

            /*if (id[N_Coordenada] == 0) {
                printf("ERROR?\n");
                for (j = 0; j < 30; j++) {
                    printf("%f ", id[ j ]);
                }
                getchar();
			}*/

            break;

        case 104:

			id[N_Coordenada] = pow(id[0], 0.6) + pow(id[1], 0.6) + pow(id[2], 0.4) - 4.0 * id[2] + 2.0 * id[3] + 5.0 * id[4] - id[5];
			//as same search techniques also consider candidate solutions outside of the bound constraints, this evaluation can generate a "nan" (pow with a non-integer base and a negative value)
			if ( isnan( id[N_Coordenada] ) ) {
				id[N_Coordenada] = DBL_MAX; // the max value is divided by 4 because of constraint handling via penalty methods ==> values are added to the objective function value
			}

			id[N_Coordenada+1] = id[0] + 2.0 * id[3] - 4.0;
			id[N_Coordenada+2] = id[1] + id[4] - 4.0;
			id[N_Coordenada+3] = id[2] + id[5] - 6.0;

            break;

        case 105:

            //solution is represented as:
            //0, 1: A, B
            //2, 3: Cx, Cy
            //4, 5: Px, Py
            //6, 7: x, y
            //8: p

			id[N_Coordenada] = 6.0 * id[0] + 16.0 * id[1] + 10.0 * id[2] + 10.0 * id[3] - 9.0 * id[6] - 15.0 * id[7];

			id[N_Coordenada+1] = 2.0 * id[2] - 2.5 * id[6] + id[4] * id[8];
			id[N_Coordenada+2] = 2.0 * id[3] - 1.5 * id[7] + id[5] * id[8];
			id[N_Coordenada+3] = -3.0 * id[0] - id[1] + id[4] * id[8] + id[5] * id[8];

			//equality constraints
            for (j = N_Coordenada+1+2; j < N_Coordenada+1+2+1; j++) {
                id[ j ] = fabs(id[j]) - epsilonEqualityConstraints;
            }

            break;
        case 106:

			//solution is
			//0-19: F1-F20
			//20, 21: fa(5, 13)
			//22, 23: fb(5, 13)
			//24, 25: fc(5, 13)
			//26-31: xa(5, 6, 9, 13, 14, 18)
			//32-37: xb(5, 6, 9, 13, 14, 18)
			//38-43: xc(5, 6, 9, 13, 14, 18)
			//44-47: ra1, rb1, rb2, rc2

			id[N_Coordenada] =  P106_A[0][0] + ( P106_A[1][0] + P106_A[2][0] * id[44] + P106_A[3][0] * id[45] + P106_B[0][0] * id[26] + P106_B[1][0] * id[32] ) * id[4]
							  + P106_A[0][1] + ( P106_A[1][1] + P106_A[2][1] * id[46] + P106_A[3][1] * id[47] + P106_B[0][1] * id[29] + P106_B[1][1] * id[35] ) * id[12];

			id[N_Coordenada+1] = id[5] * id[27] - id[44] * id[20];
			id[N_Coordenada+2] = id[13] * id[36] - id[46] * id[23];
			id[N_Coordenada+3] = id[8] * id[34] - id[45] * id[22];
			id[N_Coordenada+4] = id[17] * id[43] - id[47] * id[25];

			id[N_Coordenada+5] = id[20] - id[4] * id[26];
			id[N_Coordenada+6] = id[22] - id[4] * id[32];
			id[N_Coordenada+7] = id[24] - id[4] * id[38];

			id[N_Coordenada+8] = id[21] - id[12] * id[29];
			id[N_Coordenada+9] = id[23] - id[12] * id[35];
			id[N_Coordenada+10] = id[25] - id[12] * id[41];

			id[N_Coordenada+11] = id[20] - id[5] * id[27] - id[8] * id[28];
			id[N_Coordenada+12] = id[22] - id[5] * id[33] - id[8] * id[34];
			id[N_Coordenada+13] = id[24] - id[5] * id[39] - id[8] * id[40];

			id[N_Coordenada+14] = id[21] - id[13] * id[30] - id[17] * id[31];
			id[N_Coordenada+15] = id[23] - id[13] * id[36] - id[17] * id[37];
			id[N_Coordenada+16] = id[25] - id[13] * id[42] - id[17] * id[43];

			/*id[N_Coordenada+17] = 0.333 * id[0] + id[14] * id[30] - id[20];
			id[N_Coordenada+18] = 0.333 * id[0] + id[14] * id[36] - id[22];
			id[N_Coordenada+19] = 0.333 * id[0] + id[14] * id[42] - id[24];

			id[N_Coordenada+20] = 0.333 * id[1] + id[9] * id[28] - id[21];
			id[N_Coordenada+21] = 0.333 * id[1] + id[9] * id[34] - id[23];
			id[N_Coordenada+22] = 0.333 * id[1] + id[9] * id[40] - id[25];

			id[N_Coordenada+23] = 0.333 * id[2] + id[6] * id[27] + id[10] * id[28] + id[15] * id[30] + id[18] * id[31] - 30;
			id[N_Coordenada+24] = 0.333 * id[2] + id[6] * id[33] + id[10] * id[34] + id[15] * id[36] + id[18] * id[37] - 50;
			id[N_Coordenada+25] = 0.333 * id[2] + id[6] * id[39] + id[10] * id[40] + id[15] * id[42] + id[18] * id[43] - 30;*/


			id[N_Coordenada+17] = id[0] / 3.0 + id[14] * id[30] - id[20]; //0.333 * id[0] + id[14] * id[30] - id[20];
			id[N_Coordenada+18] = id[0]/3.0 + id[14] * id[36] - id[22];
			id[N_Coordenada+19] = id[0]/3.0 + id[14] * id[42] - id[24];

			id[N_Coordenada+20] = id[1]/3.0 + id[9] * id[28] - id[21];
			id[N_Coordenada+21] = id[1]/3.0 + id[9] * id[34] - id[23];
			id[N_Coordenada+22] = id[1]/3.0 + id[9] * id[40] - id[25];

			id[N_Coordenada+23] = id[2]/3.0 + id[6] * id[27] + id[10] * id[28] + id[15] * id[30] + id[18] * id[31] - 30;
			id[N_Coordenada+24] = id[2]/3.0 + id[6] * id[33] + id[10] * id[34] + id[15] * id[36] + id[18] * id[37] - 50;
			id[N_Coordenada+25] = id[2]/3.0 + id[6] * id[39] + id[10] * id[40] + id[15] * id[42] + id[18] * id[43] - 30;


			//equality constraints
            for (j = N_Coordenada+1+0; j < N_Coordenada+1+0+25; j++) {
                id[ j ] = fabs(id[j]) - epsilonEqualityConstraints;
            }

            break;
    }
}

void boundConstraints(double *individual, int dimension, int nGs, int nHs, double** bounds) {
    int i = 0;
    int iSumBoundViolation = dimension+1+nGs+nHs+dimension;
    individual[ iSumBoundViolation ] = 0;
    for (int j = dimension+1+nGs+nHs; j < iSumBoundViolation; j++) {
        if ( individual[ i ] < bounds[ i ][ 0 ] ) {
            individual[ j ] = bounds[ i ][ 0 ] - individual[ i ];
        } else if ( individual[ i ] > bounds[ i ][ 1 ] ) {
            individual[ j ] = individual[ i ] - bounds[ i ][ 0 ];
        } else {
            individual[ j ] = 0;
        }
        individual[ iSumBoundViolation ] += individual[ j ];
        i++;
    }

}

void sumViolation(double *individual, int dimension, int nGs, int nHs, double** bounds, int nLHs, double* maxConstraints) {
    int j;
    int iSumViolation = dimension+1+nGs+nHs+dimension+1;
    int idSumViolationWithoutLE = dimension+1+nGs+nHs+dimension+1+1+1+nLHs;
    int iSumAllViolation = dimension+1+nGs+nHs+dimension+1+1+1+nLHs+1;

//    printf("nLHs = %d\n", nLHs);
//    printf("iSumViolation = %d\n", iSumViolation);
//    printf("idSumViolationWithoutLE = %d\n", idSumViolationWithoutLE);
//    printf("iSumAllViolation = %d\n", iSumAllViolation);
//    printf("variables = from %d to %d\n", 0, dimension-1);
//    printf("constraints Gs and Hs (nonlinear) = from %d to %d\n", dimension+1, dimension+1+nGs+nHs-1);
//    printf("bound constraints = from %d to %d\n", dimension+1+nGs+nHs, dimension+1+nGs+nHs+dimension-1);
//    printf("linear equality constraints = from %d to %d\n", dimension+1+nGs+nHs+dimension+3, dimension+1+nGs+nHs+dimension+3+nLHs-1);


    individual[ iSumViolation ] = 0;
    individual[ idSumViolationWithoutLE ] = 0;
    individual[ iSumAllViolation ] = 0;
    for (j = dimension+1; j < dimension+1+nGs+nHs+dimension; j++) {
		if (individual[ j ] > 0 && maxConstraints != NULL && maxConstraints[ j ] > 0) {
			//if individual is unfeasible and
			//any solution is unfeasible with respect to the j-th constraint
			//Sum of the normalized violation values
			//individual[ iSumViolation ] += individual[ j ] / maxConstraints[ j ]; //g(x)<=0, h(x)=0 where h(x) is non-linear, and the bound constraints
			//individual[ idSumViolationWithoutLE ] += individual[ j ] / maxConstraints[ j ];
			///Sum of the normalized violation values -- it is not normalized as the normalization can cause instability when maxConstraints ~ 0
			individual[ iSumViolation ] += individual[ j ]; //g(x)<=0, h(x)=0 where h(x) is non-linear, and the bound constraints
			individual[ idSumViolationWithoutLE ] += individual[ j ];
		}
		if (individual[ j ] > 0) {
			individual[ iSumAllViolation ] += individual[ j ];
		}
    }
    //individual[ iSumViolation ] += individual[ iSumViolation-1 ]; //bound constraints
    for(j=iSumViolation+2; j<iSumViolation+2+nLHs; j++) {
		//if (individual[ j ] > 0 && maxConstraints != NULL && maxConstraints[ j ] > 0) {
		if (individual[ j ] > 0 && maxConstraints != NULL) {
			//individual[ iSumViolation ] += individual[ j ] / maxConstraints[ j ]; //h(x)=0 where h(x) is linear
			individual[ iSumViolation ] += individual[ j ]; //h(x)=0 where h(x) is linear
		}
		if (individual[ j ] > 0) {
			individual[ iSumAllViolation ] += individual[ j ];
		}
    }
}

void linearConstraintsData(int prob, double** e, double** eTransposed, double* b) {

	switch(prob){

		case 20:
			{
				int m = 6;
				int n = 4;
				int i, j, k;

				//for(int i=0; i<m+n-1; i++) { // removing 1 constraint
				for(int i=0; i<m+n; i++) {
					for (int j=0; j<m*n; j++) {
						e[i][j] = 0;
					}
				}

				//linear equality constraints

				//constraints using B
				for(j=0; j<n; j++) {
					for (i=0; i<m; i++) {
						k = i*n + j;
						e[j][k] = 1;
					}
					b[j] = P20_B[j];
				}
				//constraints using A
				//for(i=0; i<m-1; i++) { // removing the last constraint
				for(i=0; i<m; i++) {
					for (j=0; j<n; j++) {
						k = i*n + j;
						e[i+n][k] = 1;
					}
					b[i+n] = P20_A[i];
				}

				//print matrix E
				/*printf("Matrix E:\n");
				for(i=0; i<n+m; i++) { //linear equality constraints
					for (j=0; j<m*n; j++) {
						printf(" | %f", e[i][j]);
					}
					printf("\n");
				}*/

				if ( eTransposed != NULL ) {
					//for(int i=0; i<m+n-1; i++) { //removing 1 constraint
					for(int i=0; i<m+n; i++) {
						for (int j=0; j<m*n; j++) {
							eTransposed[j][i] = e[i][j];
						}
					}
				}
			}
		break;

		case 101: /// G15
			e[0][0] = 8.0;
			e[0][1] = 14.0;
			e[0][2] = 7.0;
			b[0] = 56.0;
			if ( eTransposed != NULL ) {
				for(int i=0; i<3; i++) {
					eTransposed[i][0] = e[0][i];
				}
			}
		break;

		case 102: /// G23
			for(int i=0; i<3; i++) { //linear equality constraints
				for (int j=0; j<9; j++) {
					e[i][j] = 0;
				}
				b[i] = 0;
			}
			//1-th LEC
			e[0][0] = 1;
			e[0][1] = 1;
			e[0][2] = -1;
			e[0][3] = -1;
			//2-th LEC
			e[1][2] = 1;
			e[1][5] = 1;
			e[1][4] = -1;
			//3-th LEC
			e[2][3] = 1;
			e[2][6] = 1;
			e[2][7] = -1;
			if ( eTransposed != NULL ) {
				for(int i=0; i<3; i++) {
					for (int j=0; j<9; j++) {
						eTransposed[j][i] = e[i][j];
					}
				}
			}
			break;
		case 103:
			for(int i=0; i<4; i++) { //linear equality constraints
				for (int j=0; j<10; j++) {
					e[i][j] = 0;
				}
				b[i] = 0;
			}
			//1-th LEC
			e[0][0] = 1;
			e[0][1] = 1;
			e[0][2] = -1;
			e[0][3] = -1;
			//2-th LEC
			e[1][2] = 1;
			e[1][4] = -1;
			e[1][6] = 1;
			//3-th LEC
			e[2][3] = 1;
			e[2][7] = 1;
			e[2][8] = -1;
			//4-th LEC
			e[3][5] = -1;
			e[3][6] = 1;
			e[3][7] = 1;
			if ( eTransposed != NULL ) {
				for(int i=0; i<4; i++) {
					for (int j=0; j<10; j++) {
						eTransposed[j][i] = e[i][j];
					}
				}
			}
			break;

		case 104:
			for(int i=0; i<3; i++) { //linear equality constraints
				for (int j=0; j<6; j++) {
					e[i][j] = 0;
				}
				b[i] = 0;
			}
			//1-th LEC
			e[0][0] = -3;
			e[0][1] = 1;
			e[0][3] = -3;
			//2-th LEC
			e[1][1] = -2;
			e[1][2] = 1;
			e[1][4] = -2;
			//3-th LEC
			e[2][3] = 4;
			e[2][5] = -1;
			//4-th LEC
			//e[3][6] = 1;
			//e[3][7] = 1;
			//e[3][5] = -1;
			if ( eTransposed != NULL ) {
				for(int i=0; i<3; i++) {
					for (int j=0; j<6; j++) {
						eTransposed[j][i] = e[i][j];
					}
				}
			}
			break;

		case 105:
			for(int i=0; i<3; i++) { //linear equality constraints
				for (int j=0; j<9; j++) {
					e[i][j] = 0;
				}
				b[i] = 0;
			}

			//1-th LEC
			e[0][0] = -1;
			e[0][1] = -1;
			e[0][4] = 1;
			e[0][5] = 1;
			//2-th LEC
			e[1][2] = -1;
			e[1][4] = -1;
			e[1][6] = 1;
			//3-th LEC
			e[2][3] = -1;
			e[2][5] = -1;
			e[2][7] = 1;
			if ( eTransposed != NULL ) {
				for(int i=0; i<3; i++) {
					for (int j=0; j<9; j++) {
						eTransposed[j][i] = e[i][j];
					}
				}
			}
			break;

		case 106:
			for(int i=0; i<13; i++) { //linear equality constraints
				for (int j=0; j<48; j++) {
					e[i][j] = 0;
				}
				b[i] = 0;
			}

			//1-th LEC
			e[0][0] = 1;
			e[0][1] = 1;
			e[0][2] = 1;
			e[0][3] = 1;
			b[0] = 300;
			//2-th LEC
			e[1][5] = 1;
			e[1][6] = -1;
			e[1][7] = -1;
			//3-th LEC
			e[2][8] = 1;
			e[2][9] = -1;
			e[2][10] = -1;
			e[2][11] = -1;
			//4-th LEC
			e[3][13] = 1;
			e[3][14] = -1;
			e[3][15] = -1;
			e[3][16] = -1;
			//5-th LEC
			e[4][17] = 1;
			e[4][18] = -1;
			e[4][19] = -1;
			//6-th LEC
			e[5][39] = 1;
			//7-th LEC
			e[6][31] = 1;
			//8-th LEC
			e[7][26] = 1;
			e[7][32] = 1;
			e[7][38] = 1;
			b[7] = 1;
			//9-th LEC
			e[8][27] = 1;
			e[8][33] = 1;
			e[8][39] = 1;
			b[8] = 1;
			//10-th LEC
			e[9][28] = 1;
			e[9][34] = 1;
			e[9][40] = 1;
			b[9] = 1;
			//11-th LEC
			e[10][29] = 1;
			e[10][35] = 1;
			e[10][41] = 1;
			b[10] = 1;
			//12-th LEC
			e[11][30] = 1;
			e[11][36] = 1;
			e[11][42] = 1;
			b[11] = 1;
			//13-th LEC
			e[12][31] = 1;
			e[12][37] = 1;
			e[12][43] = 1;
			b[12] = 1;

			if ( eTransposed != NULL ) {
				for(int i=0; i<13; i++) {
					for (int j=0; j<48; j++) {
						eTransposed[j][i] = e[i][j];
					}
				}
			}
			break;

		default:
			printf("This problem has no definition.");
			exit(7);
	}

}


/*
//solution for the test-problem 106 -- fixed with respect to that presented in reference paper
double sol[48] = {0, 85.714, 77.143, 137.143, 57.143, 24.286, 0, 24.286, 32.857, 0, 32.857, 0, 85.714, 57.143, 57.143, 0, 0, 28.571, 0, 28.571,
		28.571, 28.571,
		24.286, 28.571,
		4.286, 28.571,
		0.5, 1.0, 0.13, 0.333, 0.5, 0,
		0.425, 0, 0.739, 0.333, 0.425, 0.15,
		0.075, 0, 0.13, 0.333, 0.075, 0.85,
		0.85, 1.0,
		0.85, 0.85};
	for(int w=0; w<48; w++) {
		populacao[0][w] = sol[w];
	}
*/

/*
//solution for the test-problem 106 -- adjusted after fixed
double sol[48] = {0, 85.714, 77.143, 137.143, 57.143, 24.286, 0, 24.286, 32.857, 0, 32.857, 0, 85.714, 57.143, 57.143, 0, 0, 28.571, 0, 28.571,
		28.571, 28.571,
		24.286, 28.571,
		4.286, 28.571,
		0.5, 1.0, 0.1305, 1/3.0, 0.5, 0,
		0.425, 0, 0.739, 1/3.0, 0.425, 0.15,
		0.075, 0, 0.1305, 1/3.0, 0.075, 0.85,
		0.85, 1.0,
		0.85, 0.85};
	for(int w=0; w<48; w++) {
		populacao[0][w] = sol[w];
	}
*/

/*
//solution for the test-problem 20-- fixed with respect to that presented in reference paper
double sol[24] = {	6, 2, 0, 0,
					0, 3, 0, 21,
					20, 0, 0, 0,
					0, 24, 0, 0,
					3, 0, 13, 0,
					0, 12, 0, 0};
	for(int w=0; w<24; w++) {
		populacao[0][w] = sol[w];
	}
*/

/*
//solution for the test-problem 103 -- fixed with respect to that presented in reference paper
double sol[10] = {0, 100, 0, 100, 0, 100, 0, 100, 200, 1};
	for(int w=0; w<10; w++) {
		populacao[0][w] = sol[w];
	}
*/

/*
//solution for the test-problem 105
double sol[9] = {300, 0, 300, 0, 300, 0, 600, 0, 3};
	for(int w=0; w<9; w++) {
		populacao[0][w] = sol[w];
	}
*/
