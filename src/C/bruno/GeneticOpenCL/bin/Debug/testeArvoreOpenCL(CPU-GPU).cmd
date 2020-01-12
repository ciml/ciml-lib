@echo off
setlocal enableextensions enabledelayedexpansion

SET parameter=popVal
SET Caminho=..\..

SET popVal=512
SET seeds=9,10,11

break>resultFiles.txt
FOR %%i IN (%popVal%) DO (
	break>resultFiles_%%i.txt
	ECHO resultFiles_%%i.txt>>resultFiles.txt
	FOR %%j IN (%seeds%) DO (
		PUSHD %Caminho%
		make -f GeneticOpenCL.cbp.mak clean
		make -f GeneticOpenCL.cbp.mak NUM_INDIV=%%i SEED=%%j TWODEVICES=1 EVOLOCL=1 AVALOCL=1 EVOLOMP=0 AVALGPU=1 DIF_CONTEXT=1
		POPD

		for %%F in ("*.dat") do (
			SET nomeArquivo=%%~nF_cpuGpu_seed_%%j_%parameter%_%%i.txt
			ECHO Extraindo dados para "!nomeArquivo!"
			ECHO !nomeArquivo!>>resultFiles_%%i.txt
			GeneticOpenCL.exe %%F>"Resultados\!nomeArquivo!"
		)
	)
)