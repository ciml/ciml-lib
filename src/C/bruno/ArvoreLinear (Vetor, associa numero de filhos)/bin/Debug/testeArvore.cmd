@echo off
setlocal enableextensions enabledelayedexpansion

SET parameter=mutacao
SET Caminho=..\..


SET valores=0.3,0.4,0.5
SET seeds=1,2,3,4,5,6,7,8,9,10,11

break>resultFiles.txt
FOR %%i IN (%valores%) DO (
	break>resultFiles_%%i.txt
	ECHO resultFiles_%%i.txt>>resultFiles.txt
	FOR %%j IN (%seeds%) DO (
		PUSHD %Caminho%
		make -f ArvoreLinear.cbp.mak clean
		make -f ArvoreLinear.cbp.mak PROB_MUT=%%i SEED=%%j
		POPD
		SET nomeArquivo=seed_%%j_%parameter%_%%i.txt
		ECHO Extraindo dados para "!nomeArquivo!"
		ECHO !nomeArquivo!>>resultFiles_%%i.txt
		ArvoreLinear.exe >!nomeArquivo!
	)
)

PAUSE