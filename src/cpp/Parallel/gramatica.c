#include "gramatica.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "utils.h"

int id_regra;

/*
 * Procura na gramática a regra relacionada ao não terminal indicado
 */
int GetRegra(t_regra * gramatica, type_simbolo simbolo){

	int i;

    for(i=0;i<=id_regra;i++){
    	if(gramatica[i].simbolo.v[1] == simbolo.v[1]) return i;
	}

	return -1;
}

size_t trimwhitespace(char *out, size_t len, char *str)
{
  if(len == 0)
    return 0;

  const char *end;
  size_t out_size;

  // Trim leading space
  while(isspace(*str)) str++;

  if(*str == 0)  // All spaces?
  {
    *out = 0;
    return 1;
  }

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace(*end)) end--;
  end++;

  // Set output size to minimum of trimmed string length and buffer size minus 1
  out_size = (end - str) < len-1 ? (end - str) : len-1;

  // Copy trimmed string and add null terminator
  memcpy(out, str, out_size);
  out[out_size] = 0;

  return out_size;
}


/*
 * Processa uma derivação de um não terminal.
 * Procura por símbolos (não terminais ou terminais) e adiciona à regra um type_simbolo para cada um.
 */
void ProcessaEscolha(char * escolha, t_escolha * nova_escolha ){

    #undef DEBUG

	nova_escolha->num_simbolos=0;

	char * elemento = "", elemento2[300] = "", *saveptr = "";

	elemento = strtok_r(escolha, DELIMITADOR_TERMINAIS, &saveptr);

	/* Encontra cada elemento da derivação*/
	while(elemento != NULL) {

		trimwhitespace(elemento2, strlen(elemento)+1, elemento);

		//printf("Elemento: %s\n", elemento);

		if(strlen(elemento2) != 0) {

			type_simbolo novo_simbolo = GetSimboloParser(elemento2);

			if(novo_simbolo.v[0]==-1){
				 printf("Gramatica inválida. (Simbolo terminal %s desconhecido).", elemento2);
				 exit (EXIT_FAILURE);
			}
			else{

				char nomeElemento[50];
				GetNomeElemento(&novo_simbolo, nomeElemento);

                #ifdef DEBUG
				    printf("Simbolo encontrado: %s (%d,%f) \n", nomeElemento, (int)novo_simbolo.v[0], novo_simbolo.v[1]);
                #endif

			}

			nova_escolha->simbolos[nova_escolha->num_simbolos].v[0] = novo_simbolo.v[0];
			nova_escolha->simbolos[nova_escolha->num_simbolos].v[1] = novo_simbolo.v[1];
			nova_escolha->num_simbolos++;
		}

		elemento = strtok_r(NULL, DELIMITADOR_TERMINAIS, &saveptr);
	}
}


void LeGramatica(char *nomeArquivo, t_regra  * Gramatica){

    InicializaNaoTerminais();

    #undef DEBUG

	/* Estrutura para ler o arquivo da gramática  */
	FILE *fptr = NULL;

	char linha[500] = "", aux[500] = "";

	//Abre o arquivo contendo a gramática para leitura
	fptr = fopen(nomeArquivo, "r");

	#ifdef DEBUG
		printf("Gramática: \n");
	#endif

	id_regra = -1;
	int quantidade_escolhas = 0;
	int count = 0;

	while(fgets(linha, 500, fptr) != NULL){

		#ifdef DEBUG
			printf("Linha - %s\n", linha);
		#endif

		//Verifica se a linha é o início de uma regra
		char * busca_regra = strstr(linha, DELIMITADOR_REGRAS);

		if(busca_regra != NULL){

			//id_regra++;

			quantidade_escolhas = 0;

			strcpy(aux, linha);

			char * pnt, * pntEscolhas;

			pnt = strtok(aux, DELIMITADOR_REGRAS);

			char * token = "";
			token = GetSimboloNT(pnt);

			#ifdef DEBUG
				printf("Regra: %s\n", token);
			#endif

			/* Insere o não terminal na tabela de símbolos da gramática */
			type_simbolo s = GetSimboloParser(token);

			id_regra = (int)s.v[1];
			count++;

            #ifdef DEBUG
                printf("Id regra: %d\n",id_regra);
			#endif

			Gramatica[id_regra].simbolo.v[0] = s.v[0];
			Gramatica[id_regra].simbolo.v[1] = s.v[1];

			pnt = strtok(NULL, DELIMITADOR_REGRAS);

			#ifdef DEBUG
				printf("Derivações: %s\n", pnt);
			#endif

			pntEscolhas = strtok(pnt, DELIMITADOR_ESCOLHAS);

			while(pntEscolhas != NULL)
			{
                ProcessaEscolha(pntEscolhas, &Gramatica[id_regra].escolhas[quantidade_escolhas]);

				if(Gramatica[id_regra].escolhas[quantidade_escolhas].num_simbolos>0){
					quantidade_escolhas++;
				}

				pntEscolhas = strtok( NULL, DELIMITADOR_ESCOLHAS);
			}

			Gramatica[id_regra].num_escolhas = quantidade_escolhas;

			//free(token);
		}
		else{

			/*Contém derivações da última regra lida */

			strcpy(aux, linha);

			#ifdef DEBUG
				printf("Derivações: %s\n", aux);
			#endif

			char *pntEscolhas = strtok(aux, DELIMITADOR_ESCOLHAS);

			while(pntEscolhas != NULL )
			{
				ProcessaEscolha(pntEscolhas, &Gramatica[id_regra].escolhas[quantidade_escolhas]);

				if(Gramatica[id_regra].escolhas[quantidade_escolhas].num_simbolos>0){
					quantidade_escolhas++;
				}

				pntEscolhas = strtok( NULL, DELIMITADOR_ESCOLHAS );
			}

			Gramatica[id_regra].num_escolhas = quantidade_escolhas;
		}
	}

	id_regra = count;

}

int Decodifica(t_regra * Gramatica, short * fenotipo, t_item_programa * programa){

    int m=0, fenotipo_ctr = 0;


	/* Inicializa o programa com o símbolo inicial */

	char elementoInicial[8] = "<cond>";

	type_simbolo inicial =  GetSimboloParser(elementoInicial);

	inicial.v[0] = NAOTERMINAL;
	inicial.v[1] = 0;

	programa[0].t.v[0]  = inicial.v[0];
	programa[0].t.v[1]  = inicial.v[1];
	programa[0].proximo = FIM_PROGRAMA;

	int program_ctr=1;

	//Mapeamento
	while(1){

		/* Verifica se todo o fenótipo foi utilizado */
		if(fenotipo_ctr == DIMENSOES_PROBLEMA-1){
		    program_ctr = -1;
		    break;
		}

		/* Procura primeiro não terminal à esquerda */
		int i=0;

		while((int)programa[i].t.v[0] != NAOTERMINAL && programa[i].proximo != -1) {

		    i = programa[i].proximo;
		}

		/* Verifica se há somente terminais */
		if((int)programa[i].t.v[0] != NAOTERMINAL) break;

		int idRegra = GetRegra(Gramatica, programa[i].t);

        int opcao = fenotipo[fenotipo_ctr++] % Gramatica[idRegra].num_escolhas;

		int proximoAnterior = programa[i].proximo;

		/* Substitui pelo primeiro simbolo */

		type_simbolo auxs = Gramatica[idRegra].escolhas[opcao].simbolos[0];
		programa[i].t.v[0] = auxs.v[0];
		programa[i].t.v[1] = auxs.v[1];

		if(Gramatica[idRegra].escolhas[opcao].num_simbolos>1){

		    programa[i].proximo = program_ctr;

		    for(m=1; m < Gramatica[idRegra].escolhas[opcao].num_simbolos; m++){

			    programa[program_ctr].t = Gramatica[idRegra].escolhas[opcao].simbolos[m];
			    programa[program_ctr].proximo = program_ctr+1;

			    program_ctr++;
		    }

		    programa[program_ctr-1].proximo = proximoAnterior;
		}
	}

	return program_ctr;
}

void ImprimeGramatica(t_regra * grammar){

    int i,j,k;
	for(i=0;i<id_regra;i++){
        printf("Regra %d", i);
        for(j=0; j< 20; j++){
            for(k=0; k< grammar[i].escolhas[j].num_simbolos; k++){
                printf(" (%d %d) ", (int)grammar[i].escolhas[j].simbolos[k].v[0],
                                    (int)grammar[i].escolhas[j].simbolos[k].v[1]);
            }
            printf(" | ");
        }
        printf("\n");
	}
}
