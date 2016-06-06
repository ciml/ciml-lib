clonalg:
	g++ -fopenmp -O2 main.cpp Clonalg.cpp ClonalgCL.cpp BaseClonalg.cpp OneMaxProblem.cpp ObjectiveFunction.cpp ElipsoidalObjectiveFunction.cpp RosenbrockObjectiveFunction.cpp  utils.cpp -lm -l OpenCL -L /usr/local/cuda-6.0/lib64 -I /usr/local/cuda-6.0/include/ -I /usr/local/cuda/include/ -I /usr/local/cuda-7.5/include/ -lm -DPROFILING -o clonalg
clonalgs:
	g++ -fopenmp -O2 main.cpp Clonalg.cpp BaseClonalg.cpp OneMaxProblem.cpp ObjectiveFunction.cpp -lm -lm -DPROFILING -DGPUS=2 -o clonalg
	clonalgsAMD:
	g++ -march=bdver1 -Ofast -fopenmp main.cpp Clonalg.cpp BaseClonalg.cpp OneMaxProblem.cpp ObjectiveFunction.cpp -lm -lm -DPROFILING -DGPUS=2 -o clonalg
clean:
	rm -f clonalg	
all:
	clean
	clonalg