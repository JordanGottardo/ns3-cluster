#!/usr/bin/python

# Invocation: 
#	./createSimpleScenario.py numNodes nodeDistance
# e.g.
#  ./createSimpleScenario.py 600 1

import sys, os
import string
import shutil
import utils

def main():
	numNodes = int(os.sys.argv[1])
	nodeDistance = int(os.sys.argv[2])
	initialX = 0
	y = 100
	z = 0
	with open("line.ns2mobility.xml", "w+") as f:
		for i in range(numNodes):
			utils.writeNodeToFile(f, i, initialX + i * nodeDistance, y, z)
	
if __name__ == "__main__":
	main()