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
        return "({0}, {1}, {2})\n".format(self.x, self.y, self.z)

    def __str__(self):
        return "Vector({0},{1},{2})".format(self.x, self.y, self.z)

class Edge:
    def __init__(self, x, y, z):
        self.source = int(x) 
        self.destination = int(y) 
        self.phase = int(z)

    def __repr__(self):
        return "({0}, {1}, {2})".format(self.source, self.destination, self.phase)

    def __str__(self):
        return "Edge({0},{1},{2})".format(self.source, self.destination, self.phase)

def isFileComplete(filePath):
    with open(filePath) as f:
        for i, l in enumerate(f):
            pass
    return (i == 1)


def plotTxRange(txRange, starterCoordX, starterCoordY, vehicleDistance, color="black", plotInterval=True):
    color = "black"
    #print("PlotTxRange" + " txRange= " + str(txRange) + " starterCoordX= " + str(starterCoordX) + " starterCoordY= " + str(starterCoordY) + " vehicleDistance= " + str(vehicleDistance))
    x = np.linspace(0, 5000, 100)
    y = np.linspace(0, 5000, 100)
    X, Y = np.meshgrid(x, y)
    realTxRange = (X - starterCoordX) ** 2 + (Y - starterCoordY) ** 2 - txRange ** 2
    CS = plt.contour(X, Y, realTxRange, [0], colors = color, label='_nolegend_')

    if (plotInterval):
        outerTxRange = (X - starterCoordX) ** 2 + (Y - starterCoordY) ** 2 - (txRange + vehicleDistance) ** 2 
        innerTxRange = (X - starterCoordX) ** 2 + (Y - starterCoordY) ** 2 - (txRange - vehicleDistance) ** 2
        plt.contour(X, Y, outerTxRange, [0], colors = color, linestyles = "dashed", label='_nolegend_')
        plt.contour(X, Y, innerTxRange, [0], colors = color, linestyles = "dashed", label='_nolegend_')
    # F = X**2 + Y**2 - 90000
    # if (color == "#840000"):
    #plt.clabel(CS, inline=1, fontsize=1)
    #CS.collections[0].set_label(str(txRange) + " m")

def findCoordsFromFile(nodeId, ns2MobilityFile):
    nodeId = str(nodeId)
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
        keyNode = afterOpenCurlyRemove[0].split(":")[0]
        destinations = afterOpenCurlyRemove[1]
        splitDestinations = destinations.split(";")
        transmissionMap[keyNode] = []
        for dest in splitDestinations:
            if(len(dest) == 0):
                continue
            transmissionMap[keyNode].append(dest)
    return transmissionMap

def parseTransmissionVector(rawTransmissionVector):
    rawEdges = rawTransmissionVector.split("_")
    transmissionVector = []
    for rawEdge in rawEdges:
        if (len(rawEdge) > 0):
            afterDashSplit = rawEdge.split("-")
            source = afterDashSplit[0]
            afterStarSplit = afterDashSplit[1].split("*")
            destination = afterStarSplit[0]
            phase = afterStarSplit[1]
            transmissionVector.append(Edge(source, destination, phase))
    return transmissionVector

def parseFile(filePath, ns2MobilityFile):
    startingVehicle = 0
    vehicleDistance = 0
    txRange = 0
    xReceivedCoords = []
    yReceivedCoords = []
    xNodeCoords = []
    yNodeCoords = []
    rawNodeIds = []
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
        rawNodeIds = line[19]
        nodeIds = filter(None, rawNodeIds.split("_"))
        xReceivedCoords, yReceivedCoords = retrieveCoords(receivedIds, ns2MobilityFile)
        xNodeCoords, yNodeCoords = retrieveCoords(rawNodeIds, ns2MobilityFile)
        rawTransmissionMap = line[20]
        receivedOnCircIds = line[21]
        rawTransmissionVector = line[22]
        receivedCoordsOnCirc = retrieveCoordsAsVector(receivedOnCircIds, ns2MobilityFile)
        receivedOnCircIds = filter(None, receivedOnCircIds.split("_"))
        transmissionMap = parseTransmissionMap(rawTransmissionMap)
        transmissionVector = parseTransmissionVector(rawTransmissionVector)
    return txRange, startingX, startingY, startingVehicle, vehicleDistance, xReceivedCoords, yReceivedCoords, xNodeCoords, yNodeCoords, transmissionMap, receivedCoordsOnCirc, receivedOnCircIds, transmissionVector, nodeIds

def plotShape(shape, pColor="red", pAlpha=0.15):
    splitCoords = shape.split( )
    xShapeCoords = []
    yShapeCoords = []
    for coord in splitCoords:
        splitCoords2 = coord.split(",")
        xShapeCoords.append(float(splitCoords2[0]))
        yShapeCoords.append(float(splitCoords2[1]))
        plt.fill(xShapeCoords, yShapeCoords, color=pColor, alpha=pAlpha)

def getBoundingBox(shape, extension=10):
    splitCoords = shape.split( )
    xShapeCoords = []
    yShapeCoords = []
    for coord in splitCoords:
        splitCoords2 = coord.split(",")
        xShapeCoords.append(float(splitCoords2[0]))
        yShapeCoords.append(float(splitCoords2[1]))
    xMin, xMax = min(xShapeCoords) - extension, max(xShapeCoords) + extension
    yMin, yMax = min(yShapeCoords) - extension, max(yShapeCoords) + extension
    return xMin, xMax, yMin, yMax

def plotBoundingBox(shape, extension=10, pColor="yellow", pAlpha=0.45):
    xMin, xMax, yMin, yMax = getBoundingBox(shape, extension)
    boundingBoxX = [xMin, xMax, xMax, xMin]
    bounbingBoxY = [yMax, yMax, yMin, yMin]
    plt.fill(boundingBoxX, bounbingBoxY, color=pColor, alpha=pAlpha)

def plotBuildings(polyFilePath, plotBuildingIds=False, ax=None):
    if (polyFilePath is None):
        return
    print("coordUtils::plotBuildings")
    tree = ET.parse(polyFilePath)
    root = tree.getroot()
    polyList = list(root.iter("poly"))
    count = 0
    print("coordUtils::plotBuildings found " + str(len(polyList)) + " buildings")
    for poly in polyList:
        polyId = poly.get("id")
        polyType = poly.get("type")
        if (polyType != "building" and polyType != "unknown"):
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
        sumX = 0
        sumY = 0
        for coord in splitCoords:
            splitCoords2 = coord.split(",")
            xShapeCoords.append(float(splitCoords2[0]))
            yShapeCoords.append(float(splitCoords2[1]))
            sumX += xShapeCoords[-1]
            sumY += yShapeCoords[-1]
        plt.fill(xShapeCoords, yShapeCoords, color="red", alpha=0.15)

        if (plotBuildingIds):
            xCenter = sumX / len(xShapeCoords)
            yCenter = sumY / len(yShapeCoords)
            ax.annotate(polyId, xy=(xCenter, yCenter), size=8)

def parseNodeList(ns2MobilityFilePath):
    coordDict = {}
    with open(ns2MobilityFilePath) as f:
        lines = f.readlines()
        numLines = len(lines)
        count = 0
        while(count < numLines):
            line = lines[count].strip()
            splitLine = line.split(" ")
            if (len(splitLine) != 4):
                count = count + 1
                continue
            id = splitLine[0].split("_")
            id = id[1].replace("(", "")
            id = id.replace(")", "")
            x = splitLine[3].strip()
            y = lines[count + 1].split(" ")[3].strip()
            z = lines[count + 2].split(" ")[3].strip()
            coords = Vector(x, y, z)
            count = count + 3
            coordDict[id] = coords
    return coordDict

def plotNodeList(ns2MobilityFilePath):
    coordDict = parseNodeList(ns2MobilityFilePath)
    xCoord = []
    yCoord = []
    for key, coord in coordDict.iteritems():
        xCoord.append(coord.x)
        yCoord.append(coord.y)
    plt.plot(xCoord, yCoord, ".", color="#32DC32")

def plotStartingNode(startingNodeId, ns2MobilityFile):
    coordDict = parseNodeList(ns2MobilityFile)
    startingX = coordDict[startingNodeId].x 
    startingY = coordDict[startingNodeId].y
    plt.plot(startingX, startingY, "ro", color="yellow", markeredgecolor="blue", markersize=5, label="Source of Alert Message")
    plotTxRange(1000, startingX, startingY, 25)

def parseJunctionList(netFilePath):
    tree = ET.parse(netFilePath)
    root = tree.getroot()
    junctionList = list(root.iter("junction"))
    return junctionList

def plotJunctions(netFilePath):
    print("coordUtils::plotJunctions")
    count = 0
    junctionList = parseJunctionList(netFilePath)
    for junction in junctionList:
        count += 1
        shape = junction.get("shape")
        if (shape is None or shape == ""):
            continue
        #print(shape)
        #if (count % 10 == 1):
        plotShape(shape)
        plotBoundingBox(shape)
    print("Plotted " + str(count) + " junctions")
         

