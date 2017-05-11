#Compilador, opcoes de compilacao.
CC        = gcc
#CFLAGS    = -g -Wall -fopenmp	 #use para debug
CFLAGS   = -O3 -march=native   #use para compilar com otimização
#CFLAGS   = -g -Wall -pg #use para fazer gproof
#CFLAGS	= -O3 -Wall -fopenmp
LDFLAGS   = -lm

#Fontes 
SRCDIR = ./
SRCS   = $(shell ls $(SRCDIR)/*.c 2>/dev/null)
DEPS   = $(shell ls $(SRCDIR)/*.h 2>/dev/null)
OBJS   = $(SRCS:.c=.o)

#Nome do Executavel
exec1  = hh2_jobshop

#Regras de compilacao
all : $(exec1)

$(exec1): $(OBJS) $(DEPS)
	$(CC) $(CFLAGS) -o $(exec1) $(OBJS) $(LDFLAGS)
	rm -f *.o *~
