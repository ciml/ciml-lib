#$ -S /bin/bash
#$ -q linux.q
#----------------------------------------------------------
# Parameters:
# 1- seed
# 2- sam
#----------------------------------------------------------

problems=(dc1.ep C17.ep z4ml.ep inc.ep dc2.ep f51m.ep multiplicador4x4.ep sao2.ep apla.ep cm85a.ep somador6x6.ep alu4.ep cu.ep)
ncol=(100 100 100 100 100 100 100 1000 1000 1000 1000 1000 1000)
evals=(1600000 3000000 2800000 4200000 4800000 4800000 3200000 8000000 6000000 6600000 7200000 8400000 11200000)

#for nos problemas
for i in `seq 12 12`;
do
    echo "./bin/cgp tables/${problems[i]} seed=${1} ncol=$((ncol[i])) maxgen=$((evals[i])) sam=${2} mutation=3 results/sam${2}_gam/${problems[i]}_seed${1}"
    echo "$(./bin/cgp tables/${problems[i]} seed=${1} ncol=$((ncol[i])) maxgen=$((evals[i])) sam=${2} mutation=3 results/sam${2}_gam/${problems[i]}_seed${1})"
    
    
done