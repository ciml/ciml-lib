#!/bin/bash
#$ -pe threaded 1
#$ -N AOS
#$ -cwd
#$ -j y
#$ -S /bin/bash
/home/guilherme.ferreira/AGA_3.2/8_2/scriptAOS.sh $1
