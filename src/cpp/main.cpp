#include <stdio.h>
#include <stdlib.h>
#include "Clonalg.h"
#include "ClonalgCL.h"
#include "TimeUtils.h"

typedef struct
{
	int generations,
		popsize,
		optimizationProblem,
		problemDimension,
		bitsPerDimension,
		nclones,
		randomInsertion;
	float mutationFactor,
		  upperLim,
		  lowerLim;
	bool parallel;
	int gpus;
	int cpus;
	float gpuRatio;
}t_parameters;

void GetArgs(int argc, char **argv, t_parameters &parameters);

int main(int argc, char **argv)
{
	t_parameters p;
	BaseClonalg *clonalg;

	GetArgs(argc, argv, p);

	double start = TimeUtils::getRealTime();
	srand(time(NULL));

	if(!p.parallel)
	{
		clonalg = new Clonalg(p.generations, p.popsize, p.optimizationProblem, p.problemDimension,
										  p.bitsPerDimension, p.mutationFactor, 0.1, p.nclones,p.randomInsertion, p.upperLim, p.lowerLim);
		clonalg->Search();

		cout << p.parallel << "\t"<<p.popsize <<"\t" << p.optimizationProblem << "\t" <<  p.problemDimension <<"\t0\t" << TimeUtils::getRealTime() - start << endl;
	}
	else
	{
		clonalg = new ClonalgCL(p.generations, p.popsize, p.optimizationProblem, p.problemDimension,
			p.bitsPerDimension, p.mutationFactor, 0.1, p.nclones,p.randomInsertion, p.upperLim, p.lowerLim, p.gpus, p.cpus, p.gpuRatio);

		double elapsed = clonalg->Search();
		cout << p.parallel << "\t"<<p.popsize <<"\t" << p.optimizationProblem << "\t" << p.problemDimension <<"\t" << p.gpus << "\t" << TimeUtils::getRealTime() - start << "\t" <<  elapsed<< endl;

	}

   delete clonalg;

   return EXIT_SUCCESS;
}

void GetArgs(int argc, char **argv, t_parameters &p)
{
	p.lowerLim = 0;
	p.upperLim = 1;
	p.bitsPerDimension = 1;
	p.optimizationProblem = 1;

	if(argc == 1)
	{
		cout << "Usage: ./clonalg generations popsize problemDimension nclones randomInsertion mutationFactor upperLim lowerLim parallel gpus cpus" << endl;
		//exit(0);

		cout << "using default values" << endl;

		p.generations = 100;
		p.popsize = 1024;
		p.problemDimension = 1024;
		p.nclones = 2;
		p.randomInsertion = 1;
		p.mutationFactor = 5.0;
		p.parallel = true;
		p.gpus = 1;
		p.cpus = 0;
		p.gpuRatio = 0;
	}
	else
	{
		if(argc < 13)
		{
			cout << "Usage: ./clonalg generations popsize optimizationProblem problemDimension bitsPerDimension nclones randomInsertion mutationFactor upperLim lowerLim parallel gpus cpus" << endl;
			exit(0);
		}

		p.generations = atoi(argv[1]);
		p.popsize = atoi(argv[2]);
		p.optimizationProblem = atoi(argv[3]);
		p.problemDimension = atoi(argv[4]);
		p.bitsPerDimension = atoi(argv[5]);
		p.nclones = atoi(argv[6]);
		p.randomInsertion = atoi(argv[7]);
		p.mutationFactor = atof(argv[8]);
		p.upperLim = atof(argv[9]);
		p.lowerLim = atof(argv[10]);
		p.parallel = atoi(argv[11]);
		p.gpus = atoi(argv[12]);
		p.cpus = atoi(argv[13]);
	}
}
