eg:
	g++ -O2 -o GE utils.h utils.c parser.h parser.c gramatica.h gramatica.c eg_opencl.h eg_opencl.c main.c -lOpenCL -I ./include -I /usr/include -I /usr/include -L /usr/local/cuda-6.0/lib64 -I /usr/local/cuda-6.0/include/ -lm -Wno-pointer-arith -Wno-write-strings -D TAMANHO_POPULACAO=512 -D NUMERO_DE_GERACOES=100 -D DIMENSOES_PROBLEMA=128 -D TAMANHO_VALOR=8 -D TAXA_DE_MUTACAO=0.05 -D TAXA_DE_RECOMBINACAO=0.9 -D TAMANHO_TORNEIO=3 -D ELITE=1 -D PROFILING -D cpu 
clean:
	rm -f GE
all:
	clean
	GE
