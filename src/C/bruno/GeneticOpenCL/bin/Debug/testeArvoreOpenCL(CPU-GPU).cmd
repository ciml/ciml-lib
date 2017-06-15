@echo off
setlocal enableextensions enabledelayedexpansion

SET parameter=mutacao
SET Caminho=..\..


SET valores=0.3,0.4,0.5
SET seeds=1,2,3,4,5

break>resultFiles.txt
FOR %%i IN (%valores%) DO (
	break>resultFiles_%%i.txt
	ECHO resultFiles_%%i.txt>>resultFiles.txt
	FOR %%j IN (%seeds%) DO (
		PUSHD %Caminho%
		make -f GeneticOpenCL.cbp.mak clean
		make -f GeneticOpenCL.cbp.mak PROB_MUT=%%i SEED=%%j TWODEVICES=1 EVOLOCL=1 AVALOCL=1 EVOLOMP=0 AVALGPU=1
		POPD
		SET nomeArquivo=cpuGpu_seed_%%j_%parameter%_%%i.txt
		ECHO Extraindo dados para "!nomeArquivo!"
		ECHO !nomeArquivo!>>resultFiles_%%i.txt
		GeneticOpenCL.exe >!nomeArquivo!
	)
)

PAUSE