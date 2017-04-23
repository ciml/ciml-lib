#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** arvgv){

	FILE *arq = fopen(arvgv[1], "r");
	
	float v1, v2, v3, v4, v5;
	float media1 = 0, media2=0, media3=0, media4=0, media5=0;
	int linhas = 0; 

	if(arq == NULL)
		printf("Erro, nao foi possivel abrir o arquivo\n");
	else
		while( (fscanf(arq,"%f\t%f\t%f\t%f\t%f\n", &v1, &v2, &v3, &v4, &v5))!=EOF ){
			
			media1+=v1;
			media2+=v2;
			media3+=v3;
			media4+=v4;
			media5+=v5;											
			
			linhas++;
		}
	
	fclose(arq);
	
	media1/=(float)linhas;
	media2/=(float)linhas;
	media3/=(float)linhas;
	media4/=(float)linhas;
	media5/=(float)linhas;	
	
	printf("%.10f\t%.10f\t%.10f\t%.10f\t%.10f", media1, media2, media3, media4, media5);
}
