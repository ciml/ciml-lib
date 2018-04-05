## Bibliotecas
import copy #Para utilizar deepcopy
from random import *
import gc
import random as r
import time

## Parâmetros Ajustáveis
nc = 500#1500 # Número de Colunas
nr = 1 # Número de Linhas
lb = 500#1500 # Levels-Back
ni = 6#4 # Número de Entradas
no = 6#18 # Número de Saídas
max_entradas = 2 # Número de máximos de conexões por porta lógica
ur = 1 # Taxa de mutação (em %)
ee_lambda = 4 #Lambda para estratégias de evolução
GLOBAL_SAM_MUTACOES = 1
GLOBAL_SAM_MUTACOES_CROSS = 1


## Parâmetros Globais
FT = [100, 110, 800, 130, 900] # Tabela de Funções
#FT = [100, 110, 800, 150, 160, 130, 900, 980, 990]
Ln = nc * nr # Número máximo de Nós
ug = (int((Ln * ur) / 100)) # Transforma porcentagem em quantidade de genes e soma um (maior inteiro seguinte ao obtido)
if ((Ln * ur) % 100) != 0:
    ug = ug + 1


## Arquivos

arquivo = open('matriz_transicao.txt','w')
arquivo.close()

arquivo2 = open('semente_populacao_inicial.txt', 'w')
arquivo2.close()

## Arrays
G = [] # Array que armazena o melhor indivíduo da geração anterior
aux_AI = [] # Array auxiliar de nós ativos/inativos
GENS_EVOL = [] # Array que armazena todos os genótipos de todos os indivíduos de cada geração (Pai + ee_lambda descendentes)


## Arrays auxiliares
fila = []
myarray = []
MEUARRAY = []
TABELA_V = [] # Array que armazena a tabela verdade dos circuitos
FITNESS_EE = [] # Array que armazena o valor das fitness dos circuitos
PORTAS_EE = [] # Array que armazena a quantidade de portas de cada circuito (acumulativo)
PORTAS_CKT = [] # Array que armazena a quantidade de portas total de cada e todos circuitos
WIRES_CKT = [] # Array que aramazena a quantidade de wires total de cada e todos circuitos
ATIVO_MOM = [] # Array que armazena os nós ativos momentâneos
MATRIZ_REGIAO_INFACTIVEL = [[[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0]]]
MATRIZ_REGIAO_FACTIVEL = [[[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0]],[[0],[0],[0],[0],[0],[0],[0],[0],[0]]]

## Arrays de mutação

#FACTIVEL
MUT_WIRE_5_FAC = [100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800]
MUT_AND_5_FAC = [900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 110, 130, 800, 800, 800]
MUT_OR_5_FAC = [900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 100, 100, 130, 130, 800, 800, 800]
MUT_XOR_5_FAC = [900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 100, 100, 100, 110, 110, 110, 800, 800, 800, 800, 800, 800, 800, 800, 800]
MUT_NOT_5_FAC = [900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130]


#INFACTIVEL
MUT_WIRE_5_INFAC = [100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 800, 800, 800]
MUT_AND_5_INFAC = [900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800]
MUT_OR_5_INFAC = [900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 900, 100, 100, 100, 100, 100, 100, 100, 100, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 800, 800, 800, 800, 800, 800, 800, 800]
MUT_XOR_5_INFAC = [900, 900, 900, 900, 900, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800]
MUT_NOT_5_INFAC = [900, 900, 900, 900, 900, 900, 900, 900, 900, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130]



### EXPERIMENTOS COMPUTACIONAIS

## Multiplicador 3x3 bits
#INPUTS = [[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1], [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1], [0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1], [0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1], [0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1], [0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1]]

#OUTPUTS = [[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1], [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1], [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0], [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0], [0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1]]


### COMPARADOR 4-1 BIT

#INPUTS = [[0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1],
#[0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1],
#[0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1],
#[0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1]]



#OUTPUTS = [[0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0],
#[0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0],
#[1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1],
#[0,1,0,1,0,0,0,0,0,0,0,1,0,0,0,0],
#[0,0,0,0,1,0,1,0,0,0,0,0,1,0,1,0],
#[1,0,1,0,0,1,0,1,1,1,1,0,0,1,0,1],
#[0,1,0,1,0,1,0,1,0,0,0,0,0,0,0,0],
#[0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0],
#[1,0,1,0,1,0,1,0,0,1,0,1,0,1,0,1],
#[0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0],
#[0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0],
#[1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1],
#[0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0],
#[0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0],
#[1,1,0,0,1,1,0,0,0,0,1,1,0,0,1,1],
#[0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0],
#[0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0],
#[1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1]]


## Somador 3 bits
INPUTS = [[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1], [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1], [0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1], [0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1], [0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1], [0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1]]

OUTPUTS = [[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1], [0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1], [0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1], [0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0]]



def limpa_array():
    GENS_EVOL.clear()
    FITNESS_EE.clear()
    PORTAS_EE.clear()
    MEUARRAY.clear()
    TABELA_V.clear()
    G.clear()
    PORTAS_CKT.clear()
    WIRES_CKT.clear()

def formato_ee(): #Cria o formato da matriz GENS_EVOL
    for i in range(ee_lambda + 1):
        GENS_EVOL.append([])

def formato_tabela_verdade(): #Cria o formato da matriz tabela verdade
    for i in range(Ln + ni + no):
        TABELA_V.append([])

def formato_array_fitness(): #Crua o formato da array de valores de fitness
    for j in range(ee_lambda + 1):
        FITNESS_EE.append([])
        PORTAS_EE.append([])
        PORTAS_CKT.append([])
        WIRES_CKT.append([])
        for k in range(no):
            FITNESS_EE[j].append([])
            PORTAS_EE[j].append([])
                        
def gera_formato_nos():
    for k in range(ee_lambda + 1):
        MEUARRAY.append([])
        for j in range(no):
            MEUARRAY[k].append([])

def gera_formato_genotipo(): #Define o formato da matriz genótipos G
    for i in range(Ln + ni):
        if i <= (ni -1):
            G.append([]) #Aloca espaços para as entradas
        else:
            G.append([]) #Aloca espaços para os nós
            for k in range(3):
                G[i].append([])
    for i in range(no):
        G.append([]) #Aloca espaço para as saidas

def populacao_inicial():
    for j in range(Ln):
        i = j + ni # Pula as posições das entradas
        n_porta = r.randint(0,len(FT)-1) # Sorteia uma porta dentre as disponíveis em FT
        porta = FT[n_porta]
        G[i][max_entradas] = porta # A porta ocupa o último locus do gene
        colunaatual = int(j/nr) + 1 # Determina a coluna atual (sem contar entrada)
        if colunaatual == 1: #Neste caso as portas lógicas só recebem inputs
            for k in range(max_entradas):
                entrada = r.randint(0, (ni-1))
                G[i][k] = entrada
        if colunaatual - lb < 1 and colunaatual != 1: #Neste caso as portas lógicas recebem inputs e nós        
            valorpossivel = (nr * (colunaatual-2)) + (nr-1) + ni #Determina todos os valores de entradas possiveis
            print(valorpossivel)
            for k in range(max_entradas):
                sorteado = r.randint(0, valorpossivel)
                G[i][k] = sorteado
        if colunaatual - lb >= 1: #Neste caso as portas logicas recebem somente nós
            primeiro_elemento = nr * (colunaatual-lb-1) + ni # O fator + ni serve para deslocar os elementos para frente, contando a quantidade de inputs.
            ultimo_elemento = (nr * (colunaatual-2)) + (nr-1) + ni
            for k in range(max_entradas):
                #entrada_ou_porta = randint(0, 1) #Determina se a entrada da porta lógica atual será uma entrada do circuito ou a saída de alguma porta lógica já existente (0: entrada; 1: porta lógica)
                elementos_portas = ultimo_elemento - primeiro_elemento + 1 #Determina quantas portas possíveis existem
                qtd_elementos = elementos_portas + ni - 1 #Determina a quantidade de valores distintos que a porta pode assumir como entrada. O -1 tem como função apenas possibilitar o uso de randint(0, qtd_elementos)
                entrada_ou_porta = r.randint(0, qtd_elementos) #Dá a mesma chance para todos os possiveis inputs
                if entrada_ou_porta >= ni:
                    G[i][k] = r.randint(primeiro_elemento, ultimo_elemento)
                else:
                    G[i][k] = r.randint(0, ni-1)

def define_saida():
    ultimo_elemento = ni + Ln - 1 #Último elemento possivel que a saída pode assumir
    primeiro_elemento = ultimo_elemento - (lb * nr) + 1 #Primeiro valor possivel que a saída pode assumir
    elementos_portas = ultimo_elemento - primeiro_elemento + 1 #Quantidade de valores existentes entre o primeiro e último possiveis valores
    qtd_elementos = elementos_portas + ni - 1 #Determina a quantidade de valores distintos que a porta pode assumir como entrada. O -1 tem como função apenas possibilitar o uso de randint(0, qtd_elementos)
    for i in range(no):
        entrada_ou_porta = r.randint(0, qtd_elementos) #Dá a mesma chance para todos os possiveis inputs
        if entrada_ou_porta >= ni: #G[Ln+ni+i] representa cada saida, sequencialmente
            G[Ln+ni+i] = r.randint(primeiro_elemento, ultimo_elemento)
        else:
            G[Ln+ni+i] = r.randint(0, ni-1)

def nos_ativos():
    fila = [] #Array que funcionará como uma fila
    myarray = [] #Array que armazenará os genes dos nós da fila
    vetor = []

    for k in range(Ln+ni):
        vetor.append("-")
    for w in range(ee_lambda + 1): #Percorre o pai e seus lambda filhos
        

        for i in range(no): #Realiza o procedimento para o número de saídas
            saida = int(Ln+ni+i) #Determina a saída a ser processada

            fila.append(GENS_EVOL[w][saida])
            noatual = GENS_EVOL[w][GENS_EVOL[w][saida]] #Obtém as informações do gene responsável pela saída
            if type(noatual) is not int: #len(noatual) == 3: #Analisa se a saída é o resultado de um nó ou é uma entrada do programa. (3 significa: entrada, entrada e porta)
                if (noatual[2] != 800 and noatual[2] != 900 and noatual[2] != 800 and noatual[2] != 980 and noatual[2] != 990): #Verifica se a porta logica é unária ou binária. Caso seja binária, entra no else.
                    fila.append(noatual[0]) #Adiciona primeira entrada do nó saída
                    fila.append(noatual[1]) #Adiciona segunda entrada do nó saída
                    myarray.append(GENS_EVOL[w][noatual[0]]) #Adiciona o gene responsável pela primeira entrada
                    myarray.append(GENS_EVOL[w][noatual[1]]) #Adiciona o gene responsável pela segunda entrada
                else:
                    fila.append(noatual[0])
                    myarray.append(GENS_EVOL[w][noatual[0]])
            else:
                fila.append(noatual) #Se tamanho é diferente de 3, então é uma entrada. Logo a fila recebe o valor da posição da entrada.

            while len(myarray) != 0: #Procedimento para esvaziar a fila
                noatual = myarray[0]
                if type(noatual) is int: #(len(noatual) != 3: #Considerando tamanho 3 do gene, isto é, duas entradas mais a função
                    fila.append("entrada")
                    myarray.pop(0)

                else:

                    if (noatual[2] != 800 and noatual[2] != 900 and noatual[2] != 800 and noatual[2] != 980 and noatual[2] != 990): #Verifica se a porta logica é unária ou binária. Caso seja binária, entra no else.
                        fila.append(noatual[0])

                        fila.append(noatual[1])

                        myarray.append(GENS_EVOL[w][noatual[0]])
                        myarray.append(GENS_EVOL[w][noatual[1]])

                        myarray.pop(0)

                    else:
                        fila.append(noatual[0])
                        myarray.append(GENS_EVOL[w][noatual[0]])
                        myarray.pop(0)

            for j in range(len(fila)): # Coloca X nas posições ativas do genótipo
                if type(fila[j]) is int:
                    if vetor[fila[j]] == "-":
                        vetor[fila[j]] = "X"

            MEUARRAY[w][i] = copy.deepcopy(vetor)

            vetor.clear()
            for k in range(Ln+ni):
                vetor.append("-")
                
            fila = [] #Limpa o vetor fila
            myarray = [] #Limpa o vetor myarray

def SAM_ativo(posicao):
    fila = [] #Array que funcionará como uma fila
    myarray = [] #Array que armazenará os genes dos nós da fila
    vetor = []
    for k in range(Ln+ni):
        vetor.append("-")

    for w in range(1): #Percorre o pai e seus lambda filhos
        for i in range(no): #Realiza o procedimento para o número de saídas
            saida = int(Ln+ni+i) #Determina a saída a ser processada
            fila.append(GENS_EVOL[posicao][saida])
            noatual = GENS_EVOL[posicao][GENS_EVOL[posicao][saida]] #Obtém as informações do gene responsável pela saída
            if type(noatual) is not int: #len(noatual) == 3: #Analisa se a saída é o resultado de um nó ou é uma entrada do programa. (3 significa: entrada, entrada e porta)
                fila.append(noatual[0]) #Adiciona primeira entrada do nó saída
                fila.append(noatual[1]) #Adiciona segunda entrada do nó saída
                myarray.append(GENS_EVOL[posicao][noatual[0]]) #Adiciona o gene responsável pela primeira entrada
                myarray.append(GENS_EVOL[posicao][noatual[1]]) #Adiciona o gene responsável pela segunda entrada
            else:
                fila.append(noatual) #Se tamanho é diferente de 3, então é uma entrada. Logo a fila recebe o valor da posição da entrada.
            while len(myarray) != 0: #Procedimento para esvaziar a fila
                noatual = myarray[0]
                if type(noatual) is int: #(len(noatual) != 3: #Considerando tamanho 3 do gene, isto é, duas entradas mais a função
                    fila.append("entrada")
                    myarray.pop(0)
                else:
                    fila.append(noatual[0])
                    fila.append(noatual[1])
                    myarray.append(GENS_EVOL[posicao][noatual[0]])
                    myarray.append(GENS_EVOL[posicao][noatual[1]])
                    myarray.pop(0)
                    
            for j in range(len(fila)): # Coloca X nas posições ativas do genótipo
                if type(fila[j]) is int:
                    if vetor[fila[j]] == "-":
                        vetor[fila[j]] = "X"

            MEUARRAY[posicao][i] = copy.deepcopy(vetor)
            vetor.clear()
            for k in range(Ln+ni):
                vetor.append("-")

            fila = [] #Limpa o vetor fila
            myarray = [] #Limpa o vetor myarray

               
def mutacao_pontual():
    for k in range(ee_lambda): #Realiza mutações e gera "ee_lambda" variações do pai
        print("ESTAMOS EM K IGUAL A", k)

        qtd_mutacao = r.randint(1, ug) #Determina quantos nós serão mutados, dentro do intervalo recomendado pela literatura
        for i in range(qtd_mutacao): #Realiza "qtd_mutacao" de mutações NESTE descendente
            G = copy.deepcopy(GENS_EVOL[0]) #Busca o pai original de volta (sempre alocado em GENS_EVOL[0])
            gene_mutado = r.randint(ni, (ni+no+Ln-1)) #O gene que será mutado pode receber qualquer valor maior que o último espaço ocupado pela entrada, isto é da posição ni (já que começa por 0) até a posição da última saida
            print(gene_mutado)
            if gene_mutado >= (Ln+ni): #Garante que o gene selecionado é uma saída
                ultimo_elemento = ni + Ln - 1 #Último elemento possivel que a saída pode assumir
                primeiro_elemento = ultimo_elemento - (lb * nr) + 1 #Primeiro valor possivel que a saída pode assumir
                elementos_portas = ultimo_elemento - primeiro_elemento + 1 #Quantidade de valores existentes entre o primeiro e último possiveis valores
                qtd_elementos = elementos_portas + ni - 1 #Determina a quantidade de valores distintos que a porta pode assumir como entrada. O -1 tem como função apenas possibilitar o uso de randint(0, qtd_elementos)
                entrada_ou_porta = r.randint(0, qtd_elementos) #Dá a mesma chance para todos os possiveis inputs
                if entrada_ou_porta >= ni: #G[Ln+ni+i] representa cada saida, sequencialmente
                    nova_saida = r.randint(primeiro_elemento, ultimo_elemento)
                    while G[gene_mutado] == nova_saida: #Garante que a mutação mudará o gene_mutado para um valor diferente do inicial
                        nova_saida = r.randint(primeiro_elemento, ultimo_elemento)
                    G[gene_mutado] = nova_saida
                else:
                    nova_saida = r.randint(0, ni-1)
                    while G[gene_mutado] == nova_saida: #Garante que a mutação mudará o gene_mutado para um valor diferente do inicial
                        nova_saida = r.randint(0, ni-1)
                    G[gene_mutado] = nova_saida    
            else:
                colunaatual = int((gene_mutado - ni)/nr) + 1 #Determina a coluna na qual o gene selecionado está
                alelo_mutado = r.randint(0, max_entradas) #Determina qual será o alelo a ser mutado
                print("Mutação em nó", gene_mutado, " alelo ", alelo_mutado, " coluna ", colunaatual)
                print("Nó correspondente: ", G[gene_mutado])
    
                if alelo_mutado == max_entradas: #Significa que a mutação ocorrerá no tipo da porta
                    porta_logica = r.randint(0, (len(FT)-1)) #Determina qual será a nova porta
                    while G[gene_mutado][alelo_mutado] == FT[porta_logica]: #Garante que a mutação mudará o gene_mutado para um valor diferente do inicial
                        porta_logica = r.randint(0, (len(FT)-1))
                    G[gene_mutado][alelo_mutado] = FT[porta_logica] #Atribui a nova porta ao gene
                else:
                    if colunaatual == 1: #Neste caso as portas lógicas só recebem inputs
                        entrada = r.randint(0, (ni-1))
                        while G[gene_mutado][alelo_mutado] == entrada:#Garante que a mutação mudará o gene_mutado para um valor diferente do inicial
                            entrada = r.randint(0, (ni-1))
                        G[gene_mutado][alelo_mutado] = entrada

                    
                    if colunaatual - lb < 1 and colunaatual != 1: #Neste caso as portas lógicas recebem inputs e nós       
                        valorpossivel = (nr * (colunaatual-2)) + (nr-1) + ni
                        sorteado = r.randint(0, valorpossivel)
                        while G[gene_mutado][alelo_mutado] == sorteado:#Garante que a mutação mudará o gene_mutado para um valor diferente do inicial
                            sorteado = r.randint(0, valorpossivel)
                        G[gene_mutado][alelo_mutado] = sorteado

            
                    if colunaatual - lb >= 1: #Neste caso as portas logicas recebem somente nós
                        primeiro_elemento = nr * (colunaatual-lb-1) + ni # O fator + ni serve para deslocar os elementos para frente, contando a quantidade de inputs.
                        ultimo_elemento = (nr * (colunaatual-2)) + (nr-1) + ni 
                        elementos_portas = ultimo_elemento - primeiro_elemento + 1 #Determina quantas portas possíveis existem
                        qtd_elementos = elementos_portas + ni - 1 #Determina a quantidade de valores distintos que a porta pode assumir como entrada. O -1 tem como função apenas possibilitar o uso de randint(0, qtd_elementos)
                        entrada_ou_porta = r.randint(0, qtd_elementos) #Dá a mesma chance para todos os possiveis inputs
                        if entrada_ou_porta >= ni:
                            novo_valor_mutacao = r.randint(primeiro_elemento, ultimo_elemento)
                            while G[gene_mutado][alelo_mutado] == novo_valor_mutacao:#Garante que a mutação mudará o gene_mutado para um valor diferente do inicial
                                novo_valor_mutacao = r.randint(primeiro_elemento, ultimo_elemento)
                            G[gene_mutado][alelo_mutado] = novo_valor_mutacao
                        else:
                            novo_valor_mutacao = r.randint(0, ni-1)
                            while G[gene_mutado][alelo_mutado] == novo_valor_mutacao:#Garante que a mutação mudará o gene_mutado para um valor diferente do inicial
                                novo_valor_mutacao = r.randint(0, ni-1)
                            G[gene_mutado][alelo_mutado] = novo_valor_mutacao
    
                print("Nó correspondente: ", G[gene_mutado])

        GENS_EVOL[k+1] = copy.deepcopy(G) #Passa o genótipo mutado para a matriz de genótipos
        print("EVOL K+1", GENS_EVOL[k+1])

def conta_portas(posicao):
    ATIVOS_LOCAL = []
    todos_ativos = []
    for i in range(len(MEUARRAY[posicao])):
        for j in range(len(MEUARRAY[posicao][i])):
            if MEUARRAY[posicao][i][j] == "X":
                todos_ativos.append(j)
    ATIVOS_LOCAL.clear()
    ATIVOS_LOCAL.append([])
    ATIVOS_LOCAL[0] = copy.deepcopy(MEUARRAY[posicao][0])
    for i in range(len(ATIVOS_LOCAL[0])):
        if ATIVOS_LOCAL[0][i] == "X":
            ATIVOS_LOCAL[0][i] == "X"
    for i in todos_ativos:
        ATIVOS_LOCAL[0][i] = "X"

    qtd_portas = 0
    qtd_wires = 0

    for i in range(len(ATIVOS_LOCAL[0])):
        if i >= ni and ATIVOS_LOCAL[0][i] == "X":
            if GENS_EVOL[posicao][i][2] == 900:
                qtd_wires = qtd_wires + 1
            elif GENS_EVOL[posicao][i][2] != 900 and GENS_EVOL[posicao][i][2] != 980 and GENS_EVOL[posicao][i][2] != 990:
                qtd_portas = qtd_portas + 1
    PORTAS_CKT[posicao] = copy.deepcopy(qtd_portas)
    WIRES_CKT[posicao] = copy.deepcopy(qtd_wires)







def mutacao_SAM_CORRETO_CROSS(posicao):

    todos_ativos = []
    for i in range(len(MEUARRAY[posicao])):
        for j in range(len(MEUARRAY[posicao][i])):
            if MEUARRAY[posicao][i][j] == "X":
                todos_ativos.append(j)
    ATIVO_MOM.clear()
    ATIVO_MOM.append([])
    ATIVO_MOM[0] = copy.deepcopy(MEUARRAY[posicao][0])
    for i in range(len(ATIVO_MOM[0])):
        if ATIVO_MOM[0][i] == "X":
            ATIVO_MOM[0][i] == "-"
    for i in todos_ativos:
        ATIVO_MOM[0][i] = "X"
    controle_ativo = True
    quantidade_de_mutacoes = 0
    controle_auxiliar = 0 # Esta variável serve para o index da lista ATIVO_MOM[g][gene_mutado] não ser violado
        #FICA FAZENDO MUTAÇÃO
    for g in range(1): # Se for range(no), fará mutação em ao menos UM nó ativo para CADA saida, se for range(1), mudará somente UM gene ativo de todo o genótipo 
        qtd_mutacao = r.randint(1, ug) #Determina quantos nós serão mutados, dentro do intervalo recomendado pela literatura
        while controle_ativo == True: #Realiza "qtd_mutacao" de mutações NESTE descendente
            G = copy.deepcopy(GENS_EVOL[0]) #Busca o pai original de volta (sempre alocado em GENS_EVOL[0])
            gene_mutado = r.randint(ni, (ni+no+Ln-1)) #O gene que será mutado pode receber qualquer valor maior que o último espaço ocupado pela entrada, isto é da posição ni (já que começa por 0) até a posição da última saida

            if gene_mutado > ni+Ln-1:
                quantidade_de_mutacoes += 1
                if quantidade_de_mutacoes == GLOBAL_SAM_MUTACOES_CROSS:
                    controle_ativo = False
                controle_auxiliar = gene_mutado
                gene_mutado = 0
            if ATIVO_MOM[g][gene_mutado] == "X" and controle_ativo == True:
                if gene_mutado > ni+Ln-1:
                    quantidade_de_mutacoes += 1
                    if quantidade_de_mutacoes == GLOBAL_SAM_MUTACOES_CROSS:
                        controle_ativo = False
                    controle_auxiliar = gene_mutado
                    gene_mutado = 0
                quantidade_de_mutacoes += 1
                if quantidade_de_mutacoes == GLOBAL_SAM_MUTACOES_CROSS:
                    controle_ativo = False

            if controle_auxiliar != 0:
                gene_mutado = controle_auxiliar
                controle_auxiliar = 0

            if gene_mutado >= (Ln+ni): #Garante que o gene selecionado é uma saída
                #print("MUTAÇÃO TIPO SAIDA")
                ultimo_elemento = ni + Ln - 1 #Último elemento possivel que a saída pode assumir
                primeiro_elemento = ultimo_elemento - (lb * nr) + 1 #Primeiro valor possivel que a saída pode assumir
                elementos_portas = ultimo_elemento - primeiro_elemento + 1 #Quantidade de valores existentes entre o primeiro e último possiveis valores
                qtd_elementos = elementos_portas + ni - 1 #Determina a quantidade de valores distintos que a porta pode assumir como entrada. O -1 tem como função apenas possibilitar o uso de randint(0, qtd_elementos)
                entrada_ou_porta = r.randint(0, qtd_elementos) #Dá a mesma chance para todos os possiveis inputs
                if entrada_ou_porta >= ni: #G[Ln+ni+i] representa cada saida, sequencialmente
                    nova_saida = r.randint(primeiro_elemento, ultimo_elemento)
                    while G[gene_mutado] == nova_saida: #Garante que a mutação mudará o gene_mutado para um valor diferente do inicial
                        nova_saida = r.randint(primeiro_elemento, ultimo_elemento)
                    G[gene_mutado] = nova_saida
                else:
                    nova_saida = r.randint(0, ni-1)
                    while G[gene_mutado] == nova_saida: #Garante que a mutação mudará o gene_mutado para um valor diferente do inicial
                        nova_saida = r.randint(0, ni-1)
                    G[gene_mutado] = nova_saida    
            else:
                colunaatual = int((gene_mutado - ni)/nr) + 1 #Determina a coluna na qual o gene selecionado está
                alelo_mutado = r.randint(0, max_entradas) #Determina qual será o alelo a ser mutado

        
                if alelo_mutado == max_entradas: #Significa que a mutação ocorrerá no tipo da porta
                    #print("MUTAÇÃO TIPO PORTA") 
                    porta_logica = r.randint(0, (len(FT)-1)) #Determina qual será a nova porta
                    while G[gene_mutado][alelo_mutado] == FT[porta_logica]: #Garante que a mutação mudará o gene_mutado para um valor diferente do inicial
                        porta_logica = r.randint(0, (len(FT)-1))
                    G[gene_mutado][alelo_mutado] = FT[porta_logica] #Atribui a nova porta ao gene
                else:
                    #print("MUTAÇÃO TIPO INPUT")
                    if colunaatual == 1: #Neste caso as portas lógicas só recebem inputs
                        entrada = r.randint(0, (ni-1))
                        while G[gene_mutado][alelo_mutado] == entrada:#Garante que a mutação mudará o gene_mutado para um valor diferente do inicial
                            entrada = r.randint(0, (ni-1))
                        G[gene_mutado][alelo_mutado] = entrada
    
                        
                    if colunaatual - lb < 1 and colunaatual != 1: #Neste caso as portas lógicas recebem inputs e nós       
                        valorpossivel = (nr * (colunaatual-2)) + (nr-1) + ni
                        sorteado = r.randint(0, valorpossivel)
                        while G[gene_mutado][alelo_mutado] == sorteado:#Garante que a mutação mudará o gene_mutado para um valor diferente do inicial
                            sorteado = r.randint(0, valorpossivel)
                        G[gene_mutado][alelo_mutado] = sorteado
    
                
                    if colunaatual - lb >= 1: #Neste caso as portas logicas recebem somente nós
                        primeiro_elemento = nr * (colunaatual-lb-1) + ni # O fator + ni serve para deslocar os elementos para frente, contando a quantidade de inputs.
                        ultimo_elemento = (nr * (colunaatual-2)) + (nr-1) + ni 
                        elementos_portas = ultimo_elemento - primeiro_elemento + 1 #Determina quantas portas possíveis existem
                        qtd_elementos = elementos_portas + ni - 1 #Determina a quantidade de valores distintos que a porta pode assumir como entrada. O -1 tem como função apenas possibilitar o uso de randint(0, qtd_elementos)
                        entrada_ou_porta = r.randint(0, qtd_elementos) #Dá a mesma chance para todos os possiveis inputs
                        if entrada_ou_porta >= ni:
                            novo_valor_mutacao = r.randint(primeiro_elemento, ultimo_elemento)
                            while G[gene_mutado][alelo_mutado] == novo_valor_mutacao:#Garante que a mutação mudará o gene_mutado para um valor diferente do inicial
                                novo_valor_mutacao = r.randint(primeiro_elemento, ultimo_elemento)
                            G[gene_mutado][alelo_mutado] = novo_valor_mutacao
                        else:
                            novo_valor_mutacao = r.randint(0, ni-1)
                            while G[gene_mutado][alelo_mutado] == novo_valor_mutacao:#Garante que a mutação mudará o gene_mutado para um valor diferente do inicial
                                novo_valor_mutacao = r.randint(0, ni-1)
                            G[gene_mutado][alelo_mutado] = novo_valor_mutacao

        SAM_ativo(posicao)
        GENS_EVOL[posicao] = copy.deepcopy(G) #Passa o genótipo mutado para a matriz de genótipos

        G = copy.deepcopy(GENS_EVOL[0])
    
    
def mutacao_SAM_CORRETO(posicao):

    todos_ativos = []
    for i in range(len(MEUARRAY[posicao])):
        for j in range(len(MEUARRAY[posicao][i])):
            if MEUARRAY[posicao][i][j] == "X":
                todos_ativos.append(j)
    ATIVO_MOM.clear()
    ATIVO_MOM.append([])
    ATIVO_MOM[0] = copy.deepcopy(MEUARRAY[posicao][0])
    for i in range(len(ATIVO_MOM[0])):
        if ATIVO_MOM[0][i] == "X":
            ATIVO_MOM[0][i] == "-"
    for i in todos_ativos:
        ATIVO_MOM[0][i] = "X"
    controle_ativo = True
    quantidade_de_mutacoes = 0
    controle_auxiliar = 0 # Esta variável serve para o index da lista ATIVO_MOM[g][gene_mutado] não ser violado
        #FICA FAZENDO MUTAÇÃO
    for g in range(1): # Se for range(no), fará mutação em ao menos UM nó ativo para CADA saida, se for range(1), mudará somente UM gene ativo de todo o genótipo 
        qtd_mutacao = r.randint(1, ug) #Determina quantos nós serão mutados, dentro do intervalo recomendado pela literatura
        while controle_ativo == True: #Realiza "qtd_mutacao" de mutações NESTE descendente
            G = copy.deepcopy(GENS_EVOL[0]) #Busca o pai original de volta (sempre alocado em GENS_EVOL[0])
            gene_mutado = r.randint(ni, (ni+no+Ln-1)) #O gene que será mutado pode receber qualquer valor maior que o último espaço ocupado pela entrada, isto é da posição ni (já que começa por 0) até a posição da última saida

            if gene_mutado > ni+Ln-1:
                quantidade_de_mutacoes += 1
                if quantidade_de_mutacoes == GLOBAL_SAM_MUTACOES:
                    controle_ativo = False
                controle_auxiliar = gene_mutado
                gene_mutado = 0
            if ATIVO_MOM[g][gene_mutado] == "X" and controle_ativo == True:
                if gene_mutado > ni+Ln-1:
                    quantidade_de_mutacoes += 1
                    if quantidade_de_mutacoes == GLOBAL_SAM_MUTACOES:
                        controle_ativo = False
                    controle_auxiliar = gene_mutado
                    gene_mutado = 0
                quantidade_de_mutacoes += 1
                if quantidade_de_mutacoes == GLOBAL_SAM_MUTACOES:
                    controle_ativo = False

            if controle_auxiliar != 0:
                gene_mutado = controle_auxiliar
                controle_auxiliar = 0

            if gene_mutado >= (Ln+ni): #Garante que o gene selecionado é uma saída
                #print("MUTAÇÃO TIPO SAIDA")
                ultimo_elemento = ni + Ln - 1 #Último elemento possivel que a saída pode assumir
                primeiro_elemento = ultimo_elemento - (lb * nr) + 1 #Primeiro valor possivel que a saída pode assumir
                elementos_portas = ultimo_elemento - primeiro_elemento + 1 #Quantidade de valores existentes entre o primeiro e último possiveis valores
                qtd_elementos = elementos_portas + ni - 1 #Determina a quantidade de valores distintos que a porta pode assumir como entrada. O -1 tem como função apenas possibilitar o uso de randint(0, qtd_elementos)
                entrada_ou_porta = r.randint(0, qtd_elementos) #Dá a mesma chance para todos os possiveis inputs
                if entrada_ou_porta >= ni: #G[Ln+ni+i] representa cada saida, sequencialmente
                    nova_saida = r.randint(primeiro_elemento, ultimo_elemento)
                    while G[gene_mutado] == nova_saida: #Garante que a mutação mudará o gene_mutado para um valor diferente do inicial
                        nova_saida = r.randint(primeiro_elemento, ultimo_elemento)
                    G[gene_mutado] = nova_saida
                else:
                    nova_saida = r.randint(0, ni-1)
                    while G[gene_mutado] == nova_saida: #Garante que a mutação mudará o gene_mutado para um valor diferente do inicial
                        nova_saida = r.randint(0, ni-1)
                    G[gene_mutado] = nova_saida    
            else:
                colunaatual = int((gene_mutado - ni)/nr) + 1 #Determina a coluna na qual o gene selecionado está
                alelo_mutado = r.randint(0, max_entradas) #Determina qual será o alelo a ser mutado

        
                if alelo_mutado == max_entradas: #Significa que a mutação ocorrerá no tipo da porta
                    #print("MUTAÇÃO TIPO PORTA") 
                    porta_logica = r.randint(0, (len(FT)-1)) #Determina qual será a nova porta
                    while G[gene_mutado][alelo_mutado] == FT[porta_logica]: #Garante que a mutação mudará o gene_mutado para um valor diferente do inicial
                        porta_logica = r.randint(0, (len(FT)-1))
                    G[gene_mutado][alelo_mutado] = FT[porta_logica] #Atribui a nova porta ao gene
                else:
                    #print("MUTAÇÃO TIPO INPUT")
                    if colunaatual == 1: #Neste caso as portas lógicas só recebem inputs
                        entrada = r.randint(0, (ni-1))
                        while G[gene_mutado][alelo_mutado] == entrada:#Garante que a mutação mudará o gene_mutado para um valor diferente do inicial
                            entrada = r.randint(0, (ni-1))
                        G[gene_mutado][alelo_mutado] = entrada
    
                        
                    if colunaatual - lb < 1 and colunaatual != 1: #Neste caso as portas lógicas recebem inputs e nós       
                        valorpossivel = (nr * (colunaatual-2)) + (nr-1) + ni
                        sorteado = r.randint(0, valorpossivel)
                        while G[gene_mutado][alelo_mutado] == sorteado:#Garante que a mutação mudará o gene_mutado para um valor diferente do inicial
                            sorteado = r.randint(0, valorpossivel)
                        G[gene_mutado][alelo_mutado] = sorteado
    
                
                    if colunaatual - lb >= 1: #Neste caso as portas logicas recebem somente nós
                        primeiro_elemento = nr * (colunaatual-lb-1) + ni # O fator + ni serve para deslocar os elementos para frente, contando a quantidade de inputs.
                        ultimo_elemento = (nr * (colunaatual-2)) + (nr-1) + ni 
                        elementos_portas = ultimo_elemento - primeiro_elemento + 1 #Determina quantas portas possíveis existem
                        qtd_elementos = elementos_portas + ni - 1 #Determina a quantidade de valores distintos que a porta pode assumir como entrada. O -1 tem como função apenas possibilitar o uso de randint(0, qtd_elementos)
                        entrada_ou_porta = r.randint(0, qtd_elementos) #Dá a mesma chance para todos os possiveis inputs
                        if entrada_ou_porta >= ni:
                            novo_valor_mutacao = r.randint(primeiro_elemento, ultimo_elemento)
                            while G[gene_mutado][alelo_mutado] == novo_valor_mutacao:#Garante que a mutação mudará o gene_mutado para um valor diferente do inicial
                                novo_valor_mutacao = r.randint(primeiro_elemento, ultimo_elemento)
                            G[gene_mutado][alelo_mutado] = novo_valor_mutacao
                        else:
                            novo_valor_mutacao = r.randint(0, ni-1)
                            while G[gene_mutado][alelo_mutado] == novo_valor_mutacao:#Garante que a mutação mudará o gene_mutado para um valor diferente do inicial
                                novo_valor_mutacao = r.randint(0, ni-1)
                            G[gene_mutado][alelo_mutado] = novo_valor_mutacao

        SAM_ativo(posicao)
        GENS_EVOL[posicao] = copy.deepcopy(G) #Passa o genótipo mutado para a matriz de genótipos

        G = copy.deepcopy(GENS_EVOL[0])


def monta_tabela(posicao): #Monta a tabela verdade e quantifica a fitness do individuo
    auxiliar = []
    qtd_portas = 0
    for i in range(no):
        for j in range(len(MEUARRAY[posicao][i])):
            if MEUARRAY[posicao][i][j] == "X":

                if j < ni:
                    #print("É UMA ENTRADA")
                    TABELA_V[j] = copy.deepcopy(INPUTS[j])
                else:
                    #print("NÃO É UMA ENTRADA")
                    entradaA = GENS_EVOL[posicao][j][0]
                    entradaB = GENS_EVOL[posicao][j][1]
                    funcao = GENS_EVOL[posicao][j][2]
                    if funcao == 100:
                        for w in range(len(TABELA_V[entradaA])):
                            auxiliar.append((TABELA_V[entradaA][w]) and (TABELA_V[entradaB][w]))
                        TABELA_V[j] = copy.deepcopy(auxiliar)
                        auxiliar = []
                        qtd_portas = qtd_portas + 1
                    if funcao == 110:
                        for w in range(len(TABELA_V[entradaA])):
                            auxiliar.append((TABELA_V[entradaA][w]) or (TABELA_V[entradaB][w]))
                        TABELA_V[j] = copy.deepcopy(auxiliar)
                        auxiliar = []
                        qtd_portas = qtd_portas + 1
                    if funcao == 130:
                        for w in range(len(TABELA_V[entradaA])):
                            auxiliar.append((((not(TABELA_V[entradaA][w])) and (TABELA_V[entradaB][w])) or ((not(TABELA_V[entradaB][w])) and (TABELA_V[entradaA][w]))))
                        TABELA_V[j] = copy.deepcopy(auxiliar)
                        auxiliar = []
                        qtd_portas = qtd_portas + 1
                    if funcao == 150:
                        for w in range(len(TABELA_V[entradaA])):
                            auxiliar.append(not((TABELA_V[entradaA][w]) and (TABELA_V[entradaB][w])))
                        TABELA_V[j] = copy.deepcopy(auxiliar)
                        auxiliar = []
                        qtd_portas = qtd_portas + 1
                    if funcao == 160:
                        for w in range(len(TABELA_V[entradaA])):
                            auxiliar.append(not((TABELA_V[entradaA][w]) or (TABELA_V[entradaB][w])))
                        TABELA_V[j] = copy.deepcopy(auxiliar)
                        auxiliar = []
                        qtd_portas = qtd_portas + 1
                    if funcao == 800:
                        for w in range(len(TABELA_V[entradaA])):
                            auxiliar.append(not(TABELA_V[entradaA][w]))
                        TABELA_V[j] = copy.deepcopy(auxiliar)
                        auxiliar = []
                        qtd_portas = qtd_portas + 1
                    if funcao == 900:
                        for w in range(len(TABELA_V[entradaA])):
                            auxiliar.append(TABELA_V[entradaA][w])
                        TABELA_V[j] = copy.deepcopy(auxiliar)
                        auxiliar = []
                    if funcao == 980:
                        for w in range(len(TABELA_V[entradaA])):
                            auxiliar.append(0)
                        TABELA_V[j] = copy.deepcopy(auxiliar)
                        auxiliar = []
                    if funcao == 990:
                        for w in range(len(TABELA_V[entradaA])):
                            auxiliar.append(1)
                        TABELA_V[j] = copy.deepcopy(auxiliar)
                        auxiliar = []
                    

        saida_ckt = GENS_EVOL[posicao][Ln+ni+i] #Determina a saida atual do circuito, que está sendo processada
        tabela_saida = TABELA_V[saida_ckt]
        if len(TABELA_V[saida_ckt]) == 0:
            print("TABELA VAZIA")
        fitness_atual = 0
        for s in range(len(OUTPUTS[i])):
            if OUTPUTS[i][s] == TABELA_V[saida_ckt][s]:
                #print("OUTPUT IGUAL")
                fitness_atual = fitness_atual + 1
        FITNESS_EE[posicao][i] = copy.deepcopy(fitness_atual)
        PORTAS_EE[posicao][i] = copy.deepcopy(qtd_portas)
        TABELA_V.clear()
        formato_tabela_verdade()

def monta_tabela_wires(posicao): #Monta a tabela verdade e quantifica a fitness do individuo
    auxiliar = []
    qtd_portas = 0
    for i in range(no):
        for j in range(len(MEUARRAY[posicao][i])):
            if MEUARRAY[posicao][i][j] == "X":
                if j < ni:
                    #print("É UMA ENTRADA")
                    TABELA_V[j] = copy.deepcopy(INPUTS[j])
                else:
                    #print("NÃO É UMA ENTRADA")
                    entradaA = GENS_EVOL[posicao][j][0]
                    entradaB = GENS_EVOL[posicao][j][1]
                    funcao = GENS_EVOL[posicao][j][2]
                    if funcao == 100:
                        for w in range(len(TABELA_V[entradaA])):
                            auxiliar.append((TABELA_V[entradaA][w]) and (TABELA_V[entradaB][w]))
                        TABELA_V[j] = copy.deepcopy(auxiliar)
                        auxiliar = []
                        qtd_portas = qtd_portas + 1
                    if funcao == 110:
                        for w in range(len(TABELA_V[entradaA])):
                            auxiliar.append((TABELA_V[entradaA][w]) or (TABELA_V[entradaB][w]))
                        TABELA_V[j] = copy.deepcopy(auxiliar)
                        auxiliar = []
                        qtd_portas = qtd_portas + 1
                    if funcao == 130:
                        for w in range(len(TABELA_V[entradaA])):
                            auxiliar.append((((not(TABELA_V[entradaA][w])) and (TABELA_V[entradaB][w])) or ((not(TABELA_V[entradaB][w])) and (TABELA_V[entradaA][w]))))
                        TABELA_V[j] = copy.deepcopy(auxiliar)
                        auxiliar = []
                        qtd_portas = qtd_portas + 1
                    if funcao == 150:
                        for w in range(len(TABELA_V[entradaA])):
                            auxiliar.append(not((TABELA_V[entradaA][w]) and (TABELA_V[entradaB][w])))
                        TABELA_V[j] = copy.deepcopy(auxiliar)
                        auxiliar = []
                        qtd_portas = qtd_portas + 1
                    if funcao == 160:
                        for w in range(len(TABELA_V[entradaA])):
                            auxiliar.append(not((TABELA_V[entradaA][w]) or (TABELA_V[entradaB][w])))
                        TABELA_V[j] = copy.deepcopy(auxiliar)
                        auxiliar = []
                        qtd_portas = qtd_portas + 1
                    if funcao == 800:
                        for w in range(len(TABELA_V[entradaA])):
                            auxiliar.append(not(TABELA_V[entradaA][w]))
                        TABELA_V[j] = copy.deepcopy(auxiliar)
                        auxiliar = []
                        qtd_portas = qtd_portas + 1
                    if funcao == 900:
                        for w in range(len(TABELA_V[entradaA])):
                            auxiliar.append(TABELA_V[entradaA][w])
                        TABELA_V[j] = copy.deepcopy(auxiliar)
                        auxiliar = []
                    if funcao == 980:
                        for w in range(len(TABELA_V[entradaA])):
                            auxiliar.append(0)
                        TABELA_V[j] = copy.deepcopy(auxiliar)
                        auxiliar = []
                    if funcao == 990:
                        for w in range(len(TABELA_V[entradaA])):
                            auxiliar.append(1)
                        TABELA_V[j] = copy.deepcopy(auxiliar)
                        auxiliar = []
                    

        saida_ckt = GENS_EVOL[posicao][Ln+ni+i] #Determina a saida atual do circuito, que está sendo processada
        tabela_saida = TABELA_V[saida_ckt]
        if len(TABELA_V[saida_ckt]) == 0:
            print("TABELA VAZIA")
        fitness_atual = 0
        for s in range(len(OUTPUTS[i])):
            if OUTPUTS[i][s] == TABELA_V[saida_ckt][s]:
                #print("OUTPUT IGUAL")
                fitness_atual = fitness_atual + 1

        if fitness_atual == (no * len(OUTPUTS[0])):
            nos_ativos_agora = []
            nos_ativos_agora.clear()
            for p in range(len(MEUARRAY[posicao])):
                for q in range(len(MEUARRAY[posicao][p])):
                    if MEUARRAY[posicao][p][q] == "X":
                        nos_ativos_agora.append(q)
            nova_lista = []
            nova_lista.clear()
            nova_lista = sorted(set(nos_ativos_agora))

            nos_ativos_saida = []
            nos_ativos_saida.clear()
            for p in range(len(nova_lista)):
                if nova_lista[p] >= ni:
                    nos_ativos_saida.append(nova_lista[p])
            wires = 0
            for p in range(len(nos_ativos_saida)):
                if GENS_EVOL[posicao][nos_ativos_saida[p]][2] == 900:
                    wires = wires + 1
            fitness_atual = fitness_atual + wires
                                
        
        FITNESS_EE[posicao][i] = copy.deepcopy(fitness_atual)
        PORTAS_EE[posicao][i] = copy.deepcopy(qtd_portas)
        TABELA_V.clear()
        formato_tabela_verdade()
    
def atualiza_pai_CGP_PADRAO(): #Analisa somente a fitness dos individuos. Em caso de empate, sorteia um.
    soma = 0
    fitness_soma = [] # Array que armazena a soma de todas as saídas de cada circuito
    aux = []
    for k in range(ee_lambda + 1):
        fitness_soma.append([])
    for i in range(len(FITNESS_EE)):
        for j in range(len(FITNESS_EE[i])):
            soma = soma + FITNESS_EE[i][j]
        fitness_soma[i] = copy.deepcopy(soma)
        soma = 0
    for s in range(len(fitness_soma)):
        if fitness_soma[s] == max(fitness_soma):
            aux.append(s)
    sorteia = r.randint(0, len(aux)-1)


    portas = 0
    portas_soma = []
    aux_portas = []
    for k in range(ee_lambda + 1):
        portas_soma.append([])
    for i in range(len(PORTAS_EE)):
        for j in range(len(PORTAS_EE[i])):
            portas = portas = PORTAS_EE[i][j]
        portas_soma[i] = copy.deepcopy(portas)
        portas = 0
    for s in range(len(portas_soma)):
        if portas_soma[s] == max(portas_soma):
            aux_portas.append(s)
    sorteia_porta = r.randint(0, len(aux_portas)-1)

    if fitness_soma[aux[sorteia]] < (no * len(OUTPUTS[0])):
        caso = 1
    else:
        caso = 2
### MATRIZ REGIÃO INFACTÍVEL
    if fitness_soma[0] < fitness_soma[aux[sorteia]] and fitness_soma[aux[sorteia]] < (no * len(OUTPUTS[0])):
        caso = 1
        for i in range(len(GENS_EVOL[0])):
            if GENS_EVOL[0][i] != GENS_EVOL[aux[sorteia]][i]:
                if i >= ni and i < ni+Ln:
                    if GENS_EVOL[0][i][2] != GENS_EVOL[aux[sorteia]][i][2]:
                        if GENS_EVOL[0][i][2] == 900:
                            if GENS_EVOL[aux[sorteia]][i][2] == 100:
                                MATRIZ_REGIAO_INFACTIVEL[0][1][0] = MATRIZ_REGIAO_INFACTIVEL[0][1][0] + 1
                            if GENS_EVOL[aux[sorteia]][i][2] == 110:
                                MATRIZ_REGIAO_INFACTIVEL[0][2][0] = MATRIZ_REGIAO_INFACTIVEL[0][2][0] + 1
                            if GENS_EVOL[aux[sorteia]][i][2] == 130:
                                MATRIZ_REGIAO_INFACTIVEL[0][3][0] = MATRIZ_REGIAO_INFACTIVEL[0][3][0] + 1
                            if GENS_EVOL[aux[sorteia]][i][2] == 800:
                                MATRIZ_REGIAO_INFACTIVEL[0][4][0] = MATRIZ_REGIAO_INFACTIVEL[0][4][0] + 1
                        if GENS_EVOL[0][i][2] == 100:
                            if GENS_EVOL[aux[sorteia]][i][2] == 900:
                                MATRIZ_REGIAO_INFACTIVEL[1][0][0] = MATRIZ_REGIAO_INFACTIVEL[1][0][0] + 1
                            if GENS_EVOL[aux[sorteia]][i][2] == 110:
                                MATRIZ_REGIAO_INFACTIVEL[1][2][0] = MATRIZ_REGIAO_INFACTIVEL[1][2][0] + 1
                            if GENS_EVOL[aux[sorteia]][i][2] == 130:
                                MATRIZ_REGIAO_INFACTIVEL[1][3][0] = MATRIZ_REGIAO_INFACTIVEL[1][3][0] + 1
                            if GENS_EVOL[aux[sorteia]][i][2] == 800:
                                MATRIZ_REGIAO_INFACTIVEL[1][4][0] = MATRIZ_REGIAO_INFACTIVEL[1][4][0] + 1
                        if GENS_EVOL[0][i][2] == 110:
                            if GENS_EVOL[aux[sorteia]][i][2] == 900:
                                MATRIZ_REGIAO_INFACTIVEL[2][0][0] = MATRIZ_REGIAO_INFACTIVEL[2][0][0] + 1
                            if GENS_EVOL[aux[sorteia]][i][2] == 100:
                                MATRIZ_REGIAO_INFACTIVEL[2][1][0] = MATRIZ_REGIAO_INFACTIVEL[2][1][0] + 1
                            if GENS_EVOL[aux[sorteia]][i][2] == 130:
                                MATRIZ_REGIAO_INFACTIVEL[2][3][0] = MATRIZ_REGIAO_INFACTIVEL[2][3][0] + 1
                            if GENS_EVOL[aux[sorteia]][i][2] == 800:
                                MATRIZ_REGIAO_INFACTIVEL[2][4][0] = MATRIZ_REGIAO_INFACTIVEL[2][4][0] + 1
                        if GENS_EVOL[0][i][2] == 130:
                            if GENS_EVOL[aux[sorteia]][i][2] == 900:
                                MATRIZ_REGIAO_INFACTIVEL[3][0][0] = MATRIZ_REGIAO_INFACTIVEL[3][0][0] + 1
                            if GENS_EVOL[aux[sorteia]][i][2] == 100:
                                MATRIZ_REGIAO_INFACTIVEL[3][1][0] = MATRIZ_REGIAO_INFACTIVEL[3][1][0] + 1
                            if GENS_EVOL[aux[sorteia]][i][2] == 110:
                                MATRIZ_REGIAO_INFACTIVEL[3][2][0] = MATRIZ_REGIAO_INFACTIVEL[3][2][0] + 1
                            if GENS_EVOL[aux[sorteia]][i][2] == 800:
                                MATRIZ_REGIAO_INFACTIVEL[3][4][0] = MATRIZ_REGIAO_INFACTIVEL[3][4][0] + 1
                        if GENS_EVOL[0][i][2] == 800:
                            if GENS_EVOL[aux[sorteia]][i][2] == 900:
                                MATRIZ_REGIAO_INFACTIVEL[4][0][0] = MATRIZ_REGIAO_INFACTIVEL[4][0][0] + 1
                            if GENS_EVOL[aux[sorteia]][i][2] == 100:
                                MATRIZ_REGIAO_INFACTIVEL[4][1][0] = MATRIZ_REGIAO_INFACTIVEL[4][1][0] + 1
                            if GENS_EVOL[aux[sorteia]][i][2] == 110:
                                MATRIZ_REGIAO_INFACTIVEL[4][2][0] = MATRIZ_REGIAO_INFACTIVEL[4][2][0] + 1
                            if GENS_EVOL[aux[sorteia]][i][2] == 130:
                                MATRIZ_REGIAO_INFACTIVEL[4][3][0] = MATRIZ_REGIAO_INFACTIVEL[4][3][0] + 1    

### MATRIZ REGIÃO FACTÍVEL
                               
    if portas_soma[0] > portas_soma[aux_portas[sorteia_porta]] and fitness_soma[aux[sorteia_porta]] >= (no * len(OUTPUTS[0])):
        print("ENTROU NA REGIÃO FACTÍVEL")
        caso = 2
        for i in range(len(GENS_EVOL[0])):
            if GENS_EVOL[0][i] != GENS_EVOL[aux[sorteia_porta]][i]:
                if i >= ni and i < ni+Ln:
                    if GENS_EVOL[0][i][2] != GENS_EVOL[aux[sorteia_porta]][i][2]:
                        if GENS_EVOL[0][i][2] == 900:
                            if GENS_EVOL[aux[sorteia_porta]][i][2] == 100:
                                MATRIZ_REGIAO_FACTIVEL[0][1][0] = MATRIZ_REGIAO_FACTIVEL[0][1][0] + 1
                            if GENS_EVOL[aux[sorteia_porta]][i][2] == 110:
                                MATRIZ_REGIAO_FACTIVEL[0][2][0] = MATRIZ_REGIAO_FACTIVEL[0][2][0] + 1
                            if GENS_EVOL[aux[sorteia_porta]][i][2] == 130:
                                MATRIZ_REGIAO_FACTIVEL[0][3][0] = MATRIZ_REGIAO_FACTIVEL[0][3][0] + 1
                            if GENS_EVOL[aux[sorteia_porta]][i][2] == 800:
                                MATRIZ_REGIAO_FACTIVEL[0][4][0] = MATRIZ_REGIAO_FACTIVEL[0][4][0] + 1
                        if GENS_EVOL[0][i][2] == 100:
                            if GENS_EVOL[aux[sorteia_porta]][i][2] == 900:
                                MATRIZ_REGIAO_FACTIVEL[1][0][0] = MATRIZ_REGIAO_FACTIVEL[1][0][0] + 1
                            if GENS_EVOL[aux[sorteia_porta]][i][2] == 110:
                                MATRIZ_REGIAO_FACTIVEL[1][2][0] = MATRIZ_REGIAO_FACTIVEL[1][2][0] + 1
                            if GENS_EVOL[aux[sorteia_porta]][i][2] == 130:
                                MATRIZ_REGIAO_FACTIVEL[1][3][0] = MATRIZ_REGIAO_FACTIVEL[1][3][0] + 1
                            if GENS_EVOL[aux[sorteia_porta]][i][2] == 800:
                                MATRIZ_REGIAO_FACTIVEL[1][4][0] = MATRIZ_REGIAO_FACTIVEL[1][4][0] + 1
                        if GENS_EVOL[0][i][2] == 110:
                            if GENS_EVOL[aux[sorteia_porta]][i][2] == 900:
                                MATRIZ_REGIAO_FACTIVEL[2][0][0] = MATRIZ_REGIAO_FACTIVEL[2][0][0] + 1
                            if GENS_EVOL[aux[sorteia_porta]][i][2] == 100:
                                MATRIZ_REGIAO_FACTIVEL[2][1][0] = MATRIZ_REGIAO_FACTIVEL[2][1][0] + 1
                            if GENS_EVOL[aux[sorteia_porta]][i][2] == 130:
                                MATRIZ_REGIAO_FACTIVEL[2][3][0] = MATRIZ_REGIAO_FACTIVEL[2][3][0] + 1
                            if GENS_EVOL[aux[sorteia_porta]][i][2] == 800:
                                MATRIZ_REGIAO_FACTIVEL[2][4][0] = MATRIZ_REGIAO_FACTIVEL[2][4][0] + 1
                        if GENS_EVOL[0][i][2] == 130:
                            if GENS_EVOL[aux[sorteia_porta]][i][2] == 900:
                                MATRIZ_REGIAO_FACTIVEL[3][0][0] = MATRIZ_REGIAO_FACTIVEL[3][0][0] + 1
                            if GENS_EVOL[aux[sorteia_porta]][i][2] == 100:
                                MATRIZ_REGIAO_FACTIVEL[3][1][0] = MATRIZ_REGIAO_FACTIVEL[3][1][0] + 1
                            if GENS_EVOL[aux[sorteia_porta]][i][2] == 110:
                                MATRIZ_REGIAO_FACTIVEL[3][2][0] = MATRIZ_REGIAO_FACTIVEL[3][2][0] + 1
                            if GENS_EVOL[aux[sorteia_porta]][i][2] == 800:
                                MATRIZ_REGIAO_FACTIVEL[3][4][0] = MATRIZ_REGIAO_FACTIVEL[3][4][0] + 1
                        if GENS_EVOL[0][i][2] == 800:
                            if GENS_EVOL[aux[sorteia_porta]][i][2] == 900:
                                MATRIZ_REGIAO_FACTIVEL[4][0][0] = MATRIZ_REGIAO_FACTIVEL[4][0][0] + 1
                            if GENS_EVOL[aux[sorteia_porta]][i][2] == 100:
                                MATRIZ_REGIAO_FACTIVEL[4][1][0] = MATRIZ_REGIAO_FACTIVEL[4][1][0] + 1
                            if GENS_EVOL[aux[sorteia_porta]][i][2] == 110:
                                MATRIZ_REGIAO_FACTIVEL[4][2][0] = MATRIZ_REGIAO_FACTIVEL[4][2][0] + 1
                            if GENS_EVOL[aux[sorteia_porta]][i][2] == 130:
                                MATRIZ_REGIAO_FACTIVEL[4][3][0] = MATRIZ_REGIAO_FACTIVEL[4][3][0] + 1    

    if caso == 1:
        GENS_EVOL[0] = copy.deepcopy(GENS_EVOL[aux[sorteia]])
    elif caso == 2:
        GENS_EVOL[0] = copy.deepcopy(GENS_EVOL[aux[sorteia_porta]])


def atualiza_pai_PORTAS(): #Quantifica a quantidade de acertos da tabela verdade e a quantidade de portas (sugestão do Heder), ao longo de todo o processo
    soma = 0
    fitness_soma = [] # Array que armazena a soma de todas as saídas de cada circuito
    aux = []
    for k in range(ee_lambda + 1):
        fitness_soma.append([])
    for i in range(len(FITNESS_EE)):
        for j in range(len(FITNESS_EE[i])):
            soma = soma + FITNESS_EE[i][j]
        fitness_soma[i] = copy.deepcopy(soma)
        soma = 0
    maior_fitness = max(fitness_soma)

    indice_maior_fitness = [] #Array que armazena os índices dos genótipos com a maior fitness existente
    iguais = [] #Array que armazena o número de portas correspondente a cada um dos genótipos com maior fitness existente
    iguais_qtd_portas = [] #Array que armazena os índices dos genótipos cujas fitness são iguais e a quantidade de portas também, caso a quantidade de portas seja a menor existente
    for i in range(len(fitness_soma)):
        if fitness_soma[i] == maior_fitness:
            indice_maior_fitness.append(i)

    for i in range(len(indice_maior_fitness)):
        iguais.append(PORTAS_EE[indice_maior_fitness[i]][no-1])
    menor_porta = min(iguais)

    for i in range(len(PORTAS_EE)):
        
        if PORTAS_EE[i][no-1] == menor_porta and fitness_soma[i] == maior_fitness:
            iguais_qtd_portas.append(i)
    
    if len(iguais_qtd_portas) > 1:
        pai = r.randint(0, len(iguais_qtd_portas)-1)
        pai = iguais_qtd_portas[pai]

    else:
        pai = iguais_qtd_portas[0]
        


    indice_maior_fitness.clear()
    iguais.clear()
    iguais_qtd_portas.clear()


    GENS_EVOL[0] = copy.deepcopy(GENS_EVOL[pai]) #Atualiza novo pai



def atualiza_pai_PORTAS_FACTIVEL(): #Quando o circuito atende a tabela verdade, conta-se o número de portas
    soma = 0
    fitness_soma = [] # Array que armazena a soma de todas as saídas de cada circuito
    aux = []
    factivel = False 
    for k in range(ee_lambda + 1):
        fitness_soma.append([])
    for i in range(len(FITNESS_EE)):
        for j in range(len(FITNESS_EE[i])):
            soma = soma + FITNESS_EE[i][j]
        fitness_soma[i] = copy.deepcopy(soma)
        soma = 0

    for i in range(len(fitness_soma)):
        if fitness_soma[i] == (no * len(OUTPUTS[0])):
            factivel = True
 

    if factivel == False:
        for s in range(len(fitness_soma)):
            if fitness_soma[s] == max(fitness_soma):
                aux.append(s)
        sorteia = r.randint(0, len(aux)-1)

        if fitness_soma[0] < fitness_soma[aux[sorteia]]:
            for i in range(len(GENS_EVOL[0])):
                if GENS_EVOL[0][i] != GENS_EVOL[aux[sorteia]][i]:

                    if i >= ni and i < ni+Ln:
                        if GENS_EVOL[0][i][2] != GENS_EVOL[aux[sorteia]][i][2]:
                            if GENS_EVOL[0][i][2] == 900:
                                if GENS_EVOL[aux[sorteia]][i][2] == 100:
                                    MATRIZ_REGIAO_INFACTIVEL[0][1][0] = MATRIZ_REGIAO_INFACTIVEL[0][1][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 110:
                                    MATRIZ_REGIAO_INFACTIVEL[0][2][0] = MATRIZ_REGIAO_INFACTIVEL[0][2][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 130:
                                    MATRIZ_REGIAO_INFACTIVEL[0][3][0] = MATRIZ_REGIAO_INFACTIVEL[0][3][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 800:
                                    MATRIZ_REGIAO_INFACTIVEL[0][4][0] = MATRIZ_REGIAO_INFACTIVEL[0][4][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 150:
                                    MATRIZ_REGIAO_INFACTIVEL[0][5][0] = MATRIZ_REGIAO_INFACTIVEL[0][5][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 160:
                                    MATRIZ_REGIAO_INFACTIVEL[0][6][0] = MATRIZ_REGIAO_INFACTIVEL[0][6][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 980:
                                    MATRIZ_REGIAO_INFACTIVEL[0][7][0] = MATRIZ_REGIAO_INFACTIVEL[0][7][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 990:
                                    MATRIZ_REGIAO_INFACTIVEL[0][8][0] = MATRIZ_REGIAO_INFACTIVEL[0][8][0] + 1
                                                            
                                                                    
                            if GENS_EVOL[0][i][2] == 100:
                                if GENS_EVOL[aux[sorteia]][i][2] == 900:
                                    MATRIZ_REGIAO_INFACTIVEL[1][0][0] = MATRIZ_REGIAO_INFACTIVEL[1][0][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 110:
                                    MATRIZ_REGIAO_INFACTIVEL[1][2][0] = MATRIZ_REGIAO_INFACTIVEL[1][2][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 130:
                                    MATRIZ_REGIAO_INFACTIVEL[1][3][0] = MATRIZ_REGIAO_INFACTIVEL[1][3][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 800:
                                    MATRIZ_REGIAO_INFACTIVEL[1][4][0] = MATRIZ_REGIAO_INFACTIVEL[1][4][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 150:
                                    MATRIZ_REGIAO_INFACTIVEL[1][5][0] = MATRIZ_REGIAO_INFACTIVEL[1][5][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 160:
                                    MATRIZ_REGIAO_INFACTIVEL[1][6][0] = MATRIZ_REGIAO_INFACTIVEL[1][6][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 980:
                                    MATRIZ_REGIAO_INFACTIVEL[1][7][0] = MATRIZ_REGIAO_INFACTIVEL[1][7][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 990:
                                    MATRIZ_REGIAO_INFACTIVEL[1][8][0] = MATRIZ_REGIAO_INFACTIVEL[1][8][0] + 1                            
                            if GENS_EVOL[0][i][2] == 110:
                                if GENS_EVOL[aux[sorteia]][i][2] == 900:
                                    MATRIZ_REGIAO_INFACTIVEL[2][0][0] = MATRIZ_REGIAO_INFACTIVEL[2][0][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 100:
                                    MATRIZ_REGIAO_INFACTIVEL[2][1][0] = MATRIZ_REGIAO_INFACTIVEL[2][1][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 130:
                                    MATRIZ_REGIAO_INFACTIVEL[2][3][0] = MATRIZ_REGIAO_INFACTIVEL[2][3][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 800:
                                    MATRIZ_REGIAO_INFACTIVEL[2][4][0] = MATRIZ_REGIAO_INFACTIVEL[2][4][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 150:
                                    MATRIZ_REGIAO_INFACTIVEL[2][5][0] = MATRIZ_REGIAO_INFACTIVEL[2][5][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 160:
                                    MATRIZ_REGIAO_INFACTIVEL[2][6][0] = MATRIZ_REGIAO_INFACTIVEL[2][6][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 980:
                                    MATRIZ_REGIAO_INFACTIVEL[2][7][0] = MATRIZ_REGIAO_INFACTIVEL[2][7][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 990:
                                    MATRIZ_REGIAO_INFACTIVEL[2][8][0] = MATRIZ_REGIAO_INFACTIVEL[2][8][0] + 1                                    
                            if GENS_EVOL[0][i][2] == 130:
                                if GENS_EVOL[aux[sorteia]][i][2] == 900:
                                    MATRIZ_REGIAO_INFACTIVEL[3][0][0] = MATRIZ_REGIAO_INFACTIVEL[3][0][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 100:
                                    MATRIZ_REGIAO_INFACTIVEL[3][1][0] = MATRIZ_REGIAO_INFACTIVEL[3][1][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 110:
                                    MATRIZ_REGIAO_INFACTIVEL[3][2][0] = MATRIZ_REGIAO_INFACTIVEL[3][2][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 800:
                                    MATRIZ_REGIAO_INFACTIVEL[3][4][0] = MATRIZ_REGIAO_INFACTIVEL[3][4][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 150:
                                    MATRIZ_REGIAO_INFACTIVEL[3][5][0] = MATRIZ_REGIAO_INFACTIVEL[3][5][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 160:
                                    MATRIZ_REGIAO_INFACTIVEL[3][6][0] = MATRIZ_REGIAO_INFACTIVEL[3][6][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 980:
                                    MATRIZ_REGIAO_INFACTIVEL[3][7][0] = MATRIZ_REGIAO_INFACTIVEL[3][7][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 990:
                                    MATRIZ_REGIAO_INFACTIVEL[3][8][0] = MATRIZ_REGIAO_INFACTIVEL[3][8][0] + 1                                    
                            if GENS_EVOL[0][i][2] == 800:
                                if GENS_EVOL[aux[sorteia]][i][2] == 900:
                                    MATRIZ_REGIAO_INFACTIVEL[4][0][0] = MATRIZ_REGIAO_INFACTIVEL[4][0][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 100:
                                    MATRIZ_REGIAO_INFACTIVEL[4][1][0] = MATRIZ_REGIAO_INFACTIVEL[4][1][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 110:
                                    MATRIZ_REGIAO_INFACTIVEL[4][2][0] = MATRIZ_REGIAO_INFACTIVEL[4][2][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 130:
                                    MATRIZ_REGIAO_INFACTIVEL[4][3][0] = MATRIZ_REGIAO_INFACTIVEL[4][3][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 150:
                                    MATRIZ_REGIAO_INFACTIVEL[4][5][0] = MATRIZ_REGIAO_INFACTIVEL[4][5][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 160:
                                    MATRIZ_REGIAO_INFACTIVEL[4][6][0] = MATRIZ_REGIAO_INFACTIVEL[4][6][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 980:
                                    MATRIZ_REGIAO_INFACTIVEL[4][7][0] = MATRIZ_REGIAO_INFACTIVEL[4][7][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 990:
                                    MATRIZ_REGIAO_INFACTIVEL[4][8][0] = MATRIZ_REGIAO_INFACTIVEL[4][8][0] + 1

                            if GENS_EVOL[0][i][2] == 150:
                                if GENS_EVOL[aux[sorteia]][i][2] == 900:
                                    MATRIZ_REGIAO_INFACTIVEL[5][0][0] = MATRIZ_REGIAO_INFACTIVEL[5][0][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 100:
                                    MATRIZ_REGIAO_INFACTIVEL[5][1][0] = MATRIZ_REGIAO_INFACTIVEL[5][1][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 110:
                                    MATRIZ_REGIAO_INFACTIVEL[5][2][0] = MATRIZ_REGIAO_INFACTIVEL[5][2][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 130:
                                    MATRIZ_REGIAO_INFACTIVEL[5][3][0] = MATRIZ_REGIAO_INFACTIVEL[5][3][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 800:
                                    MATRIZ_REGIAO_INFACTIVEL[5][4][0] = MATRIZ_REGIAO_INFACTIVEL[5][4][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 160:
                                    MATRIZ_REGIAO_INFACTIVEL[5][6][0] = MATRIZ_REGIAO_INFACTIVEL[5][6][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 980:
                                    MATRIZ_REGIAO_INFACTIVEL[5][7][0] = MATRIZ_REGIAO_INFACTIVEL[5][7][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 990:
                                    MATRIZ_REGIAO_INFACTIVEL[5][8][0] = MATRIZ_REGIAO_INFACTIVEL[5][8][0] + 1                             
                            if GENS_EVOL[0][i][2] == 160:
                                if GENS_EVOL[aux[sorteia]][i][2] == 900:
                                    MATRIZ_REGIAO_INFACTIVEL[6][0][0] = MATRIZ_REGIAO_INFACTIVEL[6][0][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 100:
                                    MATRIZ_REGIAO_INFACTIVEL[6][1][0] = MATRIZ_REGIAO_INFACTIVEL[6][1][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 110:
                                    MATRIZ_REGIAO_INFACTIVEL[6][2][0] = MATRIZ_REGIAO_INFACTIVEL[6][2][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 130:
                                    MATRIZ_REGIAO_INFACTIVEL[6][3][0] = MATRIZ_REGIAO_INFACTIVEL[6][3][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 800:
                                    MATRIZ_REGIAO_INFACTIVEL[6][4][0] = MATRIZ_REGIAO_INFACTIVEL[6][4][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 150:
                                    MATRIZ_REGIAO_INFACTIVEL[6][5][0] = MATRIZ_REGIAO_INFACTIVEL[6][5][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 980:
                                    MATRIZ_REGIAO_INFACTIVEL[6][7][0] = MATRIZ_REGIAO_INFACTIVEL[6][7][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 990:
                                    MATRIZ_REGIAO_INFACTIVEL[6][8][0] = MATRIZ_REGIAO_INFACTIVEL[6][8][0] + 1
                            if GENS_EVOL[0][i][2] == 980:
                                if GENS_EVOL[aux[sorteia]][i][2] == 900:
                                    MATRIZ_REGIAO_INFACTIVEL[7][0][0] = MATRIZ_REGIAO_INFACTIVEL[7][0][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 100:
                                    MATRIZ_REGIAO_INFACTIVEL[7][1][0] = MATRIZ_REGIAO_INFACTIVEL[7][1][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 110:
                                    MATRIZ_REGIAO_INFACTIVEL[7][2][0] = MATRIZ_REGIAO_INFACTIVEL[7][2][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 130:
                                    MATRIZ_REGIAO_INFACTIVEL[7][3][0] = MATRIZ_REGIAO_INFACTIVEL[7][3][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 800:
                                    MATRIZ_REGIAO_INFACTIVEL[7][4][0] = MATRIZ_REGIAO_INFACTIVEL[7][4][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 150:
                                    MATRIZ_REGIAO_INFACTIVEL[7][5][0] = MATRIZ_REGIAO_INFACTIVEL[7][5][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 160:
                                    MATRIZ_REGIAO_INFACTIVEL[7][6][0] = MATRIZ_REGIAO_INFACTIVEL[7][6][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 990:
                                    MATRIZ_REGIAO_INFACTIVEL[7][8][0] = MATRIZ_REGIAO_INFACTIVEL[7][8][0] + 1
                            if GENS_EVOL[0][i][2] == 990:
                                if GENS_EVOL[aux[sorteia]][i][2] == 900:
                                    MATRIZ_REGIAO_INFACTIVEL[8][0][0] = MATRIZ_REGIAO_INFACTIVEL[8][0][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 100:
                                    MATRIZ_REGIAO_INFACTIVEL[8][1][0] = MATRIZ_REGIAO_INFACTIVEL[8][1][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 110:
                                    MATRIZ_REGIAO_INFACTIVEL[8][2][0] = MATRIZ_REGIAO_INFACTIVEL[8][2][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 130:
                                    MATRIZ_REGIAO_INFACTIVEL[8][3][0] = MATRIZ_REGIAO_INFACTIVEL[8][3][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 800:
                                    MATRIZ_REGIAO_INFACTIVEL[8][4][0] = MATRIZ_REGIAO_INFACTIVEL[8][4][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 150:
                                    MATRIZ_REGIAO_INFACTIVEL[8][5][0] = MATRIZ_REGIAO_INFACTIVEL[8][5][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 160:
                                    MATRIZ_REGIAO_INFACTIVEL[8][6][0] = MATRIZ_REGIAO_INFACTIVEL[8][6][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 980:
                                    MATRIZ_REGIAO_INFACTIVEL[8][7][0] = MATRIZ_REGIAO_INFACTIVEL[8][7][0] + 1

        GENS_EVOL[0] = copy.deepcopy(GENS_EVOL[aux[sorteia]])            

    if factivel == True:
        for i in range(ee_lambda + 1):
            PORTAS_EE[i][no-1] = copy.deepcopy(PORTAS_CKT[i])
        maior_fitness = max(fitness_soma)

        indice_maior_fitness = []
        iguais = []
        iguais_qtd_portas = []
        for i in range(len(fitness_soma)):
            if fitness_soma[i] == maior_fitness:
                indice_maior_fitness.append(i)

        for i in range(len(indice_maior_fitness)):
            iguais.append(PORTAS_EE[indice_maior_fitness[i]][no-1])
        menor_porta = min(iguais)

        for i in range(len(PORTAS_EE)):

            if PORTAS_EE[i][no-1] == menor_porta and fitness_soma[i] == maior_fitness:
                iguais_qtd_portas.append(i)

        if len(iguais_qtd_portas) > 1:
            pai = r.randint(0, len(iguais_qtd_portas)-1)
            pai = iguais_qtd_portas[pai]

        else:
            pai = iguais_qtd_portas[0]

        indice_maior_fitness.clear()
        iguais.clear()
        iguais_qtd_portas.clear()

        if PORTAS_EE[0][no-1] > PORTAS_EE[pai][no-1]:
            for i in range(len(GENS_EVOL[0])):
                if GENS_EVOL[0][i] != GENS_EVOL[pai][i]:

                    if i >= ni and i != ni+Ln and i < ni+Ln:
                        if GENS_EVOL[0][i][2] != GENS_EVOL[pai][i][2]:
                            if GENS_EVOL[0][i][2] == 900:
                                if GENS_EVOL[pai][i][2] == 100:
                                    MATRIZ_REGIAO_FACTIVEL[0][1][0] = MATRIZ_REGIAO_FACTIVEL[0][1][0] + 1
                                if GENS_EVOL[pai][i][2] == 110:
                                    MATRIZ_REGIAO_FACTIVEL[0][2][0] = MATRIZ_REGIAO_FACTIVEL[0][2][0] + 1
                                if GENS_EVOL[pai][i][2] == 130:
                                    MATRIZ_REGIAO_FACTIVEL[0][3][0] = MATRIZ_REGIAO_FACTIVEL[0][3][0] + 1
                                if GENS_EVOL[pai][i][2] == 800:
                                    MATRIZ_REGIAO_FACTIVEL[0][4][0] = MATRIZ_REGIAO_FACTIVEL[0][4][0] + 1
                                if GENS_EVOL[pai][i][2] == 150:
                                    MATRIZ_REGIAO_FACTIVEL[0][5][0] = MATRIZ_REGIAO_FACTIVEL[0][5][0] + 1
                                if GENS_EVOL[pai][i][2] == 160:
                                    MATRIZ_REGIAO_FACTIVEL[0][6][0] = MATRIZ_REGIAO_FACTIVEL[0][6][0] + 1
                                if GENS_EVOL[pai][i][2] == 980:
                                    MATRIZ_REGIAO_FACTIVEL[0][7][0] = MATRIZ_REGIAO_FACTIVEL[0][7][0] + 1
                                if GENS_EVOL[pai][i][2] == 990:
                                    MATRIZ_REGIAO_FACTIVEL[0][8][0] = MATRIZ_REGIAO_FACTIVEL[0][8][0] + 1
                                                            
                                                                    
                            if GENS_EVOL[0][i][2] == 100:
                                if GENS_EVOL[pai][i][2] == 900:
                                    MATRIZ_REGIAO_FACTIVEL[1][0][0] = MATRIZ_REGIAO_FACTIVEL[1][0][0] + 1
                                if GENS_EVOL[pai][i][2] == 110:
                                    MATRIZ_REGIAO_FACTIVEL[1][2][0] = MATRIZ_REGIAO_FACTIVEL[1][2][0] + 1
                                if GENS_EVOL[pai][i][2] == 130:
                                    MATRIZ_REGIAO_FACTIVEL[1][3][0] = MATRIZ_REGIAO_FACTIVEL[1][3][0] + 1
                                if GENS_EVOL[pai][i][2] == 800:
                                    MATRIZ_REGIAO_FACTIVEL[1][4][0] = MATRIZ_REGIAO_FACTIVEL[1][4][0] + 1
                                if GENS_EVOL[pai][i][2] == 150:
                                    MATRIZ_REGIAO_FACTIVEL[1][5][0] = MATRIZ_REGIAO_FACTIVEL[1][5][0] + 1
                                if GENS_EVOL[pai][i][2] == 160:
                                    MATRIZ_REGIAO_FACTIVEL[1][6][0] = MATRIZ_REGIAO_FACTIVEL[1][6][0] + 1
                                if GENS_EVOL[pai][i][2] == 980:
                                    MATRIZ_REGIAO_FACTIVEL[1][7][0] = MATRIZ_REGIAO_FACTIVEL[1][7][0] + 1
                                if GENS_EVOL[pai][i][2] == 990:
                                    MATRIZ_REGIAO_FACTIVEL[1][8][0] = MATRIZ_REGIAO_FACTIVEL[1][8][0] + 1                            
                            if GENS_EVOL[0][i][2] == 110:
                                if GENS_EVOL[pai][i][2] == 900:
                                    MATRIZ_REGIAO_FACTIVEL[2][0][0] = MATRIZ_REGIAO_FACTIVEL[2][0][0] + 1
                                if GENS_EVOL[pai][i][2] == 100:
                                    MATRIZ_REGIAO_FACTIVEL[2][1][0] = MATRIZ_REGIAO_FACTIVEL[2][1][0] + 1
                                if GENS_EVOL[pai][i][2] == 130:
                                    MATRIZ_REGIAO_FACTIVEL[2][3][0] = MATRIZ_REGIAO_FACTIVEL[2][3][0] + 1
                                if GENS_EVOL[pai][i][2] == 800:
                                    MATRIZ_REGIAO_FACTIVEL[2][4][0] = MATRIZ_REGIAO_FACTIVEL[2][4][0] + 1
                                if GENS_EVOL[pai][i][2] == 150:
                                    MATRIZ_REGIAO_FACTIVEL[2][5][0] = MATRIZ_REGIAO_FACTIVEL[2][5][0] + 1
                                if GENS_EVOL[pai][i][2] == 160:
                                    MATRIZ_REGIAO_FACTIVEL[2][6][0] = MATRIZ_REGIAO_FACTIVEL[2][6][0] + 1
                                if GENS_EVOL[pai][i][2] == 980:
                                    MATRIZ_REGIAO_FACTIVEL[2][7][0] = MATRIZ_REGIAO_FACTIVEL[2][7][0] + 1
                                if GENS_EVOL[pai][i][2] == 990:
                                    MATRIZ_REGIAO_FACTIVEL[2][8][0] = MATRIZ_REGIAO_FACTIVEL[2][8][0] + 1                                    
                            if GENS_EVOL[0][i][2] == 130:
                                if GENS_EVOL[pai][i][2] == 900:
                                    MATRIZ_REGIAO_FACTIVEL[3][0][0] = MATRIZ_REGIAO_FACTIVEL[3][0][0] + 1
                                if GENS_EVOL[pai][i][2] == 100:
                                    MATRIZ_REGIAO_FACTIVEL[3][1][0] = MATRIZ_REGIAO_FACTIVEL[3][1][0] + 1
                                if GENS_EVOL[pai][i][2] == 110:
                                    MATRIZ_REGIAO_FACTIVEL[3][2][0] = MATRIZ_REGIAO_FACTIVEL[3][2][0] + 1
                                if GENS_EVOL[pai][i][2] == 800:
                                    MATRIZ_REGIAO_FACTIVEL[3][4][0] = MATRIZ_REGIAO_FACTIVEL[3][4][0] + 1
                                if GENS_EVOL[pai][i][2] == 150:
                                    MATRIZ_REGIAO_FACTIVEL[3][5][0] = MATRIZ_REGIAO_FACTIVEL[3][5][0] + 1
                                if GENS_EVOL[pai][i][2] == 160:
                                    MATRIZ_REGIAO_FACTIVEL[3][6][0] = MATRIZ_REGIAO_FACTIVEL[3][6][0] + 1
                                if GENS_EVOL[pai][i][2] == 980:
                                    MATRIZ_REGIAO_FACTIVEL[3][7][0] = MATRIZ_REGIAO_FACTIVEL[3][7][0] + 1
                                if GENS_EVOL[pai][i][2] == 990:
                                    MATRIZ_REGIAO_FACTIVEL[3][8][0] = MATRIZ_REGIAO_FACTIVEL[3][8][0] + 1                                    
                            if GENS_EVOL[0][i][2] == 800:
                                if GENS_EVOL[pai][i][2] == 900:
                                    MATRIZ_REGIAO_FACTIVEL[4][0][0] = MATRIZ_REGIAO_FACTIVEL[4][0][0] + 1
                                if GENS_EVOL[pai][i][2] == 100:
                                    MATRIZ_REGIAO_FACTIVEL[4][1][0] = MATRIZ_REGIAO_FACTIVEL[4][1][0] + 1
                                if GENS_EVOL[pai][i][2] == 110:
                                    MATRIZ_REGIAO_FACTIVEL[4][2][0] = MATRIZ_REGIAO_FACTIVEL[4][2][0] + 1
                                if GENS_EVOL[pai][i][2] == 130:
                                    MATRIZ_REGIAO_FACTIVEL[4][3][0] = MATRIZ_REGIAO_FACTIVEL[4][3][0] + 1
                                if GENS_EVOL[pai][i][2] == 150:
                                    MATRIZ_REGIAO_FACTIVEL[4][5][0] = MATRIZ_REGIAO_FACTIVEL[4][5][0] + 1
                                if GENS_EVOL[pai][i][2] == 160:
                                    MATRIZ_REGIAO_FACTIVEL[4][6][0] = MATRIZ_REGIAO_FACTIVEL[4][6][0] + 1
                                if GENS_EVOL[pai][i][2] == 980:
                                    MATRIZ_REGIAO_FACTIVEL[4][7][0] = MATRIZ_REGIAO_FACTIVEL[4][7][0] + 1
                                if GENS_EVOL[pai][i][2] == 990:
                                    MATRIZ_REGIAO_FACTIVEL[4][8][0] = MATRIZ_REGIAO_FACTIVEL[4][8][0] + 1

                            if GENS_EVOL[0][i][2] == 150:
                                if GENS_EVOL[pai][i][2] == 900:
                                    MATRIZ_REGIAO_FACTIVEL[5][0][0] = MATRIZ_REGIAO_FACTIVEL[5][0][0] + 1
                                if GENS_EVOL[pai][i][2] == 100:
                                    MATRIZ_REGIAO_FACTIVEL[5][1][0] = MATRIZ_REGIAO_FACTIVEL[5][1][0] + 1
                                if GENS_EVOL[pai][i][2] == 110:
                                    MATRIZ_REGIAO_FACTIVEL[5][2][0] = MATRIZ_REGIAO_FACTIVEL[5][2][0] + 1
                                if GENS_EVOL[pai][i][2] == 130:
                                    MATRIZ_REGIAO_FACTIVEL[5][3][0] = MATRIZ_REGIAO_FACTIVEL[5][3][0] + 1
                                if GENS_EVOL[pai][i][2] == 800:
                                    MATRIZ_REGIAO_FACTIVEL[5][4][0] = MATRIZ_REGIAO_FACTIVEL[5][4][0] + 1
                                if GENS_EVOL[pai][i][2] == 160:
                                    MATRIZ_REGIAO_FACTIVEL[5][6][0] = MATRIZ_REGIAO_FACTIVEL[5][6][0] + 1
                                if GENS_EVOL[pai][i][2] == 980:
                                    MATRIZ_REGIAO_FACTIVEL[5][7][0] = MATRIZ_REGIAO_FACTIVEL[5][7][0] + 1
                                if GENS_EVOL[pai][i][2] == 990:
                                    MATRIZ_REGIAO_FACTIVEL[5][8][0] = MATRIZ_REGIAO_FACTIVEL[5][8][0] + 1                             
                            if GENS_EVOL[0][i][2] == 160:
                                if GENS_EVOL[pai][i][2] == 900:
                                    MATRIZ_REGIAO_FACTIVEL[6][0][0] = MATRIZ_REGIAO_FACTIVEL[6][0][0] + 1
                                if GENS_EVOL[pai][i][2] == 100:
                                    MATRIZ_REGIAO_FACTIVEL[6][1][0] = MATRIZ_REGIAO_FACTIVEL[6][1][0] + 1
                                if GENS_EVOL[pai][i][2] == 110:
                                    MATRIZ_REGIAO_FACTIVEL[6][2][0] = MATRIZ_REGIAO_FACTIVEL[6][2][0] + 1
                                if GENS_EVOL[pai][i][2] == 130:
                                    MATRIZ_REGIAO_FACTIVEL[6][3][0] = MATRIZ_REGIAO_FACTIVEL[6][3][0] + 1
                                if GENS_EVOL[pai][i][2] == 800:
                                    MATRIZ_REGIAO_FACTIVEL[6][4][0] = MATRIZ_REGIAO_FACTIVEL[6][4][0] + 1
                                if GENS_EVOL[pai][i][2] == 150:
                                    MATRIZ_REGIAO_FACTIVEL[6][5][0] = MATRIZ_REGIAO_FACTIVEL[6][5][0] + 1
                                if GENS_EVOL[pai][i][2] == 980:
                                    MATRIZ_REGIAO_FACTIVEL[6][7][0] = MATRIZ_REGIAO_FACTIVEL[6][7][0] + 1
                                if GENS_EVOL[pai][i][2] == 990:
                                    MATRIZ_REGIAO_FACTIVEL[6][8][0] = MATRIZ_REGIAO_FACTIVEL[6][8][0] + 1
                            if GENS_EVOL[0][i][2] == 980:
                                if GENS_EVOL[pai][i][2] == 900:
                                    MATRIZ_REGIAO_FACTIVEL[7][0][0] = MATRIZ_REGIAO_FACTIVEL[7][0][0] + 1
                                if GENS_EVOL[pai][i][2] == 100:
                                    MATRIZ_REGIAO_FACTIVEL[7][1][0] = MATRIZ_REGIAO_FACTIVEL[7][1][0] + 1
                                if GENS_EVOL[pai][i][2] == 110:
                                    MATRIZ_REGIAO_FACTIVEL[7][2][0] = MATRIZ_REGIAO_FACTIVEL[7][2][0] + 1
                                if GENS_EVOL[pai][i][2] == 130:
                                    MATRIZ_REGIAO_FACTIVEL[7][3][0] = MATRIZ_REGIAO_FACTIVEL[7][3][0] + 1
                                if GENS_EVOL[pai][i][2] == 800:
                                    MATRIZ_REGIAO_FACTIVEL[7][4][0] = MATRIZ_REGIAO_FACTIVEL[7][4][0] + 1
                                if GENS_EVOL[pai][i][2] == 150:
                                    MATRIZ_REGIAO_FACTIVEL[7][5][0] = MATRIZ_REGIAO_FACTIVEL[7][5][0] + 1
                                if GENS_EVOL[pai][i][2] == 160:
                                    MATRIZ_REGIAO_FACTIVEL[7][6][0] = MATRIZ_REGIAO_FACTIVEL[7][6][0] + 1
                                if GENS_EVOL[pai][i][2] == 990:
                                    MATRIZ_REGIAO_FACTIVEL[7][8][0] = MATRIZ_REGIAO_FACTIVEL[7][8][0] + 1
                            if GENS_EVOL[0][i][2] == 990:
                                if GENS_EVOL[pai][i][2] == 900:
                                    MATRIZ_REGIAO_FACTIVEL[8][0][0] = MATRIZ_REGIAO_FACTIVEL[8][0][0] + 1
                                if GENS_EVOL[pai][i][2] == 100:
                                    MATRIZ_REGIAO_FACTIVEL[8][1][0] = MATRIZ_REGIAO_FACTIVEL[8][1][0] + 1
                                if GENS_EVOL[pai][i][2] == 110:
                                    MATRIZ_REGIAO_FACTIVEL[8][2][0] = MATRIZ_REGIAO_FACTIVEL[8][2][0] + 1
                                if GENS_EVOL[pai][i][2] == 130:
                                    MATRIZ_REGIAO_FACTIVEL[8][3][0] = MATRIZ_REGIAO_FACTIVEL[8][3][0] + 1
                                if GENS_EVOL[pai][i][2] == 800:
                                    MATRIZ_REGIAO_FACTIVEL[8][4][0] = MATRIZ_REGIAO_FACTIVEL[8][4][0] + 1
                                if GENS_EVOL[pai][i][2] == 150:
                                    MATRIZ_REGIAO_FACTIVEL[8][5][0] = MATRIZ_REGIAO_FACTIVEL[8][5][0] + 1
                                if GENS_EVOL[pai][i][2] == 160:
                                    MATRIZ_REGIAO_FACTIVEL[8][6][0] = MATRIZ_REGIAO_FACTIVEL[8][6][0] + 1
                                if GENS_EVOL[pai][i][2] == 980:
                                    MATRIZ_REGIAO_FACTIVEL[8][7][0] = MATRIZ_REGIAO_FACTIVEL[8][7][0] + 1  
    

                    
        GENS_EVOL[0] = copy.deepcopy(GENS_EVOL[pai])


def atualiza_pai_CROSSOVER():

    fitness_saida = []
    for i in range(no):
        fitness_saida.append([])

    for i in range(no):
        for j in range(len(FITNESS_EE)):
            fitness_saida[i].append(FITNESS_EE[j][i])

    indice_melhores_saidas = []
    melhores_saidas = []

    for i in range(no):
        melhores_saidas.append([])

    for i in range(len(fitness_saida)):
        melhor = max(fitness_saida[i])
        for j in range(len(fitness_saida[i])):
            if fitness_saida[i][j] == melhor:
                melhores_saidas[i].append(j)

    todos_indices = []
    for i in range(len(melhores_saidas)):
        for j in range(len(melhores_saidas[i])):
            todos_indices.append(melhores_saidas[i][j])

    qtd_iguais = []
    for i in range(ee_lambda+1):
        qtd_iguais.append(todos_indices.count(i))

        
    maiores_melhores_saidas = []
    for i in range(len(qtd_iguais)):
        if qtd_iguais[i] == max(qtd_iguais):
            maiores_melhores_saidas.append(i)
            
    INDIVIDUO_INICIAL = maiores_melhores_saidas[r.randint(0, len(maiores_melhores_saidas)-1)]
    saidas_individuo_inicial = []
    for i in range(len(melhores_saidas)):
        if INDIVIDUO_INICIAL in melhores_saidas[i]:
            saidas_individuo_inicial.append(i)

    saidas_a_processar = []
    for i in range(no):
        if i not in saidas_individuo_inicial:
            saidas_a_processar.append(i)

    individuo_saidas_a_processar = []
    for i in range(len(saidas_a_processar)):
        individuo_saidas_a_processar.append(melhores_saidas[saidas_a_processar[i]][r.randint(0, len(melhores_saidas[saidas_a_processar[i]])-1)])
        


    for i in range(len(melhores_saidas)):
        indice = r.randint(0, len(melhores_saidas[i])-1)
        indice_melhores_saidas.append(melhores_saidas[i][indice])

    novo_individuo = []
    individuo_inicial = []



    if 1 < 0:
        variavel = 0
    else:
        novo_individuo = copy.deepcopy(GENS_EVOL[INDIVIDUO_INICIAL])
        individuo_inicial.append(INDIVIDUO_INICIAL)
        gene_disponivel = []
        for i in range(len(saidas_individuo_inicial)):
            if i == 0:
                MEUARRAY[INDIVIDUO_INICIAL][0] = copy.deepcopy(MEUARRAY[INDIVIDUO_INICIAL][saidas_individuo_inicial[i]])
            else:
                for p in range(len(MEUARRAY[INDIVIDUO_INICIAL][saidas_individuo_inicial[i]])):
                    if MEUARRAY[INDIVIDUO_INICIAL][saidas_individuo_inicial[i]][p] == "X":
                        MEUARRAY[INDIVIDUO_INICIAL][0][p] = "X"
                                                               
        for i in range(len(MEUARRAY[INDIVIDUO_INICIAL][0])):
            if MEUARRAY[INDIVIDUO_INICIAL][0][i] != "X" and i >= ni and i < Ln+ni:
                gene_disponivel.append(i)
        for k in range(len(saidas_a_processar)):
            saida_atual = saidas_a_processar[k]
            individuo_saida_atual = individuo_saidas_a_processar[k]
            copia_individuo_atual = copy.deepcopy(GENS_EVOL[individuo_saidas_a_processar[k]])

            if 1 < 0:

                novo_individuo = copy.deepcopy(GENS_EVOL[individuo_saida_atual])
                individuo_inicial.append(indice_melhores_saidas[k])
                
                gene_disponivel = []
                for i in range(len(MEUARRAY[individuo_inicial[0]][0])):
                    if MEUARRAY[individuo_inicial[0]][0][i] != "X" and i >= ni and i < Ln+ni:
                        gene_disponivel.append(i)
            else:



                genes_saida_atual = []
                indice_genes_saida_atual = []
                for i in range(len(MEUARRAY[individuo_saida_atual][saida_atual])):
                    if MEUARRAY[individuo_saida_atual][saida_atual][i] == "X" and i >= ni and i < Ln+ni:
                        genes_saida_atual.append(GENS_EVOL[individuo_saida_atual][i])
                        indice_genes_saida_atual.append(i)


                saida_corrigir_um = []
                if len(genes_saida_atual) != 0:
                    saida_corrigir_um.append(genes_saida_atual[len(genes_saida_atual)-1])
                else:
                    saida_corrigir_um.append(GENS_EVOL[individuo_saida_atual][Ln+ni+saida_atual])




                while len(genes_saida_atual) != 0:
                    for i in range(len(indice_genes_saida_atual)):

                        gene_analisando = []
                        if len(genes_saida_atual) > 0:
                            for m in range(len(genes_saida_atual[0])):

                                gene_analisando.append(int(genes_saida_atual[0][m]))

                            if len(gene_disponivel) == 0:
                                print("--------------------------------------- FALTA ESPAÇO --------------------------------------")
                                exit()


                        if novo_individuo[indice_genes_saida_atual[i]] == gene_analisando:

                            genes_saida_atual.pop(0)
                            for v in range(len(gene_disponivel)):
                                if gene_disponivel[v] == indice_genes_saida_atual[i]:
                                    gene_disponivel.pop(v)
                                    break

                        else:

                            for j in range(len(gene_disponivel)):
                                if gene_disponivel[j] >= indice_genes_saida_atual[i]:

                                    novo_individuo[gene_disponivel[j]] = copy.deepcopy(gene_analisando)



                                    novo_indice = gene_disponivel[j]
                                    genes_saida_atual.pop(0)

                                    gene_disponivel.pop(j)
                                    break
                            for w in range(len(genes_saida_atual)):
                                valor_procurado = indice_genes_saida_atual[i]
                                if genes_saida_atual[w][0] == valor_procurado:
                                    genes_saida_atual[w][0] = copy.deepcopy(str(novo_indice))
                                if genes_saida_atual[w][1] == valor_procurado:
                                    genes_saida_atual[w][1] = copy.deepcopy(str(novo_indice))
                                if saida_corrigir_um[0][0] == valor_procurado:
                                    saida_corrigir_um[0][0] = copy.deepcopy(str(novo_indice))
                                if saida_corrigir_um[0][1] == valor_procurado:
                                    saida_corrigir_um[0][1] = copy.deepcopy(str(novo_indice))
                            

                saida_corrigir = []
                if type(saida_corrigir_um[0]) == list:
                    for m in range(len(saida_corrigir_um[0])):
                        saida_corrigir_um[0][m] = int(saida_corrigir_um[0][m])
                    
                    for i in range(len(novo_individuo)):
                        if novo_individuo[i] == saida_corrigir_um[0]:
                            saida_corrigir.append(i)
                else:
                    saida_corrigir.append(int(saida_corrigir_um[0]))
                    

                    
                        
                novo_individuo[ni+Ln+saida_atual] = copy.deepcopy(saida_corrigir[0])

            GENS_EVOL[individuo_saidas_a_processar[k]] = copy.deepcopy(copia_individuo_atual)

        array = copy.deepcopy(novo_individuo)
        anteriores = []
        for i in range(len(array)):
            atual = array[i]
            for j in range(len(array)):
                if array[j] == atual and i!=j:
                    novo_indice = i
                    indice_anterior = j
                    anteriores.append(indice_anterior)
                    for k in range(len(array)):
                        if k >= ni and k < Ln+ni and k > novo_indice and (novo_indice not in anteriores):
                            #print("k", k)
                            #print("array[k]", array[k])
                            if array[k][0] == indice_anterior:
                                array[k][0] = novo_indice
                            if array[k][1] == indice_anterior:
                                array[k][1] = novo_indice
        novo_individuo = copy.deepcopy(array)

        GENS_EVOL_ANTERIOR = []
        GENS_EVOL_ANTERIOR = copy.deepcopy(GENS_EVOL)        
        GENS_EVOL[0] = copy.deepcopy(novo_individuo)

        SAM_ativo(0)
        #nos_ativos()                                                         
        FITNESS_EE_ANTERIOR = []
        FITNESS_EE_ANTERIOR = copy.deepcopy(FITNESS_EE)

        acumulador = 0
        for i in range(len(FITNESS_EE[0])):
            acumulador = acumulador + FITNESS_EE[0][i]
        melhor_antes = acumulador

        monta_tabela(0)
        acumulador2 = 0
        for i in range(len(FITNESS_EE[0])):
            acumulador2 = acumulador2 + FITNESS_EE[0][i]
        melhor_depois = acumulador2
        #print("melhor depois", melhor_depois)
        if melhor_depois < melhor_antes:
            print("AQUI")







def atualiza_pai_WIRES_FACTIVEL(): #Quando o circuito atende a tabela verdade, conta-se o número de portas
    soma = 0
    fitness_soma = [] # Array que armazena a soma de todas as saídas de cada circuito
    aux = []
    factivel = False 
    for k in range(ee_lambda + 1):
        fitness_soma.append([])
    for i in range(len(FITNESS_EE)):
        for j in range(len(FITNESS_EE[i])):
            soma = soma + FITNESS_EE[i][j]
        fitness_soma[i] = copy.deepcopy(soma)
        soma = 0

    for i in range(len(fitness_soma)):
        if fitness_soma[i] == (no * len(OUTPUTS[0])):
            factivel = True
 

    if factivel == False:
        for s in range(len(fitness_soma)):
            if fitness_soma[s] == max(fitness_soma):
                aux.append(s)
        sorteia = r.randint(0, len(aux)-1)

        if fitness_soma[0] < fitness_soma[aux[sorteia]]:
            for i in range(len(GENS_EVOL[0])):
                if GENS_EVOL[0][i] != GENS_EVOL[aux[sorteia]][i]:

                    if i >= ni and i < ni+Ln:
                        if GENS_EVOL[0][i][2] != GENS_EVOL[aux[sorteia]][i][2]:
                            if GENS_EVOL[0][i][2] == 900:
                                if GENS_EVOL[aux[sorteia]][i][2] == 100:
                                    MATRIZ_REGIAO_INFACTIVEL[0][1][0] = MATRIZ_REGIAO_INFACTIVEL[0][1][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 110:
                                    MATRIZ_REGIAO_INFACTIVEL[0][2][0] = MATRIZ_REGIAO_INFACTIVEL[0][2][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 130:
                                    MATRIZ_REGIAO_INFACTIVEL[0][3][0] = MATRIZ_REGIAO_INFACTIVEL[0][3][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 800:
                                    MATRIZ_REGIAO_INFACTIVEL[0][4][0] = MATRIZ_REGIAO_INFACTIVEL[0][4][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 150:
                                    MATRIZ_REGIAO_INFACTIVEL[0][5][0] = MATRIZ_REGIAO_INFACTIVEL[0][5][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 160:
                                    MATRIZ_REGIAO_INFACTIVEL[0][6][0] = MATRIZ_REGIAO_INFACTIVEL[0][6][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 980:
                                    MATRIZ_REGIAO_INFACTIVEL[0][7][0] = MATRIZ_REGIAO_INFACTIVEL[0][7][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 990:
                                    MATRIZ_REGIAO_INFACTIVEL[0][8][0] = MATRIZ_REGIAO_INFACTIVEL[0][8][0] + 1
                                                            
                                                                    
                            if GENS_EVOL[0][i][2] == 100:
                                if GENS_EVOL[aux[sorteia]][i][2] == 900:
                                    MATRIZ_REGIAO_INFACTIVEL[1][0][0] = MATRIZ_REGIAO_INFACTIVEL[1][0][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 110:
                                    MATRIZ_REGIAO_INFACTIVEL[1][2][0] = MATRIZ_REGIAO_INFACTIVEL[1][2][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 130:
                                    MATRIZ_REGIAO_INFACTIVEL[1][3][0] = MATRIZ_REGIAO_INFACTIVEL[1][3][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 800:
                                    MATRIZ_REGIAO_INFACTIVEL[1][4][0] = MATRIZ_REGIAO_INFACTIVEL[1][4][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 150:
                                    MATRIZ_REGIAO_INFACTIVEL[1][5][0] = MATRIZ_REGIAO_INFACTIVEL[1][5][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 160:
                                    MATRIZ_REGIAO_INFACTIVEL[1][6][0] = MATRIZ_REGIAO_INFACTIVEL[1][6][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 980:
                                    MATRIZ_REGIAO_INFACTIVEL[1][7][0] = MATRIZ_REGIAO_INFACTIVEL[1][7][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 990:
                                    MATRIZ_REGIAO_INFACTIVEL[1][8][0] = MATRIZ_REGIAO_INFACTIVEL[1][8][0] + 1                            
                            if GENS_EVOL[0][i][2] == 110:
                                if GENS_EVOL[aux[sorteia]][i][2] == 900:
                                    MATRIZ_REGIAO_INFACTIVEL[2][0][0] = MATRIZ_REGIAO_INFACTIVEL[2][0][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 100:
                                    MATRIZ_REGIAO_INFACTIVEL[2][1][0] = MATRIZ_REGIAO_INFACTIVEL[2][1][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 130:
                                    MATRIZ_REGIAO_INFACTIVEL[2][3][0] = MATRIZ_REGIAO_INFACTIVEL[2][3][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 800:
                                    MATRIZ_REGIAO_INFACTIVEL[2][4][0] = MATRIZ_REGIAO_INFACTIVEL[2][4][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 150:
                                    MATRIZ_REGIAO_INFACTIVEL[2][5][0] = MATRIZ_REGIAO_INFACTIVEL[2][5][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 160:
                                    MATRIZ_REGIAO_INFACTIVEL[2][6][0] = MATRIZ_REGIAO_INFACTIVEL[2][6][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 980:
                                    MATRIZ_REGIAO_INFACTIVEL[2][7][0] = MATRIZ_REGIAO_INFACTIVEL[2][7][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 990:
                                    MATRIZ_REGIAO_INFACTIVEL[2][8][0] = MATRIZ_REGIAO_INFACTIVEL[2][8][0] + 1                                    
                            if GENS_EVOL[0][i][2] == 130:
                                if GENS_EVOL[aux[sorteia]][i][2] == 900:
                                    MATRIZ_REGIAO_INFACTIVEL[3][0][0] = MATRIZ_REGIAO_INFACTIVEL[3][0][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 100:
                                    MATRIZ_REGIAO_INFACTIVEL[3][1][0] = MATRIZ_REGIAO_INFACTIVEL[3][1][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 110:
                                    MATRIZ_REGIAO_INFACTIVEL[3][2][0] = MATRIZ_REGIAO_INFACTIVEL[3][2][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 800:
                                    MATRIZ_REGIAO_INFACTIVEL[3][4][0] = MATRIZ_REGIAO_INFACTIVEL[3][4][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 150:
                                    MATRIZ_REGIAO_INFACTIVEL[3][5][0] = MATRIZ_REGIAO_INFACTIVEL[3][5][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 160:
                                    MATRIZ_REGIAO_INFACTIVEL[3][6][0] = MATRIZ_REGIAO_INFACTIVEL[3][6][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 980:
                                    MATRIZ_REGIAO_INFACTIVEL[3][7][0] = MATRIZ_REGIAO_INFACTIVEL[3][7][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 990:
                                    MATRIZ_REGIAO_INFACTIVEL[3][8][0] = MATRIZ_REGIAO_INFACTIVEL[3][8][0] + 1                                    
                            if GENS_EVOL[0][i][2] == 800:
                                if GENS_EVOL[aux[sorteia]][i][2] == 900:
                                    MATRIZ_REGIAO_INFACTIVEL[4][0][0] = MATRIZ_REGIAO_INFACTIVEL[4][0][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 100:
                                    MATRIZ_REGIAO_INFACTIVEL[4][1][0] = MATRIZ_REGIAO_INFACTIVEL[4][1][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 110:
                                    MATRIZ_REGIAO_INFACTIVEL[4][2][0] = MATRIZ_REGIAO_INFACTIVEL[4][2][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 130:
                                    MATRIZ_REGIAO_INFACTIVEL[4][3][0] = MATRIZ_REGIAO_INFACTIVEL[4][3][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 150:
                                    MATRIZ_REGIAO_INFACTIVEL[4][5][0] = MATRIZ_REGIAO_INFACTIVEL[4][5][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 160:
                                    MATRIZ_REGIAO_INFACTIVEL[4][6][0] = MATRIZ_REGIAO_INFACTIVEL[4][6][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 980:
                                    MATRIZ_REGIAO_INFACTIVEL[4][7][0] = MATRIZ_REGIAO_INFACTIVEL[4][7][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 990:
                                    MATRIZ_REGIAO_INFACTIVEL[4][8][0] = MATRIZ_REGIAO_INFACTIVEL[4][8][0] + 1

                            if GENS_EVOL[0][i][2] == 150:
                                if GENS_EVOL[aux[sorteia]][i][2] == 900:
                                    MATRIZ_REGIAO_INFACTIVEL[5][0][0] = MATRIZ_REGIAO_INFACTIVEL[5][0][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 100:
                                    MATRIZ_REGIAO_INFACTIVEL[5][1][0] = MATRIZ_REGIAO_INFACTIVEL[5][1][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 110:
                                    MATRIZ_REGIAO_INFACTIVEL[5][2][0] = MATRIZ_REGIAO_INFACTIVEL[5][2][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 130:
                                    MATRIZ_REGIAO_INFACTIVEL[5][3][0] = MATRIZ_REGIAO_INFACTIVEL[5][3][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 800:
                                    MATRIZ_REGIAO_INFACTIVEL[5][4][0] = MATRIZ_REGIAO_INFACTIVEL[5][4][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 160:
                                    MATRIZ_REGIAO_INFACTIVEL[5][6][0] = MATRIZ_REGIAO_INFACTIVEL[5][6][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 980:
                                    MATRIZ_REGIAO_INFACTIVEL[5][7][0] = MATRIZ_REGIAO_INFACTIVEL[5][7][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 990:
                                    MATRIZ_REGIAO_INFACTIVEL[5][8][0] = MATRIZ_REGIAO_INFACTIVEL[5][8][0] + 1                             
                            if GENS_EVOL[0][i][2] == 160:
                                if GENS_EVOL[aux[sorteia]][i][2] == 900:
                                    MATRIZ_REGIAO_INFACTIVEL[6][0][0] = MATRIZ_REGIAO_INFACTIVEL[6][0][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 100:
                                    MATRIZ_REGIAO_INFACTIVEL[6][1][0] = MATRIZ_REGIAO_INFACTIVEL[6][1][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 110:
                                    MATRIZ_REGIAO_INFACTIVEL[6][2][0] = MATRIZ_REGIAO_INFACTIVEL[6][2][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 130:
                                    MATRIZ_REGIAO_INFACTIVEL[6][3][0] = MATRIZ_REGIAO_INFACTIVEL[6][3][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 800:
                                    MATRIZ_REGIAO_INFACTIVEL[6][4][0] = MATRIZ_REGIAO_INFACTIVEL[6][4][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 150:
                                    MATRIZ_REGIAO_INFACTIVEL[6][5][0] = MATRIZ_REGIAO_INFACTIVEL[6][5][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 980:
                                    MATRIZ_REGIAO_INFACTIVEL[6][7][0] = MATRIZ_REGIAO_INFACTIVEL[6][7][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 990:
                                    MATRIZ_REGIAO_INFACTIVEL[6][8][0] = MATRIZ_REGIAO_INFACTIVEL[6][8][0] + 1
                            if GENS_EVOL[0][i][2] == 980:
                                if GENS_EVOL[aux[sorteia]][i][2] == 900:
                                    MATRIZ_REGIAO_INFACTIVEL[7][0][0] = MATRIZ_REGIAO_INFACTIVEL[7][0][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 100:
                                    MATRIZ_REGIAO_INFACTIVEL[7][1][0] = MATRIZ_REGIAO_INFACTIVEL[7][1][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 110:
                                    MATRIZ_REGIAO_INFACTIVEL[7][2][0] = MATRIZ_REGIAO_INFACTIVEL[7][2][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 130:
                                    MATRIZ_REGIAO_INFACTIVEL[7][3][0] = MATRIZ_REGIAO_INFACTIVEL[7][3][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 800:
                                    MATRIZ_REGIAO_INFACTIVEL[7][4][0] = MATRIZ_REGIAO_INFACTIVEL[7][4][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 150:
                                    MATRIZ_REGIAO_INFACTIVEL[7][5][0] = MATRIZ_REGIAO_INFACTIVEL[7][5][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 160:
                                    MATRIZ_REGIAO_INFACTIVEL[7][6][0] = MATRIZ_REGIAO_INFACTIVEL[7][6][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 990:
                                    MATRIZ_REGIAO_INFACTIVEL[7][8][0] = MATRIZ_REGIAO_INFACTIVEL[7][8][0] + 1
                            if GENS_EVOL[0][i][2] == 990:
                                if GENS_EVOL[aux[sorteia]][i][2] == 900:
                                    MATRIZ_REGIAO_INFACTIVEL[8][0][0] = MATRIZ_REGIAO_INFACTIVEL[8][0][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 100:
                                    MATRIZ_REGIAO_INFACTIVEL[8][1][0] = MATRIZ_REGIAO_INFACTIVEL[8][1][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 110:
                                    MATRIZ_REGIAO_INFACTIVEL[8][2][0] = MATRIZ_REGIAO_INFACTIVEL[8][2][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 130:
                                    MATRIZ_REGIAO_INFACTIVEL[8][3][0] = MATRIZ_REGIAO_INFACTIVEL[8][3][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 800:
                                    MATRIZ_REGIAO_INFACTIVEL[8][4][0] = MATRIZ_REGIAO_INFACTIVEL[8][4][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 150:
                                    MATRIZ_REGIAO_INFACTIVEL[8][5][0] = MATRIZ_REGIAO_INFACTIVEL[8][5][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 160:
                                    MATRIZ_REGIAO_INFACTIVEL[8][6][0] = MATRIZ_REGIAO_INFACTIVEL[8][6][0] + 1
                                if GENS_EVOL[aux[sorteia]][i][2] == 980:
                                    MATRIZ_REGIAO_INFACTIVEL[8][7][0] = MATRIZ_REGIAO_INFACTIVEL[8][7][0] + 1                                     
        GENS_EVOL[0] = copy.deepcopy(GENS_EVOL[aux[sorteia]])            

    if factivel == True:
        WIRES_LOCAL = []
        for i in range(ee_lambda + 1):
            WIRES_LOCAL.append([])
            for j in range(1):
                WIRES_LOCAL[i].append([])
                WIRES_LOCAL[i][0] = copy.deepcopy(WIRES_CKT[i])
        maior_fitness = max(fitness_soma)

        indice_maior_fitness = []
        iguais = []
        iguais_qtd_portas = []
        for i in range(len(fitness_soma)):
            if fitness_soma[i] == maior_fitness:
                indice_maior_fitness.append(i)

        for i in range(len(indice_maior_fitness)):
            iguais.append(WIRES_LOCAL[indice_maior_fitness[i]][0])
        maior_wires = max(iguais)

        for i in range(len(WIRES_LOCAL)):

            if WIRES_LOCAL[i][0] == maior_wires and fitness_soma[i] == maior_fitness:
                iguais_qtd_portas.append(i)

        if len(iguais_qtd_portas) > 1:
            pai = r.randint(0, len(iguais_qtd_portas)-1)
            pai = iguais_qtd_portas[pai]

        else:
            pai = iguais_qtd_portas[0]

        indice_maior_fitness.clear()
        iguais.clear()
        iguais_qtd_portas.clear()

        if WIRES_LOCAL[0][0] < WIRES_LOCAL[pai][0]:
            for i in range(len(GENS_EVOL[0])):
                if GENS_EVOL[0][i] != GENS_EVOL[pai][i]:

                    if i >= ni and i != ni+Ln and i < ni+Ln:
                        if GENS_EVOL[0][i][2] != GENS_EVOL[pai][i][2]:
                            if GENS_EVOL[0][i][2] == 900:
                                if GENS_EVOL[pai][i][2] == 100:
                                    MATRIZ_REGIAO_FACTIVEL[0][1][0] = MATRIZ_REGIAO_FACTIVEL[0][1][0] + 1
                                if GENS_EVOL[pai][i][2] == 110:
                                    MATRIZ_REGIAO_FACTIVEL[0][2][0] = MATRIZ_REGIAO_FACTIVEL[0][2][0] + 1
                                if GENS_EVOL[pai][i][2] == 130:
                                    MATRIZ_REGIAO_FACTIVEL[0][3][0] = MATRIZ_REGIAO_FACTIVEL[0][3][0] + 1
                                if GENS_EVOL[pai][i][2] == 800:
                                    MATRIZ_REGIAO_FACTIVEL[0][4][0] = MATRIZ_REGIAO_FACTIVEL[0][4][0] + 1
                                if GENS_EVOL[pai][i][2] == 150:
                                    MATRIZ_REGIAO_FACTIVEL[0][5][0] = MATRIZ_REGIAO_FACTIVEL[0][5][0] + 1
                                if GENS_EVOL[pai][i][2] == 160:
                                    MATRIZ_REGIAO_FACTIVEL[0][6][0] = MATRIZ_REGIAO_FACTIVEL[0][6][0] + 1
                                if GENS_EVOL[pai][i][2] == 980:
                                    MATRIZ_REGIAO_FACTIVEL[0][7][0] = MATRIZ_REGIAO_FACTIVEL[0][7][0] + 1
                                if GENS_EVOL[pai][i][2] == 990:
                                    MATRIZ_REGIAO_FACTIVEL[0][8][0] = MATRIZ_REGIAO_FACTIVEL[0][8][0] + 1
                                                            
                                                                    
                            if GENS_EVOL[0][i][2] == 100:
                                if GENS_EVOL[pai][i][2] == 900:
                                    MATRIZ_REGIAO_FACTIVEL[1][0][0] = MATRIZ_REGIAO_FACTIVEL[1][0][0] + 1
                                if GENS_EVOL[pai][i][2] == 110:
                                    MATRIZ_REGIAO_FACTIVEL[1][2][0] = MATRIZ_REGIAO_FACTIVEL[1][2][0] + 1
                                if GENS_EVOL[pai][i][2] == 130:
                                    MATRIZ_REGIAO_FACTIVEL[1][3][0] = MATRIZ_REGIAO_FACTIVEL[1][3][0] + 1
                                if GENS_EVOL[pai][i][2] == 800:
                                    MATRIZ_REGIAO_FACTIVEL[1][4][0] = MATRIZ_REGIAO_FACTIVEL[1][4][0] + 1
                                if GENS_EVOL[pai][i][2] == 150:
                                    MATRIZ_REGIAO_FACTIVEL[1][5][0] = MATRIZ_REGIAO_FACTIVEL[1][5][0] + 1
                                if GENS_EVOL[pai][i][2] == 160:
                                    MATRIZ_REGIAO_FACTIVEL[1][6][0] = MATRIZ_REGIAO_FACTIVEL[1][6][0] + 1
                                if GENS_EVOL[pai][i][2] == 980:
                                    MATRIZ_REGIAO_FACTIVEL[1][7][0] = MATRIZ_REGIAO_FACTIVEL[1][7][0] + 1
                                if GENS_EVOL[pai][i][2] == 990:
                                    MATRIZ_REGIAO_FACTIVEL[1][8][0] = MATRIZ_REGIAO_FACTIVEL[1][8][0] + 1                            
                            if GENS_EVOL[0][i][2] == 110:
                                if GENS_EVOL[pai][i][2] == 900:
                                    MATRIZ_REGIAO_FACTIVEL[2][0][0] = MATRIZ_REGIAO_FACTIVEL[2][0][0] + 1
                                if GENS_EVOL[pai][i][2] == 100:
                                    MATRIZ_REGIAO_FACTIVEL[2][1][0] = MATRIZ_REGIAO_FACTIVEL[2][1][0] + 1
                                if GENS_EVOL[pai][i][2] == 130:
                                    MATRIZ_REGIAO_FACTIVEL[2][3][0] = MATRIZ_REGIAO_FACTIVEL[2][3][0] + 1
                                if GENS_EVOL[pai][i][2] == 800:
                                    MATRIZ_REGIAO_FACTIVEL[2][4][0] = MATRIZ_REGIAO_FACTIVEL[2][4][0] + 1
                                if GENS_EVOL[pai][i][2] == 150:
                                    MATRIZ_REGIAO_FACTIVEL[2][5][0] = MATRIZ_REGIAO_FACTIVEL[2][5][0] + 1
                                if GENS_EVOL[pai][i][2] == 160:
                                    MATRIZ_REGIAO_FACTIVEL[2][6][0] = MATRIZ_REGIAO_FACTIVEL[2][6][0] + 1
                                if GENS_EVOL[pai][i][2] == 980:
                                    MATRIZ_REGIAO_FACTIVEL[2][7][0] = MATRIZ_REGIAO_FACTIVEL[2][7][0] + 1
                                if GENS_EVOL[pai][i][2] == 990:
                                    MATRIZ_REGIAO_FACTIVEL[2][8][0] = MATRIZ_REGIAO_FACTIVEL[2][8][0] + 1                                    
                            if GENS_EVOL[0][i][2] == 130:
                                if GENS_EVOL[pai][i][2] == 900:
                                    MATRIZ_REGIAO_FACTIVEL[3][0][0] = MATRIZ_REGIAO_FACTIVEL[3][0][0] + 1
                                if GENS_EVOL[pai][i][2] == 100:
                                    MATRIZ_REGIAO_FACTIVEL[3][1][0] = MATRIZ_REGIAO_FACTIVEL[3][1][0] + 1
                                if GENS_EVOL[pai][i][2] == 110:
                                    MATRIZ_REGIAO_FACTIVEL[3][2][0] = MATRIZ_REGIAO_FACTIVEL[3][2][0] + 1
                                if GENS_EVOL[pai][i][2] == 800:
                                    MATRIZ_REGIAO_FACTIVEL[3][4][0] = MATRIZ_REGIAO_FACTIVEL[3][4][0] + 1
                                if GENS_EVOL[pai][i][2] == 150:
                                    MATRIZ_REGIAO_FACTIVEL[3][5][0] = MATRIZ_REGIAO_FACTIVEL[3][5][0] + 1
                                if GENS_EVOL[pai][i][2] == 160:
                                    MATRIZ_REGIAO_FACTIVEL[3][6][0] = MATRIZ_REGIAO_FACTIVEL[3][6][0] + 1
                                if GENS_EVOL[pai][i][2] == 980:
                                    MATRIZ_REGIAO_FACTIVEL[3][7][0] = MATRIZ_REGIAO_FACTIVEL[3][7][0] + 1
                                if GENS_EVOL[pai][i][2] == 990:
                                    MATRIZ_REGIAO_FACTIVEL[3][8][0] = MATRIZ_REGIAO_FACTIVEL[3][8][0] + 1                                    
                            if GENS_EVOL[0][i][2] == 800:
                                if GENS_EVOL[pai][i][2] == 900:
                                    MATRIZ_REGIAO_FACTIVEL[4][0][0] = MATRIZ_REGIAO_FACTIVEL[4][0][0] + 1
                                if GENS_EVOL[pai][i][2] == 100:
                                    MATRIZ_REGIAO_FACTIVEL[4][1][0] = MATRIZ_REGIAO_FACTIVEL[4][1][0] + 1
                                if GENS_EVOL[pai][i][2] == 110:
                                    MATRIZ_REGIAO_FACTIVEL[4][2][0] = MATRIZ_REGIAO_FACTIVEL[4][2][0] + 1
                                if GENS_EVOL[pai][i][2] == 130:
                                    MATRIZ_REGIAO_FACTIVEL[4][3][0] = MATRIZ_REGIAO_FACTIVEL[4][3][0] + 1
                                if GENS_EVOL[pai][i][2] == 150:
                                    MATRIZ_REGIAO_FACTIVEL[4][5][0] = MATRIZ_REGIAO_FACTIVEL[4][5][0] + 1
                                if GENS_EVOL[pai][i][2] == 160:
                                    MATRIZ_REGIAO_FACTIVEL[4][6][0] = MATRIZ_REGIAO_FACTIVEL[4][6][0] + 1
                                if GENS_EVOL[pai][i][2] == 980:
                                    MATRIZ_REGIAO_FACTIVEL[4][7][0] = MATRIZ_REGIAO_FACTIVEL[4][7][0] + 1
                                if GENS_EVOL[pai][i][2] == 990:
                                    MATRIZ_REGIAO_FACTIVEL[4][8][0] = MATRIZ_REGIAO_FACTIVEL[4][8][0] + 1

                            if GENS_EVOL[0][i][2] == 150:
                                if GENS_EVOL[pai][i][2] == 900:
                                    MATRIZ_REGIAO_FACTIVEL[5][0][0] = MATRIZ_REGIAO_FACTIVEL[5][0][0] + 1
                                if GENS_EVOL[pai][i][2] == 100:
                                    MATRIZ_REGIAO_FACTIVEL[5][1][0] = MATRIZ_REGIAO_FACTIVEL[5][1][0] + 1
                                if GENS_EVOL[pai][i][2] == 110:
                                    MATRIZ_REGIAO_FACTIVEL[5][2][0] = MATRIZ_REGIAO_FACTIVEL[5][2][0] + 1
                                if GENS_EVOL[pai][i][2] == 130:
                                    MATRIZ_REGIAO_FACTIVEL[5][3][0] = MATRIZ_REGIAO_FACTIVEL[5][3][0] + 1
                                if GENS_EVOL[pai][i][2] == 800:
                                    MATRIZ_REGIAO_FACTIVEL[5][4][0] = MATRIZ_REGIAO_FACTIVEL[5][4][0] + 1
                                if GENS_EVOL[pai][i][2] == 160:
                                    MATRIZ_REGIAO_FACTIVEL[5][6][0] = MATRIZ_REGIAO_FACTIVEL[5][6][0] + 1
                                if GENS_EVOL[pai][i][2] == 980:
                                    MATRIZ_REGIAO_FACTIVEL[5][7][0] = MATRIZ_REGIAO_FACTIVEL[5][7][0] + 1
                                if GENS_EVOL[pai][i][2] == 990:
                                    MATRIZ_REGIAO_FACTIVEL[5][8][0] = MATRIZ_REGIAO_FACTIVEL[5][8][0] + 1                             
                            if GENS_EVOL[0][i][2] == 160:
                                if GENS_EVOL[pai][i][2] == 900:
                                    MATRIZ_REGIAO_FACTIVEL[6][0][0] = MATRIZ_REGIAO_FACTIVEL[6][0][0] + 1
                                if GENS_EVOL[pai][i][2] == 100:
                                    MATRIZ_REGIAO_FACTIVEL[6][1][0] = MATRIZ_REGIAO_FACTIVEL[6][1][0] + 1
                                if GENS_EVOL[pai][i][2] == 110:
                                    MATRIZ_REGIAO_FACTIVEL[6][2][0] = MATRIZ_REGIAO_FACTIVEL[6][2][0] + 1
                                if GENS_EVOL[pai][i][2] == 130:
                                    MATRIZ_REGIAO_FACTIVEL[6][3][0] = MATRIZ_REGIAO_FACTIVEL[6][3][0] + 1
                                if GENS_EVOL[pai][i][2] == 800:
                                    MATRIZ_REGIAO_FACTIVEL[6][4][0] = MATRIZ_REGIAO_FACTIVEL[6][4][0] + 1
                                if GENS_EVOL[pai][i][2] == 150:
                                    MATRIZ_REGIAO_FACTIVEL[6][5][0] = MATRIZ_REGIAO_FACTIVEL[6][5][0] + 1
                                if GENS_EVOL[pai][i][2] == 980:
                                    MATRIZ_REGIAO_FACTIVEL[6][7][0] = MATRIZ_REGIAO_FACTIVEL[6][7][0] + 1
                                if GENS_EVOL[pai][i][2] == 990:
                                    MATRIZ_REGIAO_FACTIVEL[6][8][0] = MATRIZ_REGIAO_FACTIVEL[6][8][0] + 1
                            if GENS_EVOL[0][i][2] == 980:
                                if GENS_EVOL[pai][i][2] == 900:
                                    MATRIZ_REGIAO_FACTIVEL[7][0][0] = MATRIZ_REGIAO_FACTIVEL[7][0][0] + 1
                                if GENS_EVOL[pai][i][2] == 100:
                                    MATRIZ_REGIAO_FACTIVEL[7][1][0] = MATRIZ_REGIAO_FACTIVEL[7][1][0] + 1
                                if GENS_EVOL[pai][i][2] == 110:
                                    MATRIZ_REGIAO_FACTIVEL[7][2][0] = MATRIZ_REGIAO_FACTIVEL[7][2][0] + 1
                                if GENS_EVOL[pai][i][2] == 130:
                                    MATRIZ_REGIAO_FACTIVEL[7][3][0] = MATRIZ_REGIAO_FACTIVEL[7][3][0] + 1
                                if GENS_EVOL[pai][i][2] == 800:
                                    MATRIZ_REGIAO_FACTIVEL[7][4][0] = MATRIZ_REGIAO_FACTIVEL[7][4][0] + 1
                                if GENS_EVOL[pai][i][2] == 150:
                                    MATRIZ_REGIAO_FACTIVEL[7][5][0] = MATRIZ_REGIAO_FACTIVEL[7][5][0] + 1
                                if GENS_EVOL[pai][i][2] == 160:
                                    MATRIZ_REGIAO_FACTIVEL[7][6][0] = MATRIZ_REGIAO_FACTIVEL[7][6][0] + 1
                                if GENS_EVOL[pai][i][2] == 990:
                                    MATRIZ_REGIAO_FACTIVEL[7][8][0] = MATRIZ_REGIAO_FACTIVEL[7][8][0] + 1
                            if GENS_EVOL[0][i][2] == 990:
                                if GENS_EVOL[pai][i][2] == 900:
                                    MATRIZ_REGIAO_FACTIVEL[8][0][0] = MATRIZ_REGIAO_FACTIVEL[8][0][0] + 1
                                if GENS_EVOL[pai][i][2] == 100:
                                    MATRIZ_REGIAO_FACTIVEL[8][1][0] = MATRIZ_REGIAO_FACTIVEL[8][1][0] + 1
                                if GENS_EVOL[pai][i][2] == 110:
                                    MATRIZ_REGIAO_FACTIVEL[8][2][0] = MATRIZ_REGIAO_FACTIVEL[8][2][0] + 1
                                if GENS_EVOL[pai][i][2] == 130:
                                    MATRIZ_REGIAO_FACTIVEL[8][3][0] = MATRIZ_REGIAO_FACTIVEL[8][3][0] + 1
                                if GENS_EVOL[pai][i][2] == 800:
                                    MATRIZ_REGIAO_FACTIVEL[8][4][0] = MATRIZ_REGIAO_FACTIVEL[8][4][0] + 1
                                if GENS_EVOL[pai][i][2] == 150:
                                    MATRIZ_REGIAO_FACTIVEL[8][5][0] = MATRIZ_REGIAO_FACTIVEL[8][5][0] + 1
                                if GENS_EVOL[pai][i][2] == 160:
                                    MATRIZ_REGIAO_FACTIVEL[8][6][0] = MATRIZ_REGIAO_FACTIVEL[8][6][0] + 1
                                if GENS_EVOL[pai][i][2] == 980:
                                    MATRIZ_REGIAO_FACTIVEL[8][7][0] = MATRIZ_REGIAO_FACTIVEL[8][7][0] + 1  
    

                    
        GENS_EVOL[0] = copy.deepcopy(GENS_EVOL[pai])


def atualiza_pai_PORTAS_AOLONGO(): #Quantifica a quantidade de acertos da tabela verdade e a quantidade de portas (sugestão do Heder), ao longo de todo o processo
    soma = 0
    fitness_soma = [] # Array que armazena a soma de todas as saídas de cada circuito
    aux = []
    for k in range(ee_lambda + 1):
        fitness_soma.append([])
    for i in range(len(FITNESS_EE)):
        for j in range(len(FITNESS_EE[i])):
            soma = soma + FITNESS_EE[i][j]
        fitness_soma[i] = copy.deepcopy(soma)
        soma = 0
    maior_fitness = max(fitness_soma)
    indice_maior_fitness = [] #Array que armazena os índices dos genótipos com a maior fitness existente
    iguais = [] #Array que armazena o número de portas correspondente a cada um dos genótipos com maior fitness existente
    iguais_qtd_portas = [] #Array que armazena os índices dos genótipos cujas fitness são iguais e a quantidade de portas também, caso a quantidade de portas seja a menor existente
    for i in range(len(fitness_soma)):
        if fitness_soma[i] == maior_fitness:
            indice_maior_fitness.append(i)
    for i in range(ee_lambda + 1):
        PORTAS_EE[i][no-1] = copy.deepcopy(PORTAS_CKT[i])
    for i in range(len(indice_maior_fitness)):
        iguais.append(PORTAS_EE[indice_maior_fitness[i]][no-1])
    menor_porta = min(iguais)
    for i in range(len(PORTAS_EE)):
        if PORTAS_EE[i][no-1] == menor_porta and fitness_soma[i] == maior_fitness:
            iguais_qtd_portas.append(i)
    if len(iguais_qtd_portas) > 1:
        pai = r.randint(0, len(iguais_qtd_portas)-1)
        pai = iguais_qtd_portas[pai]
    else:
        pai = iguais_qtd_portas[0]
    indice_maior_fitness.clear()
    iguais.clear()
    iguais_qtd_portas.clear()

    GENS_EVOL[0] = copy.deepcopy(GENS_EVOL[pai]) #Atualiza novo pai


def atualiza_pai_WIRES_AOLONGO(): #Quantifica a quantidade de acertos da tabela verdade e a quantidade de portas (sugestão do Heder), ao longo de todo o processo
    soma = 0
    fitness_soma = [] # Array que armazena a soma de todas as saídas de cada circuito
    aux = []
    for k in range(ee_lambda + 1):
        fitness_soma.append([])
    for i in range(len(FITNESS_EE)):
        for j in range(len(FITNESS_EE[i])):
            soma = soma + FITNESS_EE[i][j]
        fitness_soma[i] = copy.deepcopy(soma)
        soma = 0
    maior_fitness = max(fitness_soma)
    indice_maior_fitness = [] #Array que armazena os índices dos genótipos com a maior fitness existente
    iguais = [] #Array que armazena o número de portas correspondente a cada um dos genótipos com maior fitness existente
    iguais_qtd_portas = [] #Array que armazena os índices dos genótipos cujas fitness são iguais e a quantidade de portas também, caso a quantidade de portas seja a menor existente
    for i in range(len(fitness_soma)):
        if fitness_soma[i] == maior_fitness:
            indice_maior_fitness.append(i)
    WIRES_LOCAL = []
    for i in range(ee_lambda + 1):
        WIRES_LOCAL.append([])
        for j in range(1):
            WIRES_LOCAL[i].append([])
            WIRES_LOCAL[i][0] = copy.deepcopy(WIRES_CKT[i])

    for i in range(len(indice_maior_fitness)):
        iguais.append(WIRES_LOCAL[indice_maior_fitness[i]][0])
    maior_wires = max(iguais)
    for i in range(len(WIRES_LOCAL)):
        if WIRES_LOCAL[i][0] == maior_wires and fitness_soma[i] == maior_fitness:
            iguais_qtd_portas.append(i)
    if len(iguais_qtd_portas) > 1:
        pai = r.randint(0, len(iguais_qtd_portas)-1)
        pai = iguais_qtd_portas[pai]
    else:
        pai = iguais_qtd_portas[0]
    indice_maior_fitness.clear()
    iguais.clear()
    iguais_qtd_portas.clear()

    GENS_EVOL[0] = copy.deepcopy(GENS_EVOL[pai]) #Atualiza novo pai

def mutacao_BiasedSAM_INFACTIVEL(posicao):

    todos_ativos = []
    for i in range(len(MEUARRAY[posicao])):
        for j in range(len(MEUARRAY[posicao][i])):
            if MEUARRAY[posicao][i][j] == "X":
                todos_ativos.append(j)
    ATIVO_MOM.clear()
    ATIVO_MOM.append([])
    ATIVO_MOM[0] = copy.deepcopy(MEUARRAY[posicao][0])
    for i in range(len(ATIVO_MOM[0])):
        if ATIVO_MOM[0][i] == "X":
            ATIVO_MOM[0][i] == "-"
    for i in todos_ativos:
        ATIVO_MOM[0][i] = "X"

    controle_ativo = True
    controle_auxiliar = 0 # Esta variável serve para o index da lista ATIVO_MOM[g][gene_mutado] não ser violado

        #FICA FAZENDO MUTAÇÃO
    for g in range(1): # Se for range(no), fará mutação em ao menos UM nó ativo para CADA saida, se for range(1), mudará somente UM gene ativo de todo o genótipo 
        qtd_mutacao = r.randint(1, ug) #Determina quantos nós serão mutados, dentro do intervalo recomendado pela literatura
        while controle_ativo == True: #Realiza "qtd_mutacao" de mutações NESTE descendente
            G = copy.deepcopy(GENS_EVOL[0]) #Busca o pai original de volta (sempre alocado em GENS_EVOL[0])
            gene_mutado = r.randint(ni, (ni+no+Ln-1)) #O gene que será mutado pode receber qualquer valor maior que o último espaço ocupado pela entrada, isto é da posição ni (já que começa por 0) até a posição da última saida

            if gene_mutado > ni+Ln-1:
                controle_ativo = False
                controle_auxiliar = gene_mutado
                gene_mutado = 0
            if ATIVO_MOM[g][gene_mutado] == "X" and controle_ativo == True:

                if gene_mutado > ni+Ln-1:
                    controle_ativo = False
                    controle_auxiliar = gene_mutado
                    gene_mutado = 0
                controle_ativo = False

            if controle_auxiliar != 0:
                gene_mutado = controle_auxiliar
                controle_auxiliar = 0

            if gene_mutado >= (Ln+ni): #Garante que o gene selecionado é uma saída
                #print("MUTAÇÃO TIPO SAIDA")
                ultimo_elemento = ni + Ln - 1 #Último elemento possivel que a saída pode assumir
                primeiro_elemento = ultimo_elemento - (lb * nr) + 1 #Primeiro valor possivel que a saída pode assumir
                elementos_portas = ultimo_elemento - primeiro_elemento + 1 #Quantidade de valores existentes entre o primeiro e último possiveis valores
                qtd_elementos = elementos_portas + ni - 1 #Determina a quantidade de valores distintos que a porta pode assumir como entrada. O -1 tem como função apenas possibilitar o uso de randint(0, qtd_elementos)
                entrada_ou_porta = r.randint(0, qtd_elementos) #Dá a mesma chance para todos os possiveis inputs
                if entrada_ou_porta >= ni: #G[Ln+ni+i] representa cada saida, sequencialmente
                    nova_saida = r.randint(primeiro_elemento, ultimo_elemento)
                    while G[gene_mutado] == nova_saida: #Garante que a mutação mudará o gene_mutado para um valor diferente do inicial
                        nova_saida = r.randint(primeiro_elemento, ultimo_elemento)
                    G[gene_mutado] = nova_saida
                else:
                    nova_saida = r.randint(0, ni-1)
                    while G[gene_mutado] == nova_saida: #Garante que a mutação mudará o gene_mutado para um valor diferente do inicial
                        nova_saida = r.randint(0, ni-1)
                    G[gene_mutado] = nova_saida    
            else:
                colunaatual = int((gene_mutado - ni)/nr) + 1 #Determina a coluna na qual o gene selecionado está
                alelo_mutado = r.randint(0, max_entradas) #Determina qual será o alelo a ser mutado

        
                if alelo_mutado == max_entradas: #Significa que a mutação ocorrerá no tipo da porta
                    #print("MUTAÇÃO TIPO PORTA")
                    porta_logica = r.randint(0, 99) #Determina qual será a nova porta
                    if G[gene_mutado][alelo_mutado] == 900:
                        G[gene_mutado][alelo_mutado] = MUT_WIRE_5_INFAC[porta_logica]
                    if G[gene_mutado][alelo_mutado] == 100:
                          G[gene_mutado][alelo_mutado] = MUT_AND_5_INFAC[porta_logica]
                    if G[gene_mutado][alelo_mutado] == 110:
                          G[gene_mutado][alelo_mutado] = MUT_OR_5_INFAC[porta_logica]
                    if G[gene_mutado][alelo_mutado] == 130:
                          G[gene_mutado][alelo_mutado] = MUT_XOR_5_INFAC[porta_logica]
                    if G[gene_mutado][alelo_mutado] == 800:
                          G[gene_mutado][alelo_mutado] = MUT_NOT_5_INFAC[porta_logica]
                else:
                    #print("MUTAÇÃO TIPO INPUT")
                    if colunaatual == 1: #Neste caso as portas lógicas só recebem inputs
                        entrada = r.randint(0, (ni-1))
                        while G[gene_mutado][alelo_mutado] == entrada:#Garante que a mutação mudará o gene_mutado para um valor diferente do inicial
                            entrada = r.randint(0, (ni-1))
                        G[gene_mutado][alelo_mutado] = entrada
    
                        
                    if colunaatual - lb < 1 and colunaatual != 1: #Neste caso as portas lógicas recebem inputs e nós       
                        valorpossivel = (nr * (colunaatual-2)) + (nr-1) + ni
                        sorteado = r.randint(0, valorpossivel)
                        while G[gene_mutado][alelo_mutado] == sorteado:#Garante que a mutação mudará o gene_mutado para um valor diferente do inicial
                            sorteado = r.randint(0, valorpossivel)
                        G[gene_mutado][alelo_mutado] = sorteado
    
                
                    if colunaatual - lb >= 1: #Neste caso as portas logicas recebem somente nós
                        primeiro_elemento = nr * (colunaatual-lb-1) + ni # O fator + ni serve para deslocar os elementos para frente, contando a quantidade de inputs.
                        ultimo_elemento = (nr * (colunaatual-2)) + (nr-1) + ni 
                        elementos_portas = ultimo_elemento - primeiro_elemento + 1 #Determina quantas portas possíveis existem
                        qtd_elementos = elementos_portas + ni - 1 #Determina a quantidade de valores distintos que a porta pode assumir como entrada. O -1 tem como função apenas possibilitar o uso de randint(0, qtd_elementos)
                        entrada_ou_porta = r.randint(0, qtd_elementos) #Dá a mesma chance para todos os possiveis inputs
                        if entrada_ou_porta >= ni:
                            novo_valor_mutacao = r.randint(primeiro_elemento, ultimo_elemento)
                            while G[gene_mutado][alelo_mutado] == novo_valor_mutacao:#Garante que a mutação mudará o gene_mutado para um valor diferente do inicial
                                novo_valor_mutacao = r.randint(primeiro_elemento, ultimo_elemento)
                            G[gene_mutado][alelo_mutado] = novo_valor_mutacao
                        else:
                            novo_valor_mutacao = r.randint(0, ni-1)
                            while G[gene_mutado][alelo_mutado] == novo_valor_mutacao:#Garante que a mutação mudará o gene_mutado para um valor diferente do inicial
                                novo_valor_mutacao = r.randint(0, ni-1)
                            G[gene_mutado][alelo_mutado] = novo_valor_mutacao
        

        SAM_ativo(posicao)
        GENS_EVOL[posicao] = copy.deepcopy(G) #Passa o genótipo mutado para a matriz de genótipos

        G = copy.deepcopy(GENS_EVOL[0])


def mutacao_BiasedSAM_FACTIVEL(posicao):

    todos_ativos = []
    for i in range(len(MEUARRAY[posicao])):
        for j in range(len(MEUARRAY[posicao][i])):
            if MEUARRAY[posicao][i][j] == "X":
                todos_ativos.append(j)
    ATIVO_MOM.clear()
    ATIVO_MOM.append([])
    ATIVO_MOM[0] = copy.deepcopy(MEUARRAY[posicao][0])
    for i in range(len(ATIVO_MOM[0])):
        if ATIVO_MOM[0][i] == "X":
            ATIVO_MOM[0][i] == "-"
    for i in todos_ativos:
        ATIVO_MOM[0][i] = "X"

    quantidade_de_mutacoes = 0
    controle_ativo = True
    controle_auxiliar = 0 # Esta variável serve para o index da lista ATIVO_MOM[g][gene_mutado] não ser violado

        #FICA FAZENDO MUTAÇÃO
    for g in range(1): # Se for range(no), fará mutação em ao menos UM nó ativo para CADA saida, se for range(1), mudará somente UM gene ativo de todo o genótipo 
        qtd_mutacao = r.randint(1, ug) #Determina quantos nós serão mutados, dentro do intervalo recomendado pela literatura
        while controle_ativo == True: #Realiza "qtd_mutacao" de mutações NESTE descendente
            G = copy.deepcopy(GENS_EVOL[0]) #Busca o pai original de volta (sempre alocado em GENS_EVOL[0])
            gene_mutado = r.randint(ni, (ni+no+Ln-1)) #O gene que será mutado pode receber qualquer valor maior que o último espaço ocupado pela entrada, isto é da posição ni (já que começa por 0) até a posição da última saida

            if gene_mutado > ni+Ln-1:
                quantidade_de_mutacoes +=1
                if quantidade_de_mutacoes == GLOBAL_SAM_MUTACOES:
                    controle_ativo = False
                controle_auxiliar = gene_mutado
                gene_mutado = 0
            if ATIVO_MOM[g][gene_mutado] == "X" and controle_ativo == True:

                if gene_mutado > ni+Ln-1:
                    quantidade_de_mutacoes +=1
                    if quantidade_de_mutacoes == GLOBAL_SAM_MUTACOES:
                        controle_ativo = False
                    controle_auxiliar = gene_mutado
                    gene_mutado = 0
                quantidade_de_mutacoes += 1
                if quantidade_de_mutacoes == GLOBAL_SAM_MUTACOES:
                    controle_ativo = False

            if controle_auxiliar != 0:
                gene_mutado = controle_auxiliar
                controle_auxiliar = 0

            if gene_mutado >= (Ln+ni): #Garante que o gene selecionado é uma saída
                #print("MUTAÇÃO TIPO SAIDA")
                ultimo_elemento = ni + Ln - 1 #Último elemento possivel que a saída pode assumir
                primeiro_elemento = ultimo_elemento - (lb * nr) + 1 #Primeiro valor possivel que a saída pode assumir
                elementos_portas = ultimo_elemento - primeiro_elemento + 1 #Quantidade de valores existentes entre o primeiro e último possiveis valores
                qtd_elementos = elementos_portas + ni - 1 #Determina a quantidade de valores distintos que a porta pode assumir como entrada. O -1 tem como função apenas possibilitar o uso de randint(0, qtd_elementos)
                entrada_ou_porta = r.randint(0, qtd_elementos) #Dá a mesma chance para todos os possiveis inputs
                if entrada_ou_porta >= ni: #G[Ln+ni+i] representa cada saida, sequencialmente
                    nova_saida = r.randint(primeiro_elemento, ultimo_elemento)
                    while G[gene_mutado] == nova_saida: #Garante que a mutação mudará o gene_mutado para um valor diferente do inicial
                        nova_saida = r.randint(primeiro_elemento, ultimo_elemento)
                    G[gene_mutado] = nova_saida
                else:
                    nova_saida = r.randint(0, ni-1)
                    while G[gene_mutado] == nova_saida: #Garante que a mutação mudará o gene_mutado para um valor diferente do inicial
                        nova_saida = r.randint(0, ni-1)
                    G[gene_mutado] = nova_saida    
            else:
                colunaatual = int((gene_mutado - ni)/nr) + 1 #Determina a coluna na qual o gene selecionado está
                alelo_mutado = r.randint(0, max_entradas) #Determina qual será o alelo a ser mutado

        
                if alelo_mutado == max_entradas: #Significa que a mutação ocorrerá no tipo da porta
                    #print("MUTAÇÃO TIPO PORTA")
                    porta_logica = r.randint(0, 99) #Determina qual será a nova porta
                    if G[gene_mutado][alelo_mutado] == 100:
                          G[gene_mutado][alelo_mutado] = MUT_AND_5_FAC[porta_logica]
                    if G[gene_mutado][alelo_mutado] == 110:
                          G[gene_mutado][alelo_mutado] = MUT_OR_5_FAC[porta_logica]
                    if G[gene_mutado][alelo_mutado] == 130:
                          G[gene_mutado][alelo_mutado] = MUT_XOR_5_FAC[porta_logica]
                    if G[gene_mutado][alelo_mutado] == 900:
                          G[gene_mutado][alelo_mutado] = MUT_WIRE_5_FAC[porta_logica]            
                    if G[gene_mutado][alelo_mutado] == 800:
                          G[gene_mutado][alelo_mutado] = MUT_NOT_5_FAC[porta_logica]
                          
                else:
                    #print("MUTAÇÃO TIPO INPUT")
                    if colunaatual == 1: #Neste caso as portas lógicas só recebem inputs
                        entrada = r.randint(0, (ni-1))
                        while G[gene_mutado][alelo_mutado] == entrada:#Garante que a mutação mudará o gene_mutado para um valor diferente do inicial
                            entrada = r.randint(0, (ni-1))
                        G[gene_mutado][alelo_mutado] = entrada
    
                        
                    if colunaatual - lb < 1 and colunaatual != 1: #Neste caso as portas lógicas recebem inputs e nós       
                        valorpossivel = (nr * (colunaatual-2)) + (nr-1) + ni
                        sorteado = r.randint(0, valorpossivel)
                        while G[gene_mutado][alelo_mutado] == sorteado:#Garante que a mutação mudará o gene_mutado para um valor diferente do inicial
                            sorteado = r.randint(0, valorpossivel)
                        G[gene_mutado][alelo_mutado] = sorteado
    
                
                    if colunaatual - lb >= 1: #Neste caso as portas logicas recebem somente nós
                        primeiro_elemento = nr * (colunaatual-lb-1) + ni # O fator + ni serve para deslocar os elementos para frente, contando a quantidade de inputs.
                        ultimo_elemento = (nr * (colunaatual-2)) + (nr-1) + ni 
                        elementos_portas = ultimo_elemento - primeiro_elemento + 1 #Determina quantas portas possíveis existem
                        qtd_elementos = elementos_portas + ni - 1 #Determina a quantidade de valores distintos que a porta pode assumir como entrada. O -1 tem como função apenas possibilitar o uso de randint(0, qtd_elementos)
                        entrada_ou_porta = r.randint(0, qtd_elementos) #Dá a mesma chance para todos os possiveis inputs
                        if entrada_ou_porta >= ni:
                            novo_valor_mutacao = r.randint(primeiro_elemento, ultimo_elemento)
                            while G[gene_mutado][alelo_mutado] == novo_valor_mutacao:#Garante que a mutação mudará o gene_mutado para um valor diferente do inicial
                                novo_valor_mutacao = r.randint(primeiro_elemento, ultimo_elemento)
                            G[gene_mutado][alelo_mutado] = novo_valor_mutacao
                        else:
                            novo_valor_mutacao = r.randint(0, ni-1)
                            while G[gene_mutado][alelo_mutado] == novo_valor_mutacao:#Garante que a mutação mudará o gene_mutado para um valor diferente do inicial
                                novo_valor_mutacao = r.randint(0, ni-1)
                            G[gene_mutado][alelo_mutado] = novo_valor_mutacao
        

        SAM_ativo(posicao)
        GENS_EVOL[posicao] = copy.deepcopy(G) #Passa o genótipo mutado para a matriz de genótipos

        G = copy.deepcopy(GENS_EVOL[0])


def COSPE_ativo(posicao):
    for j in range(no):
        print("SAIDA: ", j)
        for i in range(len(MEUARRAY[posicao][j])):
            if MEUARRAY[posicao][j][i] == "X":
                print("I = ", i, "GENE = ", GENS_EVOL[posicao][i])

## Programas

def main_SAM_G_2P_CROSSOVER():
    seed_inicial = 4 #Mudar este valor quando tiver que executar alguma seed específica que não começa por 0.
    qtd_testes = 1
    teste_atual = 0 ## Referência para imprimir a população inicial
    
    for y in range(qtd_testes):
        numero_avaliacoes = 0
        controle = True
        geracao = 0
        soma_fitness = []
        auxiliar = 0
        geracao_aux = 0
        formato_ee()
        formato_array_fitness()
        gera_formato_nos()
        formato_tabela_verdade()
        gera_formato_genotipo()
        r.seed(y + seed_inicial)
        populacao_inicial()
        define_saida()
        for j in range(ni):
            G[j] = copy.deepcopy(j)
        GENS_EVOL[0] = copy.deepcopy(G)
        mutacao_pontual()
        nos_ativos()
        arquivo2 = open('semente_populacao_inicial.txt', 'a')
        arquivo2.write("\n POPULAÇÃO INICIAL " + str(y+seed_inicial) + "\n")
        arquivo2.write(str(GENS_EVOL))
        arquivo2.close()
        arquivo_inicio = open('informacoes_processo.txt', 'a')
        arquivo_inicio.write("\n SEED: " + str(y+seed_inicial))
        arquivo_inicio.close()
        primeira_convergencia = False
        muda_atualiza_pai = False
        print("ENTROU")
        print("GERAÇÃO: ", geracao)
        while geracao <= 200000:
            tempo_inicio = time.time()

            gc.collect()

            if muda_atualiza_pai == False:
                for k in range(ee_lambda):
                    mutacao_SAM_CORRETO_CROSS(k+1)
            else:
                for k in range(ee_lambda):
                    mutacao_SAM_CORRETO(k+1)

            nos_ativos()

            portas_pai = 0
            portas_filho = 0
            if primeira_convergencia == True:
                portas_pai = copy.deepcopy(PORTAS_CKT[0])
            for w in range(ee_lambda + 1):

                conta_portas(w)

                auxiliar2 = 0

                monta_tabela(w)

                numero_avaliacoes = numero_avaliacoes + 1

                for b in range(len(FITNESS_EE[w])):
                    auxiliar2 = auxiliar2 + FITNESS_EE[w][b]

                if auxiliar2 == (no * len(OUTPUTS[0])) and primeira_convergencia == False:
                    print(" CONVERGIU COM NÚMERO DE AVALIAÇÕES IGUAL A: ", numero_avaliacoes)
                    primeira_convergencia = True
                    muda_atualiza_pai = True
                    COSPE_ativo(w)
                    print("NÚMERO DE PORTAS: ", PORTAS_CKT[w])
                    print("INDIVIDUO: ", w)
                    arquivo_teste = open('convergencias.txt', 'a')
                    arquivo_teste.write("\n SEED: " + str(y+seed_inicial) + "\n")
                    arquivo_teste.write("CONVERGIU COM NÚMERO DE AVALIAÇÕES IGUAL A: " + str(numero_avaliacoes))
                    for j in range(no):
                        arquivo_teste.write("\n SAIDA: " + str(j))
                        for i in range(len(MEUARRAY[w][j])):
                            if MEUARRAY[w][j][i] == "X":
                                arquivo_teste.write("\n I = " + str(i) + " GENE = " + str(GENS_EVOL[w][i]))
                    
                    arquivo_teste.write("\n NÚMERO DE PORTAS: " + str(PORTAS_CKT[w]))
                    arquivo_teste.write("\n NÚMERO DE WIRES: " + str(WIRES_CKT[w]))
                    arquivo_teste.write("\n INDIVIDUO: " + str(w))
                    arquivo_teste.write("\n")
                    arquivo_teste.close()
                    geracao = 300000
                    break
                    print("GENS_EVOL: ", GENS_EVOL[w])



            if muda_atualiza_pai == True:
                atualiza_pai_PORTAS_FACTIVEL()
            else:
                #atualiza_pai_PORTAS_FACTIVEL()
                atualiza_pai_CROSSOVER()

            if primeira_convergencia == True:
                portas_filho = copy.deepcopy(PORTAS_CKT[0])

            if portas_filho < portas_pai:
                arquivo_aolongo = open('informacoes_processo.txt', 'a')
                arquivo_aolongo.write("\n MUDANÇA NO NÚMERO DE PORTAS")
                arquivo_aolongo.write("\n PAI:" + str(portas_pai))
                arquivo_aolongo.write("\n FILHO: " + str(portas_filho))
                arquivo_aolongo.write("\n NÚMERO DE AVALIAÇÕES: " + str(numero_avaliacoes))
                arquivo_aolongo.close()

            soma_fitness.clear()

            for b in range(len(FITNESS_EE)):
                for c in range(len(FITNESS_EE[b])):
                    auxiliar = auxiliar + FITNESS_EE[b][c]
                soma_fitness.append(auxiliar)
                auxiliar = 0

            geracao = geracao + 1

            if geracao <= 20:
                print("\nGERAÇÃO : " + str(geracao))
                print(soma_fitness)
            if geracao % 1000 == 0:
                print("\nGERAÇÃO : " + str(geracao))
                print(soma_fitness)



        print("FIM", geracao)
        tempo_fim = time.time()
        arquivo_aolongo2 = open('informacoes_processo.txt', 'a')
        arquivo_aolongo2.write("\n FIM \n")
        arquivo_aolongo2.write("Tempo decorrido: " + str((tempo_fim - tempo_inicio)/60) + " minutos.")
        arquivo_aolongo2.write("\n")
        arquivo_aolongo2.close()
        
        #if controle == True:
        #    arquivo = open('matriz_transicao.txt','a')
        #    arquivo.write("\n")
        #    arquivo.write("NÃO CONVERGIU")
        #    arquivo.write("\n")
        #    arquivo.write(str(MATRIZ_REGIAO_INFACTIVEL))
        #    arquivo.write("\n")
        #else:
        #    arquivo = open('matriz_transicao.txt','a')
        #    arquivo.write("\n")
        #    arquivo.write("MATRIZ REGIÃO FACTÍVEL")
        #    arquivo.write("\n")
        #    arquivo.write(str(MATRIZ_REGIAO_FACTIVEL))
        #    arquivo.write("\n")
        arquivo = open('matriz_transicao.txt', 'a')
        arquivo.write("\n MATRIZ INFACTIVEL \n")
        arquivo.write(str(MATRIZ_REGIAO_INFACTIVEL))
        arquivo.write("\n MATRIZ FACTIVEL \n")
        arquivo.write(str(MATRIZ_REGIAO_FACTIVEL))
        arquivo.close()

        COSPE_ativo(0)
        
        for i in range(ee_lambda + 1):
            conta_portas(i)
        print("NÚMERO DE PORTAS: ", PORTAS_EE[0][no-1])
        if primeira_convergencia == True:
            arquivo_otimizado = open('convergencias_otimizado.txt', 'a')
            arquivo_otimizado.write("\n SEED: " + str(y+seed_inicial) + "\n")
            arquivo_otimizado.write("CONVERGIU COM NÚMERO DE AVALIAÇÕES IGUAL A: " + str(numero_avaliacoes))
            for j in range(no):
                arquivo_otimizado.write("\n SAIDA: " + str(j))
                for i in range(len(MEUARRAY[0][j])):
                    if MEUARRAY[0][j][i] == "X":
                        arquivo_otimizado.write("\n I = " + str(i) + " GENE = " + str(GENS_EVOL[w][i]))
                    
            arquivo_otimizado.write("\n NÚMERO DE PORTAS: " + str(PORTAS_CKT[0]))
            arquivo_otimizado.write("\n NÚMERO DE WIRES: " + str(WIRES_CKT[0]))
            arquivo_otimizado.write("\n INDIVIDUO: " + str(w))
            arquivo_otimizado.write("\n")
            arquivo_otimizado.close()

        
        
        limpa_array()

        teste_atual = teste_atual + 1

        numero_avaliacoes = 0

###########################################################

