#
#
# parameters
# 1 - constraint_handling       # 0-TS+DELEqC-II, 1-APM+DELEqC-II, 2-TS, 3-APM
# 2 - bound_handling			# 0- bound, 1-diff scaled (required to DELEqC)


constraint_handling=${1}
bound_handling=${2}  


pop_size=100
cr=0.7
f=0.9
de_variant=3
max_generations=50000
epsilon=0.0001


echo "constraint handling: "${constraint_handling}" -- bound handling: "${bound_handling}

# DE with crossover

for problem in {101..105}; do

	for seed in {1..100}; do
	
		echo "problem: "${problem}" -- seed: "${seed}

		./bin/Release/DE ${pop_size} ${cr} ${f} ${max_generations} ${problem} ${de_variant} ${seed} ${epsilon} ${constraint_handling} ${bound_handling} | bzip2 -9 -c > results_deleqc-ii_cec2018_${pop_size}_${cr}_${f}_${max_generations}_${problem}_${de_variant}_${seed}_${epsilon}_${constraint_handling}_${bound_handling}.out.bz2
		
	done

done

# DE with mutation only

cr=1.0
f=0.6

for problem in {101..105}; do

	for seed in {1..100}; do
		
		echo "problem: "${problem}" -- seed: "${seed}

		./bin/Release/DE ${pop_size} ${cr} ${f} ${max_generations} ${problem} ${de_variant} ${seed} ${epsilon} ${constraint_handling} ${bound_handling} | bzip2 -9 -c > results_deleqc-ii_cec2018_${pop_size}_${cr}_${f}_${max_generations}_${problem}_${de_variant}_${seed}_${epsilon}_${constraint_handling}_${bound_handling}.out.bz2
		
	done

done




## experiments for CEC 2018
## experiments of the new proposals
#for constraint_handling in {0..1}; do
	#./run_experiments_cec2018.sh ${constraint_handling} 1
#done
## experiments using APM and tournament selection
#for constraint_handling in {2..3}; do
	#for bound_handling in {0..1}; do 
		#./run_experiments_cec2018.sh ${constraint_handling} ${bound_handling}
	#done
#done
