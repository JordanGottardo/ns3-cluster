#!/usr/bin/python

import os
import sys
import getopt
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import csv
import xml.etree.ElementTree as ET

#3D coordinate class
class Vector:
    def __init__(self, x, y, z):
        self.x = float(x) 
        self.y = float(y) 
        self.z = float(z)
    
    def __hash__(self):
        return hash((self.x, self.y, self.z))
    
    def __eq__(self, other):
        return (self.x, self.y, self.z) == (other.x, other.y, other.z)

    def __ne__(self, other):
        return not(self == other)  

    def __repr__(self):
        return "({0}, {1}, {2})".format(self.x, self.y, self.z)

    def __str__(self):
        return "Vector({0},{1},{2})".format(self.x, self.y, self.z)

    def _privateM(self)
  

def plotTxRange(txRange, starterCoordX, starterCoordY, vehicleDistance, color, plotInterval):
    x = np.linspace(0, 3000, 100)
    y = np.linspace(0, 3000, 100)
    X, Y = np.meshgrid(x, y)
    realTxRange = (X - starterCoordX) ** 2 + (Y - starterCoordY) ** 2 - txRange ** 2
    CS = plt.contour(X, Y, realTxRange, [0], colors = color)

    if (plotInterval):
        outerTxRange = (X - starterCoordX) ** 2 + (Y - starterCoordY) ** 2 - (txRange + vehicleDistance) ** 2 
        innerTxRange = (X - starterCoordX) ** 2 + (Y - starterCoordY) ** 2 - (txRange - vehicleDistance) ** 2
        plt.contour(X, Y, outerTxRange, [0], colors = color, linestyles = "dashed")
        plt.contour(X, Y, innerTxRange, [0], colors = color, linestyles = "dashed")
    # F = X**2 + Y**2 - 90000
    # if (color == "#840000"):
    plt.clabel(CS, inline=1, fontsize=10)
    CS.collections[0].set_label(str(txRange) + " m")

def retrieveCoords(coords):
    xCoords = []
    yCoords = []
    coords = coords.split("_")
    for coord in coords:
        splitCoord = coord.split(":")
        if (len(splitCoord) != 3):
            continue;
        xCoords.append(float(splitCoord[0]))
        yCoords.append(float(splitCoord[1]))
    return xCoords, yCoords

def retrieveCoordsAsVector(rawCoords):
    coords = []
    splitCoords = rawCoords.split(";")
    for c in splitCoords:
        if (len(c) == 0):
            continue
        splitC = c.split(":")
        coords.append(Vector(splitC[0], splitC[1], splitC[2]))
    return coords

def buildVectorFromCoords(coords):
    splitCoords = coords.split(":")
    vector = Vector(splitCoords[0], splitCoords[1], splitCoords[2])
    return vector

def parseTransmissionMap(rawTransmissionMap):
    count = 0
    afterCloseCurlyRemove = rawTransmissionMap.split("}")
    transmissionMap = {}
    for s in afterCloseCurlyRemove:
        if(len(s) == 0):
            continue
        afterOpenCurlyRemove = s.split("{")
        #print("after open curly remove")
        #print(afterOpenCurlyRemove)
        keyVector = buildVectorFromCoords(afterOpenCurlyRemove[0])
        
        #print("key vector")
        #print(keyVector)
        destinations = afterOpenCurlyRemove[1]
        #print("destinations")
        #print(destinations)
        splitDestinations = destinations.split(";")
        #print("splitDestinations")
        #print(splitDestinations)
        transmissionMap[keyVector] = []
        for dest in splitDestinations:
            #print("dest")
            #print(dest)
            if(len(dest) == 0):
                continue
            destVector = buildVectorFromCoords(dest)
            transmissionMap[keyVector].append(destVector)
    print[transmissionMap]
    return transmissionMap


def parseFile(filePath):
    startingVehicle = 0
    vehicleDistance = 0
    txRange = 0
    xReceivedCoords = []
    yReceivedCoords = []
    xNodeCoords = []
    yNodeCoords = []
    nodeCoords = []
    receivedCoords = []
    receivedCoordsOnCirc = []
    startingX = 0
    startingY = 0
    with open(filePath, "r") as file:
        csvFile = csv.reader(file, delimiter=",")
        next(csvFile)
        line = next(csvFile)
        txRange = int(line[2])
        startingX = float(line[14])
        startingY = float(line[15])
        startingVehicle = int(line[16])
        vehicleDistance = int(line[17])
        receivedCoords = line[18]
        nodeCoords = line[19]
        xReceivedCoords, yReceivedCoords = retrieveCoords(receivedCoords)
        xNodeCoords, yNodeCoords = retrieveCoords(nodeCoords)
        rawTransmissionMap = line[20]
        rawReceivedOnCircCoords = line[21]
        receivedCoordsOnCirc = retrieveCoordsAsVector(rawReceivedOnCircCoords)
        transmissionMap = parseTransmissionMap(rawTransmissionMap)
    return txRange, startingX, startingY, startingVehicle, vehicleDistance, xReceivedCoords, yReceivedCoords, xNodeCoords, yNodeCoords, transmissionMap, receivedCoordsOnCirc

def plotBuildings(polyFilePath):
    tree = ET.parse(polyFilePath)
    root = tree.getroot()
    polyList = list(root.iter("poly"))
    count = 0
    for poly in polyList:
        polyType = poly.get("type")
        if (polyType != "building" and polyType != "amenity"):
        #if (polyType == "water" or polyType == "residential" or polyType == "landuse" or polyType == "natural"
        #or polyType == "historic"):
        #amenity
        #leisure
            continue
        count = count + 1
        coords = poly.get("shape")
        splitCoords = coords.split( )
        xShapeCoords = []
        yShapeCoords = []
        for coord in splitCoords:
            splitCoords2 = coord.split(",")
            xShapeCoords.append(float(splitCoords2[0]))
            yShapeCoords.append(float(splitCoords2[1]))
        plt.fill(xShapeCoords, yShapeCoords, color="red", alpha=0.15)