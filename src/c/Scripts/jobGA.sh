#!/bin/bash
#$ -pe threaded 1
#$ -N GA
#$ -cwd
#$ -j y
#$ -S /bin/bash
/home/guilherme.ferreira/GA/LOX_IN_LS/scriptGA.sh $1
