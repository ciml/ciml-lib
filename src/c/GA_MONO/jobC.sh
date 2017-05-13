#!/bin/bash
#$ -pe threaded 1
#$ -N AOS
#$ -cwd
#$ -j y
#$ -S /bin/bash
/home/guilherme.ferreira/GA_MONO/1/script.sh $1
