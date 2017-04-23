#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include <stdio.h>

#define FIM_PROGRAMA -1
#define TAMANHO_MAX_PROGRAMA 4098

//todo: testar regressão
#define DATABASE(y) (dataBase[linha*NUM_VARIAVEIS + y ])

typedef enum { DEFAULT, NAOTERMINAL, OPERADOR_BINARIO, OPERADOR_UNARIO,
NUMERO_INTEIRO, NUMERO_COM_PONTO, VARIAVEL, OPERADOR_TERNARIO, OPERADOR_ESPECIAL} TipoSimbolo;

typedef enum { T_SOMA, T_SUB, T_MUL, T_DIV, T_POT,
T_MENOR,T_MENORIGUAL,T_IGUAL, T_MAIOR,T_MAIORIGUAL,T_DIFERENTE, T_ELOGICO, T_OULOGICO} OperadoresBinarios;
typedef enum { T_SEN, T_COS, T_SQRT, T_MENOS, T_LOG} OperadoresUnarios;
typedef enum { T_CONDICIONAL } OperadoresTernarios;
typedef enum { T_DELIMITADOR_FLOAT, T_PONTO } OperadorEspecial;

/* Definição das estruturas de dados */

typedef struct {
	float v[2];
} type_simbolo;

typedef struct {
	int id;
	char str[50];
} t_elemento;

typedef struct{
	type_simbolo t;
	int proximo;
} t_item_programa;


typedef struct{
	int num_simbolos;
	type_simbolo simbolos[20];
} t_escolha;

typedef struct{
	type_simbolo simbolo;
	int num_escolhas;
	t_escolha escolhas[30];
} t_regra;

typedef struct{
	t_item_programa programa[TAMANHO_MAX_PROGRAMA];
}t_prog;

typedef struct{
    int numRegistros;
    int numVariaveis;
    float * registros;
}Database;

typedef struct NoExpressao{
    char expr[TAMANHO_MAX_PROGRAMA];
    struct NoExpressao * proximo;
} No;

typedef struct arvore{
    float v[2];
    struct arvore *esq, *dir, *meio;
}arvore;

/* Protótipo das funções */

No * EmpilhaExpressao(No * pilha, char * expressao);

char * GetSimboloNT(char * origem);

type_simbolo GetSimboloParser(char * s);

int GetNaoTerminal(char s[]);

int GetOperadorBinario(char s[]);

int GetOperadorUnario(char s[]);

int GetVariavel(char s[]);

void LeVariaveis(char s[]);

void InicializaNaoTerminais();

short GetQtdVariaveis();

void GetNomeElemento(type_simbolo *s, char *nome);

void GetNomeElemento2(int type, int value, char *nome);

Database * DatabaseRead(char nomeArquivo[]);

float AvaliaSemMatrizConfusao(t_item_programa programa[], float registro[], int linha, int MODELO);

float Avalia(t_item_programa programa[], float registro[], int linha, int MODELO, int **matrizDeConfusao);

void ImprimePosfixa(t_item_programa * programa);

void ImprimeInfixa(t_item_programa * programa);

void GetProgramaInfixo(t_item_programa *programa, char * textoPrograma);

void PreprocessProgram(t_item_programa *program);

int ObtemAlturaArvore(arvore *arv);
int ObtemNumeroDeNosArvore(arvore *arv);

arvore* CriaArvore(t_item_programa *programa);
void DesalocaArvore(arvore *arv);

#endif // PARSER_H_INCLUDED
