#!/usr/bin/python

import sys, os
import string
import shutil

def writeNodeToFile(f, nodeId, x, y, z = 0):
	f.write("$node_(" + str(nodeId) + ") set X_ " + str(x) + "\n")
	f.write("$node_(" + str(nodeId) + ") set Y_ " + str(y) + "\n")
	f.write("$node_(" + str(nodeId) + ") set Z_ " + str(z) + "\n")
	f.write('$ns_ at 0.0 "$node_(' + str(nodeId) + ') setdest 0 0 0.00"\n')



def createPolyFile(filePath, roadNumber, roadDistance, roadSize, initialX = 0, initialY = 0):
	buildingsPerRow = roadNumber - 1
	buildingWidth = roadDistance - roadSize
	id = 0

	fileIntro = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\n<additional xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"http://sumo.dlr.de/xsd/additional_file.xsd\">"

	with open(filePath, "w+") as f:
		f.writelines(fileIntro)
		for row in range(buildingsPerRow):
			for col in range(buildingsPerRow):
				bottomLeftX = initialX + roadDistance * col + roadSize / 2 
				bottomLeftY = initialY + roadDistance * row + roadSize / 2 
				
				bottomRightX = bottomLeftX + buildingWidth
				bottomRightY = bottomLeftY

				topRightX = bottomLeftX + buildingWidth
				topRightY = bottomLeftY + buildingWidth

				topLeftX = bottomLeftX
				topLeftY = bottomLeftY + buildingWidth
				
				line = "<poly id=\"b" 
				line += str(id)
				id += 1	
				line+= "\" type=\"building\" color=\"90,102,171\" fill=\"1\" layer=\"-1.00\" shape=\""
				line += str(bottomLeftX) + "," + str(bottomLeftY) + " " + str(bottomRightX) + "," + str(bottomRightY) + " " + str(topRightX) + "," + str(topRightY) + " " + str(topLeftX) + "," + str(topLeftY) + " " + str(bottomLeftX) + "," + str(bottomLeftY) + "\"/>\n"
				f.writelines(line)
		f.writelines("</additional>")