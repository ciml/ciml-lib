#Opcoes de compilacao:
#-OBR Operation Based Representation (default: Job Based Representation)
#-NEH Nawaz, Enscore, Ham (N�o compilar quando definir OBR)
#-IG Inicializa a iterated greedy (N�o compilar quando definir OBR)
#-LS Somente busca local
#-GA Default Genetic Algorithm
#-AP Adaptive Pursuit
#-PPC Predictive Parameter Control
#-PPCR Predictive Paramenter Control with range
CC        = gcc

#CFLAGS    = -g -Wall -DLS -DNEH #use para debug
#CFLAGS    = -g -Wall -DAP -DIG -DNEH #use para debug

CFLAGS   = -O3 -DAP -DIG #-DNEH #use para compilar com otimiza��o (Genetic Algorithm w/ NEH)

#CFLAGS    = -O3 -march=native -DLS #-DNEH #use para debug
#CFLAGS    = -O3 -march=native -DAP -DIG -DNEH #use para debug

LDFLAGS   = -lm

#Fontes
SRCDIR = ./
SRCS   = $(shell ls $(SRCDIR)/*.c 2>/dev/null)
DEPS   = $(shell ls $(SRCDIR)/*.h 2>/dev/null)
OBJS   = $(SRCS:.c=.o)

#Nome do Executavel
exec1  = aga_jsp

#Regras de compilacao
all : $(exec1)

$(exec1): $(OBJS) $(DEPS)
	$(CC) $(CFLAGS) -o $(exec1) $(OBJS) $(LDFLAGS)
	rm -f *.o *~
