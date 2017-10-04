#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include <sys/time.h>
#include "representacao.h"

char *ltrim(char *s)
{
    while(isspace(*s)) s++;
    return s;
}

char *rtrim(char *s)
{
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
}

char *trim(char *s)
{
    return rtrim(ltrim(s));
}

int  next_power_of_2(int a){

    return (int)pow(2, (int) ceil( log2( a ) ) );
}

int is_power_of_2( int n )
{
   return (n & -n) == n;
}

/*
    Obtém o número de registros e variáveis do banco de dados
*/
void get_info_bancoDeDados(char *nomeArquivo, int *tamanho, int *qtdVariaveis){

    (*tamanho) = 0;
    (*qtdVariaveis) = 0;

    check(nomeArquivo != NULL, "Banco de dados inválido");

    FILE * fp = fopen(nomeArquivo, "r");

    check(fp != NULL, "Banco de dados inválido");

    int linhas = 0, ch;

    while(!feof(fp))
    {
        ch = fgetc(fp);
        if(ch == '\n')
        {
            (*tamanho)++;
        }
    }

    rewind(fp);

    (*tamanho)--;

    char linha[500], *saveptr;
	fgets(linha,500, fp);

    char * pntVariaveis = strtok_r(linha, "\t", &saveptr);

    while( pntVariaveis != NULL){
		(*qtdVariaveis)++;
 		 pntVariaveis = strtok_r( NULL, "\t", &saveptr);
	}

	fclose(fp);
}


//int binario_para_decimal(short *binarios, int inicio, int fim){
int binario_para_decimal(char *binarios, int inicio, int fim){

    int i,n=1; int valorNumerico=0;

    for(i=fim-1; i>=inicio; i--, n=n<<1){
        valorNumerico += n*((int)binarios[i]);
    }

    return valorNumerico;
}

//void gray_para_binario(short *gray, short *binarios){
void gray_para_binario(char *gray, char  *binarios){

    int i,j;

    for(i=0; i< TAMANHO_INDIVIDUO; i++){
        binarios[i] = gray[i];
    }

    int start;
    int end = 0;
    for (j = 0; j < DIMENSOES_PROBLEMA; j++) {
        start = end;
        end += TAMANHO_VALOR;
        for (i = start + 1; i < end; i++) {
            binarios[i] = binarios[i - 1] ^ binarios[i];
        }
    }
}

void obtem_fenotipo_individuo(individuo *p, short fenotipo[]){

    int i, j=0;

    //char genotipo_binario[TAMANHO_INDIVIDUO];
    //gray_para_binario(p->genotipo, genotipo_binario);

    for(i=0; i<DIMENSOES_PROBLEMA; i++, j+=TAMANHO_VALOR){

       //fenotipo[i] = binario_para_decimal(p->genotipo, j, j+TAMANHO_VALOR);
       //printf("%d - %d\n", i, fenotipo[i]);
    }
}

double getTime()
{
    struct timeval tv;
    gettimeofday(&tv,0);
    return (double)tv.tv_sec + 1.0e-6*(double)tv.tv_usec;
}
