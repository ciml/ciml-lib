from __future__ import print_function
import os
import sys
from numpy import *

root= sys.argv[1]
roots = {root+'/cpuCpu', root+'/cpuGpu', root+'/GpuGpu', root+'/seqSeq'}
tableEvol = [[]]
tabelAval = [[]]
tabelTotal = [[]]

modelsTrain = [[]]
modelsTest = [[]]
modelsValidation = [[]]

i = 0

for rootdir in sorted(roots):
	#print(rootdir, end="\n")
	train = []
	test = []
	validation = []

	minTrain = float("inf")
	idTrain = ''
	minValidation = float("inf")
	idValidation = ''
	minTest = float("inf")
	idTest = ''

	for subdir, dirs, files in sorted(os.walk(rootdir)):
		for file in files:
			filepath = subdir + os.sep + file
			print(filepath)
			with open(filepath) as f:
				lines = f.readlines()
	

			k = (len(lines))
			for x in range(0, k):
				fitnesses = lines[x].split(' ')
				f1 = float(fitnesses[0])
				f2 = float(fitnesses[1])
				f3 = float(fitnesses[2])

				print(filepath, end=' ')
				print(f1, end=' ')
				print(f2, end=' ')
				print(f3, end='\n')
				
				if f1 < minTrain:
					minTrain = f1
					idTrain = filepath

				if f2 < minTest:
					minTest = f2
					idTest = filepath

				if f3 < minValidation:
					minValidation = f3
					idValidation = filepath

				train.append(f1)
				test.append(f2)
				validation.append(f3)
	
	modelsTrain.append([])
	modelsTrain[i].append(idTrain)

	tableEvol.append([])
	tableEvol[i].append(min(train))
	tableEvol[i].append(median(train))
	tableEvol[i].append(mean(train))
	tableEvol[i].append(std(train))
	tableEvol[i].append(max(train))

	modelsTest.append([])
	modelsTest[i].append(idTest)

	tabelAval.append([])
	tabelAval[i].append(min(test))
	tabelAval[i].append(median(test))
	tabelAval[i].append(mean(test))
	tabelAval[i].append(std(test))
	tabelAval[i].append(max(test))

	modelsValidation.append([])
	modelsValidation[i].append(idValidation)	

	tabelTotal.append([])
	tabelTotal[i].append(min(validation))
	tabelTotal[i].append(median(validation))
	tabelTotal[i].append(mean(validation))
	tabelTotal[i].append(std(validation))
	tabelTotal[i].append(max(validation))

	i = i + 1
print("min & median & mean & std & max ", end='\\\ \n')
for line in tableEvol:
	for column in line:
		print("{0:.5f}".format(float(column)), end=' & ')
	print('\\', end='\n')

#for line in modelsTrain:
#	for column in line:
#		print(column, end=' ')
#	print('\\', end='\n')

print("min & median & mean & std & max ", end='\\\ \n')
for line in tabelAval:
	for column in line:
		print("{0:.5f}".format(float(column)), end=' & ')
	print('\\', end='\n')

#for line in modelsTest:
#	for column in line:
#		print(column, end=' ')
#	print('\\', end='\n')

print("min & median & mean & std & max ", end='\\\ \n')
for line in tabelTotal:
	for column in line:
		print("{0:.5f}".format(float(column)), end=' & ')
	print('\\', end='\n')

#for line in modelsValidation:
#	for column in line:
#		print(column, end=' ')
#	print('\\', end='\n')
