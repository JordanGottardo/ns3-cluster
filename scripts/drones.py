#!/usr/bin/python
"""
@file    junctions.py
@author  Jordan Gottardo [jordan.gottardo@studenti.unipd.it]
@date    2019-04-19
"""

#example
# ./junctions.py ns2MobilityFilePath

from __future__ import print_function
from __future__ import absolute_import
import os
import sys
sys.path.insert(0, './drawCoords')
import random
import coordUtils

import sumolib

def main():
	ns2MobilityFilePath = sys.argv[1]
	nodeList = coordUtils.parseNodeList(ns2MobilityFilePath)
	outFilePath = os.path.join(os.path.dirname(ns2MobilityFilePath), os.path.splitext(os.path.splitext(os.path.basename(ns2MobilityFilePath))[0])[0] + ".drones")
	nodesInsideJunctions = set()
	
	with open(outFilePath, "w") as f: 
		for nodeId, nodeCoords in nodeList.iteritems():
			
				if (isNodeInsideJunction(nodeCoords, junction) and nodeId not in nodesInsideJunctions):
					nodesInsideJunctions.add(nodeId)
					line = nodeId + " " + junction.get("id") + "\n"
					f.writelines(line)

if __name__ == "__main__":
	main()