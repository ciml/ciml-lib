eg:
	gcc -O2 -o GE utils.h utils.c parser.h parser.c gramatica.h gramatica.c operadores_geneticos.c ag.c eg.c main.c -I ./include -I /usr/include -lm -Wno-pointer-arith -Wno-write-strings -D TAMANHO_POPULACAO=500 -D NUMERO_DE_GERACOES=100 -D DIMENSOES_PROBLEMA=128 -D TAMANHO_VALOR=8 -D TAXA_DE_MUTACAO=0.05 -D TAXA_DE_RECOMBINACAO=0.9 -D TAMANHO_TORNEIO=3 -D ELITE=1 -D PROFILING
clean:
	rm -f AG
all:
	clean 
	eg
