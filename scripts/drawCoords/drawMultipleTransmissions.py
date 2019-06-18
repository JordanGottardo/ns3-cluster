#!/usr/bin/python2
#coding=utf-8
#Invocation:
#   ./drawMultipleTransmissions.py
# OR
#   ./drawMultipleTransmissions.py path.csv
# example: ./drawSingleTransmission.py /home/jordan/MEGA/Universita_mia/Magistrale/Tesi/ns3-cluster/ns-3.26/out/scenario-urbano-con-coord/cw-32-1024/Padova/d25/b0/st500-500/Padova-25-cw-32-1024-b0-st500-500-1550077028283.csv 10

import os
import sys
import getopt
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import csv
import xml.etree.ElementTree as ET
import coordUtils as coordUtils


plt.rcParams["figure.figsize"] = [10, 10]
circRadius = 2000
baseFolder = "../../ns-3.26/out/scenario-urbano-con-coord/cw-32-1024/Padova/d25/"

def createOrderedSourcesList(transmissionVector):
    orderedSourcesList = []
    for edge in transmissionVector:
        source = edge.source
        if (source not in orderedSourcesList):
            orderedSourcesList.append(source)
    return orderedSourcesList

def plotSingleTransmission(relativeFileName, outFileBasePath, ns2MobilityFile, polyFilePath):
    print("Plotting multiple transmission " + relativeFileName)
    startingVehicle = 0
    vehicleDistance = 0
    txRange = 0
    xReceivedCoords = []
    yReceivedCoords = []
    xNodeCoords = []
    yNodeCoords = []
    startingX = 0
    startingY = 0
    transmissionMap = {}
    receivedCoordsOnCirc = []
    receivedOnCircIds = []
    transmissionVector = []
    nodeIds = []

    txRange, startingX, startingY, startingVehicle, vehicleDistance, xReceivedCoords, yReceivedCoords, xNodeCoords, yNodeCoords, transmissionMap, receivedCoordsOnCirc, receivedOnCircIds, transmissionVector, nodeIds = coordUtils.parseFile(relativeFileName, ns2MobilityFile)

    nodeCoordsMap = {}
    color1 = "#840000"

    orderedSourcesList = createOrderedSourcesList(transmissionVector)
    count = 0
    for orderedSource in orderedSourcesList:
        print("source " + str(count))
        count += 1
    
        plt.plot(xNodeCoords, yNodeCoords, ".", markersize=5, color="#A00000", label="Not reached by Alert Message") #red
        coordUtils.plotTxRange(circRadius, startingX, startingY, vehicleDistance, color1, True)

        for edge in transmissionVector:
            if (edge.source not in orderedSourcesList[0:count]):
                continue
            lineColor = "0.8"
            sourceColor = "#560589"
            forwarderLabel = "Previous forwarder"
            if (edge.source == orderedSource):
                lineColor = "0.35"
                sourceColor = "#bf59ff"
                forwarderLabel = "Latest forwarder"
            source = edge.source
            destination = edge.destination
            if (not source in nodeCoordsMap):
                nodeCoordsMap[source] = coordUtils.findCoordsFromFile(edge.source, ns2MobilityFile)
            if (not destination in nodeCoordsMap):
                nodeCoordsMap[destination] = coordUtils.findCoordsFromFile(edge.destination, ns2MobilityFile)
            sourceCoord = nodeCoordsMap[source]
            destCoord = nodeCoordsMap[destination]
            
            c1 = np.array((sourceCoord.x, sourceCoord.y, sourceCoord.z))
            c2 = np.array((destCoord.x, destCoord.y, destCoord.z))
            plt.plot(destCoord.x, destCoord.y, ".", color="#32DC32", markersize=5, label="Reached by Alert Message") #green  
            plt.plot(sourceCoord.x, sourceCoord.y, "ro", color=sourceColor,  markersize=5, label=forwarderLabel)
            plt.plot([sourceCoord.x, destCoord.x], [sourceCoord.y, destCoord.y], color=lineColor, linewidth=0.3, alpha=0.7)
        plt.plot(startingX, startingY, "ro", color="yellow", markeredgecolor="blue", markersize=5, label="Source of Alert Message")

        coordUtils.plotTxRange(circRadius, startingX, startingY, vehicleDistance, color1, True)
        plt.legend(loc="upper center", framealpha=1.0)
        coordUtils.plotBuildings(polyFilePath)

        #Save file 
            
        if not os.path.exists(os.path.dirname(outFileBasePath)):
            try:
                os.makedirs(os.path.dirname(outFileBasePath))
            except OSError as exc: # Guard against race condition
                if exc.errno != errno.EEXIST:
                    raise
        plt.savefig(outFileBasePath + "-" + str(count) + " .pdf")
        plt.clf()
        

def main():
    print("Draw multiple transmissions")
    relativeFileName = sys.argv[1]
    ns2MobilityFile = sys.argv[2]
    polyFilePath = None
    if (len(sys.argv) > 3):
        polyFilePath = sys.argv[3]
    plotSingleTransmission(relativeFileName, "./out/singlefileMultipleTransmission/multipleTransmission", ns2MobilityFile, polyFilePath)

if __name__ == "__main__":
    main()
