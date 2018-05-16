from __future__ import print_function
import os
import sys
from numpy import *
import pdb


root = sys.argv[1]
roots = {root+'/cpuCpu', root+'/cpuGpu', root+'/GpuGpu', root+'/seqSeq'}
#tableTrain = [[]]
#tableTest = [[]]
#tableValidation = [[]]

tableResult = [[]]

modelsEvol = [[]]
modelsTest = [[]]
modelsValidation = [[]]

j = 0

for rootdir in sorted(roots):
	#print(rootdir, end="\n")
	evol = []
	aval = []
	total = []

	minEvol = float("inf")
	idEvol = ''
	minTotal = float("inf")
	idTotal = ''
	minAval = float("inf")
	idAval = ''

	#pdb.set_trace()
	for subdir, dirs, files in sorted(os.walk(rootdir)):
		for file in files:
			#pdb.set_trace()
			filepath = subdir + os.sep + file
			#print(filepath)
			with open(filepath) as f:
				lines = f.readlines()

			k = (len(lines))
			for i in range(0, k):
				fitnesses = lines[i].split(' ')
				f1 = float(fitnesses[0])
				f2 = float(fitnesses[1])
				f3 = float(fitnesses[2])

				#print(filepath, end=' ')
				#print(f1, end=' ')
				#print(f2, end=' ')
				#print(f3, end='\n')

				#pdb.set_trace()
				if f1 < minEvol:
					minEvol = f1
					idEvol = filepath

				if f2 < minAval:
					minAval = f2
					idAval = filepath

				if f3 < minTotal:
					minTotal = f3
					idTotal = filepath

				evol.append(f1)
				aval.append(f2)
				total.append(f3)
	
#	modelsTrain.append([])
#	modelsTrain[j].append(idTrain)
#
#	tableTrain.append([])
#	tableTrain[j].append(min(train))
#	tableTrain[j].append(median(train))
#	tableTrain[j].append(mean(train))
#	tableTrain[j].append(std(train))
#	tableTrain[j].append(max(train))
#
#	modelsTest.append([])
#	modelsTest[j].append(idTest)
#
#	tableTest.append([])
#	tableTest[j].append(min(test))
#	tableTest[j].append(median(test))
#	tableTest[j].append(mean(test))
#	tableTest[j].append(std(test))
#	tableTest[j].append(max(test))
#
#	modelsValidation.append([])
#	modelsValidation[j].append(idValidation)	
#
#	tableValidation.append([])
#	tableValidation[j].append(min(validation))
#	tableValidation[j].append(median(validation))
#	tableValidation[j].append(mean(validation))
#	tableValidation[j].append(std(validation))
#	tableValidation[j].append(max(validation))

	tableResult.append([])

	tableResult[j].append(min(evol))
	tableResult[j].append(min(aval))
	tableResult[j].append(min(total))

	tableResult[j].append(median(evol))
	tableResult[j].append(median(aval))
	tableResult[j].append(median(total))

	tableResult[j].append(mean(evol))
	tableResult[j].append(mean(aval))
	tableResult[j].append(mean(total))

	tableResult[j].append(std(evol))
	tableResult[j].append(std(aval))
	tableResult[j].append(std(total))

	tableResult[j].append(max(evol))
	tableResult[j].append(max(aval))
	tableResult[j].append(max(total))

	j = j + 1
print("min & median & mean & std & max ", end='\\\ \n')

print("evol & aval & total & evol & aval & total & evol & aval & total & evol & aval & total & evol & aval & total ", end='\\\ \n')
for line in tableResult:
	for column in line:
		print("{0:.5f}".format(float(column)), end=' & ')
	print('\\', end='\n')






#for line in modelsTrain:
#	for column in line:
#		print(column, end=' ')
#	print('\\', end='\n')
#
#print("min & median & mean & std & max ", end='\\\ \n')
#for line in tableTest:
#	for column in line:
#		print("{0:.5f}".format(float(column)), end=' & ')
#	print('\\', end='\n')

#for line in modelsTest:
#	for column in line:
#		print(column, end=' ')
#	print('\\', end='\n')
#
#print("min & median & mean & std & max ", end='\\\ \n')

#for line in tableValidation:
#	for column in line:
#		print("{0:.5f}".format(float(column)), end=' & ')
#	print('\\', end='\n')

#for line in modelsValidation:
#	for column in line:
#		print(column, end=' ')
#	print('\\', end='\n')
