#!/bin/bash

for g in 100
do
	for p in 0 1
	do
		echo "Parallel: $p"		
		echo "p: $p"
		for n in 480 720 960 1024 2048 4096
		do
			echo "PopSize: $n"
			for d in 128 256 512 1024
			do
				if [ "$p" = 0 ]; then
		            echo "Dimensoes: $d"
					for i in {1..5}
					do					
						echo "$i"
						qsub -q all.q@compute-0-12.local,all.q@compute-0-14.local -pe threaded 1 clonalg.job $g $n $d 0 0
					done 
		        else
		            echo "Dimensoes: $d"
		            for GPU in 1 2
		            do
		            	echo "GPUS: $GPU"
						for i in {1..5}
						do					
							echo "$i"
							qsub -q all.q@compute-0-12.local,all.q@compute-0-14.local -pe threaded 32 clonalg.job $g $n $d $p $GPU
						done
					done 
		        fi				
			done
		done
	done
	
done


