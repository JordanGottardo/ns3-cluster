#!/usr/bin/python
"""
@file    junctions.py
@author  Jordan Gottardo [jordan.gottardo@studenti.unipd.it]
@date    2019-04-19
"""

#example
# ./junctions.py ns2MobilityFilePath netFilePath

from __future__ import print_function
from __future__ import absolute_import
import os
import sys
sys.path.insert(0, './drawCoords')
import random
import coordUtils

import sumolib
#from sumolib import route2trips
#from sumolib.miscutils import euclidean
#from sumolib.net.lane import SUMO_VEHICLE_CLASSES

def isNodeInsideJunction(nodeCoords, junction):
	shape = junction.get("shape")
	if (shape is None or shape == ""):
		return False
	xMin, xMax, yMin, yMax = coordUtils.getBoundingBox(shape, 10)
	return nodeCoords.x >= xMin and nodeCoords.x <= xMax and nodeCoords.y >= yMin and nodeCoords.y <= yMax

def main():
	ns2MobilityFilePath = sys.argv[1]
	netFilePath = sys.argv[2]
	nodeList = coordUtils.parseNodeList(ns2MobilityFilePath)
	outFilePath = "./nodeIntersectionMap"
	nodesInsideJunctions = set()
	#print(nodeList)
	junctionList = coordUtils.parseJunctionList(netFilePath)
	with open(outFilePath, "w") as f: 
		for nodeId, nodeCoords in nodeList.iteritems():
			for junction in junctionList:
				if (isNodeInsideJunction(nodeCoords, junction) and nodeId not in nodesInsideJunctions):
					nodesInsideJunctions.add(nodeId)
					line = nodeId + " " + junction.get("id") + "\n"
					f.writelines(line)
				




			
if __name__ == "__main__":
	main()