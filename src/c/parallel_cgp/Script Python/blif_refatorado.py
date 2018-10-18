#!/usr/bin/env python

from __future__ import print_function
import itertools
import re
from sys import argv


def extraiInformacoesCabecalho(file_blif):
    informacoes = file_blif.readline().split()
    informacoes.pop(0)
    while True:
        ultimaPosicao = len(informacoes) - 1
        if informacoes[ultimaPosicao] == "\\":
            informacoes.pop(ultimaPosicao)
            informacoes += file_blif.readline().split()
        else:
            break
    return informacoes


def extraiInformacoesNames(line):
    informacoes = line.split()
    informacoes.pop(0)
    while True:
        ultimaPosicao = len(informacoes) - 1
        if informacoes[ultimaPosicao] == "\\":
            informacoes.pop(ultimaPosicao)
            informacoes += file.readline().split()
        else:
            break
    return informacoes


def calculaCaractereMaiusculoUsado(entradasEmNames, numero, entradas):
    return chr(65 + entradas.index(entradasEmNames[numero]))


def defineEntradasUsadasEDefineOsCaracteres(names, entradas, subCircuito):
    caractereParaCadaEntrada = []
    entradasEmNames = extraiInformacoesNames(names)
    for item in range(0, len(entradasEmNames)):
        if entradasEmNames[item] in entradas:
            caractere = calculaCaractereMaiusculoUsado(entradasEmNames, item, entradas)
            caractereParaCadaEntrada.append(caractere)
        else:
            if item is len(entradasEmNames) - 1:
                subCircuito.append(entradasEmNames[item])
            elif entradasEmNames[item] in subCircuitos:
                caractereParaCadaEntrada.append(entradasEmNames[item])
            else:
                subCircuitos.append(entradasEmNames[item])
                caractereParaCadaEntrada.append(entradasEmNames[item])
    return caractereParaCadaEntrada


def verificaExpressaoNula(expressaoNula):
    if expressaoNula == '':
        return 0
    else:
        return 1


def adicionaExpressaoDaSaidaAoArray(expressaoDeUmaSaida, expressaoDeTodasAsSaidas, saida):
    if verificaExpressaoNula(expressaoDeUmaSaida) and saida is not '':
        expressaoDeTodasAsSaidas[str(saida)] = str(expressaoDeUmaSaida[:-1])
    return expressaoDeTodasAsSaidas


def geraExpressaoDaLinha(line, caracteresUsados, numeroItensNaLinha):
    expressaoDaLinha = ''
    for item in range(numeroItensNaLinha):
        if line[item] == '1':
            expressaoDaLinha += str(caracteresUsados[item]) + '*'
        elif line[item] == '0':
            expressaoDaLinha += '~' + str(caracteresUsados[item]) + '*'
        elif line[item] == ' ':
            break
        else:
            pass
    return expressaoDaLinha


def adicionaSaida(nomeSaidas, subCircuito):
    tamanhoSubCircuito = len(subCircuito) - 1
    if subCircuito[tamanhoSubCircuito] in nomeSaidas:
        subCircuito.pop(tamanhoSubCircuito)
    else:
        nomeSaidas.append(subCircuito[tamanhoSubCircuito])
        subCircuito.pop(tamanhoSubCircuito)


def geradorExpressaoBooleana(file_blif, entradas, subCircuito, nomeSaidas):
    expressaoDeTodasAsSaidas = {}
    expressaoDeUmaSaida = ''
    caracteresUsados = []
    numeroDeEntradasUsado = 0
    saida = ""
    while True:
        line = file_blif.readline()
        if ".end" in line or ".exdc" in line:
            break
        if ".names" in line:
            expressaoDeTodasAsSaidas = adicionaExpressaoDaSaidaAoArray(expressaoDeUmaSaida, expressaoDeTodasAsSaidas,
                                                                       saida)
            expressaoDeUmaSaida = ""
            caracteresUsados = defineEntradasUsadasEDefineOsCaracteres(line, entradas, subCircuito)
            saida = subCircuito[len(subCircuito) - 1]
            adicionaSaida(nomeSaidas, subCircuito)
            numeroDeEntradasUsado = len(caracteresUsados)
        else:
            expressaoDeUmaSaida += geraExpressaoDaLinha(line, caracteresUsados, numeroDeEntradasUsado)[:-1] + '+'
    adicionaExpressaoDaSaidaAoArray(expressaoDeUmaSaida, expressaoDeTodasAsSaidas, saida)
    return expressaoDeTodasAsSaidas


def validaExpressaoGeral(expressaoGeral, subCircuito, nomeSaidas):
    if subCircuitos is not "":
        expressaoGeral = substituiSubCircuitosNaExpressao(expressaoGeral, nomeSaidas, subCircuito)
    expressaoGeral = removeSubCircuitosDoDicionario(expressaoGeral, subCircuito, nomeSaidas)

    return expressaoGeral


def canonicaliza(expr):
    SUBSTITUICOES = {
        '~': ' not ',
        '+': ' or ',
        '*': ' and ',
    }
    return re.sub('|'.join(re.escape(sym) for sym in SUBSTITUICOES.keys()),
                  lambda sym: SUBSTITUICOES[sym.group(0)],
                  expr).strip()


def extraiVariaveisUsadasNaExpressao(expressaoDeUmaSaida):
    return sorted(set(re.findall(r'\b[A-Za-z]\b', expressaoDeUmaSaida)))


def avaliaResultado(resultado):
    if resultado is True:
        return '1'
    elif resultado is False:
        return '0'
    else:
        return resultado


def substituiSubCircuitosNaExpressao(expressaoGeral, nomeSaidas, subCircuito):
    for item in range(len(nomeSaidas)):
        for pos in range(len(subCircuito)):
            string = expressaoGeral[str(nomeSaidas[item])]
            string = string.replace(str(subCircuito[pos]), "(" + expressaoGeral[str(subCircuito[pos])] + ")")
            expressaoGeral[str(nomeSaidas[item])] = string
    return expressaoGeral


def removeSubCircuitosDoDicionario(expressaoGeral, subCircuito, nomeSaidas):
    for item in range(len(subCircuito)):
        if subCircuito[item] not in nomeSaidas:
            del (expressaoGeral[str(subCircuito[item])])
    return expressaoGeral


def geraColunaDeSaidaDaTabelaVerdade(expressaoBooleana, numDeEntradas):
    expressaoBooleana = canonicaliza(expressaoBooleana)
    variaveisUsadas = extraiVariaveisUsadasNaExpressao(expressaoBooleana)
    NO_GLOBALS = {'__builtins__': {}}
    colunaDeSaida = []

    for valores in itertools.product(range(2), repeat=numDeEntradas):
        locals = dict(zip(variaveisUsadas, valores))
        resultado = eval(expressaoBooleana, NO_GLOBALS, locals)
        colunaDeSaida.append(avaliaResultado(resultado))
    return colunaDeSaida


def geraEntradasDaTabelaVerdade(numDeEntradas):
    return list(itertools.product([0, 1], repeat=numDeEntradas))


def substituiSubCircuitoEmSubCircuito(subCircuito, expressaoGeral):
    tamanhoSubCircuito = len(subCircuito)
    while True:
        flag = 0
        for item in range(tamanhoSubCircuito):
            expressao = expressaoGeralDoCircuito[str(subCircuito[item])]
            for itens in range(tamanhoSubCircuito):
                if subCircuito[itens] in expressao:
                    expressao = expressao.replace(str(subCircuito[itens]),
                                                  "(" + expressaoGeral[str(subCircuito[itens])] + ")")
                    flag += 1
            expressaoGeralDoCircuito[str(subCircuito[item])] = expressao
        if flag == 0:
            break
    return expressaoGeral


if __name__ == '__main__':
    caminho = argv[1]
    nomeDoArquivo = argv[2]
    # ABERTURA DO ARQUIVO
    file = open(caminho + "/" + nomeDoArquivo, "r")

    # INFORMACOES DO CABECALHO
    nomeDoModelo = extraiInformacoesCabecalho(file)
    nomeDasEntradas = extraiInformacoesCabecalho(file)
    nomeDasSaidas = extraiInformacoesCabecalho(file)

    nomeDasSaidasAux = []

    numeroDeEntradas = len(nomeDasEntradas)
    numeroDeSaidas = len(nomeDasSaidas)

    subCircuitos = []

    # EXPRESSAO BOOLEANO DO CIRCUITO
    expressaoGeralDoCircuito = geradorExpressaoBooleana(file, nomeDasEntradas, subCircuitos, nomeDasSaidasAux)
    expressaoGeralDoCircuito = substituiSubCircuitoEmSubCircuito(subCircuitos, expressaoGeralDoCircuito)
    expressaoGeralDoCircuito = validaExpressaoGeral(expressaoGeralDoCircuito, subCircuitos, nomeDasSaidas)


    if '--print' in argv:
        for item in nomeDasSaidas:
            print(str(item) + " : " + str(expressaoGeralDoCircuito[item]))
    
    print(nomeDoArquivo.upper())
    print("NOME DAS ENTRADAS:" + str(nomeDasEntradas))
    print("NOME DAS SAIDAS: " + str(nomeDasSaidas))
    
    arq = open("/home/lucasmullers/Desktop/temp.txt",'a')
    arq.write(str(expressaoGeralDoCircuito['r']))
    
    tabelaDeEntradas = geraEntradasDaTabelaVerdade(numeroDeEntradas)
    tabelaDeSaidas = []
    
    for coluna in range(len(expressaoGeralDoCircuito)):
        colunaTabelaVerdade = geraColunaDeSaidaDaTabelaVerdade(expressaoGeralDoCircuito[str(nomeDasSaidas[coluna])],
                                                               numeroDeEntradas)
        tabelaDeSaidas.append(colunaTabelaVerdade)
    arquivoSaida = argv[2].split(".")
    with open("/home/lucasmullers/Desktop/" + str(arquivoSaida[0]) + ".tt", "w") as arq:
        arq.write(str(2 ** numeroDeEntradas) + " " + str(numeroDeEntradas) + " " + str(numeroDeSaidas) + "\n")
        for i in range(2 ** numeroDeEntradas):
            for j in range(numeroDeEntradas):
                print(tabelaDeEntradas[i][j], end='')
                arq.write(str(tabelaDeEntradas[i][j]))
            for k in range(numeroDeSaidas):
                print(tabelaDeSaidas[k][i], end='')
                arq.write(str(tabelaDeSaidas[k][i]))
            print()
            arq.write("\n")
