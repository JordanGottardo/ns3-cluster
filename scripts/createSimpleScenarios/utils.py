#!/usr/bin/python

import sys, os
import string
import shutil

def writeNodeToFile(f, nodeId, x, y, z = 0):
	f.write("$node_(" + str(nodeId) + ") set X_ " + str(x) + "\n")
	f.write("$node_(" + str(nodeId) + ") set Y_ " + str(y) + "\n")
	f.write("$node_(" + str(nodeId) + ") set Z_ " + str(z) + "\n")
	f.write('$ns_ at 0.0 "$node_(' + str(nodeId) + ') setdest 0 0 0.00"\n')