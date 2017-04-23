
#define FIM_PROGRAMA -1
#define TAMANHO_MAX_PROGRAMA 128

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


float Avalia(__local t_item_programa programa[], 
	
		#ifdef Y_DOES_NOT_FIT_IN_CONSTANT_BUFFER
	 		__global const
		#else
			__constant
		#endif
		float * dataBase,
		int linha) {

   float pilha[TAMANHO_MAX_PROGRAMA];
   int topo = -1;
   float erro = 0;
   
   int i=0;

   while(i != FIM_PROGRAMA){

	   switch((int)programa[i].t.v[0])
   	   {
	   	   case NUMERO_INTEIRO:
   		   	   pilha[++topo] = (int)programa[i].t.v[1];
	   		   break;
	   	   case NUMERO_COM_PONTO:
	   		   pilha[++topo] = programa[i].t.v[1];
			   break;
	   	   case VARIAVEL:
	   		   pilha[++topo] = DATABASE(linha, (int)programa[i].t.v[1]);
	   		   break;
	   	   case OPERADOR_BINARIO:
	   		   pilha[topo-1] = OperaBinario(pilha[topo-1], pilha[topo], programa[i].t.v[1]);
	   		   topo--;
	   		   break;
	   	   case OPERADOR_UNARIO:
			   pilha[topo] = OperaUnario(pilha[topo], programa[i].t.v[1]);
			   break;
		   case NAOTERMINAL:
    		    pilha[topo] = MAXFLOAT;
    		    i = FIM_PROGRAMA;
    		    break;
	   }

	   i = programa[i].proximo;
   }

   //Erro absoluto
   return fabs( pilha[topo] - DATABASE(linha, NUM_VARIAVEIS-1));
}


