from __future__ import print_function
import os
import sys
from numpy import *

root= sys.argv[1]
roots = {root+'/0', root+'/1', root+'/2'}
tableTrain = [[]]
tableTest = [[]]
tableValidation = [[]]

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
	
			fitnesses = lines[-1].split(' ')
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

	tableTrain.append([])
	tableTrain[i].append(min(train))
	tableTrain[i].append(median(train))
	tableTrain[i].append(mean(train))
	tableTrain[i].append(std(train))
	tableTrain[i].append(max(train))

	modelsTest.append([])
	modelsTest[i].append(idTest)

	tableTest.append([])
	tableTest[i].append(min(test))
	tableTest[i].append(median(test))
	tableTest[i].append(mean(test))
	tableTest[i].append(std(test))
	tableTest[i].append(max(test))

	modelsValidation.append([])
	modelsValidation[i].append(idValidation)	

	tableValidation.append([])
	tableValidation[i].append(min(validation))
	tableValidation[i].append(median(validation))
	tableValidation[i].append(mean(validation))
	tableValidation[i].append(std(validation))
	tableValidation[i].append(max(validation))

	i = i + 1
print("min & median & mean & std & max ", end='\\\ \n')
for line in tableTrain:
	for column in line:
		print("{0:.5f}".format(float(column)), end=' & ')
	print('\\', end='\n')

for line in modelsTrain:
	for column in line:
		print(column, end=' ')
	print('\\', end='\n')

print("min & median & mean & std & max ", end='\\\ \n')
for line in tableTest:
	for column in line:
		print("{0:.5f}".format(float(column)), end=' & ')
	print('\\', end='\n')

for line in modelsTest:
	for column in line:
		print(column, end=' ')
	print('\\', end='\n')

print("min & median & mean & std & max ", end='\\\ \n')
for line in tableValidation:
	for column in line:
		print("{0:.5f}".format(float(column)), end=' & ')
	print('\\', end='\n')

for line in modelsValidation:
	for column in line:
		print(column, end=' ')
	print('\\', end='\n')
