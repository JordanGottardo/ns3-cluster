#!/usr/bin/python2
#coding=utf-8
#Invocation:
#   ./drawSingleHops.py
# OR
#   ./drawSingleHops.py path.csv
# example: ./drawSingleHops.py /home/jordan/MEGA/Universita_mia/Magistrale/Tesi/ns3-cluster/ns-3.26/out/scenario-urbano-con-coord/cw-32-1024/Padova/d25/b0/st500-500/Padova-25-cw-32-1024-b0-st500-500-1550077028283.csv

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

def findMaxHop(transmissionVector):
    return max(map(lambda edge: edge.phase, transmissionVector))

def plotHops(relativeFileName, outFileBasePath, ns2MobilityFile, polyFilePath):
    print("Plotting hops " + relativeFileName)
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

    maxHop = findMaxHop(transmissionVector)
    for hop in range(0, maxHop + 1):
        print("hop" + str(hop))

        plt.plot(xNodeCoords, yNodeCoords, ".", markersize=5, color="#A00000", label="Not reached by Alert Message") #red
        #plt.plot(xReceivedCoords, yReceivedCoords, ".", color="green")
        
        filteredTransmissionVector = filter(lambda x: x.phase <= hop, transmissionVector)           
        for edge in filteredTransmissionVector:
            #print(edge)
            lineColor = "0.8"
            sourceColor = "#560589"
            forwarderLabel = "Previous forwarder"
            if (edge.phase == hop):
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
            sourceCoord = coordUtils.findCoordsFromFile(edge.source, ns2MobilityFile)
            destCoord = coordUtils.findCoordsFromFile(edge.destination, ns2MobilityFile)
            plt.plot(destCoord.x, destCoord.y, ".", color="#32DC32", label="Reached by Alert Message") #green
            plt.plot(sourceCoord.x, sourceCoord.y, "ro", color=sourceColor, markersize=5, label=forwarderLabel)
            plt.plot([sourceCoord.x, destCoord.x], [sourceCoord.y, destCoord.y], color=lineColor, linewidth=0.3, alpha=0.7)
            
        plt.plot(startingX, startingY, "ro", color="yellow", markeredgecolor="blue", markersize=5, label="Source of Alert Message")

        color1 = "#840000"
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
        plt.savefig(outFileBasePath + "-hop" + str(hop + 1) + ".pdf")
        plt.clf()
        
        

def main():
    print("Draw hops")
    if (len(sys.argv) > 1):
        relativeFileName = sys.argv[1]
        ns2MobilityFile = sys.argv[2]
        polyFilePath = None
        if (len(sys.argv) > 3):
            polyFilePath = sys.argv[3]
        plotHops(relativeFileName, "./out/singlefileHops/outHops", ns2MobilityFile, polyFilePath)
    else:
        for buildingFolder in os.listdir(baseFolder):
            buildingPath = os.path.join(baseFolder, buildingFolder)
            if (os.path.isdir(buildingPath)):
                for protocolFolder in os.listdir(buildingPath):
                    protocolPath = os.path.join(buildingPath, protocolFolder)
                    if (os.path.isdir(protocolPath)):
                        count = 0
                        for csvFilename in os.listdir(protocolPath):
                            relativeFileName = os.path.join(protocolPath, csvFilename)
                            if (count > 2 or not coordUtils.isFileComplete(relativeFileName)):
                                continue
                            count += 1
                            outFilePath = os.path.join("./out/hops", buildingFolder, protocolFolder, os.path.splitext(csvFilename)[0] + ".pdf")
                            plotHops(relativeFileName, outFilePath)
                        

if __name__ == "__main__":
    main()
