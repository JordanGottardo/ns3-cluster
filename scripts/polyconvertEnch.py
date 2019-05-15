#!/usr/bin/python
"""
@file    polyconvertEnch.py
@author  Marco Romanelli [marco.romanelli.1@studenti.unipd.it]
@date    12-11-2017
"""

from __future__ import print_function
from __future__ import absolute_import
import os
import sys
import optparse

import sumolib
import xml.etree.cElementTree
# from sumolib.xml import parse, parse_fast, compound_object
from sumolib.shapes.polygon import Polygon, PolygonReader
from xml.sax import handler, parse

def get_options(args=None):
	optParser = optparse.OptionParser()
	optParser.add_option("-i", "--osm-file", dest="osmfile",
						 help="define the osm file (mandatory)")
	optParser.add_option("-p", "--poly-file", dest="polyfile",
						 default="poly.xml", help="define the input poly filename")
	optParser.add_option("-o", "--output-file", dest="outputfile",
						 default="3Dpoly.xml", help="define the output poly filename")
 	optParser.add_option("-f", "--floor", dest="floorH", type="float",
 						 default="2.7", help="Default height for a single house level")
	optParser.add_option("-r", "--roof", dest="roofH", type="float",
						 default="2.1", help="Default height for a roof level")
	(options, args) = optParser.parse_args(args=args)
	if not options.osmfile :
		optParser.print_help()
		sys.exit(1)

	return options

def retrieveHeights(filename, defaults):
	objects = {}
	for v in sumolib.output.parse(filename, "way"):
		elId = str(v.id)

		height = levels = roofLevels = None
		if (v.tag):
			for tag in v.tag:
				if (tag.k == "height"):
					height = float(tag.v)

				if (tag.k == "building:levels"):
					levels = float(tag.v)

				if (tag.k == "roof:levels"):
					roofLevels = float(tag.v)


		if (height != None or levels != None or roofLevels != None):
			h = estimateHeight(height, levels, roofLevels, defaults)
			objects[elId] = h

	return objects


def estimateHeight(height, levels, roofLevels, defaults):
	floorH, roofH = defaults
	
	res = 0

	if (height != None):
		res = height

	if (levels != None):
		res += levels * floorH

	if (roofLevels != None):
		res += roofLevels * roofH

	return res

def enhancedPoly(input, output, heights):
	polys = PolygonReader(False)
	parse(input, polys)

	totalpolys = 0
	enchpolys = 0
	res = []
	for poly in polys.getPolygons():
		polyId = str(poly.id)
		xml = poly.toXML()

		if (polyId in heights):
			xml = insertElementBeforeLast(xml, "height", heights[polyId])

			enchpolys +=1

		res.append(xml)

		totalpolys +=1

	perc = enchpolys / (totalpolys * 1.0) * 100
	print("[D] Total:%d, Ench:%d (%d%%)." % (totalpolys, enchpolys, perc))

	return res

def insertElementBeforeLast(s, k, v):
	pos = s.rfind("=\"")

	if (pos <= 0 or pos >= len(s)):
		return None

	pos2 = s[0:pos].rfind(' ')

	before = s[0:pos2]
	after = s[pos2:len(s)]

	return s[0:pos2] + " {k}=\"{v}\"".format(k=k, v=v) + s[pos2:len(s)]

def mean(heights):
	total = 0
	for h in heights.values():
		total += h

	return total / (len(heights) * 1.0)

def main(options):
	print("[+] Running...")

	hs = retrieveHeights(options.osmfile, (options.floorH, options.roofH))

	meanh = mean(hs)

	print("[+] I found %d infos about heights (mean h: %.2f)." % (len(hs), meanh))

	polys = enhancedPoly(options.polyfile, options.outputfile, hs)

	with open(options.outputfile, 'w') as out:
		sumolib.writeXMLHeader(out, '<?xml version="1.0" encoding="UTF-8"?>')
		out.write('<additional xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="http://sumo.dlr.de/xsd/additional_file.xsd">\n')

		for poly in polys:
			out.write("\t%s\n" % poly)

		out.write('</additional>')



if __name__ == "__main__":
	# Increase the recursion limit
	sys.setrecursionlimit(10000)

	print("[+] %s" % sys.argv[0])

	if not main(get_options()):
		sys.exit(1)

	print("[+] Done. Bye :)\n")
