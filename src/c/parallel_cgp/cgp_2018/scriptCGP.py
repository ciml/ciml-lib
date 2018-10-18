#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed May 16 13:45:55 2018

@author: lucasmullers
"""
import subprocess

if __name__ == '__main__':

    caminho = "cd /home/lucasmullers/Desktop/ic_2017/cgp_2018/;"

    for i in range(2,3):
        for j in range(1,11):
            comando = "./bin/cgp " + str(j) + " " + str(i)
            subprocess.call(caminho + comando,shell=True)
