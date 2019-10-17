#$ -S /bin/bash
#$ -q linux.q
#----------------------------------------------------------


echo "$(clear ; make msam)"

for seed in {1..12}; do
	echo "Running experiment with initial pop - seed:$seed"

	#echo "$(bash run_experiment_sam.sh $seed)"
    qsub run_experiment_sam.sh $seed
	#echo "$(bash run_experiment_pm.sh $seed)"
    qsub run_experiment_pm.sh $seed
done