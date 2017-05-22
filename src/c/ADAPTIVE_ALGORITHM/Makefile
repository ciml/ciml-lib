#Compilador, opcoes de compilacao.
CC        = gcc
#CFLAGS    = -g -Wall -DPPC #use para debug (Predictive Parameter Control)
#CFLAGS    = -g -Wall -DAP #use para debug (Adaptive Pursuit)
#CFLAGS    = -g -Wall -DGA #use para debug (Genetic Algorithm)

#CFLAGS   = -O3 -march=native -DPPC  #use para compilar com otimização (Predictive Parameter Control)
#CFLAGS   = -O3 -march=native -DAP  #use para compilar com otimização (Adaptive Pursuit)
CFLAGS   = -O3 -march=native -DGA  #use para compilar com otimização (Genetic Algorithm)

LDFLAGS   = -lm

#Fontes 
SRCDIR = ./
SRCS   = $(shell ls $(SRCDIR)/*.c 2>/dev/null)
DEPS   = $(shell ls $(SRCDIR)/*.h 2>/dev/null)
OBJS   = $(SRCS:.c=.o)

#Nome do Executavel
exec1  = aosjss

#Regras de compilacao
all : $(exec1)

$(exec1): $(OBJS) $(DEPS)
	$(CC) $(CFLAGS) -o $(exec1) $(OBJS) $(LDFLAGS)
	rm -f *.o *~
