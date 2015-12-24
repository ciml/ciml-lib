#include <stdio.h>
#include <stdlib.h>
#include "Clonalg.h"
#include "ClonalgCL.h"
#include <sys/time.h>
#include "utils.h"

typedef struct{
	int generations,
		popsize,
		problemDimension,
		nclones,
		randomInsertion;
	float mutationFactor,
		  upperLim,
		  lowerLim;
	bool parallel;
	int gpus;
}t_parameters;

void GetArgs(int argc, char **argv, t_parameters * parameters);

int main(int argc, char **argv) {

	t_parameters p;

	GetArgs(argc, argv, &p);

	double start = getRealTime();
	srand(0);

    if(!p.parallel){

        BaseClonalg *clonalg = new Clonalg(p.generations, p.popsize, p.problemDimension,
        								  1, 8, p.mutationFactor, 0.1, p.nclones,p.randomInsertion, p.upperLim, p.lowerLim);
        clonalg->Search();

    	//cout << "Parallel\tPopSize\t" << "Dimensions\tGPUS\n" << "Total time\t" << endl;
    	cout << p.parallel << "\t"<<p.popsize <<"\t" << p.problemDimension <<"\t0\t" << getRealTime() - start << endl;

    	delete clonalg;
    }
    else{
        BaseClonalg *clonalgCL = new ClonalgCL(p.generations, p.popsize, p.problemDimension,
    			  1, 8, p.mutationFactor, 0.1, p.nclones,p.randomInsertion, p.upperLim, p.lowerLim, p.gpus);

    	double elapsed = clonalgCL->Search();
    	//cout << "Parallel\tPopSize\t" << "Dimensions\tGPUS\n" << "Total time\t" << "Algorithm time" << endl;
    	cout << p.parallel << "\t"<<p.popsize <<"\t" << p.problemDimension <<"\t" << p.gpus << "\t" << getRealTime() - start << "\t" <<  elapsed<< endl;

    	delete clonalgCL;
	}

	return EXIT_SUCCESS;
}

void GetArgs(int argc, char **argv, t_parameters * p){

	if(argc == 1){
		cout << "Usage: ./clonalg generations popsize problemDimension nclones randomInsertion mutationFactor upperLim lowerLim parallel gpus" << endl;
		//exit(0);

		cout << "using default values" << endl;

		p->generations = 100;
		p->popsize = 32;
		p->problemDimension = 512;
		p->nclones = 10;
		p->randomInsertion = 0;
		p->mutationFactor = 5.0;
		p->upperLim = 1.0f;
		p->lowerLim = -1.0f;
		p->parallel = false;
		p->gpus = 1;
	}
	else if(argc == 6){
		p->generations = atoi(argv[1]);
		p->popsize = atoi(argv[2]);
		p->problemDimension = atoi(argv[3]);
		p->parallel = atoi(argv[4]);
		p->nclones = 10;
		p->randomInsertion = 0;
		p->mutationFactor = 8.5;
		p->upperLim = 1.0f;
		p->lowerLim = -1.0f;
		p->gpus = atoi(argv[5]);
	}
	else{

		if(argc < 11){
			cout << "Usage: ./clonalg generations popsize problemDimension nclones randomInsertion mutationFactor upperLim lowerLim parallel gpus" << endl;
			exit(0);
		}

		p->generations = atoi(argv[1]);
		p->popsize = atoi(argv[2]);
		p->problemDimension = atoi(argv[3]);
		p->nclones = atoi(argv[4]);
		p->randomInsertion = atoi(argv[5]);
		p->mutationFactor = atof(argv[6]);
		p->upperLim = atof(argv[7]);
		p->lowerLim = atof(argv[8]);
		p->parallel = atoi(argv[9]);
		p->gpus = atoi(argv[10]);

	}
}
