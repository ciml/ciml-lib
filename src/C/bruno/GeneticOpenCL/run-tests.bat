@echo off
setlocal enableextensions enabledelayedexpansion

SET parameter=popVal
SET Caminho=..\..

SET popVal=512
SET seeds=10,11,12,13,14,15

break>"Resultados\resultFiles.txt"
FOR %%i IN (%popVal%) DO (
	break>"Resultados\resultFiles_!%%i!.txt"
	ECHO resultFiles_%%i.txt>>resultFiles.txt

    FOR %%j IN (%seeds%) DO (
		cmake -Bcmake-build-debug -S. -G "CodeBlocks - MinGW Makefiles" -DCMAKE_C_COMPILER="C:/mingw64/bin/gcc.exe" -DCMAKE_CXX_COMPILER="C:/mingw64/bin/g++.exe" -DCMAKE_BUILD_TYPE=debug -DNUM_INDIV=%%i -DSEED=%%j -DTWODEVICES=1 -DEVOLOCL=1 -DAVALOCL=1 -DEVOLOMP=0 -DAVALGPU=1 -DDIF_CONTEXT=0
		cmake --build cmake-build-debug --target all
        cd cmake-build-debug

		for %%F in ("..\test-data\*.dat") do (
			SET nomeArquivo=%%~nF_cpuGpuInt_seed_%%j_%parameter%_%%i.txt
			ECHO Extraindo dados para "!nomeArquivo!"
			ECHO !nomeArquivo!>>resultFiles_%%i.txt
			GeneticOpenCL.exe %%F>"..\Resultados\cpuGpuInt\!nomeArquivo!"
		)
        cd ..
	)

    FOR %%j IN (%seeds%) DO (
		cmake -Bcmake-build-debug -S. -G "CodeBlocks - MinGW Makefiles" -DCMAKE_C_COMPILER="C:/mingw64/bin/gcc.exe" -DCMAKE_CXX_COMPILER="C:/mingw64/bin/g++.exe" -DCMAKE_BUILD_TYPE=debug -DNUM_INDIV=%%i -DSEED=%%j -DTWODEVICES=0 -DEVOLOCL=1 -DAVALOCL=1 -DEVOLOMP=0 -DAVALGPU=1 -DDIF_CONTEXT=1
		cmake --build cmake-build-debug --target all
        cd cmake-build-debug

		for %%F in ("..\test-data\*.dat") do (
			SET nomeArquivo=%%~nF_cpuGpuSep_seed_%%j_%parameter%_%%i.txt
			ECHO Extraindo dados para "!nomeArquivo!"
			ECHO !nomeArquivo!>>resultFiles_%%i.txt
			GeneticOpenCL.exe %%F>"..\Resultados\cpuGpuSep\!nomeArquivo!"
		)
        cd ..
	)

	FOR %%j IN (%seeds%) DO (
		cmake -Bcmake-build-debug -S. -G "CodeBlocks - MinGW Makefiles" -DCMAKE_C_COMPILER="C:/mingw64/bin/gcc.exe" -DCMAKE_CXX_COMPILER="C:/mingw64/bin/g++.exe" -DCMAKE_BUILD_TYPE=debug -DNUM_INDIV=%%i -DSEED=%%j -DTWODEVICES=0 -DEVOLOCL=1 -DAVALOCL=1 -DEVOLOMP=0 -DAVALGPU=0 -DDIF_CONTEXT=0
		cmake --build cmake-build-debug --target all
        cd cmake-build-debug

		for %%F in ("..\test-data\*.dat") do (
			SET nomeArquivo=%%~nF_cpuCpu_seed_%%j_%parameter%_%%i.txt
			ECHO Extraindo dados para "!nomeArquivo!"
			ECHO !nomeArquivo!>>resultFiles_%%i.txt
			GeneticOpenCL.exe %%F>"..\Resultados\cpuCpu\!nomeArquivo!"
		)
        cd ..
	)

    FOR %%j IN (%seeds%) DO (
        cmake -Bcmake-build-debug -S. -G "CodeBlocks - MinGW Makefiles" -DCMAKE_C_COMPILER="C:/mingw64/bin/gcc.exe" -DCMAKE_CXX_COMPILER="C:/mingw64/bin/g++.exe" -DCMAKE_BUILD_TYPE=debug -DNUM_INDIV=%%i -DSEED=%%j -DTWODEVICES=0 -DEVOLOCL=0 -DAVALOCL=0 -DEVOLOMP=0 -DAVALGPU=0 -DDIF_CONTEXT=0   
        cmake --build cmake-build-debug --target all
        cd cmake-build-debug

		for %%F in ("..\test-data\*.dat") do (
			SET nomeArquivo=%%~nF_seqSeq_seed_%%j_%parameter%_%%i.txt
			ECHO Extraindo dados para "!nomeArquivo!"
			ECHO !nomeArquivo!>>resultFiles_%%i.txt
			GeneticOpenCL.exe %%F>"..\Resultados\seqSeq\!nomeArquivo!"
		)
        cd ..
	)
)