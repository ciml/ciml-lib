#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include "utils.h"
#include "parametros.h"
#include "CL/cl.h"

void grayParaBinario(int *gray, int *binarios){

    int i,j;

    for(i=0; i< BITS_POR_DIMENSAO * DIMENSOES; i++){
        binarios[i] = gray[i];
    }

    int start;
    int end = 0;
    for (j = 0; j < DIMENSOES; j++) {
        start = end;
        end += BITS_POR_DIMENSAO;
        for (i = start + 1; i < end; i++) {
            binarios[i] = binarios[i - 1] ^ binarios[i];
        }
    }
}

int binarioParaDecimal(int *binarios, int inicio, int fim){

    int i,n=1; int valorNumerico=0;

    for(i=fim-1; i>=inicio; i--, n=n<<1){
        valorNumerico += n*(binarios[i]);
    }

    return valorNumerico;
}

double getRealTime()
{
    struct timeval tv;
    gettimeofday(&tv,0);
    return (double)tv.tv_sec + 1.0e-6*(double)tv.tv_usec;
}

char * leArquivo(char * nomeArquivo){

	FILE *f = fopen(nomeArquivo, "rb");

	if(f == NULL){
	    printf("Arquivo de kernel inválido.\n");
	    exit(1);
	}

	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

	char *string = (char*)malloc(fsize + 1);
	fread(string, fsize, 1, f);
	fclose(f);

	string[fsize] = 0;

	return string;
}

/*
  Obtém o tempo decorrido entre o início e o fim de um evento (em picosegundos)
*/
float getTempoDecorrido(cl_event event){

    cl_ulong time_start, time_end;

    //clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
    //clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);

    return (float)(time_end - time_start);
}

