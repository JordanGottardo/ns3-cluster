#!/usr/bin/python
"""
@file    sensors-fixedPositions.py
@author  Marco Romanelli [marco.romanelli.1@studenti.unipd.it]
@date    2017-10-12
"""

from __future__ import print_function
from __future__ import absolute_import
import os
import sys
import random
import bisect
import datetime
import subprocess
from collections import defaultdict
import math
import optparse
from math import sqrt
import Queue

import sumolib
import sumolib.output.convert.ns2 as ns2

global __vDistance

def distance(a, b):
	return sqrt((a.x - b.x)**2 + (a.y - b.y)**2)

def get_options(args=None):
	optParser = optparse.OptionParser()
	optParser.add_option("-n", "--net-file", dest="netfile",
						 help="define the net file (mandatory)")
	optParser.add_option("-o", "--ns2mobility-output", dest="ns2mobility",
						 help="Defines the name of the ns2 file to generate")
	optParser.add_option("-z", "--height", dest="tlsHeight", type="float",
						 default="6", help="define the height at witch place the sensors")
 	optParser.add_option("-i", "--firstID", dest="firstID", type="int",
 						 default="0", help="override first ID")
						 # read https://mutcd.fhwa.dot.gov/pdfs/2009r1r2/part4.pdf
	(options, args) = optParser.parse_args(args=args)
	if  not options.netfile or not options.ns2mobility:
		optParser.print_help()
		sys.exit(1)

	return options

def convert2ns2mobility(positions, firstID):
	res = []
	for i, pos in enumerate(positions):
		x, y, z = pos
		res.append(pos2ns2format(i + firstID, x, y, z))

	return res

def pos2ns2format(id, x, y, z):
	base = "$node_({id}) set X_ {x}\n$node_({id}) set Y_ {y}\n$node_({id}) set Z_ {z}\n$ns_ at 0.0 \"$node_({id}) setdest {x} {y} {z}\"\n".format(id=id, x=x, y=y, z=z)

	return base

def main(options):
	if (options.tlsHeight != 3):
		print("[+] Override TLS height: %d." % options.tlsHeight)

	print("[+] Reading net file...")
	net = sumolib.net.readNet(options.netfile)

	print("[+] Running...")

	positions = []
	for tls in net.getTrafficLights():
		for edge in tls.getEdges():
			idx = str(edge.getID())
			shape = edge.getShape()

			tls_pos_x, tls_pos_y = shape[len(shape)-1]

			positions.append( (tls_pos_x, tls_pos_y, options.tlsHeight) )

	print("[+] I created %d sensors." % len(positions))
	print("[+] Writing trips file.")

	data = convert2ns2mobility(positions, options.firstID)

	with open(options.ns2mobility, 'w') as file:
		for el in data:
			file.write(el)


# info: print([method_name for method_name in dir(net._edges[0])])

if __name__ == "__main__":
	# Increase the recursion limit
	sys.setrecursionlimit(10000)

	print("[+] %s" % sys.argv[0])

	if not main(get_options()):
		sys.exit(1)

	print("[+] Done. Bye :)\n")
