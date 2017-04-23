//System includes
#include <stdio.h>
#include <time.h>

#include <getopt.h>

#include "ag.h"
#include "eg_opencl.h"

#include "parser.h"
#include "gramatica.h"
#include "utils.h"

void print_usage(){
    puts("-------------------------------------------------------------------------------");
    puts("Usage: --database='file' [--grammar='file'] \n");
    puts("-------------------------------------------------------------------------------");
}


int main(int argc, char * argv[])
{
    /* Variáveis */
    //t_regra gramatica[15];
    t_regra * gramatica   = (t_regra*) malloc(sizeof(t_regra)*50);
    //individuo * populacao = (individuo*) malloc(sizeof(individuo)*TAMANHO_POPULACAO);
    //individuo populacao[TAMANHO_POPULACAO];
    char training_dataset_file[50] ;// = (char*)malloc(50*sizeof(char));
    char test_dataset_file[50] ;// = (char*)malloc(50*sizeof(char));
    char grammar_file[50] ;//= (char*)malloc(50*sizeof(char));
    char pop_file[50];// = (char*)malloc(50*sizeof(char));
    int pop_count = 0;

    //strcpy(training_dataset_file, "./data/amml_train_ge.csv");
    strcpy(training_dataset_file, "../../Problemas2/P4/10^2.txt");
    //strcpy(test_dataset_file,     "./data/amml_test.csv");
    strcpy(test_dataset_file,     "");
    //strcpy(grammar_file, "./grammars/g_amml_ge.txt");
    strcpy(grammar_file, "./grammars/g1.txt");    
    strcpy(pop_file, "");

    int seed   = time(NULL);
    int pcores = 0;
    char c;
    while(1)
    {
       static struct option long_options[] =
        {
           {"training",  required_argument, 0, 'd'},
           {"test",  required_argument, 0, 't'},
           {"pop",  required_argument, 0, 'p'},
           {"grammar",   required_argument, 0, 'g'},
           {"seed",  required_argument, 0, 's'},
           {"popcount",  required_argument, 0, 'r'},
           {0,  0,  0,  0}
        };

        int option_index = 0;

        c = getopt_long(argc, argv, "dc", long_options, &option_index);

        if (c == -1){
            break;
        }

        switch (c)
        {
            case 'd':
                //printf("Banco de dados: %s\n", optarg);
                strcpy(training_dataset_file, optarg);
                break;

            case 't':
                //printf("Banco de dados: %s\n", optarg);
                strcpy(test_dataset_file, optarg);
                break;

             case 'p':
                //printf("Banco de dados: %s\n", optarg);
                strcpy(pop_file, optarg);
                break;

            case 'g':
                strcpy(grammar_file, optarg);
                break;

            case 's':
                seed = atoi(optarg);
                break;

             case 'r':
                pop_count = atoi(optarg);
                break;

            case '?':
                puts(optarg);
                print_usage();
                exit(0);
                break;

            default:
               puts(optarg);
               print_usage();
               exit(0);
               break;
        }
    }

    Database * training_dataset = DatabaseRead(training_dataset_file);
    Database * test_dataset     = NULL;
        
    if(strcmp(test_dataset_file, "") != 0){
    	test_dataset = DatabaseRead(test_dataset_file);
	}		

    /*puts("-----------------------------------------------------------");
    printf("Tamanho do banco de dados:%d \t Número de variáveis:%d\n", training_dataset->numRegistros, training_dataset->numVariaveis);
    puts("-----------------------------------------------------------");*/

    LeGramatica(grammar_file, gramatica);
    
    int i, execucoes = 1;
    double media = 0.0f;

	printf("%d\t%d\t", TAMANHO_POPULACAO, training_dataset->numRegistros);
	
	srand(seed);

    double inicio = getTime();        

   	eg_paralela(gramatica, training_dataset, test_dataset, pop_file, pop_count, 0, 0);
   	
    double tempo = getTime()-inicio;

    //printf("%d\t%d\t%lf\n", TAMANHO_POPULACAO, training_dataset->numRegistros, media);
    
    printf("%f\n", tempo);

    free(training_dataset->registros);
    free(training_dataset);
	if(test_dataset != NULL)
    {    
    	free(test_dataset->registros);
    	free(test_dataset);
    }
    
    //free(populacao);
    //free(training_dataset_file);
    //free(test_dataset_file);
    //free(grammar_file);
    //free(pop_file);
    free(gramatica);
    
    return 0;
}
