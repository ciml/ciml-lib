from __future__ import print_function
import os
import sys
import pdb

root = sys.argv[1]

for i in next(os.walk(root))[1] :
    #print(root+"\\"+i)
    os.system("process3.py " +"\"" + root + "\\" + i + "\"")
