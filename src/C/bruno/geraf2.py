 # -*- coding: latin-1 -*-
import sys
import random
from math import exp, pow, cos, sin

nome_arquivo = sys.argv[1]
total = int(sys.argv[2])


arq = open("f2"+str(total*total)+".dat", "w")
arq.write(str(total*total)+ " 3 1\nX1 X2 Y\n\n")

x1 = -5

for i in range(total):
	x2 = -5
	for j in range(total):
		result = exp(-x1)*pow(x1,3)*sin(x1)*((cos(x1)*pow(sin(x1), 2)) - 1) * (x2 - 5)
		arq.write(str(x1) + " " + str(x2) + " " + str(result) + "\n")
		x2 += float(10/float(total))
	x1 += float(10/float(total))

arq.write("\n1\n4 4\n10 11 12 13 14 15 16 17")