@echo off
setlocal enableextensions enabledelayedexpansion

SET parameter=popVal
SET Caminho=..\..

SET popVal=100
SET seeds=10,11,12,13,14

break>resultFiles.txt
FOR %%i IN (%popVal%) DO (
	break>resultFiles_%%i.txt
	ECHO resultFiles_%%i.txt>>resultFiles.txt

    FOR %%j IN (%seeds%) DO (
		PUSHD %Caminho%
		make -f GeneticOpenCL.cbp.mak clean
		make -f GeneticOpenCL.cbp.mak NUM_INDIV=%%i SEED=%%j TWODEVICES=0 EVOLOCL=0 AVALOCL=0 EVOLOMP=0 AVALGPU=0 DIF_CONTEXT=0
		POPD

		for %%F in ("*.dat") do (
			SET nomeArquivo=%%~nF_seqSeq_seed_%%j_%parameter%_%%i.txt
			ECHO Extraindo dados para "!nomeArquivo!"
			ECHO !nomeArquivo!>>resultFiles_%%i.txt
			GeneticOpenCL.exe %%F>"Resultados\seqseq\!nomeArquivo!"
		)
	)

	FOR %%j IN (%seeds%) DO (
		PUSHD %Caminho%
		make -f GeneticOpenCL.cbp.mak clean
		make -f GeneticOpenCL.cbp.mak NUM_INDIV=%%i SEED=%%j TWODEVICES=1 EVOLOCL=1 AVALOCL=1 EVOLOMP=0 AVALGPU=1 DIF_CONTEXT=0
		POPD

		for %%F in ("*.dat") do (
			SET nomeArquivo=%%~nF_cpuGpuInt_seed_%%j_%parameter%_%%i.txt
			ECHO Extraindo dados para "!nomeArquivo!"
			ECHO !nomeArquivo!>>resultFiles_%%i.txt
			GeneticOpenCL.exe %%F>"Resultados\cpugpuInt\!nomeArquivo!"
		)
	)

    FOR %%j IN (%seeds%) DO (
		PUSHD %Caminho%
		make -f GeneticOpenCL.cbp.mak clean
		make -f GeneticOpenCL.cbp.mak NUM_INDIV=%%i SEED=%%j TWODEVICES=0 EVOLOCL=1 AVALOCL=1 EVOLOMP=0 AVALGPU=1 DIF_CONTEXT=1
		POPD

		for %%F in ("*.dat") do (
			SET nomeArquivo=%%~nF_cpuGpuSep_seed_%%j_%parameter%_%%i.txt
			ECHO Extraindo dados para "!nomeArquivo!"
			ECHO !nomeArquivo!>>resultFiles_%%i.txt
			GeneticOpenCL.exe %%F>"Resultados\cpugpuSep\!nomeArquivo!"
		)
	)

	FOR %%j IN (%seeds%) DO (
		PUSHD %Caminho%
		make -f GeneticOpenCL.cbp.mak clean
		make -f GeneticOpenCL.cbp.mak NUM_INDIV=%%i SEED=%%j TWODEVICES=0 EVOLOCL=1 AVALOCL=1 EVOLOMP=0 AVALGPU=0 DIF_CONTEXT=0
		POPD

		for %%F in ("*.dat") do (
			SET nomeArquivo=%%~nF_cpuCpu_seed_%%j_%parameter%_%%i.txt
			ECHO Extraindo dados para "!nomeArquivo!"
			ECHO !nomeArquivo!>>resultFiles_%%i.txt
			GeneticOpenCL.exe %%F>"Resultados\cpucpu\!nomeArquivo!"
		)
	)
)