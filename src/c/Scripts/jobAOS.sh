#!/bin/bash
#$ -pe threaded 1
#$ -N AOS
#$ -cwd
#$ -j y
#$ -S /bin/bash

SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")

/home/guilherme.ferreira/AP1/2_2_LS/scriptAOS.sh $1
