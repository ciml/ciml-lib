@echo off
setlocal enableextensions enabledelayedexpansion

SET parameter=popVal
SET Caminho=..\..

SET popVal=10,100,128,512,1024
SET seeds=10

break>"Resultados\resultFiles.txt"
FOR %%i IN (%popVal%) DO (
	break>"Resultados\resultFiles_!%%i!.txt"
	ECHO resultFiles_%%i.txt>>resultFiles.txt

    FOR %%j IN (%seeds%) DO (
		cmake -Bcmake-build-debug -S. -G "CodeBlocks - MinGW Makefiles" -DCMAKE_C_COMPILER="C:/mingw64/bin/gcc.exe" -DCMAKE_CXX_COMPILER="C:/mingw64/bin/g++.exe" -DCMAKE_BUILD_TYPE=debug -DNUM_INDIV=%%i -DSEED=%%j -DTWODEVICES=0 -DEVOLOCL=0 -DAVALOCL=0 -DEVOLOMP=0 -DAVALGPU=0 -DDIF_CONTEXT=0
		cmake --build cmake-build-debug --target all
        cd cmake-build-debug

		for %%F in ("..\test-data\*.dat") do (
			SET nomeArquivo=%%~nF_seq_seed_%%j_%parameter%_%%i.txt
			ECHO Extraindo dados para "!nomeArquivo!"
			ECHO !nomeArquivo!>>resultFiles_%%i.txt
			GeneticOpenCL.exe %%F>"..\Resultados\seq\!nomeArquivo!"
		)
        cd ..
	)

	
)