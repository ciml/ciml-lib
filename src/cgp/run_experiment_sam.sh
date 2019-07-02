#$ -S /bin/bash
#$ -q linux.q
#----------------------------------------------------------
# Parameters:
# 1- seed
#----------------------------------------------------------

problema=(cm42a.ep majority.ep C17.ep cm82a.ep decod.ep cm138a.ep z4ml.ep f51m.ep 9symml.ep alu2.ep x2.ep cm85a.ep cm151a.ep cm162a.ep cu.ep cmb.ep pm1.ep tcon.ep sct.ep cc.ep frg1.ep)
evals=(3200000 2000000 3000000 2000000 3000000 4800000 4200000 4800000 5400000 4000000 6000000 4400000 7200000 11200000 11200000 9600000 9600000 10200000 11400000 12600000 16800000)
ncols=(120 26 17 123 104 117 383 485 786 1350 140 468 125 162 207 123 1575 62 366 218 1226)
ngates=(80 17 11 82 69 78 255 323 524 900 93 312 83 108 138 82 1050 41 244 145 817)
mediangen=(5469 2449 20449 662897 267871 199311 3292863 1312549 11384439 2524951 550441 102293 590447 3816927 28264511 9210735 290285 21393861 3294157 5785037)

#for nos problemas
for i in `seq 0 20`;
do
    #echo "Solving problem:${problema[i]} with SAM - seed:${1}"
    echo "$(./bin/cgp tables/${problema[i]} seed=${1} ncol=$((ncols[i])) maxgen=$((evals[i])) mutation=1 ngates=$((ngates[i])) mediangen=$((mediangen[i])) results/sam/${problema[i]}_seed${1})"
done




