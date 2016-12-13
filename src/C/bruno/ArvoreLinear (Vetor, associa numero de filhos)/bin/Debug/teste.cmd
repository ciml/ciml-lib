@echo off
setlocal enableextensions enabledelayedexpansion
SET Pathname=C:\Users\bruno\Dropbox\GIT\ciml-lib\src\C\bruno\ArvoreLinear (Vetor, associa numero de filhos)\bin\Debug
cd %Pathname%

SET seedValue=0
SET parameterValue=3
SET parameter=mutacao

SET nomeArquivo=seed_%seedValue%_%parameter%_%parameterValue%.txt
ECHO %nomeArquivo%

FOR /L %%G IN (0,1,11) DO (
	ECHO Executando...  !seedValue!
	FOR /L %%N IN (0,1,2) DO (
		SET nomeArquivo=seed_!seedValue!_%parameter%_!parameterValue!.txt
		ECHO Extraindo dados para "!nomeArquivo!"
		ECHO[
		ArvoreLinear.exe >!nomeArquivo!
		SET /A parameterValue+=1
	)
	SET /A seedValue+=1
)



PAUSE