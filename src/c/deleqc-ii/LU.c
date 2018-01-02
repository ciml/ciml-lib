#include <stdlib.h>
#include <stdio.h>
#include<math.h>

//Funções auxiliares para impressão de variáveis-----------------------------
void PrintDoubleVector(double *vec, unsigned int c_size){
    int i;
    for(i=0 ; i<c_size ; i++) printf("%8.4f ",vec[i]);
    printf("\n");
}

void PrintUnsIntVector(unsigned int *vec, unsigned int c_size){
    int i;
    for(i=0 ; i<c_size ; i++) printf("%8d ",vec[i]);
    printf("\n");
}

void PrintUpperMatrix(double **mat, int *p, int r_size, int c_size){
    int i,j;
    for(i=0 ; i<r_size ; i++){
        for(j=0 ; j<c_size ; j++){
            if(i>j) printf("%8.4lf ",0.0);
            else printf("%8.4lf ",mat[p[i]][j]);
        }
        printf("\n");
    }
}

void PrintLowerMatrix(double **mat, int *p, int r_size, int c_size){
    int i,j;
    for(i=0 ; i<r_size ; i++){
        for(j=0 ; j<c_size ; j++){
            if(i<j) printf("%8.4lf ",0.0);
            else if(i==j) printf("%8.4lf ",1.0);
            else printf("%8.4lf ",mat[p[i]][j]);
        }
        printf("\n");
    }
}
//-----------------------------------------------------------------------------

//Função responsável pela decomposição da matriz A em matrizes LU--------------
//retorna o resultado na própria matriz A
void lu(double **A,  int *p,  int n){
    double max;	//atual valor do pivo
    int imax;	//atual valor do índice do pivo
    int temp;	//variavel auxiliar
    double m;	//fator multiplicativo

    int k,i,j;
    for(k=0 ; k<n-1 ; k++){
        //Encontrar o pivo--------
        max = abs(A[p[k]][k]);
        imax = k;
        for(i=k+1 ; i<n ; i++){
            if( max < fabs(A[p[i]][k]) ){
                max = fabs(A[p[i]][k]);
                imax = i;
            }
        }
	//------------------------

	//Troca 'vitual das linhas da matriz A'---
        temp = p[k];
        p[k] = p[imax];
        p[imax] = temp;
	//----------------------------------------

        //Anular elementos--------
        for(i=k+1 ; i<n ; i++){
			if ( A[p[k]][k] == 0 ) {
				printf("The linear system cannot be solved.");
				exit(8);
			}
            m = A[p[i]][k]/A[p[k]][k];
            for(j=k ; j<n ; j++) A[p[i]][j] = A[p[i]][j] - m*A[p[k]][j];
            A[p[i]][k] = m;
        }
	//------------------------
    }
}
//-----------------------------------------------------------------------------

//Função para resolver sistemas do tipo Ax=b usando----------------------------
//decomposição LU
double* lu_solve(double **A,  int *p, double *b,  int n){
    double *aux2 = (double*) malloc (n*sizeof(double)); //variável para amazenar valores parciais da resposta final
    double *resF = (double*) malloc (n*sizeof(double)); //variável para amazenar valores finais

    int k,j;
    //Substituição progressiva-------------------------
    for(k=0 ; k<n ; k++){
        //printf("\nk=%d\n", k);
        aux2[k] = b[p[k]];
        for(j=0 ; j<k ; j++){
            //printf("\nj=%d\n", j);
             aux2[k] = aux2[k] - A[p[k]][j]*aux2[j];
        }
        //aux2[k] = aux2[k]/1.0; //comentei esse conta aqui pois nao me parece que serve p nada
    }
    //-------------------------------------------------

    //Substituição regressiva--------------------------
    //obs: como usamos tipo 'unsigned int', para que
    //o comando 'for' funcionasse corretamente, foi necessário
    //uma adaptação na variável k (-1 == 1)
    for(k=n ; k>0 ; k--){
        resF[k-1] = aux2[k-1];
        for(j=k ; j<n ; j++){
            resF[k-1] = resF[k-1] - A[p[k-1]][j]*resF[j];
        }
        resF[k-1] = resF[k-1]/A[p[k-1]][k-1];
    }
    //-------------------------------------------------

    free(aux2);

    return resF;
}

