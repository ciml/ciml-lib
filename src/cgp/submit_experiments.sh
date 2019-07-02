#$ -S /bin/bash
#$ -q linux.q
#----------------------------------------------------------


echo "$(clear ; make)"

for seed in {1..30}; do
	echo "Running experiment with initial pop - seed:$seed"

	#echo "$(bash run_experiment_sam.sh $seed)"
    qsub run_experiment_sam.sh $seed
	#echo "$(bash run_experiment_pm.sh $seed)"
    qsub run_experiment_pm.sh $seed
done

problema=(cmb.ep cordic.ep vda.ep)
evals=(40000000 4000000000 1000)
ncol=(2000 2000 2000)

for seed in {1..6}; do
	for i in `seq 0 2`; do
		qsub ./bin/cgp tables/${problema[i]} seed=$seed ncol=$((ncol[i])) maxgen=$((evals[i])) mutation=1 results/sam/${problema[i]}_initpop_seed$seed)
		qsub ./bin/cgp tables/${problema[i]} seed=$seed ncol=$((ncol[i])) maxgen=$((evals[i])) mutation=2 results/sam_gam/${problema[i]}_initpop_seed$seed)
	done
done
