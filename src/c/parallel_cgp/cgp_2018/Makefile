all:
	gcc -Wall -O3 -g -std=c11 -c ./src/individuo.c -I ./include/ -o ./obj/individuo.o
	gcc -Wall -O3 -g -std=c11 -c ./src/operacaoTxt.c -I ./include/ -o ./obj/operacaoTxt.o
	gcc -Wall -O3 -g -std=c11 -c ./src/paraleloGPU.c -I ./include/ -o ./obj/paraleloGPU.o
	gcc -Wall -O3 -g -std=c11 main.c ./obj/*.o -I ./include -o ./bin/cgp -lOpenCL

run:
	./bin/cgp 1

clear:
	rm ./obj/*.o
	rm ./bin/*
