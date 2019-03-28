#!/usr/bin/python

# Invocation: 
#	./createSimpleScenario.py numNodes nodeDistance
# e.g.
#  ./createSimpleScenario.py 600 1

import sys, os
import string
import shutil

def main():
	numNodes = int(os.sys.argv[1])
	nodeDistance = int(os.sys.argv[2])
	initialX = 0
	with open("testScenario.ns2mobility.xml", "w+") as f:
		for i in range(numNodes):
			f.write("$node_(" + str(i) + ") set X_ " + str(initialX + i) + "\n")
			f.write("$node_(" + str(i) + ") set Y_ 0\n")
			f.write("$node_(" + str(i) + ") set Z_ 0\n")
			f.write('$ns_ at 0.0 "$node_(' + str(i) + ') setdest 0 0 0.00"\n')
	
if __name__ == "__main__":
	main()