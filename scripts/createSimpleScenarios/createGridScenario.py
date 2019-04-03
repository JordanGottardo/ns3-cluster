#!/usr/bin/python

# Invocation: 
#	./createGridScenario.py roadLength roadNumber roadDistance nodeDistance
# e.g.
#  ./createGridScenario.py 4000 12 300 12

import sys, os
import string
import shutil
import utils

def main():
	roadLength = int(os.sys.argv[1])
	roadNumber = int(os.sys.argv[2])
	roadDistance = int(os.sys.argv[3])
	nodeDistance = int(os.sys.argv[4])
	nodesPerRoad = roadLength // nodeDistance
	initialX = 0
	initialY = 0
	nodeId = 0

	with open("gridScenario.ns2mobility.xml", "w+") as f:
		# vertical roads
		for x in range(roadNumber):
			print("x= " + str(x))
			for y in range(nodesPerRoad):
				utils.writeNodeToFile(f, nodeId, x * roadDistance, y * nodeDistance, 0)
				nodeId += 1
		
		# horizontal roads
		for y in range(roadNumber):
			for x in range(nodesPerRoad):
				utils.writeNodeToFile(f, nodeId, x * nodeDistance, y * roadDistance, 0)
				nodeId += 1
	print("Created grid with " + str(nodeId + 1) + "nodes")
	
if __name__ == "__main__":
	main()