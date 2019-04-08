#!/usr/bin/python

# Invocation: 
#	./createGridScenario.py roadLength roadNumber roadDistance roadSize nodeDistance
# e.g.
#  ./createGridScenario.py 4200 14 300 10 25
# or (gridSmall)
#  ./createGridScenario.py 1200 5 300 10 25

import sys, os
import string
import shutil
import utils



def main():
	roadLength = int(os.sys.argv[1])
	roadNumber = int(os.sys.argv[2])
	roadDistance = int(os.sys.argv[3])
	roadSize = int(os.sys.argv[4])
	nodeDistance = int(os.sys.argv[5])
	
	nodesPerRoad = roadLength // nodeDistance
	initialX = 100
	initialY = 100
	nodeId = 0
	polyFilePath = "./grid/gridScenario.poly.xml"

	with open("./grid/gridScenario.ns2mobility.xml", "w+") as f:
		# vertical roads
		for x in range(roadNumber):
			print("x= " + str(x))
			for y in range(nodesPerRoad):
				utils.writeNodeToFile(f, nodeId, initialX + x * roadDistance, initialY + y * nodeDistance, 0)
				nodeId += 1
		
		# horizontal roads
		for y in range(roadNumber):
			for x in range(nodesPerRoad):
				utils.writeNodeToFile(f, nodeId, initialX + x * nodeDistance, initialY + y * roadDistance, 0)
				nodeId += 1
	print("Created grid with " + str(nodeId + 1) + "nodes")
	utils.createPolyFile(polyFilePath, roadNumber, roadDistance, roadSize, initialX, initialY)

if __name__ == "__main__":
	main()