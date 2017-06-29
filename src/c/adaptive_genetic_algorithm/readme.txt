O código possui diferentes versões de Algoritmo Genético, verificar o Makefile antes de compilar.

O padrão da representação é Job Based Representation
   -OBR para Operation Based Representation

Definir:
   -GA para algoritmo genético padrão
   -LS somente busca local
   -IG para ativar AG híbrido
   -AP para algoritmo genético adaptativo utilizando Adaptive Pursuit
   -PPC para algoritmo genético adaptativo utilizando Predictive Parameter Control (discreto)
   -PPCR para algoritmo genético adaptativo utilizando Predictive Parameter Control (contínuo)
   -NEH para inicializar parte da população com a heurística construtiva NEH(Nawaz, Enscore, Ham)

Algoritmo Genético:
   -Crossover: PMX crossover
   -Mutação: Shift mutation

As técnicas Adaptativas atuam nos seguintes parâmetros:
   -Taxa de crossover
   -Taxa de mutação
   -Operador de crossover empregado
   -Operador de mutação empregado

As técnicas implementadas de crossover são:
   -PMX crossover
   -LOX crossover
   -op crossover

As técnicas de mutação empregadas são:
   -Shift mutation
   -Interchange mutation

A técnica de busca local empregada é:
   -integrated Greedy, disponível para problemas de Flexible Flow Shop Scheduling Problems

Para executar o AG "default" utilize:
   ./nome_do_executável nome_do_arquivo_teste numero_da_repeticao
Para selecionar os parâmetros do AG utilize:
   ./nome_do_executável nome_do_arquivo_teste numero_da_repeticao tamanho_da_população taxa_de_crossover taxa_de_mutação taxa_local_search

Para executar o AG adaptativo "default" utilize:
   ./nome_do_executável nome_do_arquivo_teste numero_da_repeticao
Para selecionar os parâmetros do AG adaptativo utilize:
   ./nome_do_executável nome_do_arquivo_teste numero_da_repeticao tamanho_da_população

################## VERSÕES ##################
Versão 3.3 - AGA_3.3 - Adição de método de busca local e correção no método PPCR

Para todos os algoritmos foi inserida a possibilidade de aplicar busca local
Para isso deve-se definir "IG" para um algoritmo híbrido (ver Makefile) ou "LS" para somente busca local.

Nos métodos adaptativos a busca local também tem sua probabilidade de ocorrência ajustada no decorrer da busca.
Os métodos de recompensa estão descritos nesse log na Versão 3.2.1 e 3.2


Versão 3.2.1 - AGA_3.2.1 - Adição de novo método de recompensa

Para os métodos ADAPTIVE PURSUIT e os Preditivos - Definir nos arquivos adaptivepursuit.c ou predictiveparametercontrol.c
-CR_V3
  A recompensa do crossover é calculada por:
  recompensa = fitness_melhor_pai/fitness_melhor_filho

  A recompensa da mutação é calculada por:
  recompensa = fitness_melhor_pai/fitness_alcançado


Versão 3.2 - AGA_3.2 - Melhorias nos Métodos de Recompensa, correção de erros nos métodos adaptativos e melhorias menores

Para os métodos ADAPTIVE PURSUIT e os Preditivos - Definir nos arquivos adaptivepursuit.c ou predictiveparametercontrol.c
-CR_V0
   A recompensa do crossover é calculada por:
   recompensa = (fitness_pai1 + fitness_pai2)/(fitness_filho1 + fitness_filho2)

   A recompensa da mutação é calculada por:
   recompensa = (fitness_pai1 + fitness_pai2)/(fitness_offspring * 2)

-CR_V1 (default para o AP)
   A recompensa do crossover é calculada por:
   recompensa = fitness_melhor_pai/fitness_melhor_filho

   A recompensa da mutação é calculada por:
   recompensa = fitness_médio_da_população/fitness_alcançado

-CR_V2
   A recompensa do crossover é calculada por:
   recompensa = fitness_médio_da_população/fitness_melhor_filho

   A recompensa da mutação é calculada por:
   recompensa = fitness_médio_da_população/fitness_alcançado
