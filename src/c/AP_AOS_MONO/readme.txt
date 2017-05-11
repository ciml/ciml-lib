Esse código emprega a técnica de ADAPTIVE PURSUIT para resolver o JSP mono-objetivo(makespan)

A adaptação ocorre nos parâmetros:
-Taxa de crossover
-Taxa de mutação
-Operador de crossover empregado
-Operador de mutação empregado

As técnicas implementadas de crossover são:
-PMX crossover
-LOX crossover
-op crossover

As técnicas de mutação impregadas são:
-Shift mutaxion
-Interchange mutation

####################################################
Compilar utilizando o Makefile


####################################################
Para executar o arquivo na máquina pessoal

executavel input_do_problema iteracao tam_da_populacao(opcional)

###################################################
Para executar no cluster

jobC.sh input_do_problema
