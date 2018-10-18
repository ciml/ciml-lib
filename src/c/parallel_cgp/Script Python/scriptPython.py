#!/usr/bin/env python

import subprocess


def leituraTXT(entrada):
    file = open(str(entrada), "r")
    nomeArquivosDeEntrada = []
    while True:
        line = file.readline()
        if ".EOF" in line:
            break
        else:
            nomeArquivosDeEntrada.append(line)
    return nomeArquivosDeEntrada


if __name__ == '__main__':

    nomeEntradas = leituraTXT("truthtables.txt")
    tabelasGeradas = leituraTXT("gerados.txt")

    nomeEntradas = list(set(nomeEntradas) - set(tabelasGeradas))
    nomeEntradas.sort()
    print(nomeEntradas)

    with open("gerados.txt", "a") as arq:
        for i in range(len(nomeEntradas)):
            comando = "python /home/lucasmullers/Desktop/ic_2017/Script\ Python/blif_refatorado.py /home/lucasmullers/Downloads/IC-BLIF/MCNC/Combinational/blif/DDD "
            #comando = "python /home/lucasmullers/Desktop/github/Script\ Python/blif_refatorado.py /home/lucasmullers/Downloads/IC-BLIF/MCNC/Combinational/blif/DDD "
            comando += nomeEntradas[i]
            subprocess.call(comando, shell=True)
            arq.write(str(nomeEntradas[i]))
    subprocess.call("shutdown now")

