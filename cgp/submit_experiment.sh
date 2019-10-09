#$ -S /bin/bash
#$ -q linux.q
#----------------------------------------------------------

echo "$(clear ; make)"

for seed in {2..4}; do
	echo "Running experiment - seed:$seed"

	echo "$(bash run_experiment.sh $seed ${1})"
done