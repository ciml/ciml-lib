 # -*- coding: latin-1 -*-
import sys
import random
from math import exp, pow 

arq = open("dados/exp4096.dat", "w")
arq.write("4096 3 1\nX1 X2 Y\n\n")

y1 = -5
for i in range(64):
	y2 = -5
	for j in range(64):
		value = exp(-(pow(y1 - 1, 2)))/(1.2+pow(y2 - 2.5, 2))
		arq.write(str(y1) + " " + str(y2) + " " + str(value))
		arq.write("\n")
		y2 = y2 + 10/32.0
	y1 = y1 + 10/32.0


arq.write("\n1\n4 4\n10 11 12 13 14 15 16 17")
