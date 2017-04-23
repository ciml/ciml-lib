#include "parser.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include "utils.h"

t_elemento naoTerminais[40];

t_elemento operadores_binarios[] = {
								{T_SOMA,  "+"},
								{T_SUB,  "-"},
								{T_MUL,  "*"},
								{T_DIV,  "/"},
								{T_POT,  "pot"},
								{T_MENOR,  "!>"},
								{T_MENORIGUAL,  "!>="},
								{T_IGUAL,  "="},
								{T_MAIOR,  ">"},
								{T_MAIORIGUAL,  ">="},
								{T_DIFERENTE,  "!="},
								{T_ELOGICO,  "AND"},
								{T_OULOGICO,  "OR"}

							 };

t_elemento operadores_unarios[] = {
								{T_SEN, "sen"},
								{T_COS, "cos"},
								{T_SQRT, "sqrt"},
								{T_MENOS, "menos"},
								{T_LOG, "log"}
							 };

t_elemento operadores_ternarios[] = {
                                     { T_CONDICIONAL, "if-else" }
                                    };

t_elemento operadores_especiais[] = {
                                     { T_DELIMITADOR_FLOAT, "float"},
                                     { T_PONTO, "."}
                                    };

t_elemento variaveis[30];

//Ultimo não terminal inserido
int idNaoTerminal = -1;
int idVariavel    = -1;


void InicializaNaoTerminais(){
    int i;
    for(i=0; i < sizeof(naoTerminais)/sizeof(t_elemento);i++){
        strcpy(naoTerminais[i].str, "");
    }
}

int GetOperadorBinario(char s[]){

	int i, tam = sizeof(operadores_binarios)/sizeof(t_elemento);

	for(i=0;i<tam;i++){
		if(!strcmp(s, operadores_binarios[i].str)) return operadores_binarios[i].id;
	}

	return -1;
}

int GetOperadorUnario(char s[]){

	int i, tam = sizeof(operadores_unarios)/sizeof(t_elemento);
	for(i=0;i<tam;i++){
		if(!strcmp(s, operadores_unarios[i].str)) return operadores_unarios[i].id;
	}

	return -1;
}

int GetOperadorTernario(char s[]){

	int i, tam = sizeof(operadores_ternarios)/sizeof(t_elemento);
	for(i=0;i<tam;i++){
		if(!strcmp(s, operadores_ternarios[i].str)) return operadores_ternarios[i].id;
	}

	return -1;
}

int GetOperadorEspecial(char s[]){

	int i, tam = sizeof(operadores_especiais)/sizeof(t_elemento);
	for(i=0;i<tam;i++){
		if(!strcmp(s, operadores_especiais[i].str)) return operadores_especiais[i].id;
	}

	return -1;
}


int GetVariavel(char s[]){

	int i, tam = sizeof(variaveis)/sizeof(t_elemento);

	for(i=0;i<tam;i++){
		if(!strcmp(s, variaveis[i].str)) return variaveis[i].id;
	}

	return -1;
}

int GetNaoTerminal(char s[]){

	int i;

	for(i=0; i<=idNaoTerminal; i++){
		if(!strcmp(s, naoTerminais[i].str)) return naoTerminais[i].id;
	}

	/* Não terminal não encontrado */
	idNaoTerminal++;

	naoTerminais[idNaoTerminal].id = idNaoTerminal;
	strcpy(naoTerminais[idNaoTerminal].str, s);

	return idNaoTerminal;
}

short GetQtdVariaveis(){
	return idVariavel+1;
}

void LeVariaveis(char s[]){

    char *token;
    char *rest = s;

    idVariavel =  0;

    while((token = strtok_r(rest, "\t", &rest) ))
    {
        if(token != '\0' && token != NULL){

            variaveis[idVariavel].id = idVariavel;
            strcpy(variaveis[idVariavel].str, token);

            //printf("Variavel %d: %s\n", variaveis[idVariavel].id, variaveis[idVariavel].str);

            idVariavel++;
        }
    }

    idVariavel--;
}

void DatabaseReadLine(char *s, Database *bancoDeDados, int indice){

	char * valorPtr, *saveptr;
	int i = 0;

	valorPtr = strtok_r(s, "\t", &saveptr);

	//while(countValor < GetQtdVariaveis()){

	for(i=0;i<GetQtdVariaveis(); i++){
        bancoDeDados->registros[indice*bancoDeDados->numVariaveis + i] = atof(valorPtr);
        //printf("%f \n", bancoDeDados->registros[indice*bancoDeDados->numVariaveis + i]);

        valorPtr = strtok_r(NULL, "\t", &saveptr);
	}
	//printf("\n");

    /*while(1){

        /*float value = atof(valorPtr);
		bancoDeDados->registros[indice*bancoDeDados->numVariaveis + i] = value;

		if(i < GetQtdVariaveis()-1){
            valorPtr = strtok_r(NULL, "\t", &saveptr);
            i++;
        }
        else
            break;
    }*/

        //printf("Lendo variável %d\n", countValor);

        /*float value = atof(valorPtr);

		bancoDeDados->registros[indice*bancoDeDados->numVariaveis + i] = value;
		valorPtr = strtok_r(NULL, "\t", &saveptr);
        i++;*/
	//}
}

Database * DatabaseRead(char * nomeArquivo){

    Database * bancoDeDados = (Database *) malloc(sizeof(Database));

    get_info_bancoDeDados(nomeArquivo, &bancoDeDados->numRegistros, &bancoDeDados->numVariaveis);

    long tamanhoBanco = sizeof(float)*(bancoDeDados->numRegistros)*(bancoDeDados->numVariaveis);

    //printf("Registros: %ld\n Variaveis:%d\nTamanho: %d\n",bancoDeDados->numRegistros, bancoDeDados->numVariaveis,tamanhoBanco);

    bancoDeDados->registros = (float*) malloc(tamanhoBanco);

    memset(bancoDeDados->registros, 0, tamanhoBanco);

    if(bancoDeDados == NULL){
        printf("Erro alocar memória.");
        exit(1);
    }

	FILE *arq = fopen(nomeArquivo, "r");

	if(arq == NULL){
        printf("Erro ao abrir arquivo '%s'\n", nomeArquivo);
        exit(1);
	}

	char linha[1024];
	fgets(linha,1024, arq);

	/* Processa primeira linha (possui os nomes das variáveis) */
    LeVariaveis(linha);

    int count=0;
    //printf("-----------------------------------------------------------\n\n");

	while(count < bancoDeDados->numRegistros && fgets(linha,1024, arq) != NULL){
        DatabaseReadLine(linha, bancoDeDados, count);
		count++;
	}

	fclose(arq);

	return bancoDeDados;
}

void GetNomeElemento(type_simbolo *s, char *nome){

    //printf("Buscando: (%f,%f)", s->v[0],s->v[1]);

	switch((int)s->v[0]){

		case NAOTERMINAL:
            strcpy(nome, naoTerminais[(int)s->v[1]].str);
			break;

		case OPERADOR_BINARIO:
			strcpy(nome, operadores_binarios[(int)s->v[1]].str);
			break;
        case OPERADOR_TERNARIO:
            strcpy(nome, operadores_ternarios[(int)s->v[1]].str);
			break;
		case OPERADOR_ESPECIAL:
			strcpy(nome, operadores_especiais[(int)s->v[1]].str);
			break;
        case OPERADOR_UNARIO:
            strcpy(nome, operadores_unarios[(int)s->v[1]].str);
			break;
		case VARIAVEL:
			strcpy(nome, variaveis[(int)s->v[1]].str);
			break;
		case NUMERO_INTEIRO:
			sprintf(nome, "%d", (int)s->v[1]);
			break;

		case NUMERO_COM_PONTO:
			sprintf(nome, "%.2f", s->v[1]);
            //strcat(nome, "f");
			break;
		default:
			break;
	}
}

char * GetSimboloNT(char * origem){

    if(strlen(origem) > 0){

        /*char *token = (char*)malloc(sizeof(char)*(strlen(origem)+1));

        strcpy(token, "");

        int i,j=0;

        for(i=0;i<strlen(origem);i++) {
            if(!isspace(origem[i])){
                token[j++] = origem[i];
            }
        }
        token[i] = '\0';

        strcpy(origem, token);

        free(token);*/

        origem = trim(origem);

        return origem;
	}

	return origem;
}

type_simbolo GetSimboloParser(char * s){

	int j=0;
	type_simbolo simbolo;

    if(GetVariavel(s)!=-1){

		simbolo.v[0] = VARIAVEL;
		simbolo.v[1] = GetVariavel(s);
	}
	else if(s[0]=='<'){

		simbolo.v[0] = NAOTERMINAL;

		//Obtem id do não terminal
		simbolo.v[1] = GetNaoTerminal(s);
	}

	else if(isdigit(s[0])){

		while(isdigit(s[j])) j++;

		if(s[j]!= '.'){
			simbolo.v[0] = NUMERO_INTEIRO;
			simbolo.v[1] = atoi(s);
		}
		else{
			j++;

			while(isdigit(s[j])) j++;

			simbolo.v[0] = NUMERO_COM_PONTO;
			simbolo.v[1] = atof(s);
		}
	}
	else if(GetOperadorBinario(s)!=-1){

		simbolo.v[0] = OPERADOR_BINARIO;
		simbolo.v[1] = GetOperadorBinario(s);
	}

	else if(GetOperadorUnario(s)!=-1){

        //printf("Unario:\n");

		simbolo.v[0] = OPERADOR_UNARIO;
		simbolo.v[1] = GetOperadorUnario(s);
	}

	else if(GetOperadorTernario(s)!=-1){

        //printf("Ternario:\n");

		simbolo.v[0] = OPERADOR_TERNARIO;
		simbolo.v[1] = GetOperadorTernario(s);
	}

	else if(GetOperadorEspecial(s)!=-1){

        //printf("Especial: %s\n", s);

		simbolo.v[0] = OPERADOR_ESPECIAL;
		simbolo.v[1] = GetOperadorEspecial(s);
	}


	else{
		/* Elemento inválido */
		simbolo.v[0] = -1;
		simbolo.v[1] = 0;
	}
	return simbolo;
}

float OperaTernario(short v, float a, float b, float t){

    if(t == T_CONDICIONAL){
        if(v){
            return b;
        }
        else{
            return a;
        }
    }
}

float OperaBinario(float a, float b, float x){

	if(x == T_SOMA)
		return a+b;
	if(x == T_SUB)
		return a-b;
	if(x == T_MUL)
		return a*b;
    if(x == T_POT)
		return pow(a,b);
	if(x == T_DIV){
		if(b!=0)
            return a/b;
		else return 1;
	}

	if(x == T_MAIOR){
        return (a > b);
	}
	if(x == T_MAIORIGUAL){
        return (a >= b);
	}
	if(x == T_IGUAL){
        return (a == b);
	}
	if(x == T_MENOR){
        return (a < b);
	}
	if(x == T_MENORIGUAL){
        return (a <= b);
	}
	if(x == T_DIFERENTE){
        return (a != b);
	}
	if(x == T_ELOGICO){
        return a && b;
	}
    if(x == T_OULOGICO){
        return a || b;
	}

	return 0;
}

float OperaUnario(float a, float x){

	if(x == T_SEN)
		return (float)sin(a);
	if(x == T_COS)
		return (float)cos(a);
	if(x == T_SQRT)
		return (float)sqrt(fabs(a));
    if(x == T_LOG)
        return (float)log(fabs(a));
	if(x == T_MENOS){
		return a*(-1);
	}
	return 0;
}

/*
    Does the program preprecessing:

    1) Finds and concatenate float values:
*/

void PreprocessProgram(t_item_programa *program){

    int index=0, index_temp = 0;
    float value;

    while(index != FIM_PROGRAMA){

        //if float number delimiter found
        if(program[index].t.v[0] == OPERADOR_ESPECIAL && program[index].t.v[1] == T_DELIMITADOR_FLOAT){

            value = 0;

            index_temp = program[index].proximo;

            int count = 0;
            int id = index_temp;
            int l;

            //Searches for the floating point
            while(program[id].t.v[0] != OPERADOR_ESPECIAL){
                count++;
                id = program[id].proximo;
            }

            for(l=1; l<=count; l++){
                value += program[index_temp].t.v[1]*pow(10, count-l);
                index_temp = program[index_temp].proximo;
            }

            if(program[index_temp].t.v[0] == OPERADOR_ESPECIAL && program[index_temp].t.v[1] == T_PONTO){
                //printf(" PONTO ");
            }
            else{
                printf(" DEVERIA TER UM PONTO AQUI\n");
                exit(1);
            }

            index_temp = program[index_temp].proximo;

            l=1;
            while((int)program[index_temp].t.v[0] == NUMERO_INTEIRO){

                value += program[index_temp].t.v[1]*pow(10, -l);
                index_temp = program[index_temp].proximo;
                l++;
            }

            program[index].t.v[0] = NUMERO_COM_PONTO;
            program[index].t.v[1] = value;

            program[index].proximo = program[index_temp].proximo;
        }

        index = program[index].proximo;
    }
}

#define LP 997.0f
#define MP 998.0f
#define BP 999.0f

float ConservativePredictorScore(float predicted, float expected){

    if(predicted == expected)
        return 0.0f;

    if(expected == LP){
        if(predicted == MP)
            return -2.0f;
        else
            return -4.0f;
    }
    else if(expected == MP){
        return -1.0f;
    }
    else{
        if(predicted == LP)
            return -2.0f;
        else
            return -1.0f;
    }
}

float LiberalsPredictorScore(float predicted, float expected){

    if(predicted == expected)
        return 0.0f;

    if(expected == LP){
        if(predicted == MP)
            return -1.0f;
        else
            return -2.0f;
    }
    else if(expected == MP){
        return -1.0f;
    }
    else{
        if(predicted == LP)
            return -4.0f;
        else
            return -2.0f;
    }
}

arvore * AlocaNo(float v[2]){

    arvore *arv = malloc(sizeof(arvore));

    arv->dir = arv->esq = arv->meio = NULL;
    arv->v[0] = v[0];
    arv->v[1] = v[1];

    return arv;
}

arvore* CriaArvore(t_item_programa *programa){

   arvore* pilha[TAMANHO_MAX_PROGRAMA];
   int topo   = -1;
   float erro = 0;

   arvore * noAux;

   int idx=0;

   while(idx != FIM_PROGRAMA){

       switch((int)programa[idx].t.v[0])
	   {
	   	   case NUMERO_INTEIRO:
	   		   //printf(" (num) ");
               noAux = AlocaNo(programa[idx].t.v);
	   		   pilha[++topo] = noAux;
	   		   break;
	   	   case NUMERO_COM_PONTO:
	   		   //printf(" (num) ");
	   		   noAux = AlocaNo(programa[idx].t.v);
	   		   pilha[++topo] = noAux;

			   break;
	   	   case VARIAVEL:
	   		   //printf(" (Variável) : %d %f\n", (int)programa[idx].t.v[1], DATABASE((int)programa[idx].t.v[1]));
	   		   noAux = AlocaNo(programa[idx].t.v);
	   		   pilha[++topo] = noAux;
	   		   break;
	   	   case OPERADOR_BINARIO:
	   		   //printf(" (op) ");
               noAux = AlocaNo(programa[idx].t.v);
               noAux->esq = pilha[topo];
               noAux->dir = pilha[topo-1];
	   		   topo--;
	   		   pilha[topo] = noAux;

	   		   break;
           case OPERADOR_TERNARIO:

	   		   noAux = AlocaNo(programa[idx].t.v);
	   		   noAux->meio = pilha[topo];
               noAux->esq = pilha[topo-1];
               noAux->dir = pilha[topo-2];
	   		   topo=topo-2;
	   		   pilha[topo] = noAux;

	   		   break;

	   	   case OPERADOR_UNARIO:
			   //printf(" (op unario) ");
			   //pilha[topo] = OperaUnario(pilha[topo], programa[idx].t.v[1]);

			   noAux = AlocaNo(programa[idx].t.v);
			   noAux->esq = pilha[topo];

			   pilha[topo] = noAux;

			   break;
	   }

	   idx = programa[idx].proximo;

	   //printf("Topo: %f\n", pilha[topo]);
   }

   return pilha[topo];
}

void DesalocaArvore(arvore *arv){

    if(arv != NULL)
    {
        DesalocaArvore(arv->esq);
        DesalocaArvore(arv->meio);
        DesalocaArvore(arv->dir);

        free(arv);
    }
}

int ObtemAlturaArvore(arvore* arv){

    if(arv==NULL)
        return 0;

    int alturaEsq, alturaMeio, alturaDir;

    alturaEsq  = ObtemAlturaArvore(arv->esq);
    alturaMeio = ObtemAlturaArvore(arv->meio);
    alturaDir  = ObtemAlturaArvore(arv->dir);

    if(alturaEsq >= alturaDir && alturaEsq >=alturaMeio)
        return 1 + alturaEsq;
    if(alturaDir >= alturaEsq && alturaDir >=alturaMeio)
        return 1 + alturaDir;
    return 1 + alturaMeio;
}

int ObtemNumeroDeNosArvore(arvore* arv)
{
    if(arv==NULL)
        return 0;

    int nosEsq, nosMeio, nosDir;

    nosEsq = ObtemNumeroDeNosArvore(arv->esq);
    nosMeio = ObtemNumeroDeNosArvore(arv->meio);
    nosDir  = ObtemNumeroDeNosArvore(arv->dir);

    return 1 + nosEsq + nosDir + nosMeio;
}

float AvaliaSemMatrizConfusao(t_item_programa programa[], float dataBase[], int linha,
int MODELO) {

   float pilha[TAMANHO_MAX_PROGRAMA];
   int topo   = -1;
   float erro = 0;

   short NUM_VARIAVEIS = GetQtdVariaveis();

   int idx=0;

   while(idx != FIM_PROGRAMA){

       switch((int)programa[idx].t.v[0])
	   {
	   	   case NUMERO_INTEIRO:
	   		   //printf(" (num) ");
	   		   pilha[++topo] = programa[idx].t.v[1];
	   		   break;
	   	   case NUMERO_COM_PONTO:
	   		   //printf(" (num) ");
	   		   pilha[++topo] = programa[idx].t.v[1];
			   break;
	   	   case VARIAVEL:
	   		   //printf(" (Variável) : %d %f\n", (int)programa[idx].t.v[1], DATABASE((int)programa[idx].t.v[1]));
	   		   pilha[++topo] = DATABASE((int)programa[idx].t.v[1]);
	   		   break;
	   	   case OPERADOR_BINARIO:
	   		   //printf(" (op) ");
	   		   pilha[topo-1] = OperaBinario(pilha[topo-1], pilha[topo], programa[idx].t.v[1]);
	   		   topo--;
	   		   break;
           case OPERADOR_TERNARIO:
	   		   pilha[topo-2] = OperaTernario(pilha[topo], pilha[topo-1], pilha[topo-2], programa[idx].t.v[1]);
	   		   topo = topo-2;
	   		   break;

	   	   case OPERADOR_UNARIO:
			   //printf(" (op unario) ");
			   pilha[topo] = OperaUnario(pilha[topo], programa[idx].t.v[1]);
			   break;
	   }

	   idx = programa[idx].proximo;

	   //printf("Topo: %f\n", pilha[topo]);
   }

   //printf("------------------------------------------------------------\n");
   //printf("%f = %f\n", pilha[topo], DATABASE(NUM_VARIAVEIS-1));
   //return pilha[topo] - DATABASE(NUM_VARIAVEIS-1);

    //return pilha[topo] != DATABASE(NUM_VARIAVEIS-1);
    float predicted = pilha[topo];
    float expected  = DATABASE(NUM_VARIAVEIS-1);

    if(MODELO == 0)
        return ConservativePredictorScore(predicted, expected);
    else if(MODELO == 1)
        return LiberalsPredictorScore(predicted, expected);
    else{
        //avaliação sem pesos
        return predicted != expected;
    }
}


float Avalia(t_item_programa programa[], float dataBase[], int linha, int MODELO, int **matrizDeConfusao) {

   float pilha[TAMANHO_MAX_PROGRAMA];
   int topo   = -1;
   float erro = 0;

   short NUM_VARIAVEIS = GetQtdVariaveis();

   int idx=0;

   while(idx != FIM_PROGRAMA){

       switch((int)programa[idx].t.v[0])
	   {
	   	   case NUMERO_INTEIRO:
	   		   //printf(" (num) ");
	   		   pilha[++topo] = programa[idx].t.v[1];
	   		   break;
	   	   case NUMERO_COM_PONTO:
	   		   //printf(" (num) ");
	   		   pilha[++topo] = programa[idx].t.v[1];
			   break;
	   	   case VARIAVEL:
	   		   //printf(" (Variável) : %d %f\n", (int)programa[idx].t.v[1], DATABASE((int)programa[idx].t.v[1]));
	   		   pilha[++topo] = DATABASE((int)programa[idx].t.v[1]);
	   		   break;
	   	   case OPERADOR_BINARIO:
	   		   //printf(" (op) ");
	   		   pilha[topo-1] = OperaBinario(pilha[topo-1], pilha[topo], programa[idx].t.v[1]);
	   		   topo--;
	   		   break;
           case OPERADOR_TERNARIO:
	   		   pilha[topo-2] = OperaTernario(pilha[topo], pilha[topo-1], pilha[topo-2], programa[idx].t.v[1]);
	   		   topo = topo-2;
	   		   break;

	   	   case OPERADOR_UNARIO:
			   //printf(" (op unario) ");
			   pilha[topo] = OperaUnario(pilha[topo], programa[idx].t.v[1]);
			   break;
	   }

	   idx = programa[idx].proximo;

	   //printf("Topo: %f\n", pilha[topo]);
   }

   //printf("------------------------------------------------------------\n");
   //printf("%f = %f\n", pilha[topo], DATABASE(NUM_VARIAVEIS-1));
   //return pilha[topo] - DATABASE(NUM_VARIAVEIS-1);

    //return pilha[topo] != DATABASE(NUM_VARIAVEIS-1);
    float predicted = pilha[topo];
    float expected  = DATABASE(NUM_VARIAVEIS-1);

    //printf("%d %f %f \n", linha, predicted-997, expected-997);

    //Atualiza matriz de confusao
    //matrizDeConfusao[(int)expected-997][(int)predicted-997]+=1;

	return fabs(expected - predicted);

    /*if(MODELO == 0)
        return ConservativePredictorScore(predicted, expected);
    else if(MODELO == 1)
        return LiberalsPredictorScore(predicted, expected);
    else{
        //avaliação sem pesos
        return predicted != expected;
    }
    */
}

No * EmpilhaExpressao(No * pilha, char * expressao){

    No * novo = (No *) malloc(sizeof(No));

    if(novo == NULL){
        printf("Erro ao alocar memória.\n");
        exit(1);
    }

    novo->proximo = pilha;
    strcpy(novo->expr, expressao);

    //printf("Empilhei: %s\n", expressao);

    return novo;
}

void ImprimePosfixa(t_item_programa * programa){

	char nomeElemento[50];

	int idx = 0;

	printf("\"");

	while(idx != FIM_PROGRAMA){
		GetNomeElemento(&programa[idx].t, nomeElemento);
		printf("(%s) ", nomeElemento);
		idx = programa[idx].proximo;
	}

	printf("\"");
	//printf("\n");
}

void ImprimeInfixa(t_item_programa *programa){

   char text[TAMANHO_MAX_PROGRAMA];
   GetProgramaInfixo(programa, text);
   printf("%s",text);
}

void GetProgramaInfixo(t_item_programa *programa, char * textoPrograma){

   No * p = NULL;

   int i=0;

   char aux1[TAMANHO_MAX_PROGRAMA*1000];
   char aux2[TAMANHO_MAX_PROGRAMA*1000];
   int index;

   while(i != FIM_PROGRAMA){

	   switch((int)programa[i].t.v[0])
	   {
	   	   case NUMERO_INTEIRO:

	   		   GetNomeElemento(&programa[i].t, aux2);
	   		   p = EmpilhaExpressao(p, aux2);

	   		   //printf("Numero inteiro:%s\n", aux2);

	   		   break;

	   	   case NUMERO_COM_PONTO:

               GetNomeElemento(&programa[i].t, aux2);
	   		   p = EmpilhaExpressao(p, aux2);

	   		   //printf("Numero real:%s\n", aux2);

			   break;
	   	   case VARIAVEL:

               GetNomeElemento(&programa[i].t, aux2);
	   		   p = EmpilhaExpressao(p, aux2);

	   		   //printf("Variavel:%s\n", aux2);

	   		   break;

	   	   case OPERADOR_BINARIO:

               //strcpy(aux1, "((float)( ");
               strcpy(aux1, "( ");
	   		   strcat(aux1,  p->proximo->expr);

               GetNomeElemento(&programa[i].t, aux2);

               strcat(aux1,  " ");
               strcat(aux1, aux2);

               strcat(aux1,  " ");
               strcat(aux1,  p->expr);
               strcat(aux1,  " ");

               /*if(programa[i].t.v[1] == T_MUL || programa[i].t.v[1] == T_DIV){
                   strcat(aux1,  "+(0.0)");
               }*/

               strcat(aux1,  ")");

               No * aux;

               aux = p;
               p = p->proximo;
               free(aux);

               strcpy(p->expr, aux1);

               //printf("Operador binário:%s\n", aux1);

	   		   break;

	   	   case OPERADOR_UNARIO:

			   strcpy(aux1, "");

			   GetNomeElemento(&programa[i].t, aux1);

               //strcat(aux1, "((float)(");
               strcat(aux1, "(");
               strcat(aux1,  p->expr);
               strcat(aux1, ")");

               strcpy(p->expr, aux1);

               //printf("Operador unário:%s\n", aux1);

			   break;

            case OPERADOR_TERNARIO:

               strcpy(aux1, " \n \t if( ");
	   		   strcat(aux1,  p->expr);

	   		   strcat(aux1, " )");
               strcat(aux1,  "  ");

               strcat(aux1,  p->proximo->expr);
               strcat(aux1,  " \n else ");

               strcat(aux1,  p->proximo->proximo->expr);

               strcat(aux1,  "; \n");

               aux = p;
               p = p->proximo;
               free(aux);

               strcpy(p->expr, aux1);
               printf("Operador ternario:%s\n", aux1);



	   		   break;

           case OPERADOR_ESPECIAL:


                index = programa[i].proximo;
                while(1){

                    if((int)programa[index].t.v[0] == OPERADOR_ESPECIAL && (int)programa[index].t.v[1] == T_DELIMITADOR_FLOAT)
                    {
                        index = programa[index].proximo;
                        break;
                    }

                    if((int)programa[index].t.v[0] == OPERADOR_ESPECIAL && (int)programa[index].t.v[1] == T_PONTO){
                        printf(". ");
                    }
                    else{
                        printf("(%d %d) ", (int)programa[index].t.v[0], (int)programa[index].t.v[1]);
                    }

                    index = programa[index].proximo;
                }

                printf("\n-------------------------------------------------------------\n");

                i = index;

                break;
	   }

	   i = programa[i].proximo;
   }

   strcpy(textoPrograma, p->expr);
   free(p);
}
