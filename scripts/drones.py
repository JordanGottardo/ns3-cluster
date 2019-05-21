#!/usr/bin/python
"""
@file    drones.py
@author  Jordan Gottardo [jordan.gottardo@studenti.unipd.it]
@date    2019-04-19
"""

#example
# ./drones.py ns2MobilityFilePath

from __future__ import print_function
from __future__ import absolute_import
import os
import sys
sys.path.insert(0, './drawCoords')
import random
import coordUtils

import sumolib

def generateDrone(nodeCoords, id, z):
	line = ""
	line += "$node_(" + str(id) + ") set X_ " + str(nodeCoords.x) + "\n"
	line += "$node_(" + str(id) + ") set Y_ " + str(nodeCoords.y) + "\n"	
	line += "$node_(" + str(id) + ") set Z_ " + str(z) + "\n"		
	line += '$ns_ at 0.0 "$node_(' + str(id) + ') setdest 0.00 0.00 0.00"\n'
	return line

def main():
	ns2MobilityFilePath = sys.argv[1]
	nodeList = coordUtils.parseNodeList(ns2MobilityFilePath)
	#outFilePath = os.path.join(os.path.dirname(ns2MobilityFilePath), os.path.splitext(os.path.splitext(os.path.basename(ns2MobilityFilePath))[0])[0] + ".drones")
	line = ""
	maxId = int(max(nodeList, key=int)) + 1
	print(maxId)
	
	with open(ns2MobilityFilePath, "a+") as f: 
		for nodeId in nodeList:
			nodeCoords = nodeList[nodeId]
			rand = random.randint(0, 99)
			if (rand >= 50):
				line += generateDrone(nodeCoords, maxId, 30)
				maxId += 1
			rand = random.randint(0, 99)
			if (rand >= 50):
				line += generateDrone(nodeCoords, maxId, 60)
				maxId += 1
		f.writelines(line)
	
if __name__ == "__main__":
	main()