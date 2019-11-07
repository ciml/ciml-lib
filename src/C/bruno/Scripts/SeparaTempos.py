from __future__ import print_function
import os
import sys
from numpy import *

root = sys.argv[1]
roots = {root+'\\seq', root+'\\cpucpu', root+'\\gpugpu', root+'\\cpugpu'}

tableResult = [[]]

modelsEvol = [[]]
modelsTest = [[]]
modelsValidation = [[]]

j = 0

for rootdir in sorted(roots):
    times = []
    print(rootdir)
    for subdir, dirs, files in sorted(os.walk(rootdir)):
        for file in files:
            nfile = file.split('_')
            if not os.path.exists("ResultsTime\\" + nfile[0]):
                os.makedirs("ResultsTime\\" + nfile[0])
            if not os.path.exists("ResultsTime\\" + nfile[0] + '\\' + nfile[1]):
                os.makedirs("ResultsTime\\" + nfile[0] + '\\' + nfile[1])    

            if not os.path.exists("ResultsModel\\" + nfile[0]):
                os.makedirs("ResultsModel\\" + nfile[0])
            if not os.path.exists("ResultsModel\\" + nfile[0] + '\\' + nfile[1]):
                os.makedirs("ResultsModel\\" + nfile[0] + '\\' + nfile[1])    

            newfileTimes = open("ResultsTime\\" + nfile[0] + '\\' + nfile[1] + '\\' + "times_" + nfile[0] + '_' + nfile[1] + '_' + nfile[4] + '_' + nfile[5],'a')
            newfileModel = open("ResultsModel\\" + nfile[0] + '\\' + nfile[1] + '\\' + "models_" + nfile[0] + '_' + nfile[1] + '_' + nfile[4] + '_' + nfile[5],'a')
            filepath = subdir + os.sep + file
            #print(filepath)
            f = open(filepath)
            next = f.readline()
            while next.strip() != "FIM":
                next = f.readline()
            #print(next)
            next = f.readline()
            times = next.split(' ')
            newfileTimes.write(times[0] + ' ' + times[1] + ' ' + times[2])
            evol = float(times[0])
            aval = float(times[1])
            total = float(times[2])
            while "MELHOR:" not in next: #next.strip() != "FIM":
                next = f.readline()
            newfileModel.write(next)

            newfileModel.close()
            newfileTimes.close()
            f.close()