 # -*- coding: latin-1 -*-
 # generate data of a given function
import sys
import random
from math import exp, pow, cos, sin

nome_arquivo = sys.argv[1]
total = int(sys.argv[2])


arq = open("f11"+str(total*total)+".dat", "w")
arq.write(str(total*total)+ " 3 1\nX1 X2 Y\n\n")

x1 = -5

for i in range(total):
	x2 = -5
	for j in range(total):
		result = (pow(x1,3)/5) + (pow(x2,3)/2) - x2 - x1
		arq.write(str(x1) + " " + str(x2) + " " + str(result) + "\n")
		x2 += float(10/float(total))
	x1 += float(10/float(total))

arq.write("\n1\n4 4\n10 11 12 13 14 15 16 17")



#result = exp(-x1)*pow(x1,3)*sin(x1)*((cos(x1)*pow(sin(x1), 2)) - 1) * (x2 - 5)
#result = (x1 - 3) * (x2 - 3) + 2 * sin((x1-4)*(x2-4))
#result = (pow(x1 - 3 , 4) + pow(x2 - 3 , 3) - (x2-3) )/(pow(x2-2 , 4) + 10)

#result = (1/(1+(1/pow(x1,4)))) + (1/(1+(1/pow(x2,4)))) nao consegui, mudar range de valores

#result = pow(x1,4)-pow(x1,3)+(pow(x2,2)/2) - x2
#result = 8/(2+pow(x1,2)+pow(x2,2))