#!/bin/bash
#$ -pe threaded 1
#$ -N AOS
#$ -cwd
#$ -j y
#$ -S /bin/bash
/home/guilherme.ferreira/adaptive_genetic_algorithm/scriptAOS.sh $1
