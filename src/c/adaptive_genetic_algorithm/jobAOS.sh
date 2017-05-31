#!/bin/bash
#$ -pe threaded 1
#$ -N AOS
#$ -cwd
#$ -j y
#$ -S /bin/bash
/home/guilherme.ferreira/Adaptive_Pursuit/scriptAOS.sh $1
