#!/bin/bash
#$ -pe threaded 1
#$ -N AOS
#$ -cwd
#$ -j y
#$ -S /bin/bash
/home/guilherme.ferreira/AP_AOS_MONO/3/script.sh $1
