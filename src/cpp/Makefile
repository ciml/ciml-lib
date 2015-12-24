clonalg:
	g++ -fopenmp main.cpp Clonalg.cpp ClonalgCL.cpp BaseClonalg.cpp OneMaxProblem.cpp ObjectiveFunction.cpp utils.cpp -lm -l OpenCL -l OpenCL -L /usr/local/cuda-6.0/lib64 -I /usr/local/cuda-6.0/include/ -lm -DPROFILING -DGPUS=2 -o clonalg

clean:
	rm -f clonalg	
all:
	clean
	clonalg
