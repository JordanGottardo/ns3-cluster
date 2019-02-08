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

def findCoordsFromFile(nodeId, ns2MobilityFile):
    with open(ns2MobilityFile, "r") as file:
        lines = file.readlines()
        numLines = len(lines)
        count = 0
        while(count < numLines):
            line = lines[count]
            splitLine = line.split(" ")
            if (len(splitLine) != 4):
                count = count + 1
                continue
            id = splitLine[0].split("_")
            id = id[1].replace("(", "")
            id = id.replace(")", "")
            line = file.readline()
            if (id == nodeId):
                x = splitLine[3].strip()
                y = lines[count + 1].split(" ")[3].strip()
                z = lines[count + 2].split(" ")[3].strip()
                return Vector(x, y, z)
            count = count + 1
    print("error: coordinate not found for node")
    print(nodeId)
            


def retrieveCoords(ids, ns2MobilityFile):
    xCoords = []
    yCoords = []
    splitIds = ids.split("_")

    for id in splitIds:
        if (len(id) == 0):
            continue
        coords = findCoordsFromFile(id, ns2MobilityFile)
        xCoords.append(coords.x)
        yCoords.append(coords.y)
    return xCoords, yCoords

def retrieveCoordsAsVector(ids, ns2MobilityFile):
    coords = []
    splitIds = ids.split("_")
    for id in splitIds:
        if (len(id) == 0):
            continue
        coord = findCoordsFromFile(id, ns2MobilityFile)
        coords.append(coord)
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
        #keyVector = buildVectorFromCoords(afterOpenCurlyRemove[0])
        keyNode = afterOpenCurlyRemove[0].split(":")[0]
        #print("key vector")
        #print(keyVector)
        destinations = afterOpenCurlyRemove[1]
        #print("destinations")
        #print(destinations)
        splitDestinations = destinations.split(";")
        #print("splitDestinations")
        #print(splitDestinations)
        transmissionMap[keyNode] = []
        for dest in splitDestinations:
            #print("dest")
            #print(dest)
            if(len(dest) == 0):
                continue
            #destVector = buildVectorFromCoords(dest)
            transmissionMap[keyNode].append(dest)
    #print[transmissionMap]
    return transmissionMap


def parseFile(filePath, ns2MobilityFile):
    startingVehicle = 0
    vehicleDistance = 0
    txRange = 0
    xReceivedCoords = []
    yReceivedCoords = []
    xNodeCoords = []
    yNodeCoords = []
    nodeIds = []
    receivedIds = []
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
        receivedIds = line[18]
        nodeIds = line[19]
        xReceivedCoords, yReceivedCoords = retrieveCoords(receivedIds, ns2MobilityFile)
        xNodeCoords, yNodeCoords = retrieveCoords(nodeIds, ns2MobilityFile)
        rawTransmissionMap = line[20]
        receivedOnCircIds = line[21]
        receivedCoordsOnCirc = retrieveCoordsAsVector(receivedOnCircIds, ns2MobilityFile)
        receivedOnCircIds = filter(None, receivedOnCircIds.split("_"))
        transmissionMap = parseTransmissionMap(rawTransmissionMap)
    return txRange, startingX, startingY, startingVehicle, vehicleDistance, xReceivedCoords, yReceivedCoords, xNodeCoords, yNodeCoords, transmissionMap, receivedCoordsOnCirc, receivedOnCircIds

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