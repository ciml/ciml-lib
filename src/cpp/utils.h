/*
 * utils.h
 *
 *  Created on: 27/01/2015
 *      Author: igorsr
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <CL/cl.h>

#define MIN(a,b) ((a<=b ? a : b))

#define BIT_SET(a,b) ((a) |= (1<<(b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1<<(b)))
#define BIT_FLIP(a,b) ((a) ^= (1<<(b)))
#define BIT_CHECK(a,b) ((a) & (1<<(b)))

/*---------------------------------------------------------------------
 * Function:  grayParaBinario
 * Purpose:   Converte array em código de gray para binário
 * In args:   gray: array contendo uma string binária em código de gray
 *            binario: saída da função, array contendo string binária
 * Out arg:   --
 */
void grayParaBinario(int *gray, int *binario);

/*---------------------------------------------------------------------
 * Function:  binarioParaDecimal
 * Purpose:   Converte array de binários em decimal
 * In args:   binario: array contendo string binária
              inicio: inteiro indicando a posição inicial da string que contém
              o valor que se deseja converter
              fim: inteiro indicando a posição final da string que contém
              o valor que se deseja converter
 * Out arg:   Valor decimal inteiro correspodente ao trecho do array binário 
              entre [inicio, fim]
 */
int binarioParaDecimal(int *binario, int inicio, int fim);


/*---------------------------------------------------------------------
 * Function:  getRealTime
 * Purpose:   Obtém o tempo atual da máquina em segundos, 
              utilizando uma chamada ao sistema operacional disponível em 
              <sys/time.h>. Atualmente compatível somente com UNIX (testado 
              somente em Linux).
              Esta função pode ser utilizada como um "relógio de parede", 
              que fornece o tempo passado entre dois eventos, independente do                
              tempo de CPU ou do número de ciclos de clock.
                            
 * Out arg:   Tempo atual em segundos 
 */
double getRealTime();

/*---------------------------------------------------------------------
 * Function:  leArquivo
 * Purpose:   Lê e retorna todo o conteúdo de um arquivo em formato texto.
 * In args:   nomeArquivo: string contendo o caminho e o nome do arquivo 
              para leitura
 * Out arg:   String com todo o conteúdo do arquivo.
 */
char * leArquivo(char * nomeArquivo);


float getTempoDecorrido(cl_event event);




#endif /* UTILS_H_ */
