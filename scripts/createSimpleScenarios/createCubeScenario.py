#!/usr/bin/python

# Invocation: 
#	./createCubeScenario.py nodesPerRoad nodeDistance
# e.g.

#	./createCubeScenario.py 30 75			 Cube-75
#	./createCubeScenario.py 20 150			 Cube-150

import sys, os
import string
import shutil
import utils



def main():
	roadNumber = int(os.sys.argv[1])
	nodeDistance = int(os.sys.argv[2])
	

	initialX = 100
	initialY = 100
	initialZ = 100
	nodeId = 0

	edgeLength = roadNumber * nodeDistance
	print("edge of cube is long " + str(edgeLength))
	print("middle in " + (str(edgeLength / 2 + initialX)))

	with open("./cube/cubeScenario.ns2mobility.xml", "w+") as f:
		for z in range(roadNumber):
			for y in range(roadNumber):
				for x in range(roadNumber):
					#print ("x= " + str(x) + " y= " + str(y) + " z= " + str(z))
					utils.writeNodeToFile(f, nodeId, initialX + x * nodeDistance, initialY + y * nodeDistance, initialZ + z * nodeDistance)
					nodeId += 1

	print("Created grid with " + str(nodeId + 1) + " nodes")

if __name__ == "__main__":
	main()